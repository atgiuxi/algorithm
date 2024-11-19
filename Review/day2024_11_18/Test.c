#include <stdio.h>

int size = 0;

void showArray(int* p)
{
	for(int i = 0; i < size;i++)
	{
		printf("%d ",p[i]);
	}
	printf("\n");
}

void insertIntoArray(int* p,int pos,int x)
{
	int end = size;
	for(int i = size -1;i >= pos -1;i--)
	{
		p[end] = p[i];
		end--;
	}
	p[pos - 1] = x;
}

void deleteFromArray(int* p,int pos)
{
	for(int i = pos-1;i<size-1;i++)
	{
		p[i] = p[i + 1];
	}
}


#include <stdlib.h>
#include <assert.h>
#define N 100

typedef struct SeqList
{
	int arr[N];
	int size;
}seqList_t;

seqList_t* createEmptySeqlist()
{
	seqList_t* seq = (seqList_t*)malloc(sizeof(seqList_t));
	if(seq == NULL)
	{
		perror("malloc fail");
		return NULL;
	}
	seq->size = 0;
	return seq;
}

int isFullSeqlist(seqList_t *p)
{
	return p->size == N;
}

int isEmptySeqlist(seqList_t*p)
{
	return p->size == 0 ? 1 : 0;
}

int insertIntoSeqlist(seqList_t* seq,int pos,int data)
{
	int size = seq->size;
	for(int i = size -1;i >= pos;i--)
	{
		seq->arr[size--] = seq->arr[i];
	}

	seq->arr[pos] = data;
	seq->size++;

	return 0;
}

void showSeqlist(seqList_t *seq)
{
	for(int i = 0;i < seq->size;i++)
	{
		printf("%d ",seq->arr[i]);
	}
	printf("\n");
}

int deleteFromSeqlist(seqList_t* seq,int pos)
{
	assert(seq);
	assert(pos >=0 && pos <seq->size);

	for(int i = pos;i < seq->size - 1;i++)
	{
		seq->arr[i] = seq->arr[i + 1];
	}
	seq->size--;
	return 0;
}

int searchSeqlist(seqList_t *p,int x)
{
	int i;
	for(i = 0;i < p->size;i++)
	{
		if(x == p->arr[i])
		return i;
	}
	return -1;
}

void clearSeqlist(seqList_t *p)
{
	p->size = 0;
}

int main()
{

	seqList_t* seq = createEmptySeqlist();
	insertIntoSeqlist(seq,0,1);
	insertIntoSeqlist(seq,0,1);
	insertIntoSeqlist(seq,0,1);

	showSeqlist(seq);


	return 0;
}