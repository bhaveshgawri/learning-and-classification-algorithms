#ifndef PROBABILISTIC_GEN_MODEL_BIN_CLASSIFICATION
#define PROBABILISTIC_GEN_MODEL_BIN_CLASSIFICATION
#include <bits/stdc++.h>
using namespace std;

#define nl '\n'

typedef vector<double> Point4d;
typedef vector<Point4d> Matrix;
/*
Point4d: Stores a data point having 4 features.
                To store a specific example.

Matrix:  Stores multiple Point4d.
                To store training and test examples.
                Although it contains Points4d, their size is
                modified to 5 where 5th position contains class
                label of that data point.
*/

class ProbGenModelMLE
{
private:
	int features;
	int C1_train_exp;
	int C2_train_exp;
public:
	ProbGenModelMLE();
	Matrix readFromInputFile(string);
	void getPosNegCount(Matrix);
	double calculatePrior(bool);
	Point4d calculateMean(Matrix, bool);
	Matrix getCovarienceMatrix(Matrix, Point4d, Point4d);
	Matrix calculateInverse4x4(Matrix);
	void testAndPrint(Matrix, Matrix, double, Point4d, double, Point4d);	
};

#endif