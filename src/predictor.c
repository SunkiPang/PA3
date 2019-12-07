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
	g_hash_table_foreach(read, print_prob, 0x0);
	fclose(f);
	
	

	char ch[STRING_SIZE];
	while(!feof(stdin)){
		printf("Enter the message : ");
		scanf("%s",ch);
	}




		/*
			   여기는 불러오는 부분만 해결되면 atof로 바꿔서 더해주는 부분입니다.
			if  (gp == NULL){
				printf("check4\n");
				continue;
			}
			
			else {
				sum = sum + atof(g_hash_table_get_value(read, strdup(t)));
			}	*/							      
		//free(temp);
}

