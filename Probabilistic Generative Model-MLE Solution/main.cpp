#include <bits/stdc++.h>
#include "Probalistic_Generative_Model_Binary_Classification.hpp"
using namespace std;

#define dimensions 4

int main(){
	string train = "./data/train.txt";
	string test = "./data/test.txt";

	ProbGenModelMLE pgmMLE;

	Matrix training_exp = pgmMLE.readFromInputFile(train);
	Matrix testing_exp = pgmMLE.readFromInputFile(test);

	freq_pair exp_count = pgmMLE.getPosNegCount(training_exp);

	double prior_p = pgmMLE.calculatePrior(1);
	double prior_n = pgmMLE.calculatePrior(0);
	
	Point4d mean_p = pgmMLE.calculateMean(training_exp, 1);
	Point4d mean_n = pgmMLE.calculateMean(training_exp, 0);

	Matrix S1 = pgmMLE.calculateS_1class(training_exp, mean_p, 1);
	Matrix S2 = pgmMLE.calculateS_1class(training_exp, mean_n, 0);
	Matrix S = pgmMLE.calculateS(S1, exp_count.first, S2, exp_count.second);

	Matrix covarience_matrix = pgmMLE.getCovarienceMatrix(S);
	Matrix covarience_matrix_inv = pgmMLE.calculateInverse4x4(covarience_matrix);

	pgmMLE.testAndPrint(testing_exp, covarience_matrix_inv, 
						prior_p, mean_p, prior_n, mean_n);	
}