#pragma once
#ifndef STATES_Archer2_H_H
#define STATES_Archer2_H_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"


class StateAttackArcher2 : public State
{
	GameObject* m_go;
public:
	StateAttackArcher2(const std::string& stateID, GameObject* go);
	virtual ~StateAttackArcher2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateIdleArcher2 : public State
{
	GameObject* m_go;
public:
	StateIdleArcher2(const std::string& stateID, GameObject* go);
	virtual ~StateIdleArcher2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


class StateDeadArcher2 : public State
{
	GameObject* m_go;
public:
	StateDeadArcher2(const std::string& stateID, GameObject* go);
	virtual ~StateDeadArcher2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateShootArcher2 : public State
{
	GameObject* m_go;
public:
	StateShootArcher2(const std::string& stateID, GameObject* go);
	virtual ~StateShootArcher2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif