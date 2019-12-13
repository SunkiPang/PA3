#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <math.h>
#include "../include/libstemmer.h"
#define STRING_SIZE 128

void sum_prob(float, float);
void makeHash(FILE *, GHashTable *);
float cal_prob();
void print_prob (gpointer key, gpointer value, gpointer userdata) {
        char * t = key ;
        char * d = value ;
        printf("(%s, %s)\n", t, d) ;
}
float neg_sum, non_sum;
int word_count = 0;
int main() {
        FILE *f = fopen("../data/model.csv", "r");
        GHashTable *read = g_hash_table_new(g_str_hash, g_str_equal);
        makeHash(f, read);
        // 제대로 hash 생성되었는지 확인
//              g_hash_table_foreach(read, print_prob, 0x0);
        fclose(f);
		struct sb_stemmer * stemmer;
		//stemmer = sb_stemmer_new("english", 0x0);
		printf("word count : %d\n\n",word_count);
        while(!feof(stdin)) {
                neg_sum = 0;
                non_sum = 0;
                char *temp;
				const char *temp2;
				char ch[STRING_SIZE];
                char words[STRING_SIZE][STRING_SIZE] = {0,};
				printf("Enter the message : ");
                gets(ch);
                int count = 0;
                temp = strtok(ch, " ");
                while(temp != NULL) {
                        //입력받은 문장을 단어로 쪼개서 배열에 입력
                        //temp2 = sb_stemmer_stem(stemmer, temp, strlen(temp));
						//printf("%s\n",temp2);
						strcpy(words[count],temp);
                        temp = strtok(NULL, " ");
                        count++;
                }
                for (int i = 0; i < count; i++) {
                        //내용 확인용 printf
                //      printf("%s\n",words[i]);
                        char * returned;
                        returned = g_hash_table_lookup(read, strdup(words[i]));
                        if(returned != NULL) {
                        //      printf("%s %s\n",words[i], returned);
                                char word4Prob[STRING_SIZE];
                                strcpy(word4Prob, returned);
                                if(word4Prob != NULL) {
                                        char* tmp4Prob = strtok(word4Prob, ", ");
                                        float prob[2];
                                        for (int i = 0; tmp4Prob != NULL ; i++) {
                                                prob[i] = atof(tmp4Prob);
                                                tmp4Prob = strtok(NULL, ", ");
                                        }
                        //              printf("neg: %f, non_neg[1]: %f\n", prob[0], prob[1]);
                                        sum_prob(prob[0], prob[1]);
                                }
                        }
                }
                        float result = cal_prob();
                        result = result * 100;
                        printf("%f\t", result);
                        if(result > 80){
                                printf("Negative Sentence\n");
                        } else {
                                printf("Non-negative Sentence\n");
                        }

                }
                return 0;
}
void sum_prob(float neg, float non) {
        neg_sum = neg_sum + log10f(neg);
        non_sum = non_sum + log10f(non);
        //printf("%f %f\n", neg_sum, non_sum);
}
//최종값 = p(M|s)/(p(M|S)+p(M|notS))
//prob는 최종 확률값
float cal_prob() {
        float neg = pow(10.0, neg_sum);
        float non = pow(10.0, non_sum);
        float prob = 0;
        return prob = neg / (neg + non);
}
void makeHash(FILE *f, GHashTable *read) {
        char * line = 0x0 ;
        size_t r ;
        size_t n = 0 ;
        while (getline(&line, &n, f) >= 0) {
                char *t;
                char * _line = line ;
                char *prob;
                t = strtok(line, ", ");
                prob = strtok(NULL, "\n");
                g_hash_table_insert(read, strdup(t), strdup(prob));
                free(_line);
                line = 0x0;
				word_count++;
        }
}
