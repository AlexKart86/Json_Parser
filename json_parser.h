#ifndef JSON_PARSER_H
#define JSON_PARSER_H 1


#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


//Json value types
typedef enum {
   json_long,
   json_double,
   json_string,
   json_boolean,
   json_section,
   json_array
} json_value_type;

extern const char* json_value_type_str[];

//Pair: <key: value>
typedef struct _section_item{
  char* name; //key
  struct _json_value* value; //value
} section_item;

//
typedef struct _value{
  struct _json_value *parent; //link to parent section
  json_value_type type;//type of value
  union{
    bool b_value;
    long l_value;
    double d_value;
    char* s_value;
    //array of values
    struct{
      unsigned int len;
      struct _value **values;
    } arr;
    //section
    struct{
      unsigned int len;
      section_item **sections;
    } section;
  } u;
} json_value;

json_value* json_parse(const char* file_name, char* error);

//For testing
void print_json(json_value* arr);

//Destroy and nil values
void free_and_nil(json_value **val);

#ifdef __cplusplus
}      /* extern "C" */
#endif  /* __cplusplus */

#endif /*JSON_PARSER_H*/
