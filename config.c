#include "config.h"
#include "json_parser.h"

json_value* root;
json_value* current;
char error[1000];

void raise_error(char* error_msg ){
    if (strlen(error_msg) > 0){
       fputs(error_msg, stderr);
       exit(-1);
    }

}

void config_create(const char *filename){
  root = json_parse(filename, error);
  if (root == NULL)
   raise_error(error)

}
