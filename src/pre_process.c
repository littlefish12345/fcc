#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pre_process.h>
#include <file_process.h>
#include <string_process.h>

extern string_chain_part *include_path_tail;

string *find_header_in_search_path(string *name, char sp) {
    string_chain_part *now = include_path_tail;
    string *path;
    while (now != NULL) {
        path = copy_string(now->str);
        add_back_string(path, "/", 1);
        add_back_string(path, name->data, name->len);
        if (file_exists(path->data)) {
            return path;
        }
        free_string(path);
        now = now->prev;
    }
    if (sp == '"') {
        path = convert_char("./", 2);
        add_back_string(path, name->data, name->len);
        if (file_exists(path->data)) {
            return path;
        }
        free_string(path);
    }
    return NULL;
}

string_chain_part *process_include(string_chain_part *line_head) {
    string_chain_part *now = line_head;
    string *include_command_str = convert_char("#include", 8);
    while (now != NULL) {
        if (string_part_equal(now->str, include_command_str, 0, 0, 8)) {
            char sp = '\0';
            int start = 8;
            int end;
            for (; start < now->str->len; ++start) {
                if (now->str->data[start] == '<') {
                    sp = '<';
                    break;
                } else if (now->str->data[start] == '"') {
                    sp = '"';
                    break;
                }
            }
            if (sp == '\0') {
                printf("cc: fatal error: include error\n");
                exit(-1);
            }
            end = start+1;
            if (sp == '<') {
                while (now->str->data[end] != '>') {
                    if (now->str->data[end] == '\0') {
                        printf("cc: fatal error: include error\n");
                        exit(-1);
                    }
                    ++end;
                }
            } else {
                while (now->str->data[end] != '"') {
                    if (now->str->data[end] == '\0') {
                        printf("cc: fatal error: include error\n");
                        exit(-1);
                    }
                    ++end;
                }
            }
            string *include_filename = convert_char(&now->str->data[start+1], end-start-1);
            string *include_filepath = find_header_in_search_path(include_filename, sp);
            if (include_filepath == NULL) {
                printf("cc: fatal error: %s: No such file or directory", include_filename->data);
                exit(-1);
            }
            free_string(include_filename);
            printf("%s\n", include_filepath->data);
        }
        now = now->next;
    }

    return line_head;
}

void recursive_process_ifdef(string_chain_part *line_head) {
    
}

string_chain_part *pre_process(char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("cc: fatal error: %s: No such file or directory\n", filename);
        exit(-1);
    }
    char *src = read_file(f);
    string *str_src = convert_char(src, get_file_size(f));
    fclose(f);
    free(src);
    string_chain_part *line_head = split_by_char(str_src, '\n');
    free(str_src);
    line_head = process_include(line_head);
    recursive_process_ifdef(line_head);
    return line_head;
}

