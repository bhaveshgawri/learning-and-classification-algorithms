#include <bits/stdc++.h>
#include "NN_handwriting.hpp"
using namespace std;

FFNNet3L::FFNNet3L(int L2_size, int iterations)
{
	// +1 for bias
	this->L1_size = 64 + 1;
	this->L2_size = L2_size + 1;
	this->L3_size = 10;
	
	this->iterations = iterations;
	
	// hyperparameters of gradient decent
	this->alpha = 1e-3;
	this->beta1 = 0.9;
	this->beta2 = 0.999;
	this->epsilon = 1e-8;
	
	this->weights_L1_L2 = Matrix(this->L1_size, Pointnd(this->L2_size, 0));
	this->weights_L2_L3 = Matrix(this->L2_size, Pointnd(this->L3_size, 0));

	this->first_moment_L1_L2 = Matrix(this->L1_size, Pointnd(this->L2_size, 0));
	this->first_moment_L2_L3 = Matrix(this->L2_size, Pointnd(this->L3_size, 0));
	this->bias_corr_1st_mom_L1_L2 =  Matrix(this->L1_size, Pointnd(this->L2_size, 0));
	this->bias_corr_1st_mom_L2_L3 =  Matrix(this->L2_size, Pointnd(this->L3_size, 0));
	
	this->second_moment_L1_L2 = Matrix(this->L1_size, Pointnd(this->L2_size, 0));
	this->second_moment_L2_L3 = Matrix(this->L2_size, Pointnd(this->L3_size, 0));
	this->bias_corr_2nd_mom_L1_L2 =  Matrix(this->L1_size, Pointnd(this->L2_size, 0));
	this->bias_corr_2nd_mom_L2_L3 =  Matrix(this->L2_size, Pointnd(this->L3_size, 0));
}


void printMatrix(Matrix matrix)
{
	int count=0;
	for (auto i: matrix){
		cout<<"row: "<<count++<<nl;
		for (auto j: i){
			cout<<j<<" ";
		}
		cout<<nl;
	}
}


Matrix FFNNet3L::readFromInputFile(string input_file)
{
	Matrix examples;
	string line;
	ifstream infile(input_file);
	while(getline(infile, line)){
		if (line.length()==0) continue;
		stringstream ss(line);
		string word;
		vector<double>tmp;
		while(getline(ss, word, ',')){
			double value = stod(word);
			tmp.push_back(value);
		}
		examples.push_back(tmp);
	}
	return examples;
}


void FFNNet3L::initialize_random_weights()
{
	int l1 = this->L1_size;
	int l2 = this->L2_size;
	int l3 = this->L3_size;

	mt19937 rng;
	rng.seed(random_device()());
	uniform_int_distribution<std::mt19937::result_type> dist(0, INT_MAX);
	for (int i=0; i<l1; i++){
		for (int j=0; j<l2-1; j++){
			// L2_size-1 to stop bias node from getting weight
			double random_num = (double)dist(rng)/INT_MAX;
			random_num = 2*random_num - 1;
			this->weights_L1_L2[i][j] = random_num;
		}
	}
	for (int i=0; i<l2; i++){
		for (int j=0; j<l3; j++){
			double random_num = (double)dist(rng)/INT_MAX;
			random_num = 2*random_num - 1;
			this->weights_L2_L3[i][j] = random_num;
		}
	}
}


void FFNNet3L::printweights()
{
	cout<<"L1-L2 weights"<<nl;
	printMatrix(this->weights_L1_L2);
	cout<<nl<<nl;
	cout<<"L2-L3 weights"<<nl;
	printMatrix(this->weights_L2_L3);
}


Matrix FFNNet3L::get_random_miniBatch(Matrix train, int miniBatch_size)
{
	random_shuffle(train.begin(), train.end());

	Matrix miniBatch;
	for (int i=0;i<miniBatch_size;i++){
		miniBatch.push_back(train[i]);
	}
	return miniBatch;
}


MatrixPair FFNNet3L::forward_Propagation(Matrix train)
{
	int l1 = this->L1_size;
	int l2 = this->L2_size;
	int l3 = this->L3_size;

	Matrix hidden_outputs;
	for (auto example: train){
		// for each 64d pt in train(miniBatch)
		Pointnd hidden_output(l2, 0);
		for (int i=0; i<l2-1; i++){
			// first hidden node is:
			for (int j=0; j<l1; j++){
				// sum of example*weight
				if (j == l1-1)
					// j==l1-1 is bias
					hidden_output[i] += 1*weights_L1_L2[j][i];
				else
					hidden_output[i] += example[j]*weights_L1_L2[j][i];
			}
		}
		// bias of hidden layer
		hidden_output[l2-1] = 1;
		hidden_outputs.push_back(hidden_output);
	}

	// ReLU
	// for (int i=0; i<(int)hidden_outputs.size();i++){
	// 	for (int j=0; j<(int)hidden_outputs[i].size();j++){
	// 		if (hidden_outputs[i][j] < 0) 
	// 			hidden_outputs[i][j] = 0;
	// 	}
	// }
	
	Matrix final_outputs;
	for (auto example: hidden_outputs){
		Pointnd output(l3, 0);
		for (int i=0; i<l3; i++){
			for (int j=0; j<l2; j++){
				output[i] += example[j]*weights_L2_L3[j][i];
			}
		}
		final_outputs.push_back(output);
	}

	// Softmax
	for (int i=0; i<(int)final_outputs.size(); i++){
		double den=0;
		for (int j=0; j<l3; j++){
			den += exp(final_outputs[i][j]);
		}
		for (int j=0; j<l3; j++){
			final_outputs[i][j] = exp(final_outputs[i][j]) / den;
		}
		// double sum=0;
		// for (int j=0;j<l3;j++) sum+=final_outputs[i][j];
		// 	cout<<sum<<nl;
	}
	
	return {hidden_outputs, final_outputs};
}


MatrixPair FFNNet3L::back_Propagation(Matrix miniBatch_nodes_L1, Matrix miniBatch_nodes_L2,
										Matrix miniBatch_errors)
{
	int l1 = this->L1_size;
	int l2 = this->L2_size;
	int l3 = this->L3_size;
	Matrix miniBatch = miniBatch_nodes_L1;
	Matrix gradient_mat_L1_L2 = Matrix(l1, Pointnd(l2, 0));
	Matrix gradient_mat_L2_L3 = Matrix(l2, Pointnd(l3, 0));
	Matrix grad_hidden_layer = Matrix(miniBatch.size(), Pointnd(l2, 0));

	// layer 2-3 weight grads
	for (int i=0;i<l2;i++){
		for (int j=0;j<l3;j++){
			for (int k=0; k<(int)miniBatch.size(); k++){
				gradient_mat_L2_L3[i][j] += miniBatch_nodes_L2[k][i]*miniBatch_errors[k][j];
			}
		}
	}

	for (int i=0; i<(int)miniBatch.size(); i++){
		for (int j=0; j<l2; j++){
			for (int k=0; k<l3; k++){
				grad_hidden_layer[i][j] += miniBatch_errors[i][k]*this->weights_L2_L3[j][k];
			}
		}
	}
	
	// RelU
	// for (int i=0; i<(int)grad_hidden_layer.size();i++){
	// 	for (int j=0; j<(int)grad_hidden_layer[i].size();j++){
	// 		if (miniBatch_nodes_L2[i][j] <= 0) 
	// 			grad_hidden_layer[i][j] = 0;
	// 	}
	// }

	// layer 1-2 weight grads
	for (int i=0; i<l1; i++){
		for (int j=0; j<l2-1; j++){
			for (int k=0; k<(int)miniBatch.size(); k++){
				gradient_mat_L1_L2[i][j] = miniBatch_nodes_L1[k][i]*grad_hidden_layer[k][j];
			}
		}
	}

	return {gradient_mat_L1_L2, gradient_mat_L2_L3};
}


