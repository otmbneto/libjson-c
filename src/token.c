#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/token.h"

int is_number(char* string,int length){

    int i = 0;
    int is_number = 1;
    int sci_not = 0; //scientific notation started flag
    int has_dot = 0; // flag for when we're into the fractional part of a float.
    while(i < length && is_number){
        switch(string[i]){
            case 'E':
            case 'e':{
                        if(sci_not){
                            is_number = 0;
                            break;
                        }
                        sci_not = 1;
                     }
                     break;
            case '+':{
                        if(!sci_not){
                            is_number = 0;
                            break;
                        }
                     }
            case '-':{
                        if(i>0 && !sci_not){
                            is_number = 0;
                        }
                        else{
                            sci_not = 0;
                        }
                     }
                     break;
            case '.': {
                        if(has_dot || i == 0){
                            is_number = 0;
                            break;
                        }
                        has_dot = 1;
                      }
                      break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': break;
            default:{
                    is_number = 0;
                    break;
            }
        }
        i++;
    }

    return is_number;
}

TOKEN_TYPE check_token_type(char* string,int str_len){

    TOKEN_TYPE t_type = UNKNOWN;
    if(strcmp("true", string) == 0){
        t_type = TRUE;
    }
    else if(strcmp("false", string) == 0){
        t_type = FALSE;
    }
    else if(strcmp("null", string) == 0){
        t_type = NONE;
    }
    //if it is shaped as a string token
    else if(str_len > 1 && string[0] == '"' && string[str_len - 1] == '"'){
        //if the string token does not have inverse bar before the las double quote.
        if(string[str_len - 2] != '\\' || (str_len > 2 && string[str_len - 2] == '\\' && string[str_len - 3] == '\\')){
            t_type = STRING;
        }
    }
    else if(is_number(string,str_len)){
        t_type = NUMBER;
    }

    return t_type;
}

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

void destroy_token(TOKEN** token){
    free((*token)->value);
    free(*token);
    *token = NULL;
}

int expand_tokens(TOKEN*** tokens,int new_length){

    TOKEN** new_list = NULL;
    int status = 1;
    new_list = realloc(*tokens,sizeof(TOKEN*)*new_length);
    if(new_list == NULL){
        printf("ERROR:realloc failed\n");
        status = 0;
    }
    *tokens = new_list;
    return status;
}

char* create_string(int length){
    return malloc(sizeof(char)*(length + 1));
}

int expand_string(char** string,int new_length){

    char* new_str = NULL;
    int status = 1;
    new_str = realloc(*string,sizeof(char)*(new_length + 1));
    if(new_str == NULL){
        printf("ERROR: realloc failed\n");
        status = 0;
    }
    *string = new_str;
    return status;
}

TOKEN** tokenize(char* filepath,int* t_count){

    int str_def_len = 10;
    char* string = create_string(str_def_len);
    int str_counter = 0;
    int i = 0;
    int token_count = 10;
    int number_index = -1; //stores temporarely the index where a number token should be stored.
    char unique_char = ' ';
    TOKEN** tokens = malloc(sizeof(TOKEN*)*token_count);
    FILE* file_pointer = fopen(filepath, "r");
    if(file_pointer == NULL){
        printf("ERROR: could not open file\n");
        return NULL;
    }
    char* c = malloc(sizeof(char) + 1);
    TOKEN_TYPE t_temp = UNKNOWN;
    while ((*c = fgetc(file_pointer)) != EOF) {
        if(i >= token_count){
            printf("Token list is too big.reallocating memory\n");
            token_count *= 2;
            expand_tokens(&tokens,token_count);
        }
        if(str_counter >= str_def_len){
            printf("string is too big.reallocating memory\n");
            str_def_len *= 2;
            expand_string(&string,str_def_len);
        }
        if(str_counter > 0 && *c != '.' && string[0] == '"'){
            unique_char = *c;
            *c = ' ';
        }
        TOKEN_TYPE t_type = UNKNOWN;
        //printf("current char %c\n",*c);
        switch(*c){

            case '{':
                tokens[i++] = create_token("{", 1, R_BRACE);
                break;

            case '}':
                tokens[i++] = create_token("}", 1, L_BRACE);
                break;

            case '[':
                tokens[i++] = create_token("[", 1, R_BRACKET);
                break;

            case ']':
                tokens[i++] = create_token("]", 1, L_BRACKET);
                break;

            case ':':
                tokens[i++] = create_token(":", 1, COLON);
                break;

            case ',':
                tokens[i++] = create_token(",", 1, COMMA);
                break;
            case '\f':
            case '\b':
            case '\t':
            case '\r':
            case '\n':
            case ' ':{
                        //if we are not in a string token
                        if(!(str_counter > 0 && string[0] == '"')){
                            break;
                        }
                     }
            default:{

                if(unique_char != ' '){
                    *c = unique_char;
                    unique_char = ' ';
                }
                if(t_temp == NUMBER && *c != '.' && *c !='e' && *c !='+' && *c !='-' && check_token_type(c,1) != NUMBER){
                    tokens[number_index] = create_token(string,str_counter,NUMBER);
                    number_index = -1;
                    str_counter = 0;
                    t_temp = UNKNOWN;
                }
                string[str_counter++] = *c;
                string[str_counter] = '\0';
                t_type = check_token_type(string,str_counter);
                if(t_type == NUMBER){
                    t_temp = NUMBER;
                    if(number_index < 0){
                        number_index = i;
                        i++;
                    }
                }
                else if(t_type != UNKNOWN){
                    tokens[i++] = create_token(string,str_counter,t_type);
                    str_counter = 0;
                }

            }
        }
    }
    tokens[i++] = create_token(c,1,END);
    *t_count = i;
    free(string);

    return tokens;
}
