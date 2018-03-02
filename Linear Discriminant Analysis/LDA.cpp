#include <bits/stdc++.h>
#include "LDA.hpp"
using namespace std;


LDA::LDA()
{
	this->features = 4;
}


/*
Read input from the input file.
*/
Matrix LDA::readFromInputFile(string input_file)
{
	Matrix examples;
	string line;
	ifstream infile(input_file);
	while(getline(infile, line)){
		if (line.length()==0) continue;
		stringstream ss(line);
		string word;
		vector<double>tmp;
		while(getline(ss, word, ',')){
			double value = stod(word);
			tmp.push_back(value);
		}
		examples.push_back(tmp);
	}
	return examples;
}


/*
Calculate the mean of the class whose label is given by 
class_label.
*/
Point4d LDA::mean(Matrix examples, bool class_label)
{
	Point4d means(this->features, 0);
	int exp_size = 0;
	for (auto example: examples){
		if (example[this->features]==class_label){
			for (int feature=0; feature < (this->features);feature++){
				means[feature] += example[feature];
			}
			exp_size++;
		}
	}
	for (int feature=0; feature < (this->features);feature++){
		means[feature] /= exp_size;
	}
	return means;
}


/*
Calculate total 'within-class' covarience matrix Sw.
*/
Matrix LDA::calculateSw(Matrix examples, Point4d mean_C1, Point4d mean_C2)
{
	Matrix Sw = Matrix(this->features, Point4d(this->features, 0.0));
	for (auto example: examples){
		Point4d pt = example;
		if (example[this->features] == 1.0){
			for (int feature=0;feature<this->features;feature++){
				pt[feature] -= mean_C1[feature];
			}
		}
		else{
			for (int feature=0;feature<this->features;feature++){
				pt[feature] -= mean_C2[feature];
			}
		}
		for (int i=0;i<this->features;i++){
			for (int j=0;j<this->features;j++){
				Sw[i][j] += pt[i]*pt[j];
			}
		}
	}
	return Sw;
}


/*
Utility function to calculate inverse of a matrix.

Source: https://stackoverflow.com/a/23806710
*/
double invf(int i,int j,const double* m)
{
	int o = 2+(j-i);
	i += 4+o;
	j += 4-o;

	#define e(a,b) m[ ((j+b)%4)*4 + ((i+a)%4) ]
	double inv =
	 + e(+1,-1)*e(+0,+0)*e(-1,+1)
	 + e(+1,+1)*e(+0,-1)*e(-1,+0)
	 + e(-1,-1)*e(+1,+0)*e(+0,+1)
	 - e(-1,-1)*e(+0,+0)*e(+1,+1)
	 - e(-1,+1)*e(+0,-1)*e(+1,+0)
	 - e(+1,-1)*e(-1,+0)*e(+0,+1);

	return (o%2)?inv : -inv;
	#undef e
}


/*
Returns the inverse of the 4x4 matrix passed as an 1d array
of size 16. Output is stored in 1d-array named out(size: 16)

Source: https://stackoverflow.com/a/23806710
*/
bool inverseMatrix4x4(const double *m, double *out)
{

	double inv[16];

	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			inv[j*4+i] = invf(i,j,m);

	double D = 0;

	for(int k=0;k<4;k++) D += m[k] * inv[k*4];

	if (D == 0) return false;

	D = 1.0 / D;

	for (int i = 0; i < 16; i++)
		out[i] = inv[i] * D;

	return true;
}


/*
Calculate inverse of 'within-class' covarience matrix Sw.
*/
Matrix LDA::calculateSwInverse(Matrix Sw)
{
	Matrix SwInverse = Matrix(this->features, Point4d(this->features, 0.0));
	double m[16], out[16];
	for (int i=0, k=0;i<this->features;i++){
		for (int j=0;j<this->features;j++){
			m[k++] = Sw[i][j];
		}
	}
	bool b = inverseMatrix4x4(m, out);
	if (!b) return SwInverse;
	else{
		for (int i=0, k=0;i<this->features;i++){
			for (int j=0;j<this->features;j++){
				SwInverse[i][j] = out[k++];
			}
		}
	}
	return SwInverse;
}


/*
Calculate weight vector and normalize it.
*/
Point4d LDA::calculateWeights(Matrix SwInverse, Point4d mean_C1, Point4d mean_C2)
{
	Point4d weights = Point4d(this->features, 0);
	Point4d mean_diff = Point4d(this->features, 0);
	for (int i=0;i<this->features;i++){
		mean_diff[i] = mean_C1[i] - mean_C2[i];
	}
	for (int i=0;i<this->features;i++){
		for (int j=0;j<this->features;j++){
			weights[i] += SwInverse[i][j]*mean_diff[j];
		}
	}

	// normalize weights
	double sum = 0;
	for (int i=0;i<this->features;i++){
		sum += weights[i]*weights[i];
	}
	for (int i=0;i<this->features;i++){
		weights[i] /= sqrt(sum);
	}

	return weights;
}


/*
Project testing examples to 1d by multiplying w.T * example
while maintaining the label of points.
*/
Points1d LDA::get1DPoints(Point4d w, Matrix examples)
{
	Points1d examples1d;
	for (auto example: examples){
		pair<double, bool> temp = {0, 0};
		for (int feature=0;feature<this->features;feature++){
			 temp.first += w[feature]*example[feature];
		}
		temp.second = example[this->features];
		examples1d.push_back(temp);
	}
	return examples1d;
}


/*
Calculate entropy of given points about a specific index.
*/
double calculateEntropy(int index, Points1d examples1d)
{
	double C1=0, C2=0;
	double entropy_left=0, entropy_right=0;
	for (int i=0;i<=index;i++){
		if (examples1d[i].second) C1++;
		else C2++;
	}
	C1 /= (C1+C2);
	C2 /= (C1+C2);
	if (C1!=0) entropy_left += -C1*log2(C1);
	if (C2!=0) entropy_left += -C2*log2(C2);

	C1 = C2 = 0;
	for (int i=index+1;i<examples1d.size();i++){
		if (examples1d[i].second) C1++;
		else C2++;
	}
	C1 /= (C1+C2);
	C2 /= (C1+C2);
	if (C1!=0) entropy_right += -C1*log2(C1);
	if (C2!=0) entropy_right += -C2*log2(C2);

	return (entropy_left + entropy_right)/2;
}

/*
Calculate threshold to seperate the classes.
1d points having value greater than threshold would
belong to one class and less than that to other class.
*/
double LDA::calculate1dThreshold(Points1d examples1d)
{
	double min_entropy = DBL_MAX;
	double threshold = -1;
	for (int i=0;i<examples1d.size()-1;i++){
		double temp_threshold = (examples1d[i].first+examples1d[i+1].first)/2;
		double new_entropy = calculateEntropy(i, examples1d);
		if (abs(new_entropy) < min_entropy){
			threshold = temp_threshold;
			min_entropy = abs(new_entropy);
		}
	}
	return threshold;
}

/*
Calculate and print Accuracy, Precision and Recall
*/
void LDA::testAndPrint(double threshold_1d, Points1d testing_points)
{
	int tp=0, fp=0, tn=0, fn=0;
	for (auto point: testing_points){
		if (point.first < threshold_1d && !point.second) tn++;
		if (point.first < threshold_1d && point.second) fn++;
		if (point.first > threshold_1d && point.second) tp++;
		if (point.first > threshold_1d && !point.second) fp++;
	}
	cout<<"True Positives:  "<<tp<<nl;
	cout<<"True Negatives:  "<<tn<<nl;
	cout<<"False Positives: "<<fp<<nl;
	cout<<"False Negatives: "<<fn<<nl;
	cout<<nl;
	cout<<"Accuracy:  "<<(double)(tp+tn)/(tp+fp+tn+fn)<<nl;
	cout<<"Precision: "<<(double)(tp)/(tp+fp)<<nl;
	cout<<"Recall:    "<<(double)(tp)/(tp+fn)<<nl;
	cout<<nl<<nl<<"Confusion Matrix:"<<nl;
	cout<<"\t\t"<<"Predicted: 0\tPredicted: 1"<<nl;
	cout<<"Actual: 0\t"<<tn<<"\t\t"<<fp<<nl;
	cout<<"Actual: 1\t"<<fn<<"\t\t"<<tp<<nl;
}