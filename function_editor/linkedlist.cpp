#include "./linkedlist.h"
#include <stdlib.h>
#include <string.h>

void addStringList(stringList *item, char *s)
{
    stringList *c = (stringList *) malloc(sizeof(stringList));
    c->text = (char *) malloc(strlen(s) + 1 * sizeof(char));
    strcpy(c->text, s);
    c->next = item;
    item = c;
}

void addi()
{
}
