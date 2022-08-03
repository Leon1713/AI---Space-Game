#include "StatesBase2.h"
#include "SceneData.h"


static const float ENERGY_DROP_RATE = 0.2f;
static const float FULL_SPEED = 8.f;
static const float HUNGRY_SPEED = 4.f;

StateAttack2::StateAttack2(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StateAttack2::~StateAttack2()
{
}

void StateAttack2::Enter()
{
}

void StateAttack2::Update(double dt)
{
	static float STime = 0.f;
	static float time = 0.f;
	//static bool first = 1;
	if (m_go->Health <= 0)
	{
			m_go->sm->SetNextState("Destroyed2");
			return;
	}
	if (!m_go->nearest || m_go->nearest->Health <= 0) // no enemy
	{
		if (time >= .2f) {
			m_go->nearest = nullptr;
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(m_go, MessageWRU::SEARCH_TYPE::NEAREST_ENEMY_BASE2, 5 * SceneData::GetInstance()->getGridSize())); // find nearest enemy for base2
			time = 0;
		}
		else
			m_go->sm->SetNextState("Spawning2");
	}
	else
	{
		if (m_go->nearest->Health > 0 && m_go->nearest->sm->GetCurrentState() != "DeadSoldier" || m_go->nearest->sm->GetCurrentState() != "DeadArcher")
		{

			if (STime >= .2f) {
				PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_TYPE::SPAWN_PROJECTILE2));
				// send message to generate projectile // 
				//first = 0; // replace with on hit
				STime = 0;
			}

		}
		else 
		{
			m_go->nearest = NULL;
			m_go->sm->SetNextState("Spawning2");

		}
	}
	time += dt * SceneData::GetInstance()->getSpeed();
	STime += dt * SceneData::GetInstance()->getSpeed();
}

void StateAttack2::Exit()
{
}



StateSpawn2::StateSpawn2(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StateSpawn2::~StateSpawn2()
{
}

void StateSpawn2::Enter()
{
	m_go->moveSpeed = HUNGRY_SPEED;
	m_go->nearest = NULL;
}

void StateSpawn2::Update(double dt)
{
	if (m_go->Health <= 0)
	{
		m_go->sm->SetNextState("Destroyed2");
		return;
	}
	static float elaspedTime = 0.f;
	if (elaspedTime >= 3.f)
	{
		//float chance = 1;
		//srand(time(NULL));
		float chance = Math::RandFloatMinMax(0, 1);
		//PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go,MessageSpawn::SPAWN_TYPE::SPAWN_ARCHER2));
		if (chance >= .8f) {
			PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_ARCHER2));
			std::cout << "Archer2" << '\n';
		}
		else  if (chance >= .33333f && chance < .8f) {
			PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_SOLDIER2));
			std::cout << "Soldier2" << '\n';
		}
		else if(chance <.33333f)
		{
			PostOffice::GetInstance()->Send("Scene", new MessageSpawn(m_go, MessageSpawn::SPAWN_TANK2));
			std::cout << "Tank2" << '\n';
		}
		
		elaspedTime = 0;
	}
	else
	{
		m_go->sm->SetNextState("Attack2");
	}
	elaspedTime += dt * SceneData::GetInstance()->getSpeed();
	
}

void StateSpawn2::Exit()
{
}

StateDestroyed2::StateDestroyed2(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{

}

StateDestroyed2::~StateDestroyed2()
{
}

void StateDestroyed2::Enter()
{
}

void StateDestroyed2::Update(double dt)
{
	if (m_go->Health > 0) {
		m_go->sm->SetNextState("Spawning2");
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

void StateDestroyed2::Exit()
{

}
