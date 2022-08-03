#include "StatePlayer.h"
#include "SceneData.h"
#include "TurnManager.h"
#include "SceneTurn.h"
#include "Application.h"




StatePlayerIdle::StatePlayerIdle(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StatePlayerIdle::~StatePlayerIdle()
{
}

void StatePlayerIdle::Enter()
{
	//if (m_go->isTurnBase)
	//{
		//TurnManager::GetInstance()->Add(m_go);
	//}
}

void StatePlayerIdle::Update(double _dt)
{
	m_go->energy = 6;
	m_go->sm->SetNextState("PlayerMovement");
}

void StatePlayerIdle::Exit()
{

}

StatePlayerAttack::StatePlayerAttack(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StatePlayerAttack::~StatePlayerAttack()
{
}

void StatePlayerAttack::Enter()
{

}

void StatePlayerAttack::Update(double _dt)
{

}

void StatePlayerAttack::Exit()
{

}


StatePlayerDead::StatePlayerDead(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StatePlayerDead::~StatePlayerDead()
{
}

void StatePlayerDead::Enter()
{

}

void StatePlayerDead::Update(double _dt)
{

}

void StatePlayerDead::Exit()
{

}

StatePlayerMovement::StatePlayerMovement(const std::string& stateID, GameObject* go)
	:State(stateID),
	m_go(go)
{

}

StatePlayerMovement::~StatePlayerMovement()
{
}

void StatePlayerMovement::Enter()
{
}

void StatePlayerMovement::Update(double dt)
{ 
	static bool LMouseState = false;
	static double time = 0.0;
	//if (m_go->turn && m_go->isTurnBase)
	//{
	//	if (m_go->energy <= 0)
	//	{
	//		m_go->energy = 0;
	//	}
	//	// if attack is selected go to attack
	//	
	//	// else if end turn selected
	//	if (m_go->endTurn)
	//	{
	//		m_go->endTurn = false;
	//		TurnManager::GetInstance()->remove(m_go);
	//		if (m_go->isTurnBase)
	//		{
	//			TurnManager::GetInstance()->Add(m_go);
	//		}
	//	}
	//	else // more on this
	//	{
	//		if (m_go->nearest != nullptr)
	//		{

	//		}

	//		else if(time >= 0.3f && m_go->canAttack.empty())
	//		{
	//			time = 0;
	//			PostOffice::GetInstance()->Send("sceneTurn", new MessageWRU(m_go, MessageWRU::NEAREST_GRID_ENEMY, m_go->range)); // get selectable enemy
	//		}
	//	}
	//}
	if(m_go->turn)
	{
		if (!LMouseState && Application::IsMousePressed(0))
		{

			MazePt m_end;
			LMouseState = true;
			std::cout << "LBUTTON DOWN" << std::endl;
			double x, y;
			Application::GetCursorPos(&x, &y);
			int w = Application::GetWindowWidth();
			int h = Application::GetWindowHeight();
			float posX = static_cast<float>(x) / w * SceneData::GetInstance()->getWorldWidth();
			float posY = (h - static_cast<float>(y)) / h * SceneData::GetInstance()->getWorldHeight();
			if (posX < SceneData::GetInstance()->getNoGrid() * SceneData::GetInstance()->getGridSize() && posY < SceneData::GetInstance()->getNoGrid() * SceneData::GetInstance()->getGridSize())
			{
				m_end.Set((int)(posX / SceneData::GetInstance()->getGridSize()), (int)(posY / SceneData::GetInstance()->getGridSize()));
				
					if (m_go->active)
						SceneTurn::BFSLimit(m_go, m_end, INT_MAX,m_go->maze, m_go->energy);
			}
				
		}
		else if (LMouseState && !Application::IsMousePressed(0))
		{
			LMouseState = false;
			std::cout << "LBUTTON UP" << std::endl;
		}
		if (m_go->energy == 0 || Application::IsKeyPressed('P'))
		{
			TurnManager::GetInstance()->remove(m_go);
			SceneData::GetInstance()->setTurn(SceneData::GetInstance()->getTurn() + 1);
			m_go->sm->SetNextState("PlayerIdle");
			TurnManager::GetInstance()->Add(m_go);
		}
	}
	time += dt;
}

void StatePlayerMovement::Exit()
{
}
