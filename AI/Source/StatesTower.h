#pragma once
#ifndef STATES_TOWER_H_H
#define STATES_TOWER_H_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"


class StateTowerAttack : public State
{
	GameObject* m_go;
public:
	StateTowerAttack(const std::string& stateID, GameObject* go);
	virtual ~StateTowerAttack();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateTowerDestroyed : public State
{
	GameObject* m_go;
public:
	StateTowerDestroyed(const std::string& stateID, GameObject* go);
	virtual ~StateTowerDestroyed();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif
