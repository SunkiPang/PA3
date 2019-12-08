#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <math.h>
#define STRING_SIZE 128
void makeHash(FILE *, GHashTable *);
char word[STRING_SIZE];
void print_prob (gpointer key, gpointer value, gpointer userdata) {
	char * t = key ;
	char * d = value ;
	printf("(%s, %s)\n", t, d) ;
}
int main() {
	FILE *f = fopen("./model_space.csv", "r");
	GHashTable *read = g_hash_table_new(g_str_hash, g_str_equal);
	makeHash(f, read);
	// 제대로 hash 생성되었는지 확인
	//g_hash_table_foreach(read, print_prob, 0x0);
	fclose(f);
	char *temp;
	char ch[STRING_SIZE];
	while(!feof(stdin)) {
		char words[STRING_SIZE][STRING_SIZE] = {0,};
		printf("Enter the message : ");
		gets(ch);
		temp = strtok(ch, " ");
		int count = 0;
		while(temp != NULL) {
			//입력받은 문장을 단어로 쪼개서 배열에 입력
			strcpy(words[count],temp);
			temp = strtok(NULL, " ");
			count++;
		}
		for (int i = 0; i < count; i++) {
			char * returned;
			returned = g_hash_table_lookup(read, strdup(words[i]));
			if(returned != NULL) {
				printf("%s %s\n",words[i], returned); // to check which word and its pro
				char word4Prob[STRING_SIZE];
				strcpy(word4Prob, returned);
				if(word4Prob != NULL) {
					char* tmp4Prob = strtok(word4Prob, " ");
					float prob[2];
					for (int i = 0; tmp4Prob != NULL ; i++) {
						prob[i] = atof(tmp4Prob);
						tmp4Prob = strtok(NULL, " ");
					}
					printf("prob[0]: %f, prob[1]: %f\n", prob[0], prob[1]);
				}
			}
		}
	}
}
void makeHash(FILE *f, GHashTable *read) {
	char * line = 0x0 ;
	size_t r ;
	size_t n = 0 ;
	while (getline(&line, &n, f) >= 0) {
		char *t;
		char * _line = line ;
		char *prob;
		t = strtok(line, " ");
		prob = strtok(NULL, "\n");
		g_hash_table_insert(read, strdup(t), strdup(prob));
		free(_line);
		line = 0x0;
	}
}

