typedef struct node {
    struct node *next;
    struct node *prev;
    int val;
} NODE;

typedef struct queue {
    NODE *first;
    NODE *last;
    NODE *current;
    int len;
} QUEUE;

QUEUE *new_queue();
int popleft(QUEUE *);
int next(QUEUE *);
void append(QUEUE *, int);
QUEUE *copy(QUEUE *);
