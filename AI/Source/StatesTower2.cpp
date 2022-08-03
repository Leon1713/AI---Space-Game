#include "StatesTower2.h"
#include "SceneData.h"
#include "Application.h"




StateTower2Attack::StateTower2Attack(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StateTower2Attack::~StateTower2Attack()
{
}

void StateTower2Attack::Enter()
{
}

void StateTower2Attack::Update(double dt)
{
	

	if (m_go->Health <= 0)
	{
		m_go->sm->SetNextState("TDestroyed2");
		return;
	}
	static float STime = 0.f;
	static float time = 0.f;
	//static bool first = 1;
	if (!m_go->nearest || m_go->nearest->Health <= 0 || !m_go->active) // no enemy
	{
		if (time >= .320f) {
			m_go->nearest = nullptr;
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SEARCH_TYPE::NEAREST_ENEMY_BASE2, 5 * SceneData::GetInstance()->getGridSize())); // find nearest enemy for base
			time = 0;
		}
	}
	else
	{
		if (m_go->nearest->Health > 0 && m_go->nearest->sm->GetCurrentState() != "DeadSoldier" || m_go->nearest->sm->GetCurrentState() != "DeadArcher" || m_go->nearest->sm->GetCurrentState() == "DeadTank")
		{

			if (STime >= .5f) {
				if ((m_go->pos - m_go->nearest->pos).LengthSquared() <= (5 * SceneData::GetInstance()->getGridSize()) * (5 * SceneData::GetInstance()->getGridSize()))
					PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_TYPE::SPAWN_PROJECTILE2));
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

void StateTower2Attack::Exit()
{
}

StateTower2Destroyed::StateTower2Destroyed(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateTower2Destroyed::~StateTower2Destroyed()
{
}

void StateTower2Destroyed::Enter()
{
}

void StateTower2Destroyed::Update(double dt)
{
	if (m_go->Health > 0) {
		m_go->sm->SetNextState("TAttack2");
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

void StateTower2Destroyed::Exit()
{

}
