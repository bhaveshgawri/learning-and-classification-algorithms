#include <bits/stdc++.h>
#include "Probalistic_Generative_Model_Binary_Classification.cpp"
using namespace std;

#define dimensions 4

int main(){
	string train = "./data/train.txt";
	string test = "./data/test.txt";

	ProbGenModelMLE pgmMLE;

	Matrix training_exp = pgmMLE.readFromInputFile(train);
	Matrix testing_exp = pgmMLE.readFromInputFile(test);

	pgmMLE.getPosNegCount(training_exp);

	double prior_C1 = pgmMLE.calculatePrior(1);
	double prior_C2 = pgmMLE.calculatePrior(0);
	
	Point4d mean_C1 = pgmMLE.calculateMean(training_exp, 1);
	Point4d mean_C2 = pgmMLE.calculateMean(training_exp, 0);

	Matrix covarience_matrix = pgmMLE.getCovarienceMatrix(training_exp, mean_C1, mean_C2);
	Matrix covarience_matrix_inv = pgmMLE.calculateInverse4x4(covarience_matrix);

	pgmMLE.testAndPrint(testing_exp, covarience_matrix_inv, 
						prior_C1, mean_C1, prior_C2, mean_C2);	
}
