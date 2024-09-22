#include <stdio.h>

#include <help.h>

const char *help_message = "Usage: cc [options] file...\nOptions:\n  --help\t\tDisplay this information.\n";

void print_help_message() {
    printf("%s", help_message);
}
