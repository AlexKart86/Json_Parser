#include "config.h"
#include "json_parser.h"
#include <stdio.h>
#include <stdarg.h>

//--------Internal functions-----------//

json_value* root;
json_value* current;
char error[1000];

//Macro for make string from format string
//in functions with variable params
#define FORMAT(msg, fmt) \
     va_list arg; \
     va_start(arg, (fmt));\
     vsprintf(msg, (fmt), (arg));\
     va_end(arg);

//Macro for format input string + extract array index from string
#define PARSE_INPUT_STR(msg, fmt, idx) \
     FORMAT( (msg), (fmt)); \
     str_extract_idx( (msg), (idx));

//Macro for lookup value
//Correct parse arrays
#define LOOKUP(key, idx) \
   ( (idx) >=0) ? lookup_array( (key), (idx) ) : lookup_with_check(v_key);



//Print error to stderr and end program
void raise_error(char* error_msg, ... ){
    char error[1000];
    FORMAT(error, error_msg);
    //format(error, error_msg, )
    if (strlen(error) > 0){
       fputs(error, stderr);
       exit(-1);
    }
}

//Raise error if type of val <> type
void check_value_type(char* section_name, json_value* val, const json_value_type type){
    if (val->type != type){
        raise_error("%s: expected type %s but %s given", section_name,
                 json_value_type_str[type], json_value_type_str[val->type]);
    }
}

//Find value by key in currect section
json_value* lookup(char *key){
   check_value_type(key, current, json_section);
   for (int i=0; i<current->u.section.len; ++i){
     if (!strcmp(key, current->u.section.sections[i]->name))
        return current->u.section.sections[i]->value;
   }
   return NULL;
}

//Find section value by key
json_value* lookup_with_check(char *key){
  json_value *val = lookup(key);
  if (val == NULL){
    raise_error("Item %s not found in current section", key);
  }
  return val ;
}

//Separate input string into section name and array index
//For example id[4] was separated into id and 4
//If section doesn't have [ function doesn't change str and
//set -1 at index;
void str_extract_idx(char *str, int* index){
  if (strchr(str, '[') == NULL)
      *index = -1;
  else
  {
    strtok(str, "[");
    char *idx = strtok(NULL, "]");
    *index = atoi(idx);
  }
}

json_value* lookup_array(char *key, int index){
  json_value* arr = lookup_with_check(key);
  check_value_type(key, arr, json_array);
  if (arr->u.arr.len <= index )
    raise_error("Index %d out of bounds in array %s", index, key);
  return arr->u.arr.values[index];
}

//--------Interface functions-----------//

void config_create(const char *filename){
  root = json_parse(filename, error);
  if (root == NULL)
   raise_error(error);
  current = root;
}

bool config_get_bool(const char *key, ...){
  char v_key[1000];
  int idx;
  PARSE_INPUT_STR(v_key, key, &idx);
  json_value* val = LOOKUP(v_key, idx);
  check_value_type(v_key, val, json_boolean);
  return val->u.b_value;
}

long config_get_long(const char *key, ...){
  char v_key[1000];
  int idx;
  PARSE_INPUT_STR(v_key, key, &idx);
  json_value* val = LOOKUP(v_key, idx);
  check_value_type(v_key, val, json_long);
  return val->u.l_value;
}

double config_get_double(const char *key, ...){
  char v_key[1000];
  int idx;
  PARSE_INPUT_STR(v_key, key, &idx);
  json_value* val = LOOKUP(v_key, idx);
  check_value_type(v_key, val, json_double);
  return val->u.d_value;
}


char * config_get_string(const char *key, ...){
  char v_key[1000];
  int idx;
  PARSE_INPUT_STR(v_key, key, &idx);
  json_value* val = LOOKUP(v_key, idx);
  check_value_type(v_key, val, json_string);
  char *res = malloc(strlen(val->u.s_value)+1);
  strcpy(res, val->u.s_value);
  return res;
}

void config_open_section(const char *key, ...){
  char v_key[1000];
  FORMAT(v_key, key);
  json_value* val = lookup_with_check(v_key);
  check_value_type(v_key, val, json_section);
  current = val;
}

void config_close_section(void){
  if (current->parent == NULL)
    raise_error("Error: this is a root section");
  current = current->parent;
}



//Obtain size of array
unsigned int config_get_arr_size(const char *key, ...){
  char v_key[1000];
  FORMAT(v_key, key);
  json_value* arr = lookup_with_check(v_key);
  check_value_type(v_key, arr, json_array);
  return arr->u.arr.len;
}

bool config_exist_value(const char *key, ...){
  char v_key[1000];
  int idx;
  PARSE_INPUT_STR(v_key, key, &idx);
  json_value* val =  lookup(v_key);
  //Additional checks for array
  if (idx >= 0) {
    if ( val == NULL || val->type != json_array ||
         config_get_arr_size(v_key)-1 < idx)
       return false;
    else
        return true;
  }
  else
    return val != NULL;
}


void config_destroy(void){
    free_and_nil(&root);
    current = NULL;
}


