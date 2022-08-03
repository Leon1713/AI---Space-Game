#pragma once
#ifndef STATES_PLAYER_H_H
#define STATES_PLAYER_H_H

#include "State.h"
#include "GameObject.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
class StatePlayerIdle : public State // when not player turn
{
	GameObject* m_go;
public:
	StatePlayerIdle(const std::string& stateID, GameObject* go);
	virtual ~StatePlayerIdle();
	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();

};

class StatePlayerMovement : public State // move or moving
{
	GameObject* m_go;
public:
	StatePlayerMovement(const std::string& stateID, GameObject* go);
	virtual ~StatePlayerMovement();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StatePlayerAttack : public State // near enemie can choose to attack or go back to move. // highlight grid red
{
	GameObject* m_go;
public:
	StatePlayerAttack(const std::string& stateID, GameObject* go);
	virtual ~StatePlayerAttack();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};


class StatePlayerDead : public State // Game over
{
	GameObject* m_go;
public:
	StatePlayerDead(const std::string& stateID, GameObject* go);
	virtual ~StatePlayerDead();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif