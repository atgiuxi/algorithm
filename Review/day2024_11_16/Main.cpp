#include <vector>
#include <iostream>
#include <time.h>
#include <stdlib.h>


using namespace std;

struct SkiplistNode
{	
	int _data;
	vector<SkiplistNode*> _nextV;

	SkiplistNode(int data,int level)
		:_data(data)
		,_nextV(level,nullptr)
	{}
};


class Skiplist
{
	typedef SkiplistNode Node;
private:
	Node* _head;
	int _maxLevel = 32;
	double _p = 0.25;

public:
	Skiplist()
	{
		_head = new Node(-1,1);
	}

	bool search(int target)
	{
		Node* cur = _head;
		int level = cur->_nextV.size() -1;

		while(level >= 0)
		{
			if(cur->_nextV[level] && cur->_nextV[level]->_data < target)
			{
				cur = cur->_nextV[level];
			}
			else if(cur->_nextV[level] == nullptr || cur->_nextV[level]->_data > target)
			{
				level--;
			}
			else {return true;}
		}
		return false;
	}

	vector<Node*> findPervV(int num)
	{
		Node* cur = _head;
		int level = _head->_nextV.size() -1;
		vector<Node*> prevV(level + 1,nullptr);

		while(level >= 0)
		{			
			if(cur->_nextV[level] != nullptr && cur->_nextV[level]->_data < num)
			{
				cur = cur->_nextV[level];
			}
			else if(cur->_nextV[level] == nullptr || cur->_nextV[level]->_data >= num)
			{
				prevV[level] = cur;
				level--;
			}
		}
		return prevV;	
	}

	void add(int num)
	{
		int n = randomLevel();

		int level = _head->_nextV.size() -1;
		vector<Node*> prevV = findPervV(num);

		if(n > _head->_nextV.size())
		{
			_head->_nextV.resize(n,nullptr);
			prevV.resize(n,_head);
		}

		Node* newNode = new Node(num,n);	
		for(int i = 0;i < n;i++)
		{			
			newNode->_nextV[i] = prevV[i]->_nextV[i];
			prevV[i]->_nextV[i] = newNode;
		}
	}

	int randomLevel()
	{
		Node* cur = _head;
		srand(time(NULL));

		int level= 1;
		while(rand() <= RAND_MAX*_p && level < _maxLevel) { level++;}

		return level;
	}

	bool erase(int num)
	{
		vector<Node*> prevV = findPervV(num);

		if(prevV[0]->_nextV[0] == nullptr || prevV[0]->_nextV[0]->_data != num)
		{
			return false;
		}
		else
		{
			Node* del = prevV[0]->_nextV[0];

			for(int i = 0;i < del->_nextV.size();i++)
			{
				prevV[i]->_nextV[i] = del->_nextV[i];
			}
			delete del;

			int index = _head->_nextV.size() -1;

			while (index >= 0) 
			{
				if(_head->_nextV[index] == nullptr) {index--;}
				else {break;}	
			}

			return true;
		}
	}


	void print()
	{
		Node* cur = _head->_nextV[0];

		while(cur != nullptr)
		{			
			cout << cur->_data << "->";	
			cur = cur->_nextV[0];
		}
		cout << "NULL" << endl;
	}

};

int main()
{
	Skiplist list;	
	list.add(1);
	list.add(3);
	list.add(6);

	cout << list.search(6) << endl;

	list.erase(9);
	list.erase(6);

	cout << list.search(6) << endl;

	list.print();
	return 0;
}