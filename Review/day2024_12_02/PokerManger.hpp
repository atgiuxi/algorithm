#pragma once
#include <string>
#include <vector>
#include <map>
/*	
	扑克牌管理类
*/

class PokerManger
{
private:
	PokerManger()
	{
		_colors.push_back("红桃");
		_colors.push_back("黑桃");
		_colors.push_back("方片");
		_colors.push_back("草花");

		_points.push_back("A");
		_points.push_back("2");
		_points.push_back("3");
		_points.push_back("4");
		_points.push_back("5");
		_points.push_back("6");
		_points.push_back("7");
		_points.push_back("8");
		_points.push_back("9");
		_points.push_back("10");
		_points.push_back("J");
		_points.push_back("Q");
		_points.push_back("K");

		_socres["A"] = 1;
		_socres["2"] = 2;
		_socres["3"] = 3;
		_socres["4"] = 4;
		_socres["5"] = 5;
		_socres["6"] = 6;
		_socres["7"] = 7;
		_socres["8"] = 8;
		_socres["9"] = 9;
		_socres["10"] = 10;
		_socres["J"] = 1;
		_socres["Q"] = 1;
		_socres["K"] = 1;
	}

	PokerManger(const PokerManger&);
	PokerManger operator=(const PokerManger&);
	~PokerManger();
public:
	static PokerManger* GetInstance()
	{
		if(_manger == nullptr) _manger = new PokerManger();
		return _manger;
	}

	static PokerManger* _manger;
	std::vector<std::string> _colors;
	std::vector<std::string> _points;
	std::map<std::string,int> _socres;	
};
PokerManger* PokerManger::_manger = nullptr;

