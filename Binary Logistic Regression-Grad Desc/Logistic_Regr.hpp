#ifndef LOGISTIC_REGR
#define LOGISTIC_REGR
#include <bits/stdc++.h>
using namespace std;

#define nl '\n'

typedef vector<pair<double, bool>> Points1d;
typedef vector<double> Point4d;
typedef vector<Point4d> Matrix;

class LogisticRegression
{
private:
	int dimensions;
	int iterations;
	double learning_rate;
public:
	LogisticRegression(double);
	Matrix readFromInputFile(string);
	Point4d gradientDesent(Point4d, Matrix);
	void testAndPrint(Point4d, Matrix);
};

#endif