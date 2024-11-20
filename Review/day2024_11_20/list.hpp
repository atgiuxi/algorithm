#include <iostream>
#include <assert.h>

namespace xiYan
{
	template<class T>
	struct LinkedListNode
	{
		T _data;
		LinkedListNode<T> *_prev;
		LinkedListNode<T> *_next;

		LinkedListNode(const T& data = T())
			:_data(data)
			,_prev(nullptr)
			,_next(nullptr)
		{}
	};

	template<class T>
	class list
	{
		typedef LinkedListNode<T> Node;		
	private:	
		Node* _head;
		int _size;
	public:
		list()
			:_size(0)
		{
			Node* node = new Node(-1);
			_head = node;

			_head->_next = _head;
			_head->_prev = _head;
		}
	public:
		// 1.在pos位置之前插入
		void insert(Node* pos,const T& data)
		{
			Node* node = new Node(data);
			Node* prev = pos->_prev;

			node->_next = pos;
			node->_prev = prev;
			prev->_next = node;
			pos->_prev = node;

			_size++;
		}

		void erase(Node* pos)
		{
			assert(pos != _head);
		
			Node* perv = pos->_prev;
			Node* next = pos->_next;

			perv->_next = next;
			next->_prev = perv;

			delete pos;
			_size--;
		}

		void push_back(const T& data)
		{
			insert(_head,data);
		}

		void pop_back()
		{
			erase(_head->_prev);
		}

		void print()
		{
			Node* cur = _head->_next;
			while(cur != _head)
			{				
				std::cout << cur->_data << " ";
				cur = cur->_next;
			}
			std::cout << std::endl;
		}
	};

}