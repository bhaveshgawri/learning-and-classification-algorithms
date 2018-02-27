#ifndef LDA_H
#define LDA_H
#include <bits/stdc++.h>
using namespace std;

#define nl '\n'

typedef vector<pair<double, bool>> Points1d;
typedef vector<double> Point4d;
typedef vector<Point4d> Matrix;

class LDA
{
private:
	int dimensions;
public:
	LDA();
	Matrix readFromInputFile(string);
	Point4d mean(Matrix, bool);
	Matrix calculateSw(Matrix, Point4d, Point4d);
	Matrix calculateSwInverse(Matrix);
	Point4d calculateW(Matrix, Point4d, Point4d);
	Points1d get1DPoints(Point4d, Matrix);
	double calculate1dThreshold(Points1d);
	void testAndPrint(double, Points1d);
};

#endif