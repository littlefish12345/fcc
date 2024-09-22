#ifndef __STRING_PROCESS_H__
#define __STRING_PROCESS_H__

#include <stdbool.h>

struct _string {
    char *data;
    int len;
    int size;
};
typedef struct _string string;

struct _string_chain_part {
    string *str;
    struct _string_chain_part *prev;
    struct _string_chain_part *next;
};
typedef struct _string_chain_part string_chain_part;

string *convert_char(const char *original_str, int str_len);
bool string_part_equal(string *str1, string *str2, int str1_start, int str2_start, int length);
void free_string(string *str);

string_chain_part *new_string_chain_part(string *str);
void add_back_string_chain(string_chain_part *prev, string_chain_part *new_part);
string_chain_part *split_by_char(string *str, char sp);
void free_string_chain_part(string_chain_part *part);

#endif
