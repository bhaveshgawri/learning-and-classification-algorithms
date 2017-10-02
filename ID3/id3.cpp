#include <bits/stdc++.h>

#define pb push_back
#define TARGET 14
#define USE_SPLIT_INFO 0

using namespace std;

typedef struct Node{
	int attr;
	bool leaf;
	bool val;
	struct Node** next;
}Node;
/*Returns the corresponding value associated with
each type of attributes*/
int get_attr_num(int attr, const auto& inp){
	if (attr == 0){
		if (inp[attr]=="low") return 1;
		else if (inp[attr]=="medium") return 2;
		else if (inp[attr]=="high") return 3;
	}
	else if (attr==1){
		if (inp[attr]=="Private") return 1;
		else if (inp[attr]=="Self-emp-not-inc") return 2;
		else if (inp[attr]=="Self-emp-inc") return 3;
		else if (inp[attr]=="Federal-gov") return 4;
		else if (inp[attr]=="Local-gov") return 5;
		else if (inp[attr]=="State-gov") return 6;
		else if (inp[attr]=="Without-pay") return 7;
		else if (inp[attr]=="Never-worked") return 8;
	}
	else if (attr==2){
		if (inp[attr]=="low") return 1;
		else if (inp[attr]=="medium") return 2;
		else if (inp[attr]=="high") return 3;
	}
	else if (attr==3){
		if (inp[attr]=="Bachelors") return 1;
		else if (inp[attr]=="Some-college") return 2;
		else if (inp[attr]=="11th") return 3;
		else if (inp[attr]=="HS-grad") return 4;
		else if (inp[attr]=="Prof-school") return 5;
		else if (inp[attr]=="Assoc-acdm") return 6;
		else if (inp[attr]=="Assoc-voc") return 7;
		else if (inp[attr]=="9th") return 8;
		else if (inp[attr]=="7th-8th") return 9;
		else if (inp[attr]=="12th") return 10;
		else if (inp[attr]=="Masters") return 11;
		else if (inp[attr]=="1st-4th") return 12;
		else if (inp[attr]=="10th") return 13;
		else if (inp[attr]=="Doctorate") return 14;
		else if (inp[attr]=="5th-6th") return 15;
		else if (inp[attr]=="Preschool") return 16;
	}
	else if (attr==4){
		if (inp[attr]=="low") return 1;
		else if (inp[attr]=="medium") return 2;
		else if (inp[attr]=="high") return 3;
	}
	else if (attr==5){
		if (inp[attr]=="Married-civ-spouse") return 1;
		else if (inp[attr]=="Divorced") return 2;
		else if (inp[attr]=="Never-married") return 3;
		else if (inp[attr]=="Separated") return 4;
		else if (inp[attr]=="Widowed") return 5;
		else if (inp[attr]=="Married-spouse-absent") return 6;
		else if (inp[attr]=="Married-AF-spouse") return 7;
	}
	else if (attr==6){
		if (inp[attr]=="Tech-support") return 1;
		else if (inp[attr]=="Craft-repair") return 2;
		else if (inp[attr]=="Other-service") return 3;
		else if (inp[attr]=="Sales") return 4;
		else if (inp[attr]=="Exec-managerial") return 5;
		else if (inp[attr]=="Prof-specialty") return 6;
		else if (inp[attr]=="Handlers-cleaners") return 7;
		else if (inp[attr]=="Machine-op-inspct") return 8;
		else if (inp[attr]=="Adm-clerical") return 9;
		else if (inp[attr]=="Farming-fishing") return 10;
		else if (inp[attr]=="Transport-moving") return 11;
		else if (inp[attr]=="Priv-house-serv") return 12;
		else if (inp[attr]=="Protective-serv") return 13;
		else if (inp[attr]=="Armed-Forces") return 14;
	}
	else if (attr==7){
		if (inp[attr]=="Wife") return 1;
		else if (inp[attr]=="Own-child") return 2;
		else if (inp[attr]=="Husband") return 3;
		else if (inp[attr]=="Not-in-family") return 4;
		else if (inp[attr]=="Other-relative") return 5;
		else if (inp[attr]=="Unmarried") return 6;
	}
	else if (attr==8){
		if (inp[attr]=="White") return 1;
		else if (inp[attr]=="Asian-Pac-Islander") return 2;
		else if (inp[attr]=="Amer-Indian-Eskimo") return 3;
		else if (inp[attr]=="Other") return 4;
		else if (inp[attr]=="Black") return 5;
	}
	else if (attr==9){
		if (inp[attr]=="Female") return 1;
		else if (inp[attr]=="Male") return 2;
	}
	else if (attr==10){
		if (inp[attr]=="low") return 1;
		else if (inp[attr]=="medium") return 2;
		else if (inp[attr]=="high") return 3;
	}
	else if (attr==11){
		if (inp[attr]=="low") return 1;
		else if (inp[attr]=="medium") return 2;
		else if (inp[attr]=="high") return 3;
	}
	else if (attr==12){
		if (inp[attr]=="low") return 1;
		else if (inp[attr]=="medium") return 2;
		else if (inp[attr]=="high") return 3;
	}
	else if (attr==13){
		if (inp[attr]=="United-States") return 1;
		else if (inp[attr]=="Cambodia") return 2;
		else if (inp[attr]=="England") return 3;
		else if (inp[attr]=="Puerto-Rico") return 4;
		else if (inp[attr]=="Canada") return 5;
		else if (inp[attr]=="Germany") return 6;
		else if (inp[attr]=="Outlying-US(Guam-USVI-etc)") return 7;
		else if (inp[attr]=="India") return 8;
		else if (inp[attr]=="Japan") return 9;
		else if (inp[attr]=="Greece") return 10;
		else if (inp[attr]=="South") return 11;
		else if (inp[attr]=="China") return 12;
		else if (inp[attr]=="Cuba") return 13;
		else if (inp[attr]=="Iran") return 14;
		else if (inp[attr]=="Honduras") return 15;
		else if (inp[attr]=="Philippines") return 16;
		else if (inp[attr]=="Italy") return 17;
		else if (inp[attr]=="Poland") return 18;
		else if (inp[attr]=="Jamaica") return 19;
		else if (inp[attr]=="Vietnam") return 20;
		else if (inp[attr]=="Mexico") return 21;
		else if (inp[attr]=="Portugal") return 22;
		else if (inp[attr]=="Ireland") return 23;
		else if (inp[attr]=="France") return 24;
		else if (inp[attr]=="Dominican-Republic") return 25;
		else if (inp[attr]=="Laos") return 26;
		else if (inp[attr]=="Ecuador") return 27;
		else if (inp[attr]=="Taiwan") return 27;
		else if (inp[attr]=="Haiti") return 29;
		else if (inp[attr]=="Columbia") return 30;
		else if (inp[attr]=="Hungary") return 31;
		else if (inp[attr]=="Guatemala") return 32;
		else if (inp[attr]=="Nicaragua") return 33;
		else if (inp[attr]=="Scotland") return 34;
		else if (inp[attr]=="Thailand") return 35;
		else if (inp[attr]=="Yugoslavia") return 36;
		else if (inp[attr]=="El-Salvador") return 37;
		else if (inp[attr]=="Trinadad&Tobago") return 38;
		else if (inp[attr]=="Peru") return 39;
		else if (inp[attr]=="Hong") return 40;
		else if (inp[attr]=="Holand-Netherlands") return 41;
	}
}

