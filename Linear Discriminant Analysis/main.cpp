#include <bits/stdc++.h>
#include "LDA.hpp"
using namespace std;

#define dimensions 4

int main(){
	string train = "./data/train.txt";
	string test = "./data/test.txt";
	LDA lda;
	Matrix training_exp = lda.readFromInputFile(train);
	Matrix test_exp = lda.readFromInputFile(test);
	
	Point4d means_p = lda.mean(training_exp, 1);
	Point4d means_n = lda.mean(training_exp, 0);
	
	Matrix Sw = lda.calculateSw(training_exp, means_p, means_n);
	Matrix SwInverse = lda.calculateSwInverse(Sw);
	
	Point4d w = lda.calculateW(SwInverse, means_p, means_n);
	Points1d examples1d = lda.get1DPoints(w, training_exp);
	sort(examples1d.begin(), examples1d.end());
	
	double threshold_1d = lda.calculate1dThreshold(examples1d);
	// 0 if less than threshold

	Points1d testing_points = lda.get1DPoints(w, test_exp);
	lda.testAndPrint(threshold_1d, testing_points);
}