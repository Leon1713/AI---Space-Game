#include "StateArcher.h"
#include "SceneData.h"


static const float ENERGY_DROP_RATE = 0.2f;
static const float FULL_SPEED = 10;
static const float HUNGRY_SPEED = 4.f;
static float RANGE;

StateIdleArcher::StateIdleArcher(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateIdleArcher::~StateIdleArcher()
{

}

void StateIdleArcher::Enter()
{
	m_go->moveSpeed = FULL_SPEED;
	//m_go->nearest = NULL;
}

void StateIdleArcher::Update(double dt)
{
	//std::cout << "Idliing" << '\n';
	RANGE = 5.f * SceneData::GetInstance()->getGridSize();
	static float time = 0;
	m_go->moveSpeed = FULL_SPEED;
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	if (m_go->nearest)
	{
		m_go->sm->SetNextState("AttackArcher");

	}
	else
	{
		if (time >= 0.0f) {
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SEARCH_TYPE::NEAREST_ENEMY, RANGE));
			time = 0.f;
		}
	}
	time += dt * SceneData::GetInstance()->getSpeed();
}

void StateIdleArcher::Exit()
{

}


StateAttackArcher::StateAttackArcher(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateAttackArcher::~StateAttackArcher()
{

}

void StateAttackArcher::Enter()
{
	RANGE = 5.f * SceneData::GetInstance()->getGridSize();
	m_go->moveSpeed = FULL_SPEED;
	//m_go->nearest = NULL;
}

void StateAttackArcher::Update(double dt)
{
	static float time = 0;
	//std::cout << "Attacking" << '\n';
	m_go->moveSpeed = FULL_SPEED;
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	if (time >= .32f)
	{
		PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SEARCH_TYPE::NEAREST_ENEMY, RANGE));
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
	 if (m_go->nearest && m_go->Health > 0 && (m_go->nearest->pos - m_go->pos).LengthSquared() < RANGE * RANGE && m_go->nearest->type != GameObject::GO_TOWER && m_go->nearest->type != GameObject::GO_TOWER2)
	{
		m_go->sm->SetNextState("ShootArcher");
	}
	else if (m_go->nearest && (m_go->nearest->type == GameObject::GO_TOWER || m_go->nearest->type == GameObject::GO_TOWER2) && (m_go->nearest->pos - m_go->pos).LengthSquared() <= 0)
	{
		m_go->nearest = nullptr;
		m_go->reached = 1;
	}
	else if(m_go->Health > 0 && !m_go->nearest)
	{
		m_go->sm->SetNextState("IdleArcher");

	}
	else if(m_go->Health <= 0)
	{
		m_go->sm->SetNextState("DeadArcher");
	}
	 time += dt * SceneData::GetInstance()->getSpeed();
}

void StateAttackArcher::Exit()
{

}




StateDeadArcher::StateDeadArcher(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateDeadArcher::~StateDeadArcher()
{

}

void StateDeadArcher::Enter()
{
	m_go->moveSpeed = 0;
	//m_go->nearest = NULL;
}

void StateDeadArcher::Update(double dt)
{
	
	if (m_go->Health > 0) {
		m_go->sm->SetNextState("AttackArcher");
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

void StateDeadArcher::Exit()
{

}


StateShootArcher::StateShootArcher(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateShootArcher::~StateShootArcher()
{

}

void StateShootArcher::Enter()
{
	
	m_go->moveSpeed = FULL_SPEED;
	//m_go->nearest = NULL;
}

void StateShootArcher::Update(double dt)
{
	static float time = 0;

	if (m_go->nearest->type == GameObject::GO_TOWER || m_go->nearest->type == GameObject::GO_TOWER2)
		m_go->sm->SetNextState("IdleArcher");
//	std::cout << "Shooting" << '\n';
	if ((m_go->nearest->pos - m_go->pos).LengthSquared() > RANGE* RANGE || !m_go->nearest->active || m_go->nearest->Health <= 0.f)
		m_go->sm->SetNextState("AttackArcher");
	else if (m_go->Health <= 0)
	{
		m_go->sm->SetNextState("DeadArcher");
	}
	else if ((m_go->nearest->Health > 0 || m_go->nearest->Health > 0 && m_go->nearest->type == GameObject::GO_BASE2) && (m_go->nearest->pos - m_go->pos).LengthSquared() <= RANGE * RANGE)
	{
		if (time >= .8f) {
			PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_PROJECTILE));
			time = 0;
		}
		m_go->moveSpeed = 0;
	}
		time += dt * SceneData::GetInstance()->getSpeed();
}

void StateShootArcher::Exit()
{

}