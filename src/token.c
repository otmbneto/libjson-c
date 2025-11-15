#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/token.h"

TOKEN* create_token(char* value,int token_length,TOKEN_TYPE type){

    TOKEN* new_token = malloc(sizeof(TOKEN));
    if(new_token != NULL){
        new_token->value = malloc(sizeof(char)*(token_length+1));
        if(new_token->value == NULL){
            free(new_token);
            new_token = NULL;
        }
        else{
            memcpy(new_token->value, value, token_length);
            new_token->value[token_length] = '\0';
            new_token->type = type;
        }
    }
    return new_token;
}

void tokenize(char* filepath){

    char* string = NULL;
    int str_def_len = 10;
    string = malloc(sizeof(char)*(str_def_len + 1));
    int str_counter = 0;
    int i = 0;
    int token_count = 10;
    int str_started = 0;
    int spc_started = 0;
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

            case '{':   tokens[i++] = create_token(c,1,R_BRACE);
                        break;
            case '}':   tokens[i++] = create_token(c,1,L_BRACE);
                        break;
            case '[':   tokens[i++] = create_token(c,1,R_BRACKET);
                        break;
            case ']':   tokens[i++] = create_token(c,1,L_BRACKET);
                        break;
            case ',':   tokens[i++] = create_token(c,1,COMMA);
                        break;
            case ':':   tokens[i++] = create_token(c,1,COLON);
                        break;
            case '"':{
                        if(str_counter >= str_def_len){
                            printf("string is too big.reallocating memory\n");
                            str_def_len *= 2;
                            char* new_str = realloc(string,sizeof(char)*(str_def_len + 1));
                            if(new_str == NULL){
                                printf("realloc failed\n");
                                return;
                            }
                            string = new_str;
                        }
                        string[str_counter++] = *c;
                        string[str_counter] = '\0';
                        if(!str_started && !spc_started){
                            //started a string
                            str_started = 1;
                        }
                        else if(str_started && !spc_started){
                            //close string
                            tokens[i++] = create_token(string,str_counter,STRING);
                            str_started = 0;
                            str_counter = 0;
                        }
                     }
                     break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z': {
                        string[str_counter++] = *c;
                        string[str_counter] = '\0';
                        // if is not in the middle of the a string
                        if(!str_started){

                            if(strcmp("true", string) == 0){
                                tokens[i++] = create_token(string,str_counter,TRUE);
                                str_started = 0;
                                str_counter = 0;
                            }
                            else if(strcmp("false", string) == 0){
                                tokens[i++] = create_token(string,str_counter,FALSE);
                                str_started = 0;
                                str_counter = 0;
                            }
                            else if(strcmp("null", string) == 0){
                                tokens[i++] = create_token(string,str_counter,FALSE);
                                str_started = 0;
                                str_counter = 0;
                            }

                        }
                      }
                      break;
        }
    }

    //debug
    for(int j = 0;j< i; j++){

        printf("token %i: %s\n",j+1,tokens[j]->value);

    }
}
