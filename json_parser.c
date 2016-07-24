#include "json_parser.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

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
    return ch;
}


//Parsing lexeme from file
lexeme *parse_lexeme(char *error){
    lexeme *res = (lexeme *)malloc(sizeof(lexeme));
    res->str[0]='\0';
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
            add_char(res->str, ch);
          else{
            char ch1 = fgetc(in);
            switch (ch1){
             case '/':
                is_inline_comments = 1;
                break;
             case '*':
                is_block_comments = 1;
                break;
              default:
                sprintf(error, "Error: expected / or * in line %d column %d", cur_row, cur_column);
                return NULL;
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
        add_char(res->str, ch);
      }
      else if (is_block_comments){
        if (ch == '*'){
            ch = read_char();
            if (ch == '/')
                is_block_comments = 0;
        }
      }
      else if (is_inline_comments){
        if (ch == '\r' || ch == '\n'){
            is_inline_comments = 0;
        }
      }
      else if ((ch == '\t' || ch == ' ' || ch == '\n' || ch == '\r') && strlen(res->str) == 0)
      {

      }
      else {

          if ((ch == '{' ||  ch == '}' || ch == ':' || ch == '[' || ch == ']' || ch == ',') && strlen(res->str) > 0)
          {
              sprintf(error, "Invalid symbol %c on line %d column %d", ch, cur_row, cur_column);
              return NULL;
          }

          switch (ch){
          case '{':
            res->type = open_brake;
            add_char(res->str, ch);
            return res;
            break;
          case '}':
            res->type = close_brake;
            add_char(res->str, ch);
            return res;
            break;
          case '[':
            res->type = open_array;
            add_char(res->str, ch);
            break;
          case ']':
            res->type = close_array;
            add_char(res->str, ch);
            break;
          case ':':
            res->type = d_point;
            add_char(res->str, ch);
            break;
          case ',':
             res->type = comma;
             add_char(res->str, ch);
             break;
          default:
            if (isalpha(ch) || isdigit(ch) || ch == '-' || ch=='+' || ch =='.'){
                add_char(res->str, ch);
            }
            else{
              add_char(res->str, ch);
              if (!strcmp(res->str, "true")|| !strcmp(res->str, "false")){
                res->type = b_value;
                return res;
              }
              if (atol(res->str)||!strcmp(res->str, "0")){
                res->type = i_value;
                return res;
              }
              if (atof(res->str) || !strcmp(res->str, "0.0")){
                res->type = d_value;
                return res;
              }
              else{
                res->type = string;
                return res;
              }
            }
          }
      }
    }
    return NULL;
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



  while (1){
     lexeme *l = parse_lexeme(error);
     if (l != NULL){
        printf(l->str);
        printf("\n");
     }
     else
        return NULL;
  }

  fclose(in);
}
