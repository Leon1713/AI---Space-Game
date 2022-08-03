
#include "GameObject.h"
#include "ConcreteMessages.h"
#include "PostOffice.h"

bool GameObject::Handle(Message* message)
{
	MessageCheckActive* messageCheckActive = dynamic_cast<MessageCheckActive*>(message);
	if (messageCheckActive)
	{
		delete message;
		return this->active;
	}
	delete message;
	return false;
}

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	reached(false),
	active(false),
	mass(1.f),
	moveSpeed(1.f),
	energy(10.f),
	sm(NULL),
	nearest(NULL),
	start(NULL),
	end(NULL),
	turn(false),
	isTurnBase(true),
	endTurn(false)
{
	static int count = 0;
	id = ++count;
	moveLeft = moveRight = moveUp = moveDown = true;
	PostOffice::GetInstance()->Register("GameObject", this);
	int chance = Math::RandIntMinMax(0, 1);
	if (chance == 1)
	{
		t1 = 1;
		t2 = 0;
	}
	else
	{
		t2 = 1;
		t1 = 0;
	}
}

GameObject::~GameObject()
{
	if (sm)
	{
		delete sm;
		sm = NULL;
	}
}
