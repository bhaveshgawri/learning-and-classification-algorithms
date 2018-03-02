#ifndef LDA_H
#define LDA_H
#include <bits/stdc++.h>
using namespace std;

#define nl '\n'

typedef vector<pair<double, bool>> Points1d;
typedef vector<double> Point4d;
typedef vector<Point4d> Matrix;
/*
Points1D: Stores multiple 1d points
				To store examples projected on the line
				which maximizes mean seperation b/w classes
				and minimizes varience of each class.

Point4d: Stores a data point having 4 features.
                To store a specific example.

Matrix:  Stores multiple Point4d.
                To store training and test examples.
                Although it contains Points4d, their size is
                modified to 5 where 5th position contains class
                label of that data point.
*/

class LDA
{
private:
	int features;
public:
	LDA();
	Matrix readFromInputFile(string);
	Point4d mean(Matrix, bool);
	Matrix calculateSw(Matrix, Point4d, Point4d);
	Matrix calculateSwInverse(Matrix);
	Point4d calculateWeights(Matrix, Point4d, Point4d);
	Points1d get1DPoints(Point4d, Matrix);
	double calculate1dThreshold(Points1d);
	void testAndPrint(double, Points1d);
};

#endif