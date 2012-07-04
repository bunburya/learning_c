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
    /*array = (int *) malloc(30000 * sizeof(int));*/
    /*memset(array, 0, 30000 * sizeof(int));*/
    while ((c = fgetc(fp)) != EOF) {
        parse(c, 0);
    }
    return 0;
}

void parse(int c, QUEUE *loop) {
    /*putchar(c);*/
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
            /*printf("getting\n");*/
            array[i] = getchar();
            /*printf("got\n");*/
            break;
        case '[':
            handle_loop(loop);
            break;
    }
}

/* Error handling loops within loops:
 *
 * - handle_loop originally gets characters from fp; this works fine.
 *
 * - parseq called on line read by handle_loop; this passes the whole
 *   line to parse, which passes the whole line on to handle_loop,
 *   when it should only pass on the part of the line after the next
 *   '['.
 *
 * - When we are reading a line from a QUEUE rather than a file and
 *   we encounter a loop, we should not pass the whole line on to
 *   handle_loop; we should only pass on the part of the line after
 *   the '['.
 *   */

void parseq(QUEUE *ql) {
    /* Possible solution: create a copy of ql here, and use
     * popleft to read values from it.
     * */
    /*printf("\nparsing line\n");*/
    QUEUE *q = copy(ql);
    /*printf("copy len is %d\n", q->len);*/
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
    /*printf("\nentering loop\n");*/
    QUEUE *loop = new_queue();
    int looplvl = 1;
    int c;
    do {
        c = nextch(lq);
        /*putchar(c);*/
        /*printf("c is %d\n", c);*/
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
        /*printf("cell val is %d\n", array[i]);*/
    }
    /*printf("\nleaving loop\n");*/
}
