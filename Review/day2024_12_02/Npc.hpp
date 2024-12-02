#pragma once
#include <vector>
#include "Poker.hpp"
#include "PokerManger.hpp"


using namespace std;

class NPC
{
public:
	NPC()
	{
		PokerManger* manger = PokerManger::GetInstance();

		for(auto& color : manger->_colors)
		{
			for(auto& point : manger->_points)
			{
				_pokers.push_back(Poker(color,point));
			}
		}

		_pokers.push_back(Poker("大王"));
		_pokers.push_back(Poker("小王"));

		Shuffle(3);
	}

	void Swap()
	{
		srand(time(nullptr));
		for(int i = 0; i < _pokers.size();i++)
		{
			int rand_num1 = rand() % _pokers.size();	
			int rand_num2 = rand() % _pokers.size();	

			swap(_pokers[rand_num1],_pokers[rand_num2]);
		}
	}

	// 洗牌
	void Shuffle(int num)
	{
		for(int i = 0;i < num;i++)
		{
			Swap();
		}
	}

	void Show()
	{
		for(vector<Poker>::iterator it = _pokers.begin();it != _pokers.end();it++)
		{
			it->PrintPoker();
		}
		cout << endl;
	}

	// 发牌
	Poker Deal()
	{
		Poker p = _pokers.back();
		_pokers.pop_back();

		_n--;
		return p;
	}

	~NPC(){}

	int GetPokerNum(){return _n;}
private:
	vector<Poker> _pokers;
	int _n = 54;
};