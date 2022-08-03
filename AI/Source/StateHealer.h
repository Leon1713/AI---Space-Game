#pragma once
#ifndef STATES_Healer_H_H
#define STATES_Healer_H_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"

class StateHealerWander : public State
{
	// wander random direction using bfs
	//BFS limit till out of AP
	GameObject* m_go;
public:
	StateHealerWander(const std::string& stateID, GameObject* go);
	virtual ~StateHealerWander();
	void Enter();
	void Update(double _dt);
	void Exit();

	
};

class StateHealerHeal : public State // Hunt player BFSLimit to player pos;
{
	GameObject* m_go;
public:
	StateHealerHeal(const std::string& stateID, GameObject* go);
	virtual ~StateHealerHeal();
	void Enter();
	void Update(double _dt);
	void Exit();
};



#endif