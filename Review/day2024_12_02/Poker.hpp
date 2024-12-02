#pragma once
#include <string>
#include <iostream>
#include "PokerManger.hpp"

using namespace std;
class Poker
{
private:
	std::string _color;	
	std::string _point;
	int _score = 0;
	PokerManger* _poker_manger;
public:
	int GetScore()const {return _score;}

public:
	// 创建一张牌
	Poker(string color,string point)
		:_color(color)
		,_point(point)
	{
		_poker_manger = PokerManger::GetInstance();
		int score = _poker_manger->_socres[point];

		_score = score;
	}

	// 创建大小王
	Poker(string color)
		:_color(color)
	{}

	void PrintPoker()
	{		
		cout << _color <<  _point << " ";
	}

	~Poker(){}
};