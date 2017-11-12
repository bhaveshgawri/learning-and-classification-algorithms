#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define inf INT_MAX
#define FEATURE_NUM 17
#define TRAINING_INSTANCE_NUM 101

typedef struct features{
	int size;
	int value[10];
}features;

typedef struct instance{
	int label;
	int feature[FEATURE_NUM];
}instance;

typedef struct boundary{
	instance hypothesis;
	struct boundary* next;
}boundary;

typedef struct version_space{
	boundary* generic_head;
	boundary* specific_head;
}version_space;

// inf - ANY VALUE
// -inf - NULL VALUE

/*This function takes input and formats it for further use*/
void input(instance training_data[TRAINING_INSTANCE_NUM]){
	char string[100];
	for (int i = 0; i < TRAINING_INSTANCE_NUM; i++){
		scanf("%s", string);
		for (int j = 0, k = 0; j < strlen(string); j++){
			if (string[j] >= '0' && string[j] <= '9'){
				training_data[i].feature[k++] = string[j] - '0';
			}
		}
	}
}

/*This function prints the training data*/
void print(instance training_data[TRAINING_INSTANCE_NUM]){
	for (int i = 0; i < TRAINING_INSTANCE_NUM; i++){
		for (int j = 0; j < FEATURE_NUM; j++){
			printf("%d ", training_data[i].feature[j]);
		}
		printf(" : LABEL: %d\n", training_data[i].label);
	}
}

/*This function creates a new hypothesis*/
boundary* createHypothesis(){
	boundary* b_hypothesis = (boundary*)malloc(sizeof(boundary));	
	b_hypothesis->next = NULL;
	return b_hypothesis;
}

/*This function initializes a new version space
giving inf to general boundary
and -inf to specific boundary*/
version_space initialize(version_space vs){
	boundary* ghead = createHypothesis();
	boundary* shead = createHypothesis();
	
	vs.generic_head = ghead;
	vs.specific_head = shead;
	
	// label is useless for hypothesis, req for instances only
	ghead->hypothesis.label = -1;
	shead->hypothesis.label = -1;
	
	for (int i = 0; i < FEATURE_NUM - 1; i++){
		ghead->hypothesis.feature[i] = inf;
		shead->hypothesis.feature[i] = -inf;
	}
	ghead->next = NULL;
	shead->next = NULL;
	return (version_space){.generic_head = ghead, .specific_head = shead};
}

/*This function preprocosses the training data 
specifically for each `type`*/
void classify(instance training_data[TRAINING_INSTANCE_NUM], int type){
	for (int j = 0; j < TRAINING_INSTANCE_NUM; j++){
		if (training_data[j].feature[FEATURE_NUM-1] == type){
			training_data[j].label = 1;
		}
		else{
			training_data[j].label = 0;
		}
	}
}

// This function prints a linked list
void printlist(boundary* list){
	while(list){
		printf("%d\n", list->hypothesis.feature[0]);
		list = list->next;
	}
}

// This functions checks whether 2 instances are equal or not
int equal(instance h1, instance h2){
	for (int i = 0; i < FEATURE_NUM - 1; i++){
		if (h1.feature[i]!=h2.feature[i]){
			return 0;
		}
	}
	return 1;
}
// This function copies a linked list to other list
void copylist(boundary** newList, boundary* to_copy){
	if (to_copy == NULL){
		*newList = NULL;
	}
	else{
		boundary* temp = NULL;
		while(to_copy){
			boundary* newNode = createHypothesis();
			newNode->hypothesis = to_copy->hypothesis;
			to_copy = to_copy->next;
			if (temp == NULL){
				temp = newNode;
				*newList = temp;
			}
			else{
				temp->next = newNode;
				temp = temp->next;
			}
		}
	}
}

// This function removes a element from a list
void listremove(boundary** list, instance h){
	boundary *temp = *list, *prev;
	if (temp && equal(temp->hypothesis, h)){
		*list = temp->next;
		free(temp);
		return;
	}
	while (temp && !equal(temp->hypothesis, h)){
		prev = temp;
		temp = temp->next;
	}
	if (temp == NULL) return;
	prev->next = temp->next;
	free(temp);
}

// This function adds a new element to the list
void listappend(boundary** list, instance h){
	boundary* newNode = createHypothesis();
	newNode->hypothesis = h;
	boundary* last = *list;
	if (*list == NULL){
		*list = newNode;
		return;
	}
	while(last->next){
		if (equal(last->hypothesis, h)){
			free(newNode);
			return;
		}
		last = last->next;
	}
	if (equal(last->hypothesis, h)){
		free(newNode);
		return;
	}
	last->next = newNode;
	return;
}

// This function deletes a whole linked list
void deletelist(boundary** list){
	boundary* current = *list;
	boundary* next;
	while (current){
		next = current->next;
		free(current);
		current = next;
	}
	*list = NULL;
}

// This fuctions calculates the version space
void find_version_space(instance training_data[TRAINING_INSTANCE_NUM],
						version_space vs,
						features feat[FEATURE_NUM], int type){
	classify(training_data, type);
	vs = initialize(vs);

	instance g, s;
	// for each example
	for (int i = 0; i < TRAINING_INSTANCE_NUM; i++){
		// positive exp
		if (training_data[i].label == 1){
			
			// remove inconsistant instance from general set 
			boundary* gcopy = NULL;
			copylist (&gcopy, vs.generic_head);
			boundary* iterator = vs.generic_head;
			while(iterator){
				g = iterator -> hypothesis;
				iterator = iterator -> next;
				int flag = 0;
				for (int j = 0; j < FEATURE_NUM - 1; j++){
					if (g.feature[j] != training_data[i].feature[j]
						&& g.feature[j] != inf){
						flag = 1;
						break;
					}
				}
				if (flag == 1){
					listremove(&gcopy, g);
				}
			}
			copylist(&vs.generic_head, gcopy);

			// minimum_generalize the specific set
			s = vs.specific_head->hypothesis;
			listremove(&vs.specific_head, s);
			for (int j = 0; j < FEATURE_NUM - 1; j++){
				if (s.feature[j] == -inf){
					s.feature[j] = training_data[i].feature[j];
				}
				else if (s.feature[j] != training_data[i].feature[j]){
					s.feature[j] = inf;
				}
			}
			listappend(&vs.specific_head, s);
		}
		// negative exp
		else if (training_data[i].label == 0){
			boundary* gcopy = NULL;
			boundary* iterator = vs.generic_head;
			s = vs.specific_head->hypothesis;

			while(iterator){
				g = iterator -> hypothesis;
				int flag = 0;
				for (int j = 0; j < FEATURE_NUM - 1; j++){
					if (g.feature[j] != training_data[i].feature[j]
						&& g.feature[j] != inf){
						flag = 1;
						break;
					}
				}
				// if consistent append
				if (flag == 1){
					listappend(&gcopy, g);
				}
				// specilize otherwise
				else{
					instance new_g_hyp;
					for (int j = 0; j < FEATURE_NUM - 1; j++){
						if (g.feature[j] = inf){
							new_g_hyp = g;
							for (int k = 0; k < feat[j].size; k++){
								if (feat[j].value[k] != training_data[i].feature[j]){
									new_g_hyp.feature[j] = feat[j].value[k];
									// ok if new_genreal_hyp is more general than
								    // and consistent with specific hyp
									int ok = 1;
									for (int l = 0; l < FEATURE_NUM - 1; l++){
										if (new_g_hyp.feature[l] != inf &&
											s.feature[l] != -inf &&
											new_g_hyp.feature[l] != s.feature[l]){
											ok = 0;
											break;
										}
									}
									if (ok){
										listappend(&gcopy, new_g_hyp);
									}
								}
							}
						}
					}
				}
				iterator = iterator -> next;
			}
			// empty general
			deletelist(&vs.generic_head);
			boundary* to_remove = NULL;
			
			// find hypothesis which are less general than some hypothesis in G
			boundary* intertor1 = gcopy;
			boundary* iterator2;
			while(intertor1){
				instance ins1 = intertor1->hypothesis;
				boundary* iterator2 = gcopy;
				while(iterator2){
					instance ins2 = iterator2->hypothesis;
					if (equal(ins1, ins2) == 0){
						int ok = 1;
						for (int j = 0; j < FEATURE_NUM - 1; j++){
							if (ins1.feature[j] != inf &&

								// could only be used because of condition none or one or any
								ins1.feature[j] != ins2.feature[j]){
								ok = 0;
								break;
							}
						}
						if (ok){
							listappend(&to_remove, ins2);
						}
					}
					iterator2 = iterator2->next;
				}
				intertor1 = intertor1->next;
			}
			
			// remove hypothesis inconsistant this specific boundary
			// remove less general hypothesis from G
			intertor1 = gcopy;
			while(intertor1){
				instance ins1 = intertor1->hypothesis;
				int ok = 1;
				iterator2 = to_remove;
				while(iterator2){
					instance ins2 = iterator2->hypothesis;
					if (equal(ins1, ins2)){
						ok = 0;
						break;
					}
					iterator2 = iterator2->next;
				}
				if (ok){
					listappend(&vs.generic_head, ins1);
				}
				intertor1 = intertor1->next;
			}
			deletelist(&to_remove);
			deletelist(&gcopy);
		}
	}		
	
	// print hypothesis
	printf("specific\n");
	boundary* iterator = vs.specific_head;
	for (int j=0; j < FEATURE_NUM-1; j++){
		int p = iterator->hypothesis.feature[j];
			if (p==inf)
				printf("? ");
			else
				printf("%d ", p);
	}
	printf("\ngeneral\n");
	iterator = vs.generic_head;
	while (iterator){
		for (int j = 0; j < FEATURE_NUM - 1; j++){
			int p = iterator->hypothesis.feature[j];
			if (p==inf)
				printf("? ");
			else
				printf("%d ", p);
		}
		iterator = iterator -> next;
		printf("\n");
	}
}
// initialize the features of attributs
void initialize_features(features feat[FEATURE_NUM]){
	int x = 0;
	for (int i = 0; i <= 15; i++){
		if (i == 12)
			continue;	
		feat[i].size = 2;
		feat[i].value[0] = x;
		feat[i].value[1] = 1 - x;
	}
	
	feat[12].size = 5;
	for (int i = 0; i < feat[12].size; i++){
		feat[12].value[i] = 2*i;
	}
	feat[16].size = 7;
	for (int i = 0; i < feat[16].size; i++){
		feat[16].value[i] = i + 1;
	}
}

int main(){
	version_space vs;
	instance training_data[TRAINING_INSTANCE_NUM];
	features feat[FEATURE_NUM];
	initialize_features(feat);
	input (training_data);
	for (int type = 1; type <= 7; type++){
		printf("TYPE: %d\n", type);
		find_version_space (training_data, vs, feat, type);
		printf("\n");
	}
	// print (training_data);
}