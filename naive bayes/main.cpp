#include <bits/stdc++.h>
#include "NaiveBayes.hpp"
using namespace std;

#define word_count_in_dictionary 89527
#define numerator_smoothener 158
#define denominator_smoothener 3001003


int main(){
	string train_file_path = "./data/train/labeledBow.feat";
	string test_file_path = "./data/test/labeledBow.feat";
	string stop_word_file_path = "./data/stop_words";
	string vocab_word_file_path = "./data/imdb.vocab";

	cout<<"Basic Naive Bayes with Stop Words"<<endl;
	NaiveBayes bn0(numerator_smoothener, denominator_smoothener, word_count_in_dictionary);
	bn0.loadStopWords(stop_word_file_path);
	bn0.loadVocabulary(vocab_word_file_path);
	bn0.getPosNegWordsCountsFromFile(train_file_path, 0); // test
	bn0.test(test_file_path);

	cout<<endl;
	cout<<"Basic Naive Bayes without Stop Words"<<endl;
	NaiveBayes bn1(numerator_smoothener, denominator_smoothener, word_count_in_dictionary);
	bn1.loadStopWords(stop_word_file_path);
	bn1.loadVocabulary(vocab_word_file_path);
	bn1.getPosNegWordsCountsFromFile(train_file_path, 1); // test
	bn1.test(test_file_path);

	cout<<endl;
	cout<<"Binary Naive Bayes with Stop Words"<<endl;
	NaiveBayes bn2(numerator_smoothener, denominator_smoothener, word_count_in_dictionary);
	bn2.loadStopWords(stop_word_file_path);
	bn2.loadVocabulary(vocab_word_file_path);
	bn2.getPosNegWordsCountsFromFile(train_file_path, 2); // test
	bn2.test(test_file_path);

	cout<<endl;
	cout<<"Binary Naive Bayes without Stop Words"<<endl;
	NaiveBayes bn3(numerator_smoothener, denominator_smoothener, word_count_in_dictionary);
	bn3.loadStopWords(stop_word_file_path);
	bn3.loadVocabulary(vocab_word_file_path);
	bn3.getPosNegWordsCountsFromFile(train_file_path, 3); // test
	bn3.test(test_file_path);
}