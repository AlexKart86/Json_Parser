#include "config.h"
#include "json_parser.h"
#include "stdio.h"
#include "stdarg.h"

json_value* root;
json_value* current;
char error[1000];

//Macro for make string from format string
//in functions with variable params
#define format(msg, fmt) \
     va_list arg; \
     va_start(arg, (fmt));\
     vsprintf(msg, (fmt), (arg));\
     va_end(arg);



//Print error to stderr and end program
void raise_error(char* error_msg, ... ){
    char error[1000];
    format(error, error_msg);
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

json_value* lookup_with_check(char *key){
  json_value *val = lookup(key);
  if (val == NULL){
    raise_error("Item %s not found in current section", key);
  }
}


void config_create(const char *filename){
  root = json_parse(filename, error);
  if (root == NULL)
   raise_error(error);
  current = root;
}

bool config_get_bool(const char *key, ...){
  char str[1000];
  format(str, key);
  json_value* val = lookup_with_check(key);
  check_value_type(key, val, json_boolean);
  return val->u.b_value;
}

