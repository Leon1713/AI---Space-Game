#pragma once
#ifndef STATE_PROJECTILE_2_H
#define STATE_PROJECTILE_2_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"


class StatesSeekProjectile2 : public State
{
	GameObject* m_go;
public:
	StatesSeekProjectile2(const std::string& stateID, GameObject* go);
	virtual ~StatesSeekProjectile2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};



class StateHitProjectile2 : public State
{
	GameObject* m_go;
public:
	StateHitProjectile2(const std::string& stateID, GameObject* go);
	virtual ~StateHitProjectile2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif