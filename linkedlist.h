#ifndef LINKEDLIST_H
#define LINKEDLIST_H


struct stringList_{
    char *text;
    struct stringList_ *next;
};

typedef struct stringList_ stringList;

void addStringList(stringList *item, char *s);

void addi();
#endif // LINKEDLIST_H
