#include "StateArcher2.h"
#include "SceneData.h"


static const float ENERGY_DROP_RATE = 0.2f;
static const float FULL_SPEED = 10;
static const float HUNGRY_SPEED = 4.f;
static float RANGE;

StateIdleArcher2::StateIdleArcher2(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateIdleArcher2::~StateIdleArcher2()
{

}

void StateIdleArcher2::Enter()
{
	m_go->moveSpeed = FULL_SPEED;
	//m_go->nearest = NULL;
}

void StateIdleArcher2::Update(double dt)
{
//	std::cout << "Idliing" << '\n';

	static float time = 0;
	RANGE = 5.f * SceneData::GetInstance()->getGridSize();
	m_go->moveSpeed = FULL_SPEED;
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	if (m_go->nearest)
	{
		m_go->sm->SetNextState("AttackArcher2");

	}
	else
	{
		if (time >= .3f) {
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SEARCH_TYPE::NEAREST_ENEMY_BASE2, RANGE));
			time = 0.f;
		}
	}
	time += dt * SceneData::GetInstance()->getSpeed();
}

void StateIdleArcher2::Exit()
{

}


StateAttackArcher2::StateAttackArcher2(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateAttackArcher2::~StateAttackArcher2()
{

}

void StateAttackArcher2::Enter()
{
	RANGE = 5.f * SceneData::GetInstance()->getGridSize();
	m_go->moveSpeed = FULL_SPEED;
	//m_go->nearest = NULL;
}

void StateAttackArcher2::Update(double dt)
{
	//std::cout << "Attacking" << '\n';
	static float time = 0.f;
	m_go->moveSpeed = FULL_SPEED;
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	if (time >= .32f)
	{
		PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SEARCH_TYPE::NEAREST_ENEMY_BASE2, RANGE));
		time = 0.f;
	}
	if (m_go->nearest && m_go->Health > 0 /*&& (m_go->nearest->pos - m_go->pos).LengthSquared() >= RANGE * RANGE*/)
	{
		/*if (m_go->nearest->pos.x > m_go->pos.x)
			m_go->moveLeft = false;
		else
			m_go->moveRight = false;
		if (m_go->nearest->pos.y > m_go->pos.y)
			m_go->moveDown = false;
		else
			m_go->moveUp = false;*/
		float dx = m_go->nearest->pos.x - m_go->pos.x;
		float dy = m_go->nearest->pos.y - m_go->pos.y;
		if (dx > 0)
			m_go->moveLeft = false;
		else
			m_go->moveRight = false;
		if (dy > 0)
			m_go->moveDown = false;
		else
			m_go->moveUp = false;
		if (Math::FAbs(dx) > Math::FAbs(dy))
			m_go->moveDown = m_go->moveUp = false;
		else
			m_go->moveLeft = m_go->moveRight = false;
	}
	if (m_go->nearest && m_go->Health > 0 && (m_go->nearest->pos - m_go->pos).LengthSquared() <= RANGE * RANGE && m_go->nearest->type != GameObject::GO_ETOWER && m_go->nearest->type != GameObject::GO_ETOWER2)
	{
		if(m_go->nearest->type == GameObject::GO_ETOWER)
		m_go->sm->SetNextState("ShootArcher2");
		else
			m_go->sm->SetNextState("ShootArcher2");

	}
	else if (m_go->nearest && (m_go->nearest->type == GameObject::GO_ETOWER || m_go->nearest->type == GameObject::GO_ETOWER2) && (m_go->nearest->pos - m_go->pos).LengthSquared() <= 0)
	{
		m_go->moveSpeed = 0;
		m_go->nearest = nullptr;
		m_go->reached = 1;
	}
	else if(m_go->Health > 0 && !m_go->nearest)
	{
		m_go->sm->SetNextState("IdleArcher2");

	}
	else if(m_go->Health <= 0)
	{
		m_go->sm->SetNextState("DeadArcher2");
	}
	time += dt * SceneData::GetInstance()->getSpeed();
}

void StateAttackArcher2::Exit()
{

}




StateDeadArcher2::StateDeadArcher2(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateDeadArcher2::~StateDeadArcher2()
{

}

void StateDeadArcher2::Enter()
{
	m_go->moveSpeed = 0;
	//m_go->nearest = NULL;
}

void StateDeadArcher2::Update(double dt)
{
	
	if (m_go->Health > 0) {
		m_go->sm->SetNextState("AttackArcher2");
	}
	else {
		m_go->moveSpeed = 0;
		static float ElaspedTime = 0;
		ElaspedTime += dt * SceneData::GetInstance()->getSpeed();
		if (ElaspedTime >= 3.f)
		{
			m_go->active = false;
			ElaspedTime = 0;
		}
	}
}

void StateDeadArcher2::Exit()
{

}


StateShootArcher2::StateShootArcher2(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateShootArcher2::~StateShootArcher2()
{

}

void StateShootArcher2::Enter()
{
	m_go->moveSpeed = FULL_SPEED;
	//m_go->nearest = NULL;
}

void StateShootArcher2::Update(double dt)
{
	static float time = 0;
	//std::cout << "Shooting" << '\n';
	if (m_go->nearest->type == GameObject::GO_ETOWER || m_go->nearest->type == GameObject::GO_ETOWER2)
		m_go->sm->SetNextState("IdleArcher2");
	if ((m_go->nearest->pos - m_go->pos).LengthSquared() > RANGE * RANGE || !m_go->nearest->active || m_go->nearest->Health <= 0.f)
		m_go->sm->SetNextState("AttackArcher2");
	else if (m_go->Health <= 0)
	{
		m_go->sm->SetNextState("DeadArcher2");
	}
	else if ((m_go->nearest->Health > 0 || m_go->nearest->Health > 0 &&  m_go->nearest->type == GameObject::GO_BASE) && (m_go->nearest->pos - m_go->pos).LengthSquared() <= RANGE * RANGE)
	{
		if (time >= .8f) {
			PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_PROJECTILE2));
			time = 0;
		}
		m_go->moveSpeed = 0;
	}
		time += dt * SceneData::GetInstance()->getSpeed();
}

void StateShootArcher2::Exit()
{

}