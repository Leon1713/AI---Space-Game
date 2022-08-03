#pragma once
#ifndef STATES_BASE_H_H
#define STATES_BASE_H_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"


class StateAttack : public State
{
	GameObject* m_go;
public:
	StateAttack(const std::string& stateID, GameObject* go);
	virtual ~StateAttack();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


class StateSpawn : public State
{
	GameObject* m_go;
public:
	StateSpawn(const std::string& stateID, GameObject* go);
	virtual ~StateSpawn();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateDestroyed : public State
{
	GameObject* m_go;
public:
	StateDestroyed(const std::string& stateID, GameObject* go);
	virtual ~StateDestroyed();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif
