#include <bits/stdc++.h>
#include "LDA.hpp"
using namespace std;

LDA::LDA()
{
	this->dimensions = 4;
}


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


Point4d LDA::mean(Matrix examples, bool positive)
{
	Point4d means(this->dimensions, 0);
	int exp_size = 0;
	for (auto example: examples){
		if (example[this->dimensions]==positive){
			for (int dim=0; dim < (this->dimensions);dim++){
				means[dim] += example[dim];
			}
			exp_size++;
		}
	}
	for (int dim=0; dim < (this->dimensions);dim++){
		means[dim] /= exp_size;
	}
	return means;
}


Matrix LDA::calculateSw(Matrix examples, Point4d mean_p, Point4d mean_n)
{
	Matrix Sw = Matrix(this->dimensions, Point4d(this->dimensions, 0.0));
	for (auto example: examples){
		Point4d pt = example;
		if (example[this->dimensions] == 1.0){
			for (int dim=0;dim<this->dimensions;dim++){
				pt[dim] -= mean_p[dim];
			}
		}
		else{
			for (int dim=0;dim<this->dimensions;dim++){
				pt[dim] -= mean_n[dim];
			}
		}
		for (int i=0;i<this->dimensions;i++){
			for (int j=0;j<this->dimensions;j++){
				Sw[i][j] += pt[i]*pt[j];
			}
		}
	}
	return Sw;
}


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


Matrix LDA::calculateSwInverse(Matrix Sw)
{
	Matrix SwInverse = Matrix(this->dimensions, Point4d(this->dimensions, 0.0));
	double m[16], out[16];
	for (int i=0, k=0;i<this->dimensions;i++){
		for (int j=0;j<this->dimensions;j++){
			m[k++] = Sw[i][j];
		}
	}
	bool b = inverseMatrix4x4(m, out);
	if (!b) return SwInverse;
	else{
		for (int i=0, k=0;i<this->dimensions;i++){
			for (int j=0;j<this->dimensions;j++){
				SwInverse[i][j] = out[k++];
			}
		}
	}
	return SwInverse;
}


Point4d LDA::calculateW(Matrix SwInverse, Point4d mean_p, Point4d mean_n)
{
	Point4d w = Point4d(this->dimensions, 0);
	Point4d mean_diff = Point4d(this->dimensions, 0);
	for (int i=0;i<this->dimensions;i++){
		mean_diff[i] = mean_p[i] - mean_n[i];
	}
	for (int i=0;i<this->dimensions;i++){
		for (int j=0;j<this->dimensions;j++){
			w[i] += SwInverse[i][j]*mean_diff[j];
		}
	}
	return w;
}


Points1d LDA::get1DPoints(Point4d w, Matrix examples)
{
	Points1d examples1d;
	for (auto example: examples){
		pair<double, bool> temp = {0, 0};
		for (int dim=0;dim<this->dimensions;dim++){
			 temp.first += w[dim]*example[dim];
		}
		temp.second = example[this->dimensions];
		examples1d.push_back(temp);
	}
	return examples1d;
}


double calculateEntropy(int index, Points1d examples1d)
{
	double pos=0, neg=0;
	double entropy_left=0, entropy_right=0;
	for (int i=0;i<=index;i++){
		if (examples1d[i].second) pos++;
		else neg++;
	}
	pos /= (pos+neg);
	neg /= (pos+neg);
	if (pos!=0) entropy_left += -pos*log2(pos);
	if (neg!=0) entropy_left += -neg*log2(neg);

	pos = neg = 0;
	for (int i=index+1;i<examples1d.size();i++){
		if (examples1d[i].second) pos++;
		else neg++;
	}
	pos /= (pos+neg);
	neg /= (pos+neg);
	if (pos!=0) entropy_right += -pos*log2(pos);
	if (neg!=0) entropy_right += -neg*log2(neg);

	return (entropy_left + entropy_right)/2;
}


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
}