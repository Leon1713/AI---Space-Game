#pragma once
#ifndef STATES_Archer_H_H
#define STATES_Archer_H_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"


class StateAttackArcher : public State
{
	GameObject* m_go;
public:
	StateAttackArcher(const std::string& stateID, GameObject* go);
	virtual ~StateAttackArcher();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateIdleArcher : public State
{
	GameObject* m_go;
public:
	StateIdleArcher(const std::string& stateID, GameObject* go);
	virtual ~StateIdleArcher();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


class StateDeadArcher : public State
{
	GameObject* m_go;
public:
	StateDeadArcher(const std::string& stateID, GameObject* go);
	virtual ~StateDeadArcher();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateShootArcher : public State
{
	GameObject* m_go;
public:
	StateShootArcher(const std::string& stateID, GameObject* go);
	virtual ~StateShootArcher();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif