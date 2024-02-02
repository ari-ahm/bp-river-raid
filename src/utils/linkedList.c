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
        if (*head)
            (*head)->prev = nw;
        *head = nw;
        return;
    }

    list *cur = getElement(*head, i - 1);
    nw->next = cur->next;
    nw->prev = cur;
    if (cur->next)
        cur->next->prev = nw;
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
        if (*head)
            (*head)->prev = NULL;
        return;
    }

    list *last = getElement(*head, i - 1);
    list *cur = last->next;
    last->next = cur->next;
    if (cur->next)
        cur->next->prev = last;
    free(cur->val);
    free(cur);
}

void removeElementPtr(list **head, list *member)
{
    if (member->prev == NULL)
        *head = member->next;

    if (member->prev)
        member->prev->next = member->next;
    if (member->next)
        member->next->prev = member->prev;
    free(member->val);
    free(member);
}

void sort(list **begin, list *end, int (*check)(list *, list *))
{
    int size = 0;
    for (list *i = *begin; i != end; i = i->next, size++)
        ;

    if (size < 2)
        return;

    list *mid = getElement(*begin, size / 2);
    sort(begin, mid, check);
    sort(&mid, end, check);

    list *ans[size];
    list *pt1 = *begin, *pt2 = mid;
    for (int i = 0; i < size; i++)
    {
        if (pt2 == end || (pt1 != mid && check(pt2, pt1)))
        {
            ans[i] = pt1;
            pt1 = pt1->next;
        }
        else
        {
            ans[i] = pt2;
            pt2 = pt2->next;
        }
    }

    if ((*begin)->prev)
        (*begin)->prev->next = ans[0];
    if (end)
        end->prev = ans[size - 1];

    ans[0]->prev = (*begin)->prev;

    ans[size - 1]->prev = ans[size - 2];
    ans[size - 1]->next = end;

    for (int i = 1; i < size; i++)
    {
        ans[i]->prev = ans[i - 1];
        ans[i - 1]->next = ans[i];
    }

    (*begin) = ans[0];
}