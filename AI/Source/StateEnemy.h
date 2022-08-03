#pragma once
#ifndef STATES_Enemy_H_H
#define STATES_Enemy_H_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"

class StateEnemyWander : public State
{
	// wander random direction using bfs
	//BFS limit till out of AP
	GameObject* m_go;
public:
	StateEnemyWander(const std::string& stateID, GameObject* go);
	virtual ~StateEnemyWander();
	void Enter();
	void Update(double _dt);
	void Exit();

	
};

class StateEnemyHunt : public State // Hunt player BFSLimit to player pos;
{
	GameObject* m_go;
public:
	StateEnemyHunt(const std::string& stateID, GameObject* go);
	virtual ~StateEnemyHunt();
	void Enter();
	void Update(double _dt);
	void Exit();
};



#endif