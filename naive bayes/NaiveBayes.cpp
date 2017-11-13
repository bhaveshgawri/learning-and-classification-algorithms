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
/*
	Load the Stop-Words on the RAM by reading them from a file.
*/
{
	string line;
	ifstream infile(stop_word_file_path);
	while(getline(infile, line)){
		stop_words.push_back(line);
	}
	cout<<"Loaded Stop Words..."<<endl;
}


void NaiveBayes::loadVocabulary(string vocabulary_file_path)
/*
	Load the vocabulary on the RAM by reading it from the file.
*/
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
/*
	Convert string of the form `number1:number2` to integer value
	equal to number1.
	Example: "12323:2259" => 12323
*/
	string temp;
	istringstream iss(word);
	getline(iss, temp, ':');
	return atoi(temp.c_str());
}


vector<int> NaiveBayes::convertStringToIntPair(string word)
{
/*
	Convert string of the form `number1:number2` to 2 interger values
	equal to number1 and number2 and store it int a 1D array of size 2.
	Example: "12323:2259" => vector<int> v(2), where v[0] = 12323
											   and   v[1] = 2259
*/
	int i = 0;
	string temp;
	vector<int> couple(2);
	istringstream iss(word);
	
	while(getline(iss, temp, ':')){
		couple[i++] = atoi(temp.c_str());
	}
	return couple;
}


void NaiveBayes::increaseCountBasicNaiveBayes(bool _true, vector<int>couple)
{
/*
	Increase frequency of the word(count[0]) by the number of times(count[1])
	it appears in a review.
	Increase positive_count only for a positive review and
			 negative_count only for a negative review.
*/
	if (_true) positive_count[couple[0]] += couple[1];
	else negative_count[couple[0]] += couple[1];
}


void NaiveBayes::increaseCountBasicNaiveWithoutStopWords(bool _true, 
														vector<int>couple)
{
/*
	Increase frequency of the word(count[0]) by the number of times(count[1])
	it appears in a review if word(count[0]) is not a stop word.
	Increase positive_count only for a positive review and
			 negative_count only for a negative review.
*/
	if (!binary_search(stop_words.begin(), stop_words.end(), 
						vocab_words[couple[0]])){
		if (_true) positive_count[couple[0]] += couple[1];
		else negative_count[couple[0]] += couple[1];
	}
}


void NaiveBayes::increaseCountBinaryNaiveBayes(bool _true, int word)
{
/*
	Increase frequency of the word by 1 in the frequency array.
	Increase positive_count only for a positive review and
			 negative_count only for a negative review.
*/
	if (_true) positive_count[word] += 1;
	else negative_count[word] += 1;
}


void NaiveBayes::increaseCountBinaryNaiveWithoutStopWords(bool _true, int word)
{
/*
	Increase frequency of the word by 1 in the frequency array only
	if `word` is not a stop word.
	Increase positive_count only for a positive review and
			 negative_count only for a negative review.
*/
	if (!binary_search(stop_words.begin(), stop_words.end(), 
						vocab_words[word])){
		if (_true) positive_count[word] += 1;
		else negative_count[word] += 1;
	}
}


void NaiveBayes::increaseCount(int technique, bool _true, vector<int>couple)
/*
	Choose appropriate frequency increasing function depending
	on the technique.
*/
{
		if (technique == 0)
			increaseCountBasicNaiveBayes(_true, couple);
		else if (technique == 1)
			increaseCountBasicNaiveWithoutStopWords(_true, couple);
		else if (technique == 2)
			increaseCountBinaryNaive(_true, couple[0]);
		else if (technique == 3)
			increaseCountBinaryNaiveWithoutStopWords(_true, couple[0]);
}


pair<double, double> NaiveBayes::calculateProbability(
										pair<double, double> probabilities,
										int word_idx,
										int positive_wc,
										int negative_wc)
/*
	Calculate and update the probability after appropriately 
	smoothening it.
*/
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


void NaiveBayes::print_correctness()
/*
	Calculate then print the accuracy, precision, recall and
	f-measure of the Naive Bayes Algorithm with different extensions.
*/
{

	double accuracy = (double)(true_positives + false_negatives) / (
		true_positives + true_negatives + false_positives + 
		false_negatives);
	double precision = (double)true_positives / (
		true_positives + false_positives);
	double recall = (double)true_positives / (
		true_positives + false_negatives);
	double f_meausre = (double)(2 * precision * recall) /(
		precision + recall);
	
	cout << "Accuracy:  " 
		 << 100 * accuracy
		 << "%"
		 << endl;

	cout << "Precision: " 
		 << 100 * precision
		 << "%"
		 << endl;

	cout << "Recall:    "
		 << 100 * recall
		 << "%" 
		 << endl;

	cout << "F-measure: "
		 << 100 * f_meausre
		 << "%"
		 << endl;

}

void NaiveBayes::getPosNegWordsCountsFromFile(string train_feat_file_path,
											  int technique)
/*
	// Training Function
	Read the input word count from training file and increase the 
	count of positive and negative frequency arrays. 
*/
{
	// check whether technique supplied is valid or not.
	if (technique < 0 || technique > 3){
		cout<<"Not a valid technique.";
		return;
	}

	string line;
	ifstream infile(train_feat_file_path);
	
	// read file line by line (review by review)
	while(getline(infile, line)){
		string word = "", rating = "";
		bool _true;
		int idx = 0;
		
		// calculate rating
		while(line[idx] != ' '){
			rating += line[idx];
			idx++;
		}
		
		// chech whether rating is postive or not
		if (atoi(rating.c_str()) >= 7) _true = true;
		else _true = false;
		
		// read word counts and update frequenct arrays.
		for (int i=idx+1; i<line.size();i++){
			char chr = line[i];
			if (chr != ' ' && chr != '\n'){
				word += chr;
			}
			else if (word.find(':') != string::npos){
				vector<int> couple = convertStringToIntPair(word);
				increaseCount(technique, _true, couple);			
				word = "";
			}
		}
		vector<int> couple = convertStringToIntPair(word);
		increaseCount(technique, _true, couple);
	}
}


void NaiveBayes::test(string test_feat_file_path)
/*
	// Testing Function
	Reads Word count from test file, calculate probabilities of words
	wrt previouly calculated(trained) positive and negative frequency
	arrays, predict the rating and compare it with rating as extracted
	from test file.
*/
{
	// initialize and update positive and negative total word count
	int positive_wc = accumulate(positive_count.begin(), 
		positive_count.end(), 0);
	int negative_wc = accumulate(negative_count.begin(), 
		negative_count.end(), 0);
	positive_wc += denominator_smoothener;
	negative_wc += denominator_smoothener;
	
	int cor=0, tot=0;

	string line;
	ifstream infile(test_feat_file_path);
	
	// read file line by line (review by review)
	while(getline(infile, line)){
		
		bool _true;
		int idx = 0;
		string word = "";
		string rating = "";
		pair<double, double> probabilities = {0, 0};
		// first: positive_prob
		// second: negative_prob

		// calculate rating for testing
		while(line[idx] != ' '){
			rating += line[idx];
			idx++;
		}

		// check whether test rating is positive or not
		if (atoi(rating.c_str()) >= 7) _true = true;
		else _true = false;

		// read the words of the test file and update frequenct arrays
		for (int i=idx+1; i<line.size();i++){
			char chr = line[i];
			if (chr != ' ' && chr != '\n'){
				word += chr;
			}
			else if (word.find(':') != string::npos){
				int word_idx = convertStringToInt(word);
				probabilities = calculateProbability(probabilities, word_idx, positive_wc, negative_wc);
				word = "";
			}
		}
		int word_idx = convertStringToInt(word);
		probabilities = calculateProbability(probabilities, word_idx, positive_wc, negative_wc);
			
		// predict the calculated rating from probabilities
		bool predicted = (probabilities.first < probabilities.second);
		
		// increment tp, tn, fp, fn accordingly
		if (_true && predicted)
			true_positives++;
		else if (!_true && !predicted)
			false_negatives++;
		else if (!_true && predicted)
			false_positives++;
		else if (_true && !predicted)
			true_negatives++;
	}
	// print the correctness of Naive Bayes algorithm.
	print_correctness();
}