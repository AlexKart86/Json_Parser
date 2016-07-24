#include "stdio.h"
#include "stdarg.h"
#include "json_parser.h"

void config_get_string(const char *key, ...)  __attribute__ ((format (printf, 1, 2)));

void config_get_string(const char *key, ...)
{
  va_list args;
  va_start(args, key);
  vfprintf(stderr, key, args);
  va_end(args);
}

void test(char **arr, int *cnt_msg){
    cnt_msg = 10;
    arr = malloc(1);
    for (int i=0; i< *cnt_msg; ++i){
        arr = realloc(*arr, (i+1)*sizeof(char));
        char msg[100];
        sprintf(msg, "Message %d", i);
        arr[i] = malloc(strlen(msg)*sizeof(char));
        strcpy(arr[i], msg);
    }
}

void add_char1(char *str, char ch){
    int len = strlen(str);
    str[len] = ch;
    str[len+1] = '\0';
}

int main(){
 //config_get_string("sdfsdfsd %d", 2);
 //section_item s;
 //s.name = "hello";

 //char **arr;
 //int i;
 //int cnt_msg = 10;
 /*arr = malloc(1);
 for (int i=0; i< cnt_msg; ++i){
    arr = realloc(arr, (i+1)*sizeof(char));
    char msg[100];
    sprintf(msg, "Message %d", i);
    arr[i] = malloc(strlen(msg)*sizeof(char));
    strcpy(arr[i], msg);
 printf(arr[0]);
 printf(arr[1]);
 }*/

/*char error[1024];
json_value* p = json_parse("k:\\DOCS\\JSON_Parser\\sample.conf", error);
if (p==NULL){
    printf(error);
}
else{
printf("aaa");*/
 char a[100];
 a[0] = '\0';
 add_char1(a, 'd');
 add_char1(a, 'a');
 add_char1(a, 'c');
 printf(a);
 return 0;
}
