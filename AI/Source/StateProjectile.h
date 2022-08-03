#pragma once
#ifndef STATE_PROJECTILE_H
#define STATE_PROJECTILE_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"


class StatesSeekProjectile : public State
{
	GameObject* m_go;
public:
	StatesSeekProjectile(const std::string& stateID, GameObject* go);
	virtual ~StatesSeekProjectile();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};



class StateHitProjectile : public State
{
	GameObject* m_go;
public:
	StateHitProjectile(const std::string& stateID, GameObject* go);
	virtual ~StateHitProjectile();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif