#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

int main(int argc, char * argv[]){
    if(argc != 3){
        printf("usage: ./spell_check <text.txt> <wordlist.txt>\n");
        exit(1);
    }
    node* hashtable[HASH_SIZE];
    load_dictionary(argv[2], hashtable);
    char * misspelled[MAX_MISSPELLED];
    FILE *fp = fopen(argv[1], "r");
    if(fp == NULL){
        printf("Unable to open text file");
        exit(1);
    }
    int num_misspelled = check_words(fp, hashtable, misspelled);
    fclose(fp);
    printf("Misspelled Words:\n");
    for (int i = 0; i < num_misspelled; i++){
       printf("%s\n",misspelled[i]);
    }
    printf("Number Misspelled: %d\n",num_misspelled);
    /*The code below is to ensure there are no leaks by freeing the nodes from hashtable. 
    Its currently in func check_words()
    node *tmp;
    for(int i = 0; i < HASH_SIZE; i++){
        while(hashtable[i] != NULL){
            tmp = hashtable[i]->next;
            free(hashtable[i]);
            hashtable[i] = tmp;
        }
    }*/
    return 0;
}