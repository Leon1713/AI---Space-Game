#include "StateHealer.h"
#include "SceneData.h"
#include "SceneTurn.h"
#include "TurnManager.h"



StateHealerWander::StateHealerWander(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateHealerWander::~StateHealerWander()
{

}

void StateHealerWander::Enter()
{

}

void StateHealerWander::Update(double dt)
{
	static double timer = 0;
	if (m_go->turn)
	{
		timer += dt * SceneData::GetInstance()->getSpeed();
	}
	if ( timer >= 0.2f && m_go->turn && m_go->nearest == NULL)
	{
		PostOffice::GetInstance()->Send("sceneTurn", new MessageWRU(m_go, MessageWRU::NEAREST_GRID_PLAYER, 0));
		PostOffice::GetInstance()->Send("sceneTurn", new MessageMove(m_go, 0, MessageMove::MOVE_DFSONCE, NULL));
		timer = 0;
	}
	else if (m_go->nearest)
	{
		m_go->sm->SetNextState("healerHeal");
	}
	if (m_go->energy == 0)
	{
		TurnManager::GetInstance()->remove(m_go);
		SceneData::GetInstance()->setTurn(SceneData::GetInstance()->getTurn() + 1);
		m_go->energy = 5;
		TurnManager::GetInstance()->Add(m_go);
	}
		// send request to dfs
}

void StateHealerWander::Exit()
{

}

//hunt
StateHealerHeal::StateHealerHeal(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateHealerHeal::~StateHealerHeal()
{

}

void StateHealerHeal::Enter()
{

}

void StateHealerHeal::Update(double dt)
{
	static double timer = 0;
	if (m_go->nearest == NULL)
	{
		m_go->sm->SetNextState("healerWander");
		return;
	}
	if (m_go->turn)
	{
		timer += dt * SceneData::GetInstance()->getSpeed();
	}
	if (m_go->turn && abs(m_go->nearest->curr.x - m_go->curr.x) + abs(m_go->nearest->curr.y - m_go->curr.y) <= 2 && timer >= 0.2f)
	{
		m_go->nearest->energy += 1;
		m_go->energy -= 1;
		timer = 0;
	}
	if (timer >= 0.2f && m_go->turn && m_go->path.empty() && SceneData::GetInstance()->getLives() < 5)
	{
		PostOffice::GetInstance()->Send("sceneTurn", new MessageMove(m_go, 0, MessageMove::MOVE_BFSLIMIT, m_go->nearest->curr));
		timer = 0;
	}

	if (m_go->energy <= 0)
	{
		TurnManager::GetInstance()->remove(m_go);
		m_go->nearest = NULL;
		SceneData::GetInstance()->setTurn(SceneData::GetInstance()->getTurn() + 1);
		m_go->energy = 5;
		//m_go->sm->SetNextState("monsterWander");
		TurnManager::GetInstance()->Add(m_go);
	}
}

void StateHealerHeal::Exit()
{
	for (int i = 0; i < m_go->visited.size(); ++i)
	{
		m_go->visited[i] = false;
	}
	m_go->nearest = NULL;
}
