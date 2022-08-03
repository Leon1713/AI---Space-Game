#include "TurnManager.h"

void TurnManager::Add(GameObject* go)
{
	for (int i = 0; i < turnList.size(); ++i)
	{
		if (go == turnList[i])
		{
			return;
		}
	}
	turnList.push_back(go);
}



void TurnManager::remove(GameObject* go)
{
	if (turnList.empty())
	{
		return;
	}
	for (int i = 0; i < turnList.size(); ++i)
	{
		if (go == turnList[i])
		{
			turnList[i]->turn = 0;
			turnList.erase(turnList.begin() + i);
			return;
		}
	}
}

int TurnManager::getTurn(GameObject* go)
{
	for (int i = 0; i < turnList.size(); ++i)
	{
		if (go == turnList[i])
		{
			return i;
		}
	}
	return -1;
}

void TurnManager::Clear()
{
	for (int i = 0; i < turnList.size(); ++i)
	{
		turnList[i]->turn = 0;
	}
	turnList.clear();
}

void TurnManager::Update(double dt)
{
	if (turnList.empty())
		return;

	turnList[0]->turn = 1;
}

TurnManager::~TurnManager()
{
	turnList.clear();
}

TurnManager::TurnManager()
{

}
