#pragma once
#ifndef STATES_Tank2_H_H
#define STATES_Tank2_H_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"


class StateAttackTank2: public State
{
	GameObject* m_go;
public:
	StateAttackTank2(const std::string& stateID, GameObject* go);
	virtual ~StateAttackTank2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateIdleTank2 : public State
{
	GameObject* m_go;
public:
	StateIdleTank2(const std::string& stateID, GameObject* go);
	virtual ~StateIdleTank2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


class StateDeadTank2 : public State
{
	GameObject* m_go;
public:
	StateDeadTank2(const std::string& stateID, GameObject* go);
	virtual ~StateDeadTank2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif