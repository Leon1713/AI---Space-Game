#pragma once
#ifndef STATES_BASE2_H_H
#define STATES_BASE2_H_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"


class StateAttack2 : public State
{
	GameObject* m_go;
public:
	StateAttack2(const std::string& stateID, GameObject* go);
	virtual ~StateAttack2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


class StateSpawn2 : public State
{
	GameObject* m_go;
public:
	StateSpawn2(const std::string& stateID, GameObject* go);
	virtual ~StateSpawn2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateDestroyed2 : public State
{
	GameObject* m_go;
public:
	StateDestroyed2(const std::string& stateID, GameObject* go);
	virtual ~StateDestroyed2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


#endif
