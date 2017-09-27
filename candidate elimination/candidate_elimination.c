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

typedef struct version_space{
	instance specific_boundary;
	instance general_boundary;
}version_space;

void print(instance training_data[TRAINING_INSTANCE_NUM]){
	for (int i = 0; i < TRAINING_INSTANCE_NUM; i++){
		for (int j = 0; j < FEATURE_NUM; j++){
			printf("%d ", training_data[i].feature[j]);
		}
		printf(" : LABEL: %d\n", training_data[i].label);
	}
}

void initialize(version_space boundary[LABEL_TYPES]){
	for (int type = 1;type <= LABEL_TYPES; type++){
		(boundary[type]).general_boundary = (instance){.label = 1};
		(boundary[type]).specific_boundary = (instance){.label = 1};
		for (int i = 0;i < FEATURE_NUM; i++){
			(boundary[type].general_boundary).feature[i] = inf;
			(boundary[type].specific_boundary).feature[i] = -inf;
			// printf("%d %d\n", (boundary[type].general_boundary).feature[i], (boundary[type].specific_boundary).feature[i]);
		}
	}
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

void classify(instance training_data[TRAINING_INSTANCE_NUM]){
	for (int type = 1; type <= LABEL_TYPES; type++){
		instance general_boundary;
		instance specific_boundary;
		for (int j = 0; j < TRAINING_INSTANCE_NUM; j++){
			if (training_data[j].feature[FEATURE_NUM-1] == type){
				training_data[j].label = 1;
			}
			else{
				training_data[j].label = 0;
			}
		}
		// find_version_space(training_data);
	}
}

int main(){
	version_space boundary[LABEL_TYPES+1]; 
	instance training_data[TRAINING_INSTANCE_NUM]; 
	
	initialize (boundary);
	input (training_data);
	classify (training_data);
	print (training_data);
}
