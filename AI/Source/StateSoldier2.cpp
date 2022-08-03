#include "StateSoldier2.h"
#include "SceneData.h"


static const float ENERGY_DROP_RATE = 0.2f;
static const float FULL_SPEED = 10;
static const float HUNGRY_SPEED = 4.f;

StateIdleSoldier2::StateIdleSoldier2(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateIdleSoldier2::~StateIdleSoldier2()
{

}

void StateIdleSoldier2::Enter()
{
	m_go->moveSpeed = FULL_SPEED;
	//m_go->nearest = NULL;
}

void StateIdleSoldier2::Update(double dt)
{
	static float time = 0;
	m_go->moveSpeed = FULL_SPEED;
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	if (m_go->nearest)
	{
		m_go->sm->SetNextState("AttackSoldier2");
	}
	else
	{
		if (time >= .32f) {
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SEARCH_TYPE::NEAREST_ENEMY_BASE2, 0));
			time = 0.f;
		}
	}
	time += dt * SceneData::GetInstance()->getSpeed();
}

void StateIdleSoldier2::Exit()
{

}


StateAttackSoldier2::StateAttackSoldier2(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateAttackSoldier2::~StateAttackSoldier2()
{

}

void StateAttackSoldier2::Enter()
{
	m_go->moveSpeed = FULL_SPEED;
	//m_go->nearest = NULL;
}

void StateAttackSoldier2::Update(double dt)
{
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	float SPOT = 5.f * SceneData::GetInstance()->getGridSize();
	float RANGE = SceneData::GetInstance()->getGridSize();
	static float time = 0.f;
	static float stime = 0.f;
	if (time >= .32f)
	{
		PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SEARCH_TYPE::NEAREST_ENEMY_BASE2,SPOT));
		time = 0.f;
	}
	if (m_go->nearest != nullptr)
	if (m_go->nearest->type != GameObject::GO_BASE && m_go->nearest->type != GameObject::GO_ETOWER && m_go->nearest->type != GameObject::GO_ETOWER2 && m_go->nearest->type != GameObject::GO_TOWER && m_go->nearest->type != GameObject::GO_TOWER)
		m_go->moveSpeed = FULL_SPEED + 10.f;
	else
		m_go->moveSpeed = FULL_SPEED;
	if (m_go->nearest && m_go->Health > 0)
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

		float dist = (m_go->nearest->pos - m_go->pos).LengthSquared();
		if (dist <= RANGE * RANGE && m_go->nearest->type != GameObject::GO_ETOWER && m_go->nearest->type != GameObject::GO_ETOWER2)
		{
			m_go->moveSpeed = 0;
			if (stime >= 1.3f)
			{
				m_go->nearest->Health -= 3.f ;
				stime = 0;
			}
		}
		else if (dist <= 0 && (m_go->nearest->type == GameObject::GO_ETOWER || m_go->nearest->type == GameObject::GO_ETOWER2))
		{
			m_go->moveSpeed = 0;
			m_go->reached = 1;
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SEARCH_TYPE::NEAREST_ENEMY_BASE2, SPOT));

			// let just do find own tower then find enemy base.
		}
		else if (m_go->nearest->type != GameObject::GO_BASE && m_go->nearest->type != GameObject::GO_ETOWER && m_go->nearest->type != GameObject::GO_ETOWER2 && m_go->nearest->type != GameObject::GO_TOWER && m_go->nearest->type != GameObject::GO_TOWER2)
			m_go->moveSpeed = FULL_SPEED + 10.f;
		else
			m_go->moveSpeed = FULL_SPEED;
	}
	else if(m_go->Health > 0 && !m_go->nearest)
	{
		m_go->sm->SetNextState("IdleSoldier2");

	}
	else if(m_go->Health <= 0)
	{
		m_go->sm->SetNextState("DeadSoldier2");
	}
	time += dt * SceneData::GetInstance()->getSpeed();
	stime += dt * SceneData::GetInstance()->getSpeed();
}

void StateAttackSoldier2::Exit()
{

}




StateDeadSoldier2::StateDeadSoldier2(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateDeadSoldier2::~StateDeadSoldier2()
{

}

void StateDeadSoldier2::Enter()
{
	m_go->moveSpeed = 0;
	//m_go->nearest = NULL;
}

void StateDeadSoldier2::Update(double dt)
{
	
	if (m_go->Health > 0) {
		m_go->sm->SetNextState("AttackSoldier2");
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

void StateDeadSoldier2::Exit()
{

}


