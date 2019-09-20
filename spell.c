#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

bool check_word(const char* word, hashmap_t hashtable[])
{
    int bucket = hash_function(word);
    hashmap_t cursor = hashtable[bucket];
    while(cursor != NULL){
        if(strcmp(word,cursor->word) == 0){
            return true;
        }
        cursor = cursor->next;
    }
    char lower_case[LENGTH + 1];
    for(int i = 0; i < strlen(word); i++){
        lower_case[i] = tolower(word[i]);
    }
    lower_case[strlen(word)] = '\0';//this line fixed error in valgrind regarding uninitialized values-add to writeup
    bucket = hash_function(lower_case);
    cursor = hashtable[bucket];
    while(cursor != NULL){
        if(strcmp(lower_case,cursor->word) == 0){
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

bool load_dictionary(const char* dictionary, hashmap_t hashtable[])
{
    //Initialize all values in hash table to NULL.
    for (int i = 0; i <HASH_SIZE; i++){
        hashtable[i] = NULL;
    }
    //Open dict_file from path stored in dictionary.
    FILE *dict_file = fopen(dictionary,"r");
    if (dict_file == NULL){
        return false;
    }
    char word[LENGTH + 2];
    while(fgets(word,LENGTH + 2,dict_file)){
        word[strcspn(word, "\n")] = 0;
        hashmap_t new_node = malloc(sizeof(node));
        new_node->next = NULL;
        strcpy(new_node->word,word);
        int bucket = hash_function(word);
        if(hashtable[bucket] == NULL){
            hashtable[bucket] = new_node;
        }
        else{
            new_node->next = hashtable[bucket];
            hashtable[bucket] = new_node;
        }
    }
    fclose(dict_file);
    return true;
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
    int num_misspelled = 0;
    char line[4096];
    while(fgets(line,4096,fp)){
        //Split the line on spaces.
        char * wrdtok = strtok(line," .,-!?\n");
        while(wrdtok!= NULL){
            if (!check_word(wrdtok, hashtable)){
                misspelled[num_misspelled] = wrdtok;
                num_misspelled++;
            }
            wrdtok = strtok(NULL," .,-!?\n");
        }
    }
    //The code below is to ensure there are no leaks by freeing the nodes from hashtable
    node *tmp;
    for(int i = 0; i < HASH_SIZE; i++){
        while(hashtable[i] != NULL){
            tmp = hashtable[i]->next;
            free(hashtable[i]);
            hashtable[i] = tmp;
        }
    }
    return num_misspelled;
}