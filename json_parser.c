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

//strcpy + allocation memory
char *strcpy_a(char* str){
    char* res = malloc(strlen(str));
    strcpy(res, str);
    return res;
}

void add_char(char *str, char ch){
    int len = strlen(str);
    str[len] = ch;
    str[len+1] = '\0';
}

FILE *in;
//fpos_t *prev_position;
int cur_row = 1;
int cur_column = 1;

char read_char(){
    //fgetpos(in, prev_position);
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

lexeme_type detect_lexeme_type(char *str){
   if (!strcmp(str, "true")|| !strcmp(str, "false")){
                return b_value;
              }
   if (atol(str)||!strcmp(str, "0")){
                return i_value;
              }
    if (atof(str) || !strcmp(str, "0.0")){
                return d_value;
      }
    //TO DO: hex parse
       else{
          return string;
    }
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
              fseek(in, -1, SEEK_CUR);
              //fsetpos(in, prev_position);
              res->type = detect_lexeme_type(res->str);
              return res;
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
            return res;
            break;
          case ']':
            res->type = close_array;
            add_char(res->str, ch);
            return res;
            break;
          case ':':
            res->type = d_point;
            add_char(res->str, ch);
            return res;
            break;
          case ',':
             res->type = comma;
             add_char(res->str, ch);
             return res;
             break;
          default:
            if (isalpha(ch) || isdigit(ch) || ch == '-' || ch=='+' || ch =='.' || ch == '_'){
                add_char(res->str, ch);
            }
            else{
              add_char(res->str, ch);
              res->type = detect_lexeme_type(res->str);
              return res;
            }
          }
      }
    }
    return NULL;
}

json_value* new_complex_value(json_value** top, json_value** root, json_value_type v_type){
    json_value *value = (json_value *)malloc(sizeof(json_value));

    if (v_type == json_array || v_type == json_section){
        value->parent = *top;
        value->type = v_type;
        switch (v_type){
        case json_array:
            value->u.arr.len = 0;
            value->u.arr.values = NULL;
            break;
        case json_section:
            value->u.section.len = 0;
            value->u.section.sections = NULL;
            break;
        }
        value->u.section.len = 0;
        *top = value;
        if (*root == NULL){
          *root = *top;
        }
    }
    return value;
}

json_value* new_simple_value(json_value* top, lexeme lex){
  json_value *value = (json_value *)malloc(sizeof(json_value));
  value->parent = top;
  switch (lex.type){
      case string:
           value->u.s_value = strcpy_a(lex.str);
           //value->u.s_value = malloc(strlen(lex.str)+1);
           strcpy(value->u.s_value, lex.str);
           value->type = json_string;
           break;
      case d_value:
           value->u.d_value = atof(lex.str);
           value->type = json_double;
           break;
      case i_value:
           value->u.l_value = atol(lex.str);
           value->type = json_long;
           break;
      case b_value:
            value->type = json_boolean;
            if (!strcmp(lex.str, "true")){
                value->u.b_value = 1;
            }
            else
                value->u.b_value = 0;
            break;
  }
  return value;
}


//Add section to object
void add_section_item(json_value *top, char* section_name, json_value* value){
    section_item* it = (section_item *)malloc(sizeof(section_item));
    it->name = strcpy_a(section_name);
    it->value = value;
    top->u.section.len ++;
    top->u.section.sections = realloc(top->u.section.sections, sizeof(section_item)*top->u.section.len);
    top->u.section.sections[top->u.section.len-1] = it;
}


json_value* json_parse(const char* file_name, char* error){
  in = fopen(file_name, "rb");
  cur_column = 1;
  cur_row = 1;
  if (in == NULL){
    sprintf(error, "Cannot open file %s", file_name);
    return NULL;
  }

  json_value* root = NULL;
  json_value* top = NULL;
  json_value* cur_array = NULL;

  char section_name[1000];
  lexeme* prev_lex;

  while (1){
     lexeme *l = parse_lexeme(error);

     if (l != NULL){
        printf(l->str);
        printf("\n");
        switch (l->type){
        case open_brake:
            new_complex_value(&top, &root, json_section);
            break;
        case close_brake:
            if (top == NULL){
               sprintf(error, "Error: invalid symbol '}' on line %d column %d", cur_row, cur_column);
               return NULL;
            }
            top = top->parent;
            break;
        case d_point:
            strcpy(section_name, prev_lex->str);
            break;
        case open_array:
            cur_array = malloc(sizeof(json_value));
            cur_array->arr.len = 0;
            cur_array->arr.values = NULL;
            add_section_item(top, section_name, cur_array);
            break;

        case string:
        case i_value:
        case b_value:
        case d_value:
            if (prev_lex->type == d_point){
                json_value* val = new_simple_value(top, *l);
                add_section_item(top, section_name, val);
            }
            break;
        }
     }
     else
     {
        fclose(in);
        if (strlen(error) == 0)
          return root;
        else
          //TO DO free objects
          return NULL;
     }

     prev_lex = l;
  }

  fclose(in);
}
