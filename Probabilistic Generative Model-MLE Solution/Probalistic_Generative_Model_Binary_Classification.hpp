#ifndef PROBABILISTIC_GEN_MODEL_BIN_CLASSIFICATION
#define PROBABILISTIC_GEN_MODEL_BIN_CLASSIFICATION
#include <bits/stdc++.h>
using namespace std;

#define nl '\n'

typedef pair<int, int> freq_pair;
typedef vector<pair<double, bool>> Points1d;
typedef vector<double> Point4d;
typedef vector<Point4d> Matrix;

class ProbGenModelMLE
{
private:
	int dimensions;
	int pos_train_exp;
	int neg_train_exp;
public:
	ProbGenModelMLE();
	Matrix readFromInputFile(string);
	freq_pair getPosNegCount(Matrix);
	double calculatePrior(bool);
	Point4d calculateMean(Matrix, bool);
	Matrix calculateS_1class(Matrix, Point4d, bool);
	Matrix calculateS(Matrix, int, Matrix, int);
	Matrix getCovarienceMatrix(Matrix);
	Matrix calculateInverse4x4(Matrix);
	void testAndPrint(Matrix, Matrix, double, Point4d, double, Point4d);	
};

#endif