MatrixPair FFNNet3L::calc_grad(Matrix miniBatch)
{
	int l1 = this->L1_size;
	int l3 = this->L3_size;

	MatrixPair miniBatch_L2_L3_nodes = this->forward_Propagation(miniBatch);
	
	Matrix miniBatch_nodes_L1 = miniBatch;
	Matrix miniBatch_nodes_L2 = miniBatch_L2_L3_nodes.first;
	Matrix miniBatch_nodes_L3 = miniBatch_L2_L3_nodes.second;
	Matrix miniBatch_errors;
	for (int i=0; i<(int)miniBatch.size(); i++){
		Pointnd error(l3, 0);
		Pointnd true_val(l3, 0);
		true_val[miniBatch_nodes_L1[i][l1-1]] = 1;
		for (int j=0; j<l3; j++){
			error[j] = true_val[j] - miniBatch_nodes_L3[i][j];
		}
		miniBatch_errors.push_back(error);
		
		// Show output layer nodes' [true_value, predicted_val, error] 
		// cout<<miniBatch_nodes_L1[i][l1-1]<<nl;
		// for (int u=0;u<l3;u++){
		// 	cout<<'['<<true_val[u]<<", "<<miniBatch_nodes_L3[i][u]<<", "<<error[u]<<']'<<" ";
		// }
		// cout<<nl<<nl;
	}
	MatrixPair gradient_weights = this->back_Propagation(miniBatch_nodes_L1, miniBatch_nodes_L2,
														 miniBatch_errors);
	return gradient_weights;
}


void FFNNet3L::update_weights(int iteration, MatrixPair gradient_weights)
{
	int l1 = this->L1_size;
	int l2 = this->L2_size;
	int l3 = this->L3_size;
	int beta1 = this->beta1;
	int beta2 = this->beta2;

	Matrix gradient_mat_L1_L2 = gradient_weights.first;
	Matrix gradient_mat_L2_L3 = gradient_weights.second;

	for (int i=0; i<l1;i++){
		for (int j=0;j<l2-1;j++){
			this->first_moment_L1_L2[i][j] = beta1 * this->first_moment_L1_L2[i][j]
										+ (1-beta1) * gradient_mat_L1_L2[i][j];
			this->second_moment_L1_L2[i][j] = beta2 * this->second_moment_L1_L2[i][j]
										+ (1-beta2) * pow(gradient_mat_L1_L2[i][j], 2);
			this->bias_corr_1st_mom_L1_L2[i][j] = this->first_moment_L1_L2[i][j]  
										/ (1-pow(beta1, iteration+1));
			this->bias_corr_2nd_mom_L1_L2[i][j] = this->second_moment_L1_L2[i][j]  
										/ (1-pow(beta2, iteration+1));
			this->weights_L1_L2[i][j] += (this->alpha) * this->bias_corr_1st_mom_L1_L2[i][j]
										/ (sqrt(this->bias_corr_2nd_mom_L1_L2[i][j])+this->epsilon);
			
		}
	}

	for (int i=0; i<l2;i++){
		for (int j=0;j<l3;j++){
			this->first_moment_L2_L3[i][j] = beta1 * this->first_moment_L2_L3[i][j]
										+ (1-beta1) * gradient_mat_L2_L3[i][j];
			this->second_moment_L2_L3[i][j] = beta2 * this->second_moment_L2_L3[i][j]
										+ (1-beta2) * pow(gradient_mat_L2_L3[i][j], 2);
			this->bias_corr_1st_mom_L2_L3[i][j] = this->first_moment_L2_L3[i][j]  
										/ (1-pow(beta1, iteration+1));
			this->bias_corr_2nd_mom_L2_L3[i][j] = this->second_moment_L2_L3[i][j]  
										/ (1-pow(beta2, iteration+1));
			this->weights_L2_L3[i][j] += (this->alpha) * this->bias_corr_1st_mom_L2_L3[i][j]
										/ (sqrt(this->bias_corr_2nd_mom_L2_L3[i][j])+this->epsilon);
		}
	}
}


void FFNNet3L::apadtive_param_momentum_GD(Matrix train, int miniBatch_size)
{
	for (int i=0; i<this->iterations;i++){
		cout<<i<<nl;
		Matrix miniBatch = this->get_random_miniBatch(train, miniBatch_size);
		MatrixPair gradient_weights = this->calc_grad(miniBatch);
		update_weights(i, gradient_weights);
	}
}


void FFNNet3L::check_Accuracy(Matrix test)
{
	int l1 = this->L1_size;
	int l3 = this->L3_size;
	MatrixPair results = this->forward_Propagation(test);
	Matrix outputs = results.second;

	int corr = 0;
	for (int i=0; i<(int)outputs.size(); i++){
		int true_res = test[i][l1-1];
		int pred_res;
		double maxx = -DBL_MAX;
		for (int j=0; j<outputs[i].size(); j++){
			if (outputs[i][j] > maxx){
				maxx = outputs[i][j];
				pred_res = j;
			}
		}
		corr += (pred_res==true_res);
	}
	cout<<(double)corr/(double)outputs.size()<<nl;
}