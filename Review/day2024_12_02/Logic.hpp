#pragma once 
#include "Npc.hpp"
#include "Player.hpp"

class Logic
{
public:
	Logic(NPC npc = NPC()):_npc(npc){}
	Logic(vector<Player> players,NPC npc = NPC()):_npc(npc),_players(players){}

	void Join(const Player& player)
	{
		_players.push_back(player);
	}

	void Ask()
	{
		for(auto& player : _players)
		{
			if(player.GetFlog() == true)
			{
				while(1)
				{
					cout << player._name << "你是否要牌:y or n :";

					string s;
					cin >> s;
					if(s == "y" && _npc.GetPokerNum() > 0)
					{
						player.HitCards(_npc.Deal());
					}
					else {
						player._flag = false;
						break;
					}
				}
			}
		}
	}

	void Check()
	{
		for(auto& e : _players)
		{
			if(e.GetFlog() == false) continue;
			if(e._scores > 21)
			{
				std::cout << e._name + "你的点数大于21,出局!" << endl;
				e._flag = false;
			} 
		}
	}

	void Display()
	{
		for(auto& e : _players)
		{
			e.Show();
		}
	}

	bool IsStop()
	{
		if(_npc.GetPokerNum() <= 0) return false;	
		bool flag = false;

		for(auto& e : _players)
		{
			if(e.GetFlog() == true)
			{
				flag = true;
				break;
			}
		}
		return flag;
	}

	void Winner()
	{		
		Player play("EOF");
		for(auto& e : _players)
		{
			if(e._scores > 21)continue;

			if(e._scores > play._scores) play = e;
		}

		if(play._name == "EOF")
		{
			cout << "对局结束,没有赢家!" << endl;
		}
		else {
			cout << "赢家是:";
			play.Show();
		}

	}

	void Run()
	{
		while(IsStop())
		{
			Ask();
			Check();
			Display();
		}
		Winner();
	}

private:
	NPC _npc;
	vector<Player> _players;
};