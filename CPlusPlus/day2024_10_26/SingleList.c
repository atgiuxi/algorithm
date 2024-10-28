#include "SingleList.h"

Node* NewNode(int data)
{
    Node* newNode = (Node*)malloc(sizeof(Node));

    if(newNode == NULL)
    {
        perror("malloc fail");
        return NULL;
    }

    newNode->data = data;
    newNode->next = NULL;

    return newNode;
}


void PushBack(Node** ppHead,int data)
{
    Node* newNode = NewNode(data);

    if(*ppHead == NULL)
    {
        *ppHead = newNode;
        return;
    }
    else
    {
        Node* tail = *ppHead;
        while(tail->next != NULL)
        {
            tail = tail->next;
        }

        tail->next = newNode;
    }
}

void PopBack(Node** ppHead)
{
    if(*ppHead == NULL) return;

    
    Node* tail = *ppHead;

    if(tail->next == NULL)
    {
        tail->next = NULL;
    }
    while (tail->next->next != NULL)
    {
        tail = tail->next;
    }

    tail->next->next = NULL; 
}

void Print(Node* phead)
{
    while(phead != NULL)
    {
        printf("%d->",phead->data);
        phead = phead->next;
    }
    printf("NULL\n");
}