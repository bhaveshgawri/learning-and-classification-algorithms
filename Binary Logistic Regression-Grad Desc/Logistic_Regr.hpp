#ifndef LOGISTIC_REGR
#define LOGISTIC_REGR
#include <bits/stdc++.h>
using namespace std;

#define nl '\n'

typedef vector<double> Point4d;
typedef pair<double, vector<double>> Point5d;
typedef vector<Point4d> Matrix;

class LogisticRegression
{
private:
	int features;
	int iterations;
	double learning_rate;
public:
	LogisticRegression(double, int);
	Matrix readFromInputFile(string);
	Point5d gradientDesent(double, Point4d, Matrix);
	void testAndPrint(Point5d, Matrix);
};

#endif