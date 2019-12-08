#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

#define STRING_SIZE 128
void probToFloat(char*);
 char word[STRING_SIZE];

  void print_prob (gpointer key, gpointer value, gpointer userdata)
{
        char * t = key ;
        char * d = value ;

        printf("(%s, %s)\n", t, d) ;
}
        char *temp;
        char ch[STRING_SIZE];

int main(){
        FILE *f = fopen("./model.csv", "r");
        GHashTable *read = g_hash_table_new(g_str_hash, g_str_equal);
        char * line = 0x0 ;
        size_t r ;
        size_t n = 0 ;
        while (getline(&line, &n, f) >= 0) {
                char *t;
                char * _line = line ;
                char *prob;

                t = strtok(line, "|");
                prob = strtok(NULL, "\n");

                g_hash_table_insert(read, strdup(t), strdup(prob));
                free(_line);

                line = 0x0;
        }
        // 제대로 hash 생성되었는지 확인
//      g_hash_table_foreach(read, print_prob, 0x0);
        fclose(f);


        int idx = 0;    // 후에 몇개의 확률이 있는지 알고 for문으로 몇번 돌릴지 알려주기 위해 만든 변수
       
		while(!feof(stdin)){
			char words[STRING_SIZE][STRING_SIZE] = { 0, };
			printf("Enter the message : ");
            gets(ch);
            temp = strtok(ch, " ");
			int count = 0;
			
			while(temp != NULL){ //입력받은 문장을 단어로 쪼개서 배열에 입력
				strcpy(words[count],temp);
				temp = strtok(NULL, " ");
				count++;
			}
			
			for (int i = 0; i < count; i++){ //내용 확인용 printf
				printf("%s\n",words[i]);
				char * returned;
				returned = g_hash_table_lookup(read, strdup(words[i]));
			
				if(returned != NULL){
					printf("%s %s\n",words[i], returned); // to check which word and its pro
					char word4Prob[STRING_SIZE];
					strcpy(word4Prob, returned);

					if(word4Prob != NULL){
						char* tmp4Prob = strtok(word4Prob, "|"); // 1. |을 토대로 나누는거 strtok말고 다른 방법으로
						float prob[2];
						
						for(int i = 0; tmp4Prob != NULL ; i++){
							prob[i] = atof(tmp4Prob);
							tmp4Prob = strtok(NULL, "|"); // 2.
						}
						printf("prob[0]: %f, prob[1]: %f\n", prob[0], prob[1]);
					}
				}
			}
		}
}



                /*
                           여기는 불러오는 부분만 해결되면 atof로 바꿔서 더해주는 부분입니다.
                        if  (gp == NULL){
                                printf("check4\n");
                                continue;
                        }

                        else {
                                sum = sum + atof(g_hash_table_get_value(read, strdup(t)));
                        }       */
                //free(temp);

