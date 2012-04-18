#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAXOP   100 /* max size of operand/operator */
#define NUMBER  '0' /* signal that a number has been found */
#define MCO     -10 /* signal that multi-char operator has been found */
#define MAXVAL  100 /* max depth of value stack */
#define BUFSIZE 100 /* max size of character buffer */
#define MAXLINE 1000
#define TRUE    1
#define FALSE   0

/* TODO:
 * - function istext, tests to see if a char is possibly part of an
 *   operator string (alpha + punctuation, or !isdigit and !isspace).
 */

int mygetline(char [], int);

void handlemco(char []);

int getop(char []);
void push(double);
double pop();

char getch();
void ungetch(char);
char peekch();
int islimit(char);

char chbuf[BUFSIZE];
int bp = 0;

double vstack[MAXVAL];
int sp = 0; /* current position on the stack */

char line[MAXLINE];
int lp = 0;

int main() {
    /* Reverse Polish Notation calculator */
    int type;
    double op2;
    char s[MAXOP];
    mygetline(line, MAXLINE);
    while ((type = getop(s)) != '\0') {
        switch (type) {
            case NUMBER:
                push(atof(s));
                break;
            case MCO:
                handlemco(s);
                break;
            case '+':
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '/':
                op2 = pop();
                if (op2 == 0.0) {
                    printf("Error: Division by zero.\n");
                } else {
                    push(pop() / op2);
                }
                break;
            case '%':
                op2 = pop();
                if (op2 == 0.0) {
                    printf("Error: Division by zero.\n");
                } else {
                    push(fmod(pop(), op2));
                }
                break;
            case '\n':
                printf("%f\n", pop());
                break;
            default:
                printf("Error: Unknown command: %d\n", type);
        }
    }
    return 0;
}

int getop(char s[]) {
    /* Get next character or numeric operand */
    int i = 0;
    int c, ch;
    int mco = FALSE;    /* whether we have a multi-char operator */
    while ((s[0] = c = line[lp++]) == ' ' || c == '\t'); 
    if (c == '\n' || c == '\0') {
        /* preliminary check to see if we are at a newline or EOS */
        if (c == '\0') {
            lp = 0;
        }
        return c;
    }
    if (!isdigit(c) && c != '.') {
        /* c is a sign or operator */
        s[i] = c;
        if ((c == '+' || c == '-') && (isdigit(ch = line[lp]) || ch == '.')) {
            /* c is a sign, not an operator */
        } else {
            while (!isspace(s[++i] = c = line[lp++]) && !isdigit(c) && c != '.') {
                mco = TRUE;
            }
            lp--;
            s[i--] = '\0';
            if (mco == TRUE) {
                return MCO;
            } else {
                return s[i];
            }
        }
    }
    if (isdigit(c) || c == '-' || c == '+') {   /* collect integer part */
        while (isdigit(s[++i] = c = line[lp++]));
    }
    if (c == '.') { /* collect fraction part */
        while (isdigit(s[++i] = c = line[lp++]));
    }
    s[i] = '\0';
    if (c != '\0') {
        lp--;
    }
    return NUMBER;
}

void handlemco(char s[]) {
    double op2;
    if (strcmp(s, "tan") == 0) {
        push(tan(pop()));
    } else if (strcmp(s, "sin") == 0) {
        push(sin(pop()));
    } else if (strcmp(s, "cos") == 0) {
        push(cos(pop()));
    } else if (strcmp(s, "**") == 0) {
        op2 = pop();
        push(pow(pop(), op2));
    }
}

void push(double d) {
    if (sp < MAXVAL) {
        vstack[sp++] = d;
    } else {
        printf("Error: Stack size exceeds limit.\n");
        exit(1);
    }
}

double pop() {
    if (sp > 0) {
        return vstack[--sp];
    } else {
        printf("Error: Pop from empty stack.\n");
        exit(0);
        return -1.0;
    }
}

/*
char getch() {
    if (bp > 0) {
        return chbuf[--bp];
    } else {
        return getchar();
    }
}

void ungetch(char ch) {
    if (bp > BUFSIZE) {
        printf("Error: Too many characters in buffer.\n");
    } else {
        chbuf[bp++] = ch;
    }
}

char peekch() {
    char ch = getch();
    ungetch(ch);
    return ch;
}
*/

int islimit(char ch) {
    /* Test whether a character is a delimiter separating operators
     * from operands. */
    return ((ch == ' ' || ch == '\t' || ch == '\0') ? TRUE : FALSE);
}

