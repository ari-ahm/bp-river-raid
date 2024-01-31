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
        nw->prev = NULL;
        if (*head) (*head)->prev = nw;
        *head = nw;
        return;
    }

    list *cur = getElement(*head, i - 1);
    nw->next = cur->next;
    nw->prev = cur;
    if (cur->next) cur->next->prev = nw;
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
        (*head)->prev = NULL;
        return;
    }

    list *last = getElement(*head, i - 1);
    list *cur = last->next;
    last->next = cur->next;
    if (cur->next) cur->next->prev = last;
    free(cur->val);
    free(cur);
}

void removeElementPtr(list **head, list *member)
{
    if (member->prev == NULL)
        *head = member->next;
        
    if (member->prev) member->prev->next = member->next;
    if (member->next) member->next->prev = member->prev;
    free(member->val);
    free(member);
}