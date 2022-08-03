#include "StatesBase.h"
#include "SceneData.h"
#include "Application.h"


static const float ENERGY_DROP_RATE = 0.2f;
static const float FULL_SPEED = 8.f;
static const float HUNGRY_SPEED = 4.f;

StateAttack::StateAttack(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StateAttack::~StateAttack()
{
}

void StateAttack::Enter()
{
}

void StateAttack::Update(double dt)
{
	
	if (m_go->Health <= 0)
	{
		m_go->sm->SetNextState("Destroyed");
		return;
	}
	static float STime = 0.f;
	static float time = 0.f;
	//static bool first = 1;
	if (!m_go->nearest || m_go->nearest->Health <= 0) // no enemy
	{
		if (time >= .2f) {
			m_go->nearest = nullptr;
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SEARCH_TYPE::NEAREST_ENEMY, 5 * SceneData::GetInstance()->getGridSize())); // find nearest enemy for base
			time = 0;
		}
		else
			m_go->sm->SetNextState("Spawning");
	}
	else
	{
		if (m_go->nearest->Health > 0 && m_go->nearest->sm->GetCurrentState() != "DeadSoldier2" || m_go->nearest->sm->GetCurrentState() != "DeadArcher2" || m_go->nearest->sm->GetCurrentState() == "DeadTank2")
		{

			if (STime >= .2f) {
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
			m_go->sm->SetNextState("Spawning");

		}
	}
	time += dt * SceneData::GetInstance()->getSpeed();
	STime += dt * SceneData::GetInstance()->getSpeed();
}

void StateAttack::Exit()
{
}



StateSpawn::StateSpawn(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StateSpawn::~StateSpawn()
{
}

void StateSpawn::Enter()
{
	m_go->moveSpeed = HUNGRY_SPEED;
	m_go->nearest = NULL;
}

void StateSpawn::Update(double dt)
{
	static float elaspedTime = 0.f;
	if (m_go->Health <= 0)
	{
		m_go->sm->SetNextState("Destroyed");
		return;
	}
	if (elaspedTime >= 3.f)
	{

		//srand(time(NULL));
		//float chance = Math::RandFloatMinMax(0, 1);
		//static bool qState = 0, wState = 0, eState = 0;
		//////float chance = 1.f;
		//if (Application::IsKeyPressed('Q') && !qState)
		//{
		//	qState = 1;
		//	PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_ARCHER));
		//	std::cout << "Archer" << '\n';
		//}
		//else if (!Application::IsKeyPressed('Q') && qState)
		//{
		//	qState = 0;
		//}
		//else if (Application::IsKeyPressed('W') && !wState)
		//{
		//	wState = 1;
		//	PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_SOLDIER));
		//	std::cout << "Soldier" << '\n';
		//}
		//else if (!Application::IsKeyPressed('W') && wState)
		//{
		//	wState = 0;
		//}
		//else if (Application::IsKeyPressed('E') && !eState)
		//{
		//	eState = 1;
		//	PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_TANK));
		//	std::cout << "Tank" << '\n';
		//}
		//else if (!Application::IsKeyPressed('E') && eState)
		//{
		//	eState = 0;
		//}
		//else
		//{
		//	m_go->sm->SetNextState("Attack");
		//}

		//elaspedTime = 0;

		float chance = Math::RandFloatMinMax(0, 1);
		//PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go,MessageSpawn::SPAWN_TYPE::SPAWN_ARCHER2));
		if (chance >= .8f) {
			PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_ARCHER));
			std::cout << "Archer" << '\n';
		}
		else if (chance >= .33333f && chance < .8f) {
			PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_SOLDIER));
			std::cout << "Soldier" << '\n';
		}
		else if (chance < .33333f)
		{
			PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_TANK));
			std::cout << "Tank" << '\n';
		}
			elaspedTime = 0;
		//}
	}
		/*else
		{
			m_go->sm->SetNextState("Attack");
		}*/
	elaspedTime += dt;
	
}

void StateSpawn::Exit()
{
}

StateDestroyed::StateDestroyed(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateDestroyed::~StateDestroyed()
{
}

void StateDestroyed::Enter()
{
}

void StateDestroyed::Update(double dt)
{
	if (m_go->Health > 0) {
		m_go->sm->SetNextState("Spawning");
	}
	else {
		m_go->moveSpeed = 0;
		static float ElaspedTime = 0;
		if (ElaspedTime >= 3.f)
		{
			m_go->active = false;
			ElaspedTime = 0;
		}
		ElaspedTime += dt * SceneData::GetInstance()->getSpeed();
	}
}

void StateDestroyed::Exit()
{

}
