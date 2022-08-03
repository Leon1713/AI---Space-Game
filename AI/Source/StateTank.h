#pragma once
#ifndef STATES_Tank_H_H
#define STATES_Tank_H_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"


class StateAttackTank : public State
{
	GameObject* m_go;
public:
	StateAttackTank(const std::string& stateID, GameObject* go);
	virtual ~StateAttackTank();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateIdleTank : public State
{
	GameObject* m_go;
public:
	StateIdleTank(const std::string& stateID, GameObject* go);
	virtual ~StateIdleTank();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


class StateDeadTank : public State
{
	GameObject* m_go;
public:
	StateDeadTank(const std::string& stateID, GameObject* go);
	virtual ~StateDeadTank();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif