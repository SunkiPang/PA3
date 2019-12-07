#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

#define STRING_SIZE 128

 char word[STRING_SIZE];

  void print_prob (gpointer key, gpointer value, gpointer userdata)
{
	char * t = key ;
	char * d = value ;

	printf("(%s, %s)\n", t, d) ;
}

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
		prob = strtok(NULL, "|");
		//printf("%s\t", strdup(prob));
		printf("%s %s\n", t, prob);
		
		g_hash_table_insert(read, strdup(t), strdup(prob));
		free(_line);
		
		line = 0x0;
	}
	//g_hash_table_foreach(read, print_prob, 0x0);
	fclose(f);
	
	
	char * line2 = 0x0 ;
	FILE *f2 = fopen("../data/test.non-negative.csv", "r");
	size_t n2 = 0 ;
	gpointer gp;
	float result[100]; 



	while (getline(&line2, &n2, f2) >= 0) {
		char *t;
		char * _line = line ;
		//double * temp;
		double sum = 0.0;

		t = strtok(line2, " ");
		while (t != NULL) {
			//printf("%s\n", t);
			double * temp = malloc(sizeof(double));
			temp = g_hash_table_lookup(read, t); //hash table에 t(단어)가 있는지 확인
			sum = sum + *temp;	
			printf("%lf", sum);
			/*
				test파일을 불러서 단어 하나씩 나눠놓는 것까지는 구현했습니다.
				이제 해야 할 것이 이 각 단어를 key로 사용해서 hashtable의 value값을 불러와야하는데 
				위의 g_hash_table_foreach처럼 따로 함수가 있는 것 같은데 그걸 모르겠네요ㅠㅠ
				이 부분이 해결되면 그 불러온 값을 sum으로 더하기만 하면 완성입니다.
			   */
			
			/*
			   여기는 불러오는 부분만 해결되면 atof로 바꿔서 더해주는 부분입니다.
			if  (gp == NULL){
				printf("check4\n");
				continue;
			}
			
			else {
				sum = sum + atof(g_hash_table_get_value(read, strdup(t)));
			}	*/							      
			gp = NULL;
			t = strtok(NULL," ");
			//free(temp);
		}
	}
}

