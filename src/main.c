#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <help.h>
#include <file_process.h>
#include <string_process.h>
#include <search_path.h>
#include <pre_process.h>

string_chain_part *source_file_chain_tail = NULL;
string_chain_part *include_path_tail = NULL;

int main(int argc, char *argv[]) {
    string_chain_part *tmp;
    process_search_path_variables();
    for (int args_count = 1; args_count < argc; ++args_count) {
        if (argv[args_count][0] == '-') {
            if (strcmp("--help", argv[args_count]) == 0) {
                print_help_message();
                return 0;
            } else if (argv[args_count][1] == 'I') {
                tmp = new_string_chain_part(convert_char(&argv[args_count][2], strlen(argv[args_count])-2));
                insert_back_string_chain(include_path_tail, tmp);
                include_path_tail = tmp;
            }
        } else {
            if (source_file_chain_tail == NULL) {
                source_file_chain_tail = new_string_chain_part(convert_char(argv[args_count], strlen(argv[args_count])));
            } else {
                tmp = new_string_chain_part(convert_char(argv[args_count], strlen(argv[args_count])));
                insert_back_string_chain(source_file_chain_tail, tmp);
                source_file_chain_tail = tmp;
            }
        }
    }
    
    if (source_file_chain_tail == NULL) {
        printf("cc: fatal error: no input files\n");
        return -1;
    }

    tmp = source_file_chain_tail;
    while (tmp != NULL) {
        string_chain_part *processed_src = pre_process(tmp->str->data);

        tmp = tmp->next;
    }
    
    return 0;
}
