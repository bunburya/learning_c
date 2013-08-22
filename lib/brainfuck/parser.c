#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "brainfuck.h"
#define MAXLINE 998

int read_file(FILE *, struct list *);
void parse_prog(struct list *);
void parse_char(char, struct list *, struct list *);

struct list *prog;
FILE *fp;
int i = 0;

int main(int argc, char *argv[]) {
    fp = fopen(argv[1], "r");
    prog = new_list(1000);
    read_file(fp, prog);
    parse_prog(prog);
    return 0;
}

int read_file(FILE *fp, struct list *ls) {
    // Read file into memory
    char c;
    while ((c = fgetc(fp)) != EOF) {
        push(ls, c);
    }
    push(ls, '\0');
    // TODO: return 1 if problem reading from file
    return 0;
}

void parse_prog(struct list *prog) {
    struct list *cells = new_list(30000);
    struct list *loops = new_list(1000);
    int i; // position in program
    char c;
    while (c = peek(prog, i)) {
        parse_char(c, cells, loops);
    }
}
        
void parse_char(char c, struct list *cells, struct list *loops) {
    switch (c) {
        case '>':
            i++;
            break;
        case '<':
            i--;
            break;
        case '+':
            incr(cells, i);
            break;
        case '-':
            decr(cells, i);
            break;
        case '.':
            putchar(peek(cells, i));
            break;
        case ',':
            assign(cells, i, getchar());
            break;
        case '[':
            push(loops, i++);
            break;
        case ']':
            if (peek(cells, i)) {
                i = peek(loops, (loops->len)-1);
            } else {
                pop(loops);
                i++;
            }
    }
}
