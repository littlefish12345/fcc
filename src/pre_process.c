#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <defines.h>
#include <pre_process.h>
#include <file_process.h>
#include <string_process.h>

extern string_chain_part *include_path_tail;

string *find_header_in_search_path(string *name, char sp) {
    string_chain_part *now = include_path_tail;
    string *path;
    if (sp == '"') {
        path = convert_char("./", 2);
        add_back_string(path, name->data, name->len);
        if (file_exists(path->data)) {
            return path;
        }
        free_string(path);
    }
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
    return NULL;
}

string_chain_part *process_include(string_chain_part *line_head) {
    string_chain_part *now = line_head;
    string *include_command_str = convert_char("#include", 8);
    while (now != NULL) {
        if (string_part_equal(now->str, include_command_str, 0, 0, 8)) {
            char include_filename_buf[BUFFER_SIZE]; //extract filename
            char sp = '<';
            sscanf(now->str->data, "#include%*[ \t]<%[^>]", include_filename_buf);
            if (include_filename_buf[0] == '\0') {
                sscanf(now->str->data, "#include<%[^>]", include_filename_buf);
            }
            if (include_filename_buf[0] == '\0') {
                sp = '"';
                sscanf(now->str->data, "#include%*[ \t]\"%[^\"]", include_filename_buf);
                if (include_filename_buf[0] == '\0') {
                    sscanf(now->str->data, "#include\"%[^\"]", include_filename_buf);
                }
            }
            if (include_filename_buf[0] == '\0') {
                printf("cc: fatal error: include error\n");
                exit(-1);
            }

            string *include_filename = convert_char(include_filename_buf, strlen(include_filename_buf)); //search for the file
            string *include_filepath = find_header_in_search_path(include_filename, sp);
            if (include_filepath == NULL) {
                printf("cc: fatal error: %s: No such file or directory\n", include_filename->data);
                exit(-1);
            }
            free_string(include_filename);

            FILE *f = fopen(include_filepath->data, "r"); //read header file
            char *header_data = read_file(f);
            string *header_string = convert_char(header_data, get_file_size(f));
            fclose(f);
            free(header_data);
            string_chain_part *include_lines_head = split_by_char(header_string, '\n');
            free_string(header_string);
            string_chain_part *include_lines_tail = include_lines_head;
            while (include_lines_tail->next != NULL) {
                include_lines_tail = include_lines_tail->next;
            }
            
            if (now->prev == NULL) { //insert line chain
                line_head = include_lines_head;
                include_lines_tail->next = now->next;
                now->next->prev = include_lines_tail;
                free_string_chain_part(now);
            } else {
                now = now->prev;
                free_string_chain_part(now->next);
                now->next->prev = include_lines_tail;
                include_lines_tail->next = now->next;
                now->next = include_lines_head;
                include_lines_head->prev = now;
            }
            now = include_lines_head;
        } else {
            now = now->next;
        }
    }
    return line_head;
}

string_chain_part *process_def_ifdef(string_chain_part *line_head) {
    string_chain_part *now = line_head;
    string *define_command_str = convert_char("#define", 7);
    string *if_command_str = convert_char("#if", 3);
    string *ifdef_command_str = convert_char("#ifdef", 6);
    string *ifndef_command_str = convert_char("#ifndef", 7);
    string *else_command_str = convert_char("#else", 5);
    string *endif_command_str = convert_char("#endif", 6);
    while (now != NULL) {
        if (string_part_equal(define_command_str, now->str, 0, 0, 7)) {
            
        }
    }
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
    free_string(str_src);
    line_head = process_include(line_head);
    while (line_head != NULL) {
        printf("%s\n", line_head->str->data);
        line_head = line_head->next;
    }
    //line_head = process_def_ifdef(line_head);
    return line_head;
}