/*Checks whether the corresponding tuple of atribute
value is true in accordance with the decision thee*/
int predict(const auto& in, Node* head){
	if (head->leaf) return head->val;
	else return predict(in, head->next[get_attr_num(head->attr, in)]);
}

/*This function finds accuracy of prediction along with
precion and recall values using false/true - pos/neg */
auto accuracy(auto& in, Node* head){
	double true_pos = 0, false_pos = 0;
	double true_neg = 0, false_neg = 0;
	int count0=0, count1=0;
	for (auto row: in){
		int predicted_val = predict(row, head);
		if (predicted_val==1 && stoi(row[TARGET])==1) true_pos+=1;
		else if (predicted_val==0 && stoi(row[TARGET])==0) true_neg+=1;
		else if(predicted_val==1 && stoi(row[TARGET])==0) false_pos+=1;
		else if (predicted_val==0 && stoi(row[TARGET])==1) false_neg+=1;
	}
	double accr = (true_pos+true_neg)/(false_pos+false_neg+true_neg+true_pos);
	double precision = true_pos/(true_pos+false_pos);
	double recall = true_pos/(true_pos+false_neg);
	auto t = make_tuple(accr, precision, recall);
	return t;
}

/*This funtion prints the decision tree with value and attr
type of each node of the decision tree*/
void print_decision_tree(Node* head, auto& attr_val_num, int attr){
	if (head->leaf == false){
		cout<<head->attr<<" "<<attr_val_num[head->attr]<<endl;
		for (int i=1;i<=attr_val_num[head->attr];i++){
			print_decision_tree(head->next[i], attr_val_num, head->attr);
		}
	}
	else{
		cout<<"attr: "<<attr<<" value: "<<head->val<<endl;
	}
}

