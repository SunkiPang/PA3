#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/libstemmer.h"

#define SIZE 27

void print_counter (gpointer key, gpointer value, gpointer userdata){
    char * t = key ;
    int * d = value ;

    printf("(%s, %d)\n", t, *d) ;
}

void print_counter_double (gpointer key, gpointer value, gpointer userdata){
    char * t = key ;
    float * d = value ;

    printf("(%s, %f)\n", t, *d) ;
}

void removeChar (char* string, int index){
  for (int i = index; i < strlen(string); i++){
    *(string+i) = *(string+i+1);
  }
}

int removeSmallEle (gpointer key, gpointer value, gpointer userdata){
  int * d = value ;
  if (*d < 10)
    return 1;
  return 0;
}

void combiningWords (GHashTable* non_table, GHashTable* neg_table, GHashTable* combine){
  GList* non_keys = g_hash_table_get_keys (non_table);
  GList* neg_keys = g_hash_table_get_keys (neg_table);
  float *input_num;
  float non_float_size = 0., neg_float_size = 0.;

  while (non_keys != NULL){
    input_num = malloc(sizeof(float)) ;
    char* key = non_keys->data;
    non_keys = non_keys->next;
    int *non_size = (int *) g_hash_table_lookup(non_table, key);
    int *neg_size = (int *) g_hash_table_lookup(neg_table, key);

    if (neg_size == NULL){
      non_float_size = *(non_size);
      neg_float_size = 0.;
    }else{
      non_float_size = *(non_size);
      neg_float_size = *(neg_size);
    }
    *input_num = (neg_float_size)/(non_float_size + neg_float_size);
    g_hash_table_insert(combine, strdup(key), input_num);
  }
  while (neg_keys != NULL){
    input_num = malloc(sizeof(float)) ;
    char* key = neg_keys->data;
    neg_keys = neg_keys->next;
    int *combine_size = (int *) g_hash_table_lookup(combine, key);
    int *neg_size = (int *) g_hash_table_lookup(neg_table, key);
    if (combine_size == NULL){
      *input_num = 1.;
      g_hash_table_insert(combine, strdup(key), input_num);
    }
  }
}

void writeModel (FILE* model, GHashTable* combine){
  GList* model_keys = g_hash_table_get_keys (combine);
  float model_size = 0.;

  while (model_keys != NULL){
    char* key = model_keys->data;
    model_keys = model_keys->next;
    model_size = *((float *) g_hash_table_lookup(combine, key));

    fprintf(model, "%s, %f, %f\n", key, model_size, 1.0-model_size);
  }
}

int main () {
  FILE * neg_tr_data = fopen("../data/train.negative.csv", "r") ;
  FILE * non_tr_data = fopen("../data/train.non-negative.csv", "r") ;
  FILE * model = fopen("../data/model.csv", "w") ;
  GHashTable *neg_tr = g_hash_table_new(g_str_hash, g_str_equal) ;
  GHashTable *non_tr = g_hash_table_new(g_str_hash, g_str_equal) ;
  GHashTable *combine = g_hash_table_new(g_double_hash, g_str_equal) ;
  struct sb_stemmer * stemmer ;
  char * line = 0x0 ;
  size_t r ;
  size_t n = 0 ;
  char r_word[SIZE][10] = {"at", "our", "us", "if", "him", "her", "pm", "by", "am", "we", "was", "i", "she", "he", "it", "is", "are", "the", "to", "a", "an", "in", "on", "my", "me", "for", "you"};
	int temp = 0;
  while (getline(&line, &n, neg_tr_data) >= 0) {
    stemmer = sb_stemmer_new("english", 0x0) ;
    char * t ;
      char * _line = line ;
    const char * s ;

      for (t = strtok(line, " \n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t")) {
          int * d ;
      for (int j = 0; j < strlen(t); j++){
        if (t[j] >= 'A' && t[j] <= 'Z')
          t[j] = 'a' + t[j] - 'A';

        if (t[j] < 'a' || t[j] > 'z')
          removeChar (t, j--);
      }//convert each message to lowercase.

      if (strlen(t) == 0) // if every character is a emoji, just remove the word.
        continue;
	  temp = 0;
	  for(int k = 0; k < SIZE; k++){
		if(strcmp(r_word[k],t) == 0){
			temp = 1;
			break;
		}
	  }
	  if(temp == 1) continue;
      s = sb_stemmer_stem(stemmer, t, strlen(t)); // stemming the word.
          d = g_hash_table_lookup(neg_tr, s);
          if (d == NULL) {
              d = malloc(sizeof(int)) ;
              *d = 1;
              g_hash_table_insert(neg_tr, strdup(s), d) ;
          }
          else {
              *d = *d + 1 ;
          }
      }
      free(_line) ;
      line = 0x0 ;
    sb_stemmer_delete(stemmer) ;
    }

  while (getline(&line, &n, non_tr_data) >= 0) {
    stemmer = sb_stemmer_new("english", 0x0) ;
    char * t ;
    char * _line = line ;
    const char * s ;

    for (t = strtok(line, " \n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t")) {
      int * d ;
      for (int j = 0; j < strlen(t); j++){
        if (t[j] >= 'A' && t[j] <= 'Z')
          t[j] = 'a' + t[j] - 'A';
        if (t[j] < 'a' || t[j] > 'z')
          removeChar (t, j--);
      } //convert each messages to lowercase.

      if (strlen(t) == 0) // if every character is a emoji, just remove the word.
        continue;
	  temp = 0;
	  for(int k = 0; k < SIZE; k++){
		  if(strcmp(r_word[k],t) == 0){
			  temp = 1;
			  break;
		  }
	  }
	  if(temp == 1) continue;
      
	  s = sb_stemmer_stem(stemmer, t, strlen(t)); // stemming the word.
      d = g_hash_table_lookup(non_tr, s);
      if (d == NULL) {
        d = malloc(sizeof(int)) ;
        *d = 1;
        g_hash_table_insert(non_tr, strdup(s), d) ;
      }
      else {
        *d = *d + 1 ;
      }
    }
    free(_line) ;
    line = 0x0 ;
    sb_stemmer_delete(stemmer) ;
  }
  g_hash_table_foreach_remove (neg_tr, removeSmallEle, 0x0); // remove the imfrequent words.
  g_hash_table_foreach_remove (non_tr, removeSmallEle, 0x0); // remove the imfrequent words.
  combiningWords(non_tr, neg_tr, combine); // combine the non negative word and negative word.
  writeModel (model, combine); // write a data in model file.
    // g_hash_table_foreach(combine, print_counter_double, 0x0) ; // print out the all value and key.

  // int *non = (int *) g_hash_table_lookup(non_tr, "human");
  // int *neg = (int *) g_hash_table_lookup(neg_tr, "human");
    // printf("check_non: %d\n", non == NULL ? 0: *non) ;
  // printf("check_neg: %d\n", neg == NULL ? 0: *neg) ;

    fclose(neg_tr_data) ;
  fclose(non_tr_data) ;
  fclose(model) ;
}


