#pragma once
#include <vector>
#include "Poker.hpp"

class Player
{
public:
	Player(const string& name)
		:_name(name)
	{}
	Player(){}
	inline int GetScore(){return _scores;}
	inline string& GetName(){return _name;}
	inline bool GetFlog(){return _flag;}

	bool HitCards(const Poker& poker)
	{
		_pokers.push_back(poker);
		_scores += poker.GetScore();
		return true;
	}

	void Show()
	{
		cout << _name << ":";
		for(auto& e:_pokers)
		{
			e.PrintPoker();
		}
		cout << " 分数 "<< _scores; 
		cout << endl;
	}

	string _name;
	vector<Poker> _pokers;	
	int _scores = 0;
	bool _flag = true;
};