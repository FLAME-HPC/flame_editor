#include "./celinkedlist.h"
#include <stdlib.h>
#include <string.h>

void addStringList(CEstringList *item, char *s)
{
    CEstringList *c = (CEstringList *) malloc(sizeof(CEstringList));
    c->text = (char *) malloc(strlen(s) + 1 * sizeof(char));
    strcpy(c->text, s);
    c->next = item;
    item = c;
}

void addi()
{
}
