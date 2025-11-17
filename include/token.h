#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED
typedef enum TOKEN_TYPE{
    R_BRACE,
    L_BRACE,
    R_BRACKET,
    L_BRACKET,
    COMMA,
    COLON,
    TRUE,
    FALSE,
    NONE,
    NUMBER,
    STRING,
    END,
    UNKNOWN
}TOKEN_TYPE;

typedef struct TOKEN{

    TOKEN_TYPE type;
    char* value;

}TOKEN;

TOKEN_TYPE check_token_type(char* string,int str_len);
TOKEN* create_token(char* value,int token_length,TOKEN_TYPE type);
void destroy_token(TOKEN** token);
int expand_tokens(TOKEN*** tokens,int new_length);
TOKEN** tokenize(char* filepath,int* t_count);

#endif // TOKEN_H_INCLUDED
