#ifndef NX_LIB_CONFIG_H
#define NX_LIB_CONFIG_H 1

#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


void
config_create(const char *filename);


bool
config_get_bool(const char *key, ...) __attribute__ ((format (printf, 1, 2)));


long
config_get_long(const char *key, ...) __attribute__ ((format (printf, 1, 2)));


double
config_get_double(const char *key, ...)
                __attribute__ ((format (printf, 1, 2)));


char *
config_get_string(const char *key, ...)
                __attribute__ ((format (printf, 1, 2)));

//Obtain size of array
unsigned int config_get_arr_size(const char *key, ...)
                __attribute__ ((format (printf, 1, 2)));

bool
config_exist_value(const char *key, ...)
                __attribute__ ((format (printf, 1, 2)));


void
config_open_section(const char *key, ...)
                __attribute__ ((format (printf, 1, 2)));


void
config_close_section(void);


void
config_destroy(void);



#ifdef __cplusplus
}      /* extern "C" */
#endif  /* __cplusplus */


#endif  /* ! NX_LIB_CONFIG_H */