/*This function calculates the accuracy of decision tree
 when subtrees of `node` are removed */
double remove_node_and_test(Node* head, Node* node, auto& inp, auto& attr_val_num){
	int isLeaf = 0;
	if (node->leaf)
		isLeaf = 1;
	node->leaf = true;
	auto t = accuracy(inp, head);
	double accr = get<0>(t);
	// if (accr>80)
	// 	cout<<accr;
	if (!isLeaf){
		node->leaf = false;
	}
	return accr;
}

/*This function interates through all nodes of a tree and
checks which node to remove to get max accuracy*/
pair<double, Node*> prune_tree(Node* head, Node* node, auto& inp, auto& attr_val_num){
	double accr = remove_node_and_test(head, node, inp, attr_val_num);
	if (node->leaf){
		return {accr, node};
	}
	
	pair<double, Node*> to_remove = {accr, node};
	for (int i = 1;i <= attr_val_num[node->attr];i++){
		pair<double, Node*> new_arrc = prune_tree(head, node->next[i], inp, attr_val_num);
		if (new_arrc.first > accr){
			to_remove = new_arrc;
		}
	}
	return to_remove;
}

/*This functions prunes the tree to increase the prediction accuracy*/
Node* prune1(Node* head, double accr, auto& training_inp, auto& attr_val_num){
// this pruning tech increases the accuracy on testing data but is quite costly 
	double prev_accr = accr;
	pair<double, Node*> prune_output = prune_tree(head, head, training_inp, attr_val_num);
	
	while(prune_output.first > prev_accr){
		prune_output.second->leaf = true;
		prev_accr = prune_output.first;
		prune_output = prune_tree(head, head, training_inp, attr_val_num);
	}
	return head;
}
/*Overloaded form of Node* prune1()*/
void prune1(Node** head, double accr, auto& training_inp, auto& attr_val_num){
	double prev_accr = accr;
	pair<double, Node*> prune_output = prune_tree(*head, *head, training_inp, attr_val_num);
	
	while(prune_output.first > prev_accr){
		prune_output.second->leaf = true;
		prev_accr = prune_output.first;
		prune_output = prune_tree(*head, *head, training_inp, attr_val_num);
	}
}

/*This functions prunes the tree to increase the prediction accuracy*/
void prune2(Node** head, Node** node, double& accr, auto& inp, auto& attr_val_num){
// this pruning technique may slightly decrease the accuracy
	if ((*node)->leaf){
		return;
	}
	else{
		for (int i=1; i<=attr_val_num[(*node)->attr]; i++){
			prune2(head, &((*node)->next[i]), accr, inp, attr_val_num);
		}
		(*node)->leaf = true;
		auto t = accuracy(inp, *head);
		double new_accr = get<0>(t);
		// cout<<accr<<" ";
		if (new_accr > accr){
			accr = new_accr;
		}
		else{
			(*node)->leaf = false;
		}
		return;
	}
}
/*This class contains functions to implement ID3 algorithm*/
class ID3{
	public:
		/*This function inputs data from a file to a 2-d vector
		of strings*/
		vector<vector<string>> input_data(string file){
			vector<vector<string>> training_inp;
			string line;
			ifstream infile(file);
			int k = 0;
			while(getline(infile, line)){
				string word="";
				vector<string> tokens;
				for (int i=0;i<line.length();i++){
					if (line[i]!=' '){
						word+=line[i];
					}
					if (line[i]==' '){
						tokens.pb(word);
						word = "";
					}
					
				}
				tokens.pb(word);
				training_inp.pb(tokens);
			}
			cout<<"done inputting file..."<<endl;
			return training_inp;
		}

		/*checks whether all the training examples are positive or negative*/
		bool check_all_exp(const auto& training_inp, int check){
			for (auto row: training_inp){
				if (row[TARGET] != to_string(check)){
					return false;
				}
			}
			return true;
		}

		/*Finds whether most of the examples of training input are
		positive or negative*/
		bool most_common_value(const auto& training_inp){
			int count0 = 0, count1 = 0;
			for (auto row: training_inp){
				if (row[TARGET] == "0"){
					count0++;
				}
				else count1++;
			}
			return count1>count0;
		}

		/*This function calculates the entropy of the training input*/
		double entropy(const auto& in){
			double pos = 0, neg = 0;
			double e=0;
			
			for (auto row: in){
				if (row[TARGET] == "1") pos++;
				else neg++;
			}
			pos /= (pos+neg);
			neg /= (pos+neg);
			
			if (neg!=0) e += -double(neg)*log2(neg);
			if (pos!=0) e += -double(pos)*log2(pos);
			
			return e;
		}

		/*This function calculates information gain value OR 
		information gain ratio wrt `attr` depending on whether
		USE_SPLIT_INFO is set or not*/
		double inf_gain(const auto& training_inp, int attr){
			map<int, vector<vector<string>>> mp;

			for (auto row: training_inp){
				mp[get_attr_num(attr, row)].pb(row);
			}
			double E = entropy(training_inp);
			double splitInfo = 0;
			
			for (auto i: mp){
				double e = entropy(i.second);
				E -= ((double)i.second.size()/training_inp.size())*e;
				double ratio = ((double)i.second.size()/training_inp.size());
				splitInfo += (-ratio*log2(ratio));
			}
			if (USE_SPLIT_INFO)return (E/splitInfo);
			else return E;
		}

		/*implementation of ID3 algorithm*/
		void id3(auto& training_inp, auto attr_val_num, Node* node){
			// check if all are positive
			if (check_all_exp(training_inp, true)){
				node->leaf = true;
				node->val = true;
				return;
			}
			// check if all are negative
			if (check_all_exp(training_inp, false)){
				node->leaf = true;
				node->val = false;
				return;
			}
			// find the most common value
			if (attr_val_num.size() == 0){
				node->leaf = true;
				node->val = most_common_value(training_inp);
				return;
			}

			// find highest information gain atrribute
			double max_gain = 0;
			int max_gain_attr = (*attr_val_num.begin()).first;
			for (auto i: attr_val_num){
				double gain = inf_gain(training_inp, i.first);
				if (gain > max_gain){
					max_gain = gain;
					max_gain_attr = i.first;
				}
			}
			int n = attr_val_num[max_gain_attr];
			node->leaf = false;		
			node->attr = max_gain_attr;
			node->next = new Node* [n+1];
			// iterate through the max gain attribute values
			for (int i=1; i<=n;i++){
				Node* newNode = new Node();
				node->next[i] = newNode;

				vector<vector<string>> newData;
				// create new training exp for that child
				for (int j=0;j<training_inp.size();j++){
					if (get_attr_num(max_gain_attr, training_inp[j]) == i){
						newData.pb(training_inp[j]);
					}
				}
				// if size of new dataset if zero make it leaf
				if (newData.size() == 0){
					newNode->leaf = true;
					newNode->val = most_common_value(training_inp);
				}
				// otherwise recurse and delete max gain attr from
				// map so that it won't be used again
				else{
					attr_val_num.erase(max_gain_attr);
					id3(newData, attr_val_num, newNode);
				}
			}
		}

	ID3(){;}
	ID3(string file){;}
	~ID3(){;}

};
/*This function created a random forest*/
vector<Node*> create_random_forest(auto& dataset, auto& attr_val_num, int randomTrees){
	vector<Node*> randomForest;
	srand(time(NULL));
	for (int i = 0;i<randomTrees;i++){
		// make random training examples
		int training_exp = dataset.size();
		vector<vector<string>> random_training_exp;
		map<int, int> random_attributes;
		while(training_exp--){
			int random_num = rand()%dataset.size();
			random_training_exp.pb(dataset[random_num]);		
		}
		
		// make random attributes
		int attr_number = 10;
		set<int> selected_rand_nums;
		while(attr_number){
			int random_num = rand()%attr_val_num.size();
			if (selected_rand_nums.find(random_num) == selected_rand_nums.end()){
				selected_rand_nums.insert(random_num);
				attr_number--;
				random_attributes[random_num] = attr_val_num[random_num];
			}
		}
		// create and store the head node
		Node* head = new Node();
		ID3 i1;
		i1.id3(random_training_exp, random_attributes, head);
		randomForest.pb(head);
		cout<<"created tree "<<i+1<<" of "<<randomTrees<<endl;
	}
	return randomForest;
}
/*This function finds accuracy of prediction in the 
random forest along with precion and recall values 
using false/true - pos/neg */
auto random_accuracy(auto& dataset, auto& randomForest){
	double true_pos = 0, false_pos = 0;
	double true_neg = 0, false_neg = 0;
	for (auto row: dataset){
		int count0=0, count1=0;
		for (Node* randomTree: randomForest){
			int predicted_val = predict(row, randomTree);
			if (predicted_val == 1) count1++;
			else count0++;
		}
		if (count0>count1 && stoi(row[TARGET])==1) false_neg+=1;
		else if(count0<count1 && stoi(row[TARGET])==0) false_pos+=1;
		else if (count0>count1 && stoi(row[TARGET])==0) true_pos+=1;
		else if (count0<count1 && stoi(row[TARGET])==1) true_neg+=1;
	}
	double accr = (true_pos+true_neg)/(false_pos+false_neg+true_neg+true_pos);
	double precision = true_pos/(true_pos+false_pos);
	double recall = true_pos/(true_pos+false_neg);
	auto t = make_tuple(accr, precision, recall);
	return t;
}

int main(){
	map<int, int> attr_val_num, temp;
		attr_val_num[0] = 3;
		attr_val_num[1] = 8;
		attr_val_num[2] = 3;
		attr_val_num[3] = 16;
		attr_val_num[4] = 3;
		attr_val_num[5] = 7;
		attr_val_num[6] = 14;
		attr_val_num[7] = 6;
		attr_val_num[8] = 5;
		attr_val_num[9] = 2;
		attr_val_num[10] = 3;
		attr_val_num[11] = 3;
		attr_val_num[12] = 3;
		attr_val_num[13] = 41;
	temp = attr_val_num;
	
	string trainfile = "train_formatted";
	string validfile = "valid_formatted";
	string testfile  = "test_formatted";
	ID3 i1(trainfile);
	ID3 i2(validfile);
	ID3 i3(testfile);
	
	vector<vector<string>> training_inp = i1.input_data(trainfile);
	vector<vector<string>> validating_inp = i2.input_data(validfile);
	vector<vector<string>> testing_inp = i3.input_data(testfile);
	
	Node* head = new Node();
	i1.id3(training_inp, temp, head);
	// print_decision_tree(head, attr_val_num, head->attr);

	auto t =  accuracy(testing_inp, head);
	double accr = get<0>(t);
	double prec = get<1>(t);
	double recl = get<2>(t);
	// 1 DECISION TREE
	cout<<"Accuracy with ID3 on single decision tree on testing data is: "<<accr*100<<"%"<<endl;
	cout<<"Precision with ID3 on single decision tree on testing data is: "<<prec*100<<"%"<<endl;
	cout<<"Recall with ID3 on single decision tree on testing data is: "<<recl*100<<"%"<<endl;
	cout<<"F-Measure with ID3 on single decision tree on testing data is: "<<(2*prec*recl)/(prec+recl)<<endl;

	// REDUCED PRUNNING ERROR 
	cout<<endl<<endl;
	cout<<"prunning..."<<endl;
	t = accuracy(validating_inp, head);
	accr = get<0>(t);
	// prune1() is overloaded - pass by ref or pass by val
	// head = prune1(head, accr, validating_inp, attr_val_num);
	prune1(&head, accr, validating_inp, attr_val_num);
	// prune2(&head, &head, accr, validating_inp, attr_val_num);
	cout<<"prunned..."<<endl;
	
	t = accuracy(testing_inp, head);
	accr = get<0>(t);
	prec = get<1>(t);
	recl = get<2>(t);
	cout<<"Accuracy on testing data after prunning is: "<<accr*100<<"%"<<endl;
	cout<<"Precision on tesing data after prunning is: "<<prec*100<<"%"<<endl;
	cout<<"Recall on tesing data after prunning is: "<<recl*100<<"%"<<endl;
	cout<<"F-Measure on tesing data after prunning is: "<<(2*prec*recl)/(prec+recl)<<endl;
	cout<<endl<<endl;

	// RANDOM FOREST
	int randomTrees = 70;
	cout<<"generating random forest..."<<endl;
	vector<Node*> randomForest = create_random_forest(training_inp, attr_val_num, randomTrees);
	t =  random_accuracy(testing_inp, randomForest);
	double rand_accr = get<0>(t);
	double rand_prec = get<1>(t);
	double rand_recl = get<2>(t);
	cout<<"Accuracy with random forest on test data: "<<rand_accr*100<<"%"<<endl;
	cout<<"Precision with random forest on test data: "<<rand_prec*100<<"%"<<endl;
	cout<<"Recall with random forest on test data: "<<rand_recl*100<<"%"<<endl;
	cout<<"F-Measure with random forest on test data: "<<(2*rand_prec*rand_recl)/(rand_prec + rand_recl)<<endl;
}