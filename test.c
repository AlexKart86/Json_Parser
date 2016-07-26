#include "stdio.h"
#include "stdarg.h"
#include "json_parser.h"
#include "config.h"


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

 char error[2000];

 config_create("d:\\JSon_Parser\\sample.conf");

/* json_value* p = json_parse("k:\\DOCS\\JSON_Parser\\test2.json", error);
 if (strlen(error)>0) {
    printf(error);
 }
 else
 {
     /*if (p->type == json_section){
        printf("Yes!");
        for (int i=0; i < p->u.section.len; ++i){
            printf("%s\n", p->u.section.sections[i]->name);
        }
     }*/
  //   print_json(p);
 //}
 return 0;
}
