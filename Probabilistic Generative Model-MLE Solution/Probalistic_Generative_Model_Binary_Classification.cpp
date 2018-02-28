#include <bits/stdc++.h>
#include "Probalistic_Generative_Model_Binary_Classification.hpp"
using namespace std;


ProbGenModelMLE::ProbGenModelMLE()
{
	this->dimensions = 4;
	this->pos_train_exp;
	this->neg_train_exp;
}


Matrix ProbGenModelMLE::readFromInputFile(string input_file)
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


freq_pair ProbGenModelMLE::getPosNegCount(Matrix examples)
{
	int pos = 0;
	int neg = 0;
	for (auto example: examples){
		if (example[this->dimensions] == 1){
			pos++;
		}
		else{
			neg++;
		}
	}
	this->pos_train_exp = pos;
	this->neg_train_exp = neg;
	return {pos, neg};
}


double ProbGenModelMLE::calculatePrior(bool class_label)
{
	double probability;
	int total_exp = this->pos_train_exp + this->neg_train_exp;
	if (class_label==1){
		probability = (double)this->pos_train_exp / total_exp;
	} 
	else{
		probability = (double)this->neg_train_exp / total_exp;
	}
	return probability;
}


Point4d ProbGenModelMLE::calculateMean(Matrix examples, bool class_label)
{
	Point4d means(this->dimensions, 0);
	for (auto example: examples){
		if (example[this->dimensions]==class_label){
			for (int dim=0; dim < (this->dimensions);dim++){
				means[dim] += example[dim];
			}
		}
	}
	for (int dim=0; dim < (this->dimensions);dim++){
		if (class_label)
			means[dim] /= this->pos_train_exp;
		else
			means[dim] /= this->neg_train_exp;
	}
	return means;
}


Matrix ProbGenModelMLE::calculateS_1class(Matrix examples, Point4d mean, bool class_label)
{
	Matrix S = Matrix(this->dimensions, Point4d(this->dimensions, 0.0));
	for (auto example: examples){
		Point4d pt = example;
		if (example[this->dimensions] == class_label){
			for (int dim=0;dim<this->dimensions;dim++){
				pt[dim] -= mean[dim];
			}
			for (int i=0;i<this->dimensions;i++){
				for (int j=0;j<this->dimensions;j++){
					S[i][j] += pt[i]*pt[j];
				}
			}
		}
	}
	for (int i=0;i<this->dimensions;i++){
		for (int j=0;j<this->dimensions;j++){
			if (class_label)
				S[i][j] /= this->pos_train_exp;
			else
				S[i][j] /= this->neg_train_exp;
		}
	}
	return S;
}


Matrix ProbGenModelMLE::calculateS(Matrix S1, int N1, Matrix S2, int N2)
{
	Matrix S = Matrix(this->dimensions, Point4d(this->dimensions, 0.0));
	int N = N1 + N2;
	double pos_ratio = (double)N1/N;
	double neg_ratio = 1 - pos_ratio;

	for (int i=0;i<this->dimensions;i++){
		for (int j=0;j<this->dimensions;j++){
			S[i][j] = (pos_ratio*S1[i][j] + neg_ratio*S2[i][j]);
		}
	}
	return S;
}


Matrix ProbGenModelMLE::getCovarienceMatrix(Matrix S)
{
	return S;
}


double invf(int i,int j,const double* m)
{
	int o = 2+(j-i);
	i += 4+o;
	j += 4-o;

	#define e(a,b) m[ ((j+b)%4)*4 + ((i+a)%4) ]
	double inv =
	 + e(+1,-1)*e(+0,+0)*e(-1,+1)
	 + e(+1,+1)*e(+0,-1)*e(-1,+0)
	 + e(-1,-1)*e(+1,+0)*e(+0,+1)
	 - e(-1,-1)*e(+0,+0)*e(+1,+1)
	 - e(-1,+1)*e(+0,-1)*e(+1,+0)
	 - e(+1,-1)*e(-1,+0)*e(+0,+1);

	return (o%2)?inv : -inv;
	#undef e
}


bool inverseMatrix4x4(const double *m, double *out)
{

	double inv[16];

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			inv[j*4+i] = invf(i,j,m);

	double D = 0;

	for(int k=0;k<4;k++) D += m[k] * inv[k*4];

	if (D == 0) return false;

	D = 1.0 / D;

	for (int i = 0; i < 16; i++)
		out[i] = inv[i] * D;

	return true;
}


Matrix ProbGenModelMLE::calculateInverse4x4(Matrix CovarienceMatrix)
{
	Matrix covarience_matrix_inv = Matrix(this->dimensions, Point4d(this->dimensions, 0.0));
	double m[16], out[16];
	for (int i=0, k=0;i<this->dimensions;i++){
		for (int j=0;j<this->dimensions;j++){
			m[k++] = CovarienceMatrix[i][j];
		}
	}
	bool b = inverseMatrix4x4(m, out);
	if (!b) return covarience_matrix_inv;
	else{
		for (int i=0, k=0;i<this->dimensions;i++){
			for (int j=0;j<this->dimensions;j++){
				covarience_matrix_inv[i][j] = out[k++];
			}
		}
	}
	return covarience_matrix_inv;
}


double multiply(Point4d example, Point4d mean,
 				Matrix convarence_inv, int dimensions)
{
	Point4d x_minus_mean(dimensions, 0);
	double product = 0;
	for (int i=0;i<dimensions;i++){
		x_minus_mean[i] = example[i] - mean[i];
	}
	for (int i=0;i<dimensions;i++){
		double temp = 0;
		for (int j=0;j<dimensions;j++){
			temp += x_minus_mean[i]*convarence_inv[j][i];
		}
		product += temp*x_minus_mean[i];
	}
	return product;
}


bool predict(Point4d example, Matrix covarience_matrix_inv,
			double prior_p, Point4d mean_p, 
			double prior_n, Point4d mean_n, int dimensions)
{
	double pos = log(prior_p) - 0.5*multiply(example, mean_p,
									covarience_matrix_inv, dimensions);
	double neg = log(prior_n) - 0.5*multiply(example, mean_n,
									covarience_matrix_inv, dimensions);
	if (pos >= neg) return true;
	else return false;
}


void ProbGenModelMLE::testAndPrint(Matrix test_examples, 
					Matrix covarience_matrix_inv,
					double prior_p, Point4d mean_p, 
					double prior_n, Point4d mean_n)
{
	int tp=0, fp=0, tn=0, fn=0;
	for (auto example: test_examples){
		bool predicted = predict(example, covarience_matrix_inv,
						prior_p, mean_p, prior_n, mean_n, dimensions);
		bool actual = example[this->dimensions];
		if (!predicted && !actual) tn++;
		if (!predicted && actual) fn++;
		if (predicted && actual) tp++;
		if (predicted && !actual) fp++;
	}
	cout<<"True Positives:  "<<tp<<nl;
	cout<<"True Negatives:  "<<tn<<nl;
	cout<<"False Positives: "<<fp<<nl;
	cout<<"False Negatives: "<<fn<<nl;
	cout<<nl;
	cout<<"Accuracy:  "<<(double)(tp+tn)/(tp+fp+tn+fn)<<nl;
	cout<<"Precision: "<<(double)(tp)/(tp+fp)<<nl;
	cout<<"Recall:    "<<(double)(tp)/(tp+fn)<<nl;
}