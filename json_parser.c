#include "json_parser.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef enum{
  open_brake,
  close_brake,
  open_array,
  close_array,
  d_point,
  comma,
  string,
  i_value,
  b_value,
  d_value
} lexeme_type;

typedef struct _lexeme{
    lexeme_type type;
    char str[1000];
} lexeme;

void add_char(char *str, char ch){
    int len = strlen(str);
    str[len] = ch;
    str[len+1] = '\0';
}

FILE *in;
int cur_row = 1;
int cur_column = 1;

char read_char(){
    char ch = fgetc(in);
    if (ch == '\r'){
        cur_row ++;
        cur_column = 1;
    }
    else if (ch != '\n'){
        cur_column++;
    }
}


//Parsing lexeme from file
lexeme *parse_lexeme(FILE *in, char *error){
    lexeme *res = (lexeme *)malloc(sizeof(lexeme));
    char ch;
    int length = 0;
    short is_inline_comments = 0;
    short is_block_comments = 0;
    short is_in_s_quote = 0;
    short is_in_d_quote = 0;

    while ((ch = read_char()) != EOF){
      if (ch == '/')
      {
          if (is_in_s_quote || is_in_d_quote)
            add_char(res->str, ch)
          else{
            char ch1 = fgetc(in);
            switch (ch1){
             case '/':
                is_inline_comments = 1;
                break;
             case '*':
                is_block_comments = 1;
                break;
              else{
                sprintf(error, "Error: expected / or * in line %d column %d", cur_row, cur_column);
                return NULL;
              }
            }
          }
      }
      else if (ch == '\'' ){
         if (is_in_s_quote){
            res->type = string;
            return res;
         }
         else if (is_in_d_quote){
            add_char(res->str, ch);
         }
         else is_in_s_quote = 1;
      }
      else if (ch == '"'){
        if (is_in_d_quote){
            res ->type = string;
            return res;
        }
        else if (is_in_s_quote){
            add_char(res->str, ch);
        }
        else
            is_in_d_quote = 1;
      }
      else  if (is_in_d_quote || is_in_s_quote){
        add_char(res->str, ch)
      }
      else {

          if ((ch == '{' ||  ch == '}' || ch == ':' || ch == '[' || ch == ']') && strlen(res->str) > 0)
          {
              sprintf(error, "Invalid symbol on line %d column %d", cur_row, cur_column);
              return NULL;
          }

          switch (ch){
          case '{':
            res->type = open_brake;
            return res;
            break;
          case '}':
            res->type = close_brake;
            add_char(res->str, ch);
            return res;
            break;

          }
      }
    }
}



json_value* json_parse(const char* file_name, char* error){
  in = fopen(file_name, "r");
  cur_column = 1;
  cur_row = 1;
  if (in == NULL){
    sprintf(error, "Cannot open file %s", file_name);
    return NULL;
  }

  char ch;
  char buf[1000];
  int is_read_open_b = 0;



  while ((ch = fgetc(in)) != EOF){

  }

  fclose(in);
}
