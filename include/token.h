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
    END
}TOKEN_TYPE;

typedef struct TOKEN{

    TOKEN_TYPE type;
    char* value;

}TOKEN;

TOKEN* create_token(char value,int token_length,TOKEN_TYPE type);
void tokenize(char* filepath);

#endif // TOKEN_H_INCLUDED
