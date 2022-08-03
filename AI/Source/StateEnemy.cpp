#include "StateEnemy.h"
#include "SceneData.h"
#include "SceneTurn.h"
#include "TurnManager.h"



StateEnemyWander::StateEnemyWander(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateEnemyWander::~StateEnemyWander()
{

}

void StateEnemyWander::Enter()
{

}

void StateEnemyWander::Update(double dt)
{
	static double timer = 0;
	if (m_go->turn)
	{
		timer += dt * SceneData::GetInstance()->getSpeed();
	}
	if ( timer >= 0.2f && m_go->turn && m_go->nearest == NULL)
	{
		PostOffice::GetInstance()->Send("sceneTurn", new MessageMove(m_go, 0, MessageMove::MOVE_DFSONCE, NULL));
		PostOffice::GetInstance()->Send("sceneTurn", new MessageWRU(m_go, MessageWRU::NEAREST_GRID_PLAYER, 0));
		timer = 0;
	}
	else if (m_go->nearest)
	{
		m_go->sm->SetNextState("monsterHunt");
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

void StateEnemyWander::Exit()
{

}

//hunt
StateEnemyHunt::StateEnemyHunt(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateEnemyHunt::~StateEnemyHunt()
{

}

void StateEnemyHunt::Enter()
{

}

void StateEnemyHunt::Update(double dt)
{
	static double timer = 0;
	if (m_go->nearest == NULL)
	{
		m_go->sm->SetNextState("monsterWander");
		return;
	}
	if (m_go->turn)
	{
		timer += dt * SceneData::GetInstance()->getSpeed();
	}
	if (m_go->turn && abs(m_go->nearest->curr.x - m_go->curr.x) + abs(m_go->nearest->curr.y - m_go->curr.y) <= 1 && timer >= 0.3f)
	{
		SceneData::GetInstance()->setLives(SceneData::GetInstance()->getLives() - 1);
		m_go->energy -= 1;
		timer = 0;
	}
	if (timer >= 0.2f && m_go->turn && m_go->path.empty())
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

void StateEnemyHunt::Exit()
{
	for (int i = 0; i < m_go->visited.size(); ++i)
	{
		m_go->visited[i] = false;
	}
	m_go->nearest = NULL;
}
