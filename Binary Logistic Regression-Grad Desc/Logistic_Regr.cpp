#include <bits/stdc++.h>
#include "Logistic_Regr.hpp"
using namespace std;


LogisticRegression::LogisticRegression(double learning_rate, int iterations)
{
	this->features = 4;
	this->iterations = iterations;
	this->learning_rate = learning_rate;
}


Matrix LogisticRegression::readFromInputFile(string input_file)
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


double sigmoid(double bias, Point4d weights, Point4d example, int features)
{
	double a = bias;
	for (int i=0;i<features;i++){
		a += weights[i]*example[i];
	}
	return (double)1.0 / (1.0 + exp(-a));
}


Point5d calculateGradient(double bias, Point4d weights, Matrix examples, int features)
{
	double sum = 0;
	double bias_grad = 0;
	Point4d grad = Point4d(features, 0);
	for (auto example: examples){
		double y = sigmoid(bias, weights, example, features);
		double y_minus_t = y - example[features];		
		bias_grad += y_minus_t;
		for (int i=0;i<features;i++){
			grad[i] += y_minus_t*example[i];
		}
	}
	return {bias_grad, grad};
}


double findError(double bias, Point4d weights, Matrix examples, int features)
{
	double error = 0;
	for (auto example: examples){
		if (example[features]==1){
			error += log(sigmoid(bias, weights, example, features));
		}
		else{
			error += log(1-sigmoid(bias, weights, example, features));
		}
	}
	return -error;
}


Point5d LogisticRegression::gradientDesent(double bias, Point4d weights, Matrix examples)
{
	int iterations = this->iterations;
	int features = this->features;
	double lr = this->learning_rate;
	
	for (int iter=0; iter<iterations; iter++){
		double error = findError(bias, weights, examples, features);
		cout<<"error: "<<error<<nl;
		
		Point5d grad = calculateGradient(bias, weights, examples, features);
		bias = bias - lr*grad.first;
		for (int i=0;i<features;i++){
			weights[i] = weights[i] - lr*grad.second[i];
		}
	}
	return {bias, weights};
}


void LogisticRegression::testAndPrint(Point5d optimal_weights, Matrix testing_exp)
{
	int tp=0, fp=0, tn=0, fn=0;
	int features = this->features;
	for (auto example: testing_exp){
		double bias = optimal_weights.first;
		Point4d weights = optimal_weights.second;
		double sigmoid_ = sigmoid(bias, weights, example, features);
		bool predicted = (sigmoid_ >= 0.5);
		bool actual = example[features];
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