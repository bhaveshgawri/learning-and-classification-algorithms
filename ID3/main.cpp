#include <bits/stdc++.h>

#define pb push_back	
#define FEATURE_NUM 14
#define PER_LINE_MISSING_THRES 0.6
#define inf INT_MAX
using namespace std;

class Preprocessor
{
	private:
		vector<vector<string>> inp;
		vector<int>continuous_attrs_index = {0, 2, 4, 10, 11, 12};
		map<string, vector<string>> attr_list;
		
		// 1-d array
		double entropy_continuous(vector<pair<int, int>>& in, int start, int index, int end){
			double pos, neg;
			pos = neg = 0;
			double  e1=0, e2=0;
			for (int i=start;i<=index;i++){
				if (in[i].second == 1){
					pos++;
				}
				else neg++;
			}
			pos /= (pos+neg);
			neg /= (pos+neg);
			if (!pos)
				e1 = -double(neg)*log2(neg);
			else if (!neg)
				e1 = -double(pos)*log2(pos);
			else
				e1 = -double(pos)*log2(pos)-double(neg)*log2(neg);
			pos = neg = 0;
			for (int i=index+1;i<=end;i++){
				if (in[i].second == 1){
					pos++;
				}
				else neg++;
			}
			pos /= (pos+neg);
			neg /= (pos+neg);
			if (!pos)
				e2 = -double(neg)*log2(neg);
			else if (!neg)
				e2 = -double(pos)*log2(pos);
			else
				e2 = -double(pos)*log2(pos)-double(neg)*log2(neg);
			return (e1+e2)/2;
		}
		// 2-d array
		double entropy(auto& in){
			double pos, neg;
			pos = neg = 0;
			for (auto row: in){
				if (row[FEATURE_NUM] == "1"){
					pos++;
				}
				else neg++;
			}
			pos /= (pos+neg);
			neg /= (pos+neg);
			double e=0;
			if (!pos)
				e = -double(neg)*log2(neg);
			else if (!neg)
				e = -double(pos)*log2(pos);
			else
				e = -double(pos)*log2(pos)-double(neg)*log2(neg);
			return e;
		}

	public:
		void input_data(){
			string line;
			ifstream infile("test");
			int k = 0;
			while(getline(infile, line)){
				string word="";
				vector<string> tokens;
				for (int i=0;i<line.length();i++){
					if (line[i]!=' ' && line[i]!=','){
						word+=line[i];
					}
					if (line[i]==','){
						tokens.pb(word);
						word="";
					}
				}
				tokens.pb(word);
				inp.pb(tokens);
			}
			cout<<inp.size()<<endl;
			cout<<"done inputting file..."<<endl;
		}

		void input_attr_list(){
			string line;
			ifstream infile("attr_list");
			while(getline(infile, line)){
				string key;
				string word = "";
				vector<string> tokens;
				for (char c: line){
					if (c!=':' && c!=' ' && c!=','){
						word+=c;
					}
					if (c==':'){
						key = word;
						word="";
					}
					if (c==','){
						tokens.pb(word);
						word="";
					}
				}
				tokens.pb(word);
				attr_list[key] = tokens;
			}
		}

		void assign_pos_neg_to_input(){
			// selecting positive and negative inputs
			for (int i=0;i<inp.size();i++){
				if (inp[i][FEATURE_NUM] == ">50K"){
					inp[i][FEATURE_NUM] = "1";
				}
				else{
					inp[i][FEATURE_NUM] = "0";	
				}
			}
		}

		void tackle_missing(){
			// removeing those rows having more '?' than threshold
			vector<vector<string>> formatted_inp;
			for (auto row: inp){
				int missing_words = 0;
				for (string word: row){
					if (word == "?"){
						missing_words++;
					}
				}
				// cout<<double(missing_words)/inp[0].size()<<endl;
				if (double(missing_words)/inp[0].size() <= PER_LINE_MISSING_THRES){
					formatted_inp.pb(row);
				}
			}
			inp=formatted_inp;
			formatted_inp.clear();
			// replace a missing value with the most occuring value of 
			// that feature in the input
			for (int i=0;i<FEATURE_NUM;i++){
				map<string, int> attr_count;
				for (auto row: inp){
					if (row[i] != "?"){
						attr_count[row[i]]++;
					}
				}
				int max_value = -inf;
				string max_key="";
				for (auto i: attr_count){
					if (i.second>max_value){
						max_value = i.second;
						max_key = i.first;
					}
				}
				for (int j=0;j<inp.size();j++){
					if (inp[j][i] == "?"){
						inp[j][i] = max_key;
					}
				}
			}
		}


		void make_continuous(auto& in){
			for (int idx: continuous_attrs_index){
				/*
				// for Heuristic #1 and #2
				vector<pair<int, int>> temp;
				vector<pair<int, int>> attr;
				for (auto row: in){
					temp.pb({stoi(row[idx]), stoi(row[FEATURE_NUM])});
				}
				sort(temp.begin(), temp.end());
				
				// if a value of attr occurrs multile times select 0/1 based on max
				// occurence of 0/1 for that value of that attr
				// 40 40 40 40 40 40 40 50 50 50 55 56 60 65 70 70 85
				// 0  0  0  1  1  1  1  1  0  0  1  0  0  0  1  0  0
				// v v v v v v 
				// 40 50 55 56 60 65 70 85
				// 1  0  1  0  0  0  0  0
				for (int i = 0; i < temp.size(); i++){
					int count0, count1;
					if (temp[i].second == 0){
						count0 = 1;
						count1 = 0;
					}
					else{
						count0 = 0;
						count1 = 1;
					}
					
					while (temp[i].first == temp[i+1].first){
						if (temp[i+1].second == 0) count0++;
						else count1++;
						i++;
					}
					attr.pb({temp[i].first, count1 > count0 ? 1 : 0});
				}
				temp.clear();
				for (int i=0;i<attr.size();i++){
					cout<<attr[i].first<<" "<<attr[i].second<<endl;
				}
				// calculating 2 splits in the attr array based on the
				// indices which will cause the maximum non-uniform distribution
				// of outputs

				// //heuristic #1 // //
				int idx1;
				int max1 = -inf;
				for (int i=0;i<attr.size()-1;i++){
					double e = entropy_continuous(attr, 0, i, attr.size()-1);
					if (e > max1){
						max1 = e;
						idx1 = i;
					}
				}
				int idx2, idx3;
				int max2 = -inf, max3 = -inf;
				for (int i=0;i<idx1;i++){
					double e = entropy_continuous(attr, 0, i, idx1);
					if (e > max2){
						max2 = e;
						idx2 = i;
					}
				}
				for (int i=idx1+1;i<attr.size();i++){
					double e = entropy_continuous(attr, idx1+1, i, attr.size()-1);
					if (e > max2){
						max2 = e;
						idx2 = i;
					}
				}
				int hi, lo;
				if (max2>max3){
					hi = idx1;
					lo = idx2;
				}
				else{
					hi = idx3;
					lo = idx1;
				}

				// //heuristic #2 // //
				int idx1, idx2;
				double first_max = -inf, second_max = -inf;
				double e1 = entropy_continuous(attr, 0, 0, v.size()-1);
				double e2 = entropy_continuous(attr, 0, 1, v.size()-1);
				if (e1 >= e2){
					first_max = e1, idx1 = 0;
					second_max = e2, idx2 = 1;
				}
				else{
					first_max = e2, idx2 = 1;
					second_max = e1, idx1 = 0;
				}
				for (int i = 2; i < attr.size() - 1; i++){
					double e = entropy_continuous(attr, 0, i, v.size()-1);
					if (e > first_max){
						second_max = first_max;
						idx2 = idx1;
						first_max = e;
						idx1 = i;
					}
				}
				int lo = min(idx1, idx2);
				int hi = max(idx1, idx2);
				*/

				// heuristic #3
				double average=0;
				double standard_dev = 0;
				for (auto row: inp){
					average += stoi(row[idx]);
				}
				average /= inp.size();
				for (auto row: inp){
					standard_dev += pow(double(stoi(row[idx])-average), 2);
				}
				standard_dev /= (inp.size()-1);
				standard_dev = sqrt(standard_dev);

				for (int i=0;i<inp.size();i++){
					if (stoi(inp[i][idx]) > average+standard_dev){
						inp[i][idx] = "high";
					}
					else if(stoi(inp[i][idx]) < average - standard_dev){
						inp[i][idx] = "low";
					}
					else{
						inp[i][idx] = "medium";
					}
				}

				for (auto i: attr_list){
					if (i.second[0]=="continuous"){
						attr_list[i.first].pop_back();
						attr_list[i.first].pb("low");
						attr_list[i.first].pb("mid");
						attr_list[i.first].pb("high");
					}
				}
			}
		}



		Preprocessor(){
			// preprocessing and initializing functions
			input_data();
			input_attr_list();
			assign_pos_neg_to_input();
			tackle_missing();
			make_continuous(inp);
		}
		~Preprocessor(){;}
	
};

int main(){
	Preprocessor p;
	return 0;
}