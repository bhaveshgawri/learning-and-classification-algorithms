#ifndef NAIVEBAYES_H
#define NAIVEBAYES_H
#include <bits/stdc++.h>
using namespace std;

class NaiveBayes
{
private:
	int numerator_smoothener;
	int denominator_smoothener;
	
	int true_positives;
	int false_positives;
	int true_negatives;
	int false_negatives;
	
	vector<int> positive_count;
	vector<int> negative_count;
	
	vector<string> stop_words;
	vector<string> vocab_words;

public:
	NaiveBayes(int, int, int);

	void loadStopWords(string);
	void loadVocabulary(string);
	
	int convertStringToInt(string);
	vector<int> convertStringToIntPair(string);
	
	void increaseCountBasicNaiveBayes(bool, vector<int>);
	void increaseCountWithoutStopWords(bool, vector<int>);
	void increaseCountBinaryNaive(bool, int);
	void increaseCountBinaryNaiveWithoutStopWords(bool, int);
	void getPosNegWordsCountsFromFile(string, int);
	
	pair<double, double> calculateProbability(pair<double, double>, int, int, int);
	void test(string);
};

#endif