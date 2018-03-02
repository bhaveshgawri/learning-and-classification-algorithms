#include <bits/stdc++.h>
#include "Probalistic_Generative_Model_Binary_Classification.hpp"
using namespace std;


ProbGenModelMLE::ProbGenModelMLE()
{
	this->features = 4;
	this->C1_train_exp;
	this->C2_train_exp;
}

/*
Read input from the input file.
*/
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

/*
Calculate and store the counts of the two classes:
C1 and C2.
*/
void ProbGenModelMLE::getPosNegCount(Matrix examples)
{
	int C1 = 0;
	int C2 = 0;
	for (auto example: examples){
		if (example[this->features] == 1){
			C1++;
		}
		else{
			C2++;
		}
	}
	this->C1_train_exp = C1;
	this->C2_train_exp = C2;
}

/*
Calculate prior probabilities of the classes.
*/
double ProbGenModelMLE::calculatePrior(bool class_label)
{
	double probability;
	int total_exp = this->C1_train_exp + this->C2_train_exp;
	if (class_label==1){
		probability = (double)this->C1_train_exp / total_exp;
	} 
	else{
		probability = (double)this->C2_train_exp / total_exp;
	}
	return probability;
}

/*
Calculate the mean of the class whose label is given by 
class_label.
*/
Point4d ProbGenModelMLE::calculateMean(Matrix examples, bool class_label)
{
	Point4d means(this->features, 0);
	for (auto example: examples){
		if (example[this->features]==class_label){
			for (int feature=0; feature < (this->features);feature++){
				means[feature] += example[feature];
			}
		}
	}
	for (int feature=0; feature < (this->features);feature++){
		if (class_label)
			means[feature] /= this->C1_train_exp;
		else
			means[feature] /= this->C2_train_exp;
	}
	return means;
}

/*
Calculate class specific covarience matrix. 
*/
Matrix calculateS_1class(Matrix examples, Point4d mean, bool class_label,
						int train_exp_cnt, int features)
{
	Matrix S = Matrix(features, Point4d(features, 0.0));
	for (auto example: examples){
		Point4d pt = example;
		if (example[features] == class_label){
			for (int feature=0;feature<features;feature++){
				pt[feature] -= mean[feature];
			}
			for (int i=0;i<features;i++){
				for (int j=0;j<features;j++){
					S[i][j] += pt[i]*pt[j];
				}
			}
		}
	}
	for (int i=0;i<features;i++){
		for (int j=0;j<features;j++){
			S[i][j] /= train_exp_cnt;
		}
	}
	return S;
}

/*
Calculate the covarience matrix of the data from 
weighted average of class specific covarience matrices.
*/
Matrix calculateS(Matrix S1, int N1, Matrix S2, int N2,
				int features)
{
	Matrix S = Matrix(features, Point4d(features, 0.0));
	int N = N1 + N2;
	double C1_ratio = (double)N1/N;
	double C2_ratio = 1 - C1_ratio;

	for (int i=0;i<features;i++){
		for (int j=0;j<features;j++){
			S[i][j] = (C1_ratio*S1[i][j] + C2_ratio*S2[i][j]);
		}
	}
	return S;
}

/*
Calculate and return Covarince matrix of given data
with classes having some given means.
*/
Matrix ProbGenModelMLE::getCovarienceMatrix(Matrix training_exp,
								 Point4d mean_C1, Point4d mean_C2)
{

	Matrix S1 = calculateS_1class(training_exp, mean_C1, 1, this->C1_train_exp, features);
	Matrix S2 = calculateS_1class(training_exp, mean_C2, 0, this->C2_train_exp, features);
	Matrix S = calculateS(S1, this->C1_train_exp, S2, this->C2_train_exp, features);
	
	return S;
}

/*
Utility function to calculate inverse of a matrix.

Source: https://stackoverflow.com/a/23806710
*/
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

/*
Returns the inverse of the 4x4 matrix passed as an 1d array
of size 16. Output is stored in 1d-array named out(size: 16)

Source: https://stackoverflow.com/a/23806710
*/
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

/*
Calculate the inverse of covarience matrix.
*/
Matrix ProbGenModelMLE::calculateInverse4x4(Matrix CovarienceMatrix)
{
	Matrix covarience_matrix_inv = Matrix(this->features, Point4d(this->features, 0.0));
	double m[16], out[16];
	for (int i=0, k=0;i<this->features;i++){
		for (int j=0;j<this->features;j++){
			m[k++] = CovarienceMatrix[i][j];
		}
	}
	bool b = inverseMatrix4x4(m, out);
	if (!b) return covarience_matrix_inv;
	else{
		for (int i=0, k=0;i<this->features;i++){
			for (int j=0;j<this->features;j++){
				covarience_matrix_inv[i][j] = out[k++];
			}
		}
	}
	return covarience_matrix_inv;
}

/*
Returns the product of 3 matrices of sizes as:
   [1x4]   *  [4x4]	 *   [4x1] = a 1d value
(x-mean.T) * Cov_inv * (x-mean)
*/
double multiply(Point4d example, Point4d mean,
 				Matrix convarence_inv, int features)
{
	Point4d x_minus_mean(features, 0);
	double product = 0;
	for (int i=0;i<features;i++){
		x_minus_mean[i] = example[i] - mean[i];
	}
	for (int i=0;i<features;i++){
		double temp = 0;
		for (int j=0;j<features;j++){
			temp += x_minus_mean[j]*convarence_inv[j][i];
		}
		product += temp*x_minus_mean[i];
	}
	return product;
}

/*
Predict the class for a new input point.
*/
bool predict(Point4d example, Matrix covarience_matrix_inv,
			double prior_C1, Point4d mean_C1, 
			double prior_C2, Point4d mean_C2, int features)
{
	double C1 = log(prior_C1) - 0.5*multiply(example, mean_C1,
									covarience_matrix_inv, features);
	double C2 = log(prior_C2) - 0.5*multiply(example, mean_C2,
									covarience_matrix_inv, features);
	if (C1 >= C2) return true;
	else return false;
}

/*
Calculate and print Accuracy, Precision and Recall
*/
void ProbGenModelMLE::testAndPrint(Matrix test_examples, 
					Matrix covarience_matrix_inv,
					double prior_C1, Point4d mean_C1, 
					double prior_C2, Point4d mean_C2)
{
	int tp=0, fp=0, tn=0, fn=0;
	for (auto example: test_examples){
		bool predicted = predict(example, covarience_matrix_inv,
						prior_C1, mean_C1, prior_C2, mean_C2, features);
		bool actual = example[this->features];
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
	cout<<nl<<nl<<"Confusion Matrix:"<<nl;
	cout<<"\t\t"<<"Predicted: 0\tPredicted: 1"<<nl;
	cout<<"Actual: 0\t"<<tn<<"\t\t"<<fp<<nl;
	cout<<"Actual: 1\t"<<fn<<"\t\t"<<tp<<nl;
}
