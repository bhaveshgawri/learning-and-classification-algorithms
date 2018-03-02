#include <bits/stdc++.h>
#include "Logistic_Regr.hpp"
using namespace std;

#define features 4
#define learning_rate 0.003
#define iterations 8000

int main(){
	string train = "./data/train.txt";
	string test = "./data/test.txt";

	LogisticRegression lr(learning_rate, iterations);
	Matrix training_exp = lr.readFromInputFile(train);
	Matrix test_exp = lr.readFromInputFile(test);

	double bias = 1;
	Point4d weights = Point4d(features, 1);
	Point5d optimal_weights = lr.gradientDesent(bias, weights, training_exp);
	
	lr.testAndPrint(optimal_weights, test_exp);
}