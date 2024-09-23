#include <stdlib.h>
#include <string.h>

#include <search_path.h>
#include <string_process.h>

const char *builtin_include_path[] = {"/usr/include", "/usr/local/include"};
const int builtin_include_path_num = 2; //>1

extern string_chain_part *include_path_tail;

void process_include_search_path() {
    include_path_tail = new_string_chain_part(convert_char(builtin_include_path[0], strlen(builtin_include_path[0])));
    string_chain_part *tmp;
    for (int i = 1; i < builtin_include_path_num; ++i) {
        tmp = new_string_chain_part(convert_char(builtin_include_path[i], strlen(builtin_include_path[i])));
        insert_back_string_chain(include_path_tail, tmp);
        include_path_tail = tmp;
    }

    char *include_path_char = getenv("C_INCLUDE_PATH");
    if (include_path_char != NULL) {
        string_chain_part *env = split_by_char(convert_char(include_path_char, strlen(include_path_char)), ':');
        insert_back_string_chain(include_path_tail, env);
        while (include_path_tail->next != NULL) {
            include_path_tail = include_path_tail->next;
        }
    }
}

void process_search_path_variables() {
    process_include_search_path();
}
