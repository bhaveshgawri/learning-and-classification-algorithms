#include <bits/stdc++.h>
#include "Logistic_Regr.hpp"
using namespace std;


LogisticRegression::LogisticRegression(double learning_rate)
{
	this->dimensions = 4;
	this->iterations = 1000;
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


double sigmoid(Point4d w, Point4d example, int dimensions)
{
	double a = 0;
	for (int i=0;i<dimensions;i++){
		a += w[i]*example[i];
	}
	return (double)1.0 / (1.0 + exp(-a));
}


Point4d calculateGradient(Point4d w, Matrix examples, int dimensions)
{
	double sum = 0;
	Point4d grad = Point4d(dimensions, 0);
	for (auto example: examples){
		double y = sigmoid(w, example, dimensions);
		double y_t = y - example[dimensions];		
		for (int i=0;i<dimensions;i++){
			grad[i] += y_t*example[i];
		}
	}
	return grad;
}


double findError(Point4d w, Matrix examples, int dimensions)
{
	double sum = 0;
	for (auto example: examples){
		if (example[dimensions]==1){
			sum += log(sigmoid(w, example, dimensions));
		}
		else{
			sum += log(1-sigmoid(w, example, dimensions));
		}
	}
	return -sum;
}


Point4d LogisticRegression::gradientDesent(Point4d w, Matrix examples)
{
	double error;
	int iterations = this->iterations;
	int dimensions = this->dimensions;
	double lr = this->learning_rate;
	Point4d w_new = Point4d(dimensions, 0);
	Point4d grad = Point4d(dimensions, 0);

	for (int iter=0; iter<iterations; iter++){
		error = findError(w, examples, dimensions);
		cout<<"error: "<<error<<nl;
		if (error < 0.001) break;
		grad = calculateGradient(w, examples, dimensions);
		for (int i=0;i<dimensions;i++){
			w_new[i] = w[i] - lr*grad[i];
		}
		w = w_new;
	}
	return w;
}


void LogisticRegression::testAndPrint(Point4d optimal_w, Matrix testing_exp)
{
	int tp=0, fp=0, tn=0, fn=0;
	int dimensions = this->dimensions;
	for (auto example: testing_exp){
		double sigmoid_ = sigmoid(optimal_w, example, dimensions);
		bool predicted = (sigmoid_ >= 0.5);
		bool actual = example[dimensions];
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