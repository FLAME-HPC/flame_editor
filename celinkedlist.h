#ifndef CELINKEDLIST_H
#define CELINKEDLIST_H


struct stringList_{
    char *text;
    struct stringList_ *next;
};

typedef struct stringList_ CEstringList;

void addStringList(CEstringList *item, char *s);

void addi();
#endif // LINKEDLIST_H
