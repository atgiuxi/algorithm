#include "SeqList.h"

void Init(SeqList* seq)
{
	assert(seq);
	DataType* arr = (DataType*)malloc(sizeof(DataType) * INIT_CAPACITY);
	if(arr == NULL)
	{
		perror("malloc fail");
		return;
	}

	seq->arr = arr;
	seq->size = 0;
	seq->capacity = INIT_CAPACITY;	
}

void Destroy(SeqList* seq)
{
	assert(seq);
	free(seq->arr);
	seq->arr = NULL;
	seq->size = seq->capacity = 0;	
}

void CheckCapacity(SeqList* seq)
{
	assert(seq);
	if(seq->size == seq->capacity)
	{
		int reSize = seq->capacity * 2;
		DataType* p = realloc(seq->arr,reSize * sizeof(DataType));	
		if(p == NULL)
		{
			perror("realloc fail");
			return;
		}
		seq->arr = p;
	}
}

void Print(SeqList* seq)
{
	assert(seq);
	for(int i = 0;i < seq->size;i++)
	{
		printf("%d ",seq->arr[i]);
	}
	printf("\n");
}

void Insert(SeqList* seq,int pos,DataType data)
{
	assert(seq != NULL);

	CheckCapacity(seq);
	int size = seq->size;
	for(int i = size -1;i >= pos;i--)
	{
		seq->arr[size--] = seq->arr[i];
	}

	seq->arr[pos] = data;
	seq->size++;
}

void Erase(SeqList* seq,int pos)
{
	assert(seq);
	assert(pos >=0 && pos <seq->size);

	for(int i = pos;i < seq->size - 1;i++)
	{
		seq->arr[i] = seq->arr[i + 1];
	}
	seq->size--;
}