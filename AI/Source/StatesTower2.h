#pragma once
#ifndef STATES_TOWER2_H_H
#define STATES_TOWER2_H_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"


class StateTower2Attack : public State
{
	GameObject* m_go;
public:
	StateTower2Attack(const std::string& stateID, GameObject* go);
	virtual ~StateTower2Attack();
	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateTower2Destroyed : public State
{
	GameObject* m_go;
public:
	StateTower2Destroyed(const std::string& stateID, GameObject* go);
	virtual ~StateTower2Destroyed();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif
