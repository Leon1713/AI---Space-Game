#include "StatesTower.h"
#include "SceneData.h"
#include "Application.h"




StateTowerAttack::StateTowerAttack(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StateTowerAttack::~StateTowerAttack()
{
}

void StateTowerAttack::Enter()
{
}

void StateTowerAttack::Update(double dt)
{
	

	if (m_go->Health <= 0)
	{
		m_go->sm->SetNextState("TDestroyed");
		return;
	}
	static float STime = 0.f;
	static float time = 0.f;
	//static bool first = 1;
	if (!m_go->nearest || m_go->nearest->Health <= 0 || !m_go->nearest->active) // no enemy
	{
		if (time >= .320f) {
			m_go->nearest = nullptr;
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SEARCH_TYPE::NEAREST_ENEMY, 5 * SceneData::GetInstance()->getGridSize())); // find nearest enemy for base
			time = 0;
		}
	}
	else
	{
		if (m_go->nearest->Health > 0 && m_go->nearest->sm->GetCurrentState() != "DeadSoldier2" || m_go->nearest->sm->GetCurrentState() != "DeadArcher2" || m_go->nearest->sm->GetCurrentState() == "DeadTank2")
		{

			if (STime >= .5f) {
				if ((m_go->pos - m_go->nearest->pos).LengthSquared() <= (5 * SceneData::GetInstance()->getGridSize()) * (5 * SceneData::GetInstance()->getGridSize()))
					PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_TYPE::SPAWN_PROJECTILE));
				else
					m_go->nearest = nullptr;
				// send message to generate projectile // 
				//first = 0; // replace with on hit
				STime = 0;
			}


		}
		else
		{
			m_go->nearest = NULL;
		}
	}
	time += dt * SceneData::GetInstance()->getSpeed();
	STime += dt * SceneData::GetInstance()->getSpeed();
}

void StateTowerAttack::Exit()
{
}

StateTowerDestroyed::StateTowerDestroyed(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateTowerDestroyed::~StateTowerDestroyed()
{
}

void StateTowerDestroyed::Enter()
{
}

void StateTowerDestroyed::Update(double dt)
{
	if (m_go->Health > 0) {
		m_go->sm->SetNextState("TAttack");
	}
	else {
		m_go->moveSpeed = 0;
		static float ElaspedTime = 0;
		if (ElaspedTime >= 1.f)
		{
			m_go->active = false;
			ElaspedTime = 0;
		}
		ElaspedTime += dt * SceneData::GetInstance()->getSpeed();
	}
}

void StateTowerDestroyed::Exit()
{

}
