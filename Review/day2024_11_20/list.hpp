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
	struct __list__iterator
	{
		typedef LinkedListNode<T> Node;
		typedef __list__iterator<T> Self;
		Node* _node;
		__list__iterator(Node* node) :_node(node){}

		bool operator !=(Self it) {return _node != it._node;}
		T* operator->() {&_node->_data;}
		Self& operator--()
		{
			_node = _node->_prev;
			return *this;
		}
	};

	template<class T>
	class list
	{
		typedef LinkedListNode<T> Node;		
		typedef __list__iterator<T> iterator;
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
		iterator begin() {return iterator(_head->_next);}
		iterator end() {return iterator(_head);}

		iterator insert(iterator pos,const T& data)
		{
			Node* node = new Node(data);
			Node* prev = pos._node->_prev;

			node->_next = pos._node;
			node->_prev = prev;
			prev->_next = node;
			pos._node->_prev = node;


			_size++;
			return iterator(node);
		}

		iterator erase(iterator pos)
		{
			assert(pos._node != _head);
		
			Node* perv = pos._node->_prev;
			Node* next = pos._node->_next;

			perv->_next = next;
			next->_prev = perv;

			delete pos._node;
			_size--;
			return iterator(next);
		}

		void push_back(const T& data){ insert(begin(),data);}

		void pop_back(){ erase(--end()); }

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