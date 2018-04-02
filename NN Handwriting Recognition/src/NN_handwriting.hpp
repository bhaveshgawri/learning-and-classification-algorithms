#ifndef NN_HANNDWRITING_H
#define NN_HANNDWRITING_H
#include <bits/stdc++.h>
using namespace std;

#define nl '\n'


typedef vector<double> Pointnd;
typedef vector<Pointnd> Matrix;
typedef pair<Matrix, Matrix> MatrixPair;

class FFNNet3L
{
private:
	int L1_size;
	int L2_size;
	int L3_size;
	
	int iterations;
	double validation_acc_thresh;

	double alpha;
	double beta1;
	double beta2;
	double epsilon;

	Matrix weights_L1_L2;
	Matrix weights_L2_L3;

	Matrix first_moment_L1_L2;
	Matrix first_moment_L2_L3;
	Matrix bias_corr_1st_mom_L1_L2;
	Matrix bias_corr_1st_mom_L2_L3;
	
	Matrix second_moment_L1_L2;
	Matrix second_moment_L2_L3;
	Matrix bias_corr_2nd_mom_L1_L2;
	Matrix bias_corr_2nd_mom_L2_L3;

public:
	FFNNet3L(int, int, double);
	Matrix readFromInputFile(string);
	
	void initialize_random_weights();
	void printweights();
	
	Matrix get_random_miniBatch(Matrix, int);
	MatrixPair forward_Propagation(Matrix);
	MatrixPair back_Propagation(Matrix, Matrix, Matrix);
	void update_weights(int, MatrixPair);
	MatrixPair calc_grad(Matrix);

	void apadtive_param_momentum_GD(Matrix, Matrix, Matrix, int);
	double check_Accuracy(Matrix);
};

#endif