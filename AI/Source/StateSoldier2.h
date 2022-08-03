#pragma once
#ifndef STATES_SOLDIER2_H_H
#define STATES_SOLDIER2_H_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"


class StateAttackSoldier2: public State
{
	GameObject* m_go;
public:
	StateAttackSoldier2(const std::string& stateID, GameObject* go);
	virtual ~StateAttackSoldier2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateIdleSoldier2 : public State
{
	GameObject* m_go;
public:
	StateIdleSoldier2(const std::string& stateID, GameObject* go);
	virtual ~StateIdleSoldier2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


class StateDeadSoldier2 : public State
{
	GameObject* m_go;
public:
	StateDeadSoldier2(const std::string& stateID, GameObject* go);
	virtual ~StateDeadSoldier2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif