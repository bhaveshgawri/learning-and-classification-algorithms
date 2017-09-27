/*
aardvark,1,0,0,1,0,0,1,1,1,1,0,0,4,0,0,1,1
antelope,1,0,0,1,0,0,0,1,1,1,0,0,4,1,0,1,1
bass,0,0,1,0,0,1,1,1,1,0,0,1,0,1,0,0,4
bear,1,0,0,1,0,0,1,1,1,1,0,0,4,0,0,1,1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define inf INT_MAX
#define LABEL_TYPES 1 // later change to 7
#define FEATURE_NUM 17
#define TRAINING_INSTANCE_NUM 4 // later change to 101

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

void print(instance training_data[TRAINING_INSTANCE_NUM]){
	for (int i = 0; i < TRAINING_INSTANCE_NUM; i++){
		for (int j = 0; j < FEATURE_NUM; j++){
			printf("%d ", training_data[i].feature[j]);
		}
		printf(" : LABEL: %d\n", training_data[i].label);
	}
}

boundary* createHypothesis(){
	boundary* b_hypothesis = (boundary*)malloc(sizeof(boundary));	
	b_hypothesis->next = NULL;
	return b_hypothesis;
}

version_space initialize(version_space vs[LABEL_TYPES], int type){
	boundary* ghead = createHypothesis();
	boundary* shead = createHypothesis();
	
	vs[type].generic_head = ghead;
	vs[type].specific_head = shead;
	
	// label is useless for hypothesis, req for instances only
	ghead->hypothesis.label = -1;
	shead->hypothesis.label = -1;
	
	for (int i = 0;i < FEATURE_NUM; i++){
		ghead->hypothesis.feature[i] = inf;
		shead->hypothesis.feature[i] = -inf;
	}
	ghead->next = NULL;
	shead->next = NULL;
	return (version_space){.generic_head = ghead, .specific_head = shead};
}

void input(instance training_data[TRAINING_INSTANCE_NUM]){
	char string[100];
	for (int i=0; i<TRAINING_INSTANCE_NUM; i++){
		scanf("%s", string);
		for (int j = 0, k = 0; j < strlen(string); j++){
			if (string[j] >= '0' && string[j] <= '9'){
				training_data[i].feature[k++] = string[j] - '0';
			}
		}
	}
}

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

void find_version_space(instance training_data[TRAINING_INSTANCE_NUM], version_space vs[LABEL_TYPES+1]){
	for (int type = 1;type <= LABEL_TYPES;type++){
		classify(training_data, type);
		version_space heads = initialize(vs, type);
		printf("%d\n", heads.generic_head->hypothesis.label);
	}
}


int main(){
	version_space vs[LABEL_TYPES+1];
	instance training_data[TRAINING_INSTANCE_NUM]; 
	input (training_data);
	find_version_space (training_data, vs);
	// print (training_data);
}