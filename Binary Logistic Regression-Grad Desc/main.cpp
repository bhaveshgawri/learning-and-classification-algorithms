#include <bits/stdc++.h>
#include "Logistic_Regr.hpp"
using namespace std;

#define dimensions 4
#define learning_rate 0.001

int main(){
	string train = "./data/train.txt";
	string test = "./data/test.txt";

	LogisticRegression lr(learning_rate);
	Matrix training_exp = lr.readFromInputFile(train);
	Matrix test_exp = lr.readFromInputFile(test);

	Point4d w = Point4d(dimensions, 1);
	Point4d optimal_w = lr.gradientDesent(w, training_exp);
	for (int i=0;i<dimensions;i++) cout<<optimal_w[i]<<" ";cout<<nl;

	lr.testAndPrint(optimal_w, test_exp);
}