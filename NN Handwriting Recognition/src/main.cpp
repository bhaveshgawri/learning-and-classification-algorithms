#include <bits/stdc++.h>
#include "NN_handwriting.hpp"
using namespace std;

#define L2_size 10
#define iterations 500

int main(){
	string train = "../data/train.txt";
	string test = "../data/test.txt";
	string validation = "../data/validation.txt";
	
	FFNNet3L nn(L2_size, iterations);
	Matrix training_exp = nn.readFromInputFile(train);
	Matrix validation_exp = nn.readFromInputFile(validation);
	Matrix testing_exp = nn.readFromInputFile(test);

	nn.initialize_random_weights();
	nn.apadtive_param_momentum_GD(training_exp, 100);
	nn.check_Accuracy(testing_exp);
}