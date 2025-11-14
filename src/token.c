#include <stdio.h>
#include <stdlib.h>
#include "../include/token.h"

TOKEN* create_token(char value,int token_length,TOKEN_TYPE type){

    TOKEN* new_token = malloc(sizeof(TOKEN));
    if(new_token != NULL){
        new_token->value = malloc(sizeof(char)*(token_length-1));
        if(new_token->value == NULL){
            free(new_token);
            new_token = NULL;
        }
        else{
            *new_token->value = value;
            new_token->value[token_length] = '\0';
            new_token->type = type;
        }
    }
    return new_token;
}


void tokenize(char* filepath){

    int i = 0;
    int token_count = 10;
    TOKEN** tokens = malloc(sizeof(TOKEN*)*token_count);
    TOKEN** new_list = NULL;
    FILE* file_pointer = fopen(filepath, "r");
    if(file_pointer == NULL){
        printf("ERROR: could not open file\n");
        return;
    }
    char* c = malloc(sizeof(char) + 1);
    while ((*c = fgetc(file_pointer)) != EOF) {

        if(i >= token_count){
            token_count *= 2;
            new_list = realloc(tokens,sizeof(TOKEN*)*token_count);
            if(new_list == NULL){
                return;
            }
            tokens = new_list;
        }
        switch(*c){

            case '{':   tokens[i++] = create_token(*c,1,R_BRACE);
                        break;
            case '}':   tokens[i++] = create_token(*c,1,L_BRACE);
                        break;
            case '[':   tokens[i++] = create_token(*c,1,R_BRACKET);
                        break;
            case ']':   tokens[i++] = create_token(*c,1,L_BRACKET);
                        break;
            case ',':   tokens[i++] = create_token(*c,1,COMMA);
                        break;
            case ':':   tokens[i++] = create_token(*c,1,COLON);
                        break;
        }
    }

    for(int j = 0;j< i; j++){

        printf("%s\n",tokens[j]->value);

    }
}
