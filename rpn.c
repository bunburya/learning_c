#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXOP   100 /* max size of operand/operator */
#define NUMBER  '0' /* signal that a number has been found */
#define OPER    '*' /* signal that operator has been found */
#define MAXVAL  100 /* max depth of value stack */
#define BUFSIZE 100 /* max size of character buffer */
#define TRUE    1
#define FALSE   0

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

int main() {
    /* Reverse Polish Notation calculator */
    int type;
    double op2;
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
        switch (type) {
            case NUMBER:
                push(atof(s));
                break;
            case 't':
                push(tan(pop()));
                break;
            case 'c':
                push(cos(pop()));
                break;
            case 's':
                push(sin(pop()));
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
    while ((s[0] = c = getch()) == ' ' || c == '\t'); 
    if (c == '\n' || c == EOF) {
        /* preliminary check to see if we are at a newline or EOF */
        return c;
    }
    if (!isdigit(c) && c != '.') {
        /* c is a sign or operator */
        if ((c == '+' || c == '-') && (isdigit(ch = peekch()) || ch == '.')) {
            /* c is a sign, not an operator */
            s[i] = c;
        } else {
            s[i] = c;
            while (!isspace(s[++i] = c = getch()) && !isdigit(c) && c != '.') {
                mco = TRUE;
            }
            ungetch(c);
            s[i] = '\0';
            if (mco == TRUE) {
                return s[0];
            } else {
                return s[--i];
            }
        }
    }
    if (isdigit(c) || c == '-' || c == '+') {   /* collect integer part */
        while (isdigit(s[++i] = c = getch()));
    }
    if (c == '.' || c == '-' || c == '+') { /* collect fraction part */
        while (isdigit(s[++i] = c = getch()));
    }
    s[i] = '\0';
    if (c != EOF) {
        ungetch(c);
    }
    return NUMBER;
}

void push(double d) {
    if (sp < MAXVAL) {
        vstack[sp++] = d;
    } else {
        printf("Error: Stack size exceeds limit.\n");
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
    /* Look at the next character without affecting the input stream. */
    char ch = getch();
    ungetch(ch);
    return ch;
}

int islimit(char ch) {
    /* Test whether a character is a delimiter separating operators
     * from operands. */
    return ((ch == ' ' || ch == '\t' || ch == '\0') ? TRUE : FALSE);
}
