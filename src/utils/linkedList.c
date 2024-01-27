#include "linkedList.h"
#include <malloc.h>

list *getElement(list *head, int i)
{
    while (i--)
        head = head->next;
    return head;
}

void addElement(list **head, int size, int i)
{
    list *nw = (list *)malloc(sizeof(list));
    nw->val = malloc(size);
    if (i == 0)
    {
        nw->next = *head;
        *head = nw;
        return;
    }

    list *cur = getElement(*head, i - 1);
    nw->next = cur->next;
    cur->next = nw;
}

void removeElement(list **head, int i)
{
    if (i == 0)
    {
        list *tmp = (*head)->next;
        free((*head)->val);
        free(*head);
        *head = tmp;
        return;
    }

    list *last = getElement(*head, i - 1);
    list *cur = last->next;
    last->next = cur->next;
    free(cur->val);
    free(cur);
}