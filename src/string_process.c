#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <string_process.h>

string *convert_char(const char *original_str, int str_len) {
    string *str = (string *)malloc(sizeof(string));
    str->data = (char *)malloc(str_len+1);
    memcpy(str->data, original_str, str_len);
    str->data[str_len] = '\0';
    str->size = str_len+1;
    str->len = str_len;
    return str;
}

bool string_part_equal(string *str1, string *str2, int str1_start, int str2_start, int length) {
    if (str1->len < str1_start+length || str2->len < str2_start+length) {
        return false;
    }

    for (int i = 0; i < length; ++i) {
        if (str1->data[str1_start+i] != str2->data[str2_start+i]) {
            return false;
        }
    }
    return true;
}

void free_string(string *str) {
    free(str->data);
    free(str);
}

string_chain_part *new_string_chain_part(string *str) {
    string_chain_part *part = (string_chain_part *)malloc(sizeof(string_chain_part));
    part->str = str;
    part->prev = NULL;
    part->next = NULL;
    return part;
}

void add_back_string_chain(string_chain_part *prev, string_chain_part *new_part) {
    prev->next = new_part;
    new_part->prev = prev;
}

string_chain_part *split_by_char(string *str, char sp) {
    string_chain_part *head, *now, *tmp;
    head = NULL;
    int pointer = 0;
    for (int i = 0; i < str->len; ++i) {
        if (str->data[i] == sp) {
            tmp = new_string_chain_part(convert_char(&str->data[pointer], i-pointer));
            if (head == NULL) {
                head = tmp;
            } else {
                now->next = tmp;
                tmp->prev = now;
            }
            now = tmp;
            pointer = i+1;
        }
    }
    if (head == NULL) {
        head = new_string_chain_part(convert_char(str->data, str->len));
    }
    return head;
}

void free_string_chain_part(string_chain_part *part) {
    if (part->prev != NULL) {
        part->prev->next = part->next;
    }
    if (part->next != NULL) {
        part->next->prev = part->prev;
    }
    free_string(part->str);
    free(part);
}
