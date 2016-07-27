#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "config.h"

int main()
{
    printf("Begin\n");
    config_create("./sample.conf");
    bool use_zip = config_get_bool("use_zip");
    if (use_zip)
        printf("scale = %e\n", config_get_double("scale"));

    char * vendor = config_get_string("vendor");
    printf("Running sample_main by %s\n", vendor); // ABCsoft
    free(vendor);

    // long int msize = config_get_long("min_file_size"); // здесь падаем - нет такого параметра

    config_open_section("module_properties");

    vendor = config_get_string("vendor");
    printf("Running module by %s\n", vendor); //QWEsoft
    free(vendor);

    if (config_exist_value("some_value"))
        printf("WTF!\n");

    for ( int i = 0; i < config_get_arr_size("ip"); ++i)
        {
            char * ip = config_get_string("ip[%d]", i);
            printf("ip[%d]= %s\n", i, ip);
            free(ip);
        }
    config_close_section();

    // config_close_section(); // здесь падаем - больше нечего закрывать

    config_destroy();

    printf("End\n");
    return 0;
}

