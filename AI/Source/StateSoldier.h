#pragma once
#ifndef STATES_SOLDIER_H_H
#define STATES_SOLDIER_H_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"


class StateAttackSoldier : public State
{
	GameObject* m_go;
public:
	StateAttackSoldier(const std::string& stateID, GameObject* go);
	virtual ~StateAttackSoldier();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateIdleSoldier : public State
{
	GameObject* m_go;
public:
	StateIdleSoldier(const std::string& stateID, GameObject* go);
	virtual ~StateIdleSoldier();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


class StateDeadSoldier : public State
{
	GameObject* m_go;
public:
	StateDeadSoldier(const std::string& stateID, GameObject* go);
	virtual ~StateDeadSoldier();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif