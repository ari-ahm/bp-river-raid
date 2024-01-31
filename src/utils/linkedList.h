#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct List
{
    void *val;
    struct List *prev;
    struct List *next;
} list;

list *getElement(list *head, int i);
void addElement(list **head, int size, int i);
void removeElement(list **head, int i);
void removeElementPtr(list **head, list *member);

#endif