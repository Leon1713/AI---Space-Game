#include "StateSoldier.h"
#include "SceneData.h"


static const float ENERGY_DROP_RATE = 0.2f;
static const float FULL_SPEED = 10;
static const float HUNGRY_SPEED = 4.f;

StateIdleSoldier::StateIdleSoldier(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateIdleSoldier::~StateIdleSoldier()
{

}

void StateIdleSoldier::Enter()
{
	m_go->moveSpeed = FULL_SPEED;
	//m_go->nearest = NULL;
}

void StateIdleSoldier::Update(double dt)
{
	static float time = 0;
	m_go->moveSpeed = FULL_SPEED;
	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
	if (m_go->nearest)
	{
		m_go->sm->SetNextState("AttackSoldier");
	}
	else
	{
		if (time >= .0f) {
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SEARCH_TYPE::NEAREST_ENEMY, 0));
			time = 0.f;
		}
	}
	time += dt * SceneData::GetInstance()->getSpeed();
}

void StateIdleSoldier::Exit()
{

}


StateAttackSoldier::StateAttackSoldier(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateAttackSoldier::~StateAttackSoldier()
{

}

void StateAttackSoldier::Enter()
{
	m_go->moveSpeed = FULL_SPEED;
	//m_go->nearest = NULL;
}

void StateAttackSoldier::Update(double dt)
{
	float SPOT = 5.f * SceneData::GetInstance()->getGridSize();
	float RANGE = 1.f * SceneData::GetInstance()->getGridSize();
	static float time = 0.f;
	static float stime = 0.f;
	
	if (time >= .32f)
	{
		PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SEARCH_TYPE::NEAREST_ENEMY, SPOT));
		time = 0.f;
	}
	if(m_go->nearest != nullptr)
	if (m_go->nearest->type != GameObject::GO_BASE2 && m_go->nearest->type != GameObject::GO_TOWER && m_go->nearest->type != GameObject::GO_TOWER2 && m_go->nearest->type != GameObject::GO_ETOWER && m_go->nearest->type != GameObject::GO_ETOWER2)
		m_go->moveSpeed = FULL_SPEED ;
	else
	m_go->moveSpeed = FULL_SPEED;

	m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
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
		if (dist <= RANGE * RANGE && m_go->nearest->type != GameObject::GO_TOWER && m_go->nearest->type != GameObject::GO_TOWER2)
		{
			m_go->moveSpeed = 0;
			if (stime >= 1.3f)
			{
				m_go->nearest->Health -= 3.f;
				stime = 0;
			}
		}
		else if (dist <= 0 && (m_go->nearest->type == GameObject::GO_TOWER || m_go->nearest->type == GameObject::GO_TOWER2))
		{
			m_go->reached = 1;
			m_go->nearest = nullptr;
			// let just do find own tower then find enemy base.
		}
		else if (m_go->nearest->type != GameObject::GO_BASE2 && m_go->nearest->type != GameObject::GO_TOWER && m_go->nearest->type != GameObject::GO_TOWER2 && m_go->nearest->type != GameObject::GO_ETOWER && m_go->nearest->type != GameObject::GO_ETOWER2)
			m_go->moveSpeed = FULL_SPEED + 10.f;
		else
			m_go->moveSpeed = FULL_SPEED;
	}
	else if(m_go->Health > 0 && !m_go->nearest)
	{
		m_go->sm->SetNextState("IdleSoldier");

	}
	else if(m_go->Health <= 0)
	{
		m_go->sm->SetNextState("DeadSoldier");
	}
	stime += dt * SceneData::GetInstance()->getSpeed();
	time += dt * SceneData::GetInstance()->getSpeed();
}

void StateAttackSoldier::Exit()
{

}




StateDeadSoldier::StateDeadSoldier(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateDeadSoldier::~StateDeadSoldier()
{

}

void StateDeadSoldier::Enter()
{
	m_go->moveSpeed = 0;
	//m_go->nearest = NULL;
}

void StateDeadSoldier::Update(double dt)
{
	
	if (m_go->Health > 0) {
		m_go->sm->SetNextState("AttackSoldier");
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

void StateDeadSoldier::Exit()
{

}


