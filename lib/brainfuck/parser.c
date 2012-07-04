#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "brainfuck.h"
#define MAXLINE 998

void parse(int, QUEUE *);
void parseq(QUEUE *);
void handle_loop();

int array[30000];
FILE *fp;
int i = 0;

int main(int argc, char *argv[]) {
    fp = fopen(argv[1], "r");
    int c;
    while ((c = fgetc(fp)) != EOF) {
        parse(c, 0);
    }
    return 0;
}

void parse(int c, QUEUE *loop) {
    switch (c) {
        case '>':
            i++;
            break;
        case '<':
            i--;
            break;
        case '+':
            array[i]++;
            break;
        case '-':
            array[i]--;
            break;
        case '.':
            putchar(array[i]);
            break;
        case ',':
            array[i] = getchar();
            break;
        case '[':
            handle_loop(loop);
            break;
    }
}

void parseq(QUEUE *ql) {
    QUEUE *q = copy(ql);
    while (q->len > 0) {
        parse(popleft(q), q);
    }
}

int nextch(QUEUE *q) {
    if (q == 0) {
        return fgetc(fp);
    } else {
        return popleft(q);
    }
}
    
void handle_loop(QUEUE *lq) {
    QUEUE *loop = new_queue();
    int looplvl = 1;
    int c;
    do {
        c = nextch(lq);
        if (c == '[') {
            looplvl++;
        } else if (c == ']') {
            looplvl--;
        }
        if (looplvl > 0) {
            append(loop, c);
        }
    } while (looplvl > 0);
    while (array[i] != 0) {
        parseq(loop);
    }
}
