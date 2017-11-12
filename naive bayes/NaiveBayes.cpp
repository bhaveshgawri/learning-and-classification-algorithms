#include <bits/stdc++.h>
#include "NaiveBayes.hpp"
using namespace std;

NaiveBayes::NaiveBayes(int numerator_smoothener, 
						int denominator_smoothener,
						int word_count_in_dictionary)
{
	true_positives = 0;
	false_positives = 0;
	true_negatives = 0;
	false_negatives = 0;

	this->numerator_smoothener = numerator_smoothener;
	this->denominator_smoothener = denominator_smoothener;
	positive_count = vector<int>(word_count_in_dictionary, 0);
	negative_count = vector<int>(word_count_in_dictionary, 0);
}


void NaiveBayes::loadStopWords(string stop_word_file_path)
{
	string line;
	ifstream infile(stop_word_file_path);
	while(getline(infile, line)){
		stop_words.push_back(line);
	}
	cout<<"Loaded Stop Words..."<<endl;
}


void NaiveBayes::loadVocabulary(string vocabulary_file_path)
{
	string line;
	ifstream infile(vocabulary_file_path);
	while(getline(infile, line)){
		vocab_words.push_back(line);
	}
	cout<<"Loaded Vocabulary..."<<endl;
}


int NaiveBayes::convertStringToInt(string word)
{
	string temp;
	istringstream iss(word);
	getline(iss, temp, ':');
	return atoi(temp.c_str());
}


vector<int> NaiveBayes::convertStringToIntPair(string word)
{
	int i = 0;
	string temp;
	vector<int> couple(2);
	istringstream iss(word);
	
	while(getline(iss, temp, ':')){
		couple[i++] = atoi(temp.c_str());
	}
	return couple;
}


void NaiveBayes::increaseCountBasicNaiveBayes(bool pos, vector<int>couple)
{
	if (pos) positive_count[couple[0]] += couple[1];
	else negative_count[couple[0]] += couple[1];
}


void NaiveBayes::increaseCountWithoutStopWords(bool pos, vector<int>couple)
{
	if (!binary_search(stop_words.begin(), stop_words.end(), 
						vocab_words[couple[0]])){
		if (pos) positive_count[couple[0]] += couple[1];
		else negative_count[couple[0]] += couple[1];
	}
}


void NaiveBayes::increaseCountBinaryNaive(bool pos, int word)
{	
	if (pos) positive_count[word] += 1;
	else negative_count[word] += 1;
}


void NaiveBayes::increaseCountBinaryNaiveWithoutStopWords(bool pos, int word)
{
	if (!binary_search(stop_words.begin(), stop_words.end(), 
						vocab_words[word])){
		if (pos) positive_count[word] += 1;
		else negative_count[word] += 1;
	}
}


pair<double, double> NaiveBayes::calculateProbability(
										pair<double, double> probabilities,
										int word_idx,
										int positive_wc,
										int negative_wc)
{
	int word_count_in_positive = numerator_smoothener;
	int word_count_in_negative = numerator_smoothener;
	word_count_in_positive += positive_count[word_idx];
	word_count_in_negative += negative_count[word_idx];
	probabilities.first += abs(log10(
		(double)word_count_in_positive/positive_wc));
	probabilities.second += abs(log10(
		(double)word_count_in_negative/negative_wc));
	
	return probabilities;
}


void NaiveBayes::getPosNegWordsCountsFromFile(string train_feat_file_path,
												int technique)
{
	if (technique < 0 || technique > 3){
		cout<<"Not a valid technique.";
		return;
	}
	string line;
	ifstream infile(train_feat_file_path);
	while(getline(infile, line)){
		string word = "", rating = "";
		bool pos;
		int idx = 0;
		
		while(line[idx] != ' '){
			rating += line[idx];
			idx++;
		}
		
		if (atoi(rating.c_str()) >= 7) pos = true;
		else pos = false;
		
		for (int i=idx+1; i<line.size();i++){
			char chr = line[i];
			if (chr != ' ' && chr != '\n'){
				word += chr;
			}
			else{
				if (word.find(':') != string::npos){
					vector<int> couple = convertStringToIntPair(word);
					if (technique==0)
						increaseCountBasicNaiveBayes(pos, couple);
					else if (technique==1)
						increaseCountWithoutStopWords(pos, couple);
					else if (technique==2)
						increaseCountBinaryNaive(pos, couple[0]);
					else if (technique==3)
						increaseCountBinaryNaiveWithoutStopWords(pos, 
							couple[0]);
					word = "";
				}
			}
		}
		vector<int> couple = convertStringToIntPair(word);
		if (technique==0)
			increaseCountBasicNaiveBayes(pos, couple);
		else if (technique==1)
			increaseCountWithoutStopWords(pos, couple);
		else if (technique==2)
			increaseCountBinaryNaive(pos, couple[0]);
		else if (technique==3)
			increaseCountBinaryNaiveWithoutStopWords(pos, couple[0]);
	}
}


void NaiveBayes::test(string test_feat_file_path)
{
	int positive_wc = accumulate(positive_count.begin(), 
		positive_count.end(), 0);
	int negative_wc = accumulate(negative_count.begin(), 
		negative_count.end(), 0);
	positive_wc += denominator_smoothener;
	negative_wc += denominator_smoothener;
	
	int cor=0, tot=0;

	string line;
	ifstream infile(test_feat_file_path);
	while(getline(infile, line)){
		
		bool pos;
		int idx = 0;
		string word = "";
		string rating = "";
		pair<double, double> probabilities = {0, 0};
		// first: positive_prob
		// second: negative_prob

		while(line[idx] != ' '){
			rating += line[idx];
			idx++;
		}
		if (atoi(rating.c_str()) >= 7) pos = true;
		else pos = false;
		for (int i=idx+1; i<line.size();i++){
			char chr = line[i];
			if (chr != ' ' && chr != '\n'){
				word += chr;
			}
			else{
				if (word.find(':') != string::npos){
					int word_idx = convertStringToInt(word);
					probabilities = calculateProbability(probabilities, word_idx, positive_wc, negative_wc);
					word = "";
				}
			}
		}
		int word_idx = convertStringToInt(word);
		probabilities = calculateProbability(probabilities, word_idx, positive_wc, negative_wc);
			

		bool predicted = (probabilities.first >= probabilities.second);
		if ((pos && predicted) || ((!pos) && (!predicted))){
			cor++;
		}
		tot++;
	}
	cout<<(double)cor/tot<<endl;
	cout<<cor<<" "<<tot<<endl;
}