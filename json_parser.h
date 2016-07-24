//Json value types
typedef enum {
   json_long,
   json_double,
   json_string,
   json_boolean,
   json_section,
   json_array
} json_value_type;

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
    short b_value;
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
      section_item *sections;
    };
  } u;
} json_value;

json_value* json_parse(const char* file_name, char* error);


