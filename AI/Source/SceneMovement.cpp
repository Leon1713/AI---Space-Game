#include "SceneMovement.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "StatesFish.h"
#include "StatesShark.h"
#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "StatesBase.h"
#include "StateSoldier.h"
#include "StatesBase2.h"
#include "StateProjectile.h"
#include "StateArcher.h"
#include "StateProjectile2.h"
#include "StateArcher2.h"
#include "StateSoldier2.h"
#include "StateTank.h"
#include "StatesTower.h"
#include "StatesTower2.h"
#include "StateTank2.h"
SceneMovement::SceneMovement()
{
}

SceneMovement::~SceneMovement()
{
}

void SceneMovement::Init()
{
	SceneBase::Init();


	cPostOffice = PostOffice::GetInstance();
	cPostOffice->Register("Scene", this);



	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	SceneData::GetInstance()->setSpeed(1.f);
	m_speed = 1.f;
	
	Math::InitRNG();

	SceneData::GetInstance()->SetObjectCount(0);
	SceneData::GetInstance()->SetFishCount(0);
	m_noGrid = 20;
	m_gridSize = m_worldHeight / m_noGrid;
	m_gridOffset = m_gridSize / 2;
	m_hourOfTheDay = 0;

	m_goList.empty();


	Base = FetchGO(GameObject::GO_BASE);
	Base->Health = 150;
	Base->pos.Set(m_gridSize * 9.5, m_gridSize * 18.5, 0);
	Base->scale.Set(m_gridSize * 1, m_gridSize * 1, m_gridSize * 1);
	Base->team = GameObject::GAMEOBJECT_TEAM::T_1;

	Base2 = FetchGO(GameObject::GO_BASE2);
	Base2->Health = 150;
	Base2->pos.Set(m_gridSize * 9.5, m_gridSize * 1.5, 0);
	Base2->scale.Set(m_gridSize * 1, m_gridSize * 1, m_gridSize * 1);
	Base2->team = GameObject::GAMEOBJECT_TEAM::T_2;

	BTower01 = FetchGO(GameObject::GO_TOWER);
	BTower01->Health = 100;
	BTower01->pos.Set(m_gridSize * 1.5, m_gridSize * 18.5, 0);
	BTower01->scale.Set(m_gridSize * 1, m_gridSize * 1, m_gridSize * 1);
	BTower01->team = GameObject::GAMEOBJECT_TEAM::T_1;
	
	BTower02 = FetchGO(GameObject::GO_TOWER2);
	BTower02->Health = 100;
	BTower02->pos.Set(m_gridSize * 18.5, m_gridSize * 18.5, 0);
	BTower02->scale.Set(m_gridSize * 1, m_gridSize * 1, m_gridSize * 1);
	BTower02->team = GameObject::GAMEOBJECT_TEAM::T_1;

	ETower01 = FetchGO(GameObject::GO_ETOWER);
	ETower01->Health = 100;
	ETower01->pos.Set(m_gridSize * 1.5, m_gridSize * 1.5, 0);
	ETower01->scale.Set(m_gridSize * 1, m_gridSize * 1, m_gridSize * 1);
	ETower01->team = GameObject::GAMEOBJECT_TEAM::T_2;

	ETower02 = FetchGO(GameObject::GO_ETOWER2);
	ETower02->Health = 100;
	ETower02->pos.Set(m_gridSize * 18.5, m_gridSize * 1.5, 0);
	ETower02->scale.Set(m_gridSize * 1, m_gridSize * 1, m_gridSize * 1);
	ETower02->team = GameObject::GAMEOBJECT_TEAM::T_2;


	//Base2->active = false;
	cPostOffice = PostOffice::GetInstance();
	
}

GameObject* SceneMovement::FetchGO(GameObject::GAMEOBJECT_TYPE type)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active && go->type == type)
		{
			go->active = true;
			return go;
		}
	}
	for (unsigned i = 0; i < 5; ++i)
	{
		GameObject *go = new GameObject(type);
		m_goList.push_back(go);
		if (type == GameObject::GO_FISH)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateTooFull("TooFull", go));
			go->sm->AddState(new StateFull("Full", go));
			go->sm->AddState(new StateHungry("Hungry", go));
			go->sm->AddState(new StateDead("Dead", go));
		}
		else if (type == GameObject::GO_SHARK)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateCrazy("Crazy", go));
			go->sm->AddState(new StateNaughty("Naughty", go));
			go->sm->AddState(new StateHappy("Happy", go));
		}
		else if (type == GameObject::GO_SOLDIER)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateIdleSoldier("IdleSoldier", go));
			go->sm->AddState(new StateAttackSoldier("AttackSoldier", go));
			go->sm->AddState(new StateDeadSoldier("DeadSoldier",go));
		
			// idle(win)n
			// yeah
		}
		else if (type == GameObject::GO_SOLDIER2)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateIdleSoldier2("IdleSoldier2", go));
			go->sm->AddState(new StateAttackSoldier2("AttackSoldier2", go));
			go->sm->AddState(new StateDeadSoldier2("DeadSoldier2", go));

			// idle(win)n
			// yeah
		}
		else if (type == GameObject::GO_BASE)
		{
			go->sm = new StateMachine();
			//go->sm->AddState(new StateAttack("Attack", go));
			go->sm->AddState(new StateSpawn("Spawning", go));
			go->sm->AddState(new StateDestroyed("Destroyed", go));

			 //attacking( within 5 grid)
		}
		else if (type == GameObject::GO_TOWER)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateTowerAttack("TAttack", go));
			//go->sm->AddState(new StateSpawn("Spawning", go));
			go->sm->AddState(new StateTowerDestroyed("TDestroyed", go));

			//attacking( within 5 grid)
		}
		else if (type == GameObject::GO_TOWER2)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateTowerAttack("TAttack", go));
			//go->sm->AddState(new StateSpawn("Spawning", go));
			go->sm->AddState(new StateTowerDestroyed("TDestroyed", go));

			//attacking( within 5 grid)
		}
		else if (type == GameObject::GO_ETOWER)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateTower2Attack("TAttack2", go));
			//go->sm->AddState(new StateSpawn("Spawning", go));
			go->sm->AddState(new StateTower2Destroyed("TDestroyed2", go));

			//attacking( within 5 grid)
		}
		else if (type == GameObject::GO_ETOWER2)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateTower2Attack("TAttack2", go));
			//go->sm->AddState(new StateSpawn("Spawning", go));
			go->sm->AddState(new StateTower2Destroyed("TDestroyed2", go));

			//attacking( within 5 grid)
		}
		else if (type == GameObject::GO_BASE2)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateSpawn2("Spawning2", go));
			//go->sm->AddState(new StateAttack2("Attack2", go));
			go->sm->AddState(new StateDestroyed2("Destroyed2", go));

			// attacking( within 5 grid)
		}
		else if (type == GameObject::GO_PROJECTILE2)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StatesSeekProjectile2("SeekProjectile2", go));
			go->sm->AddState(new StateHitProjectile2("HitProjectile2", go));
		}
		else if (type == GameObject::GO_PROJECTILE)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StatesSeekProjectile("SeekProjectile", go));
			go->sm->AddState(new StateHitProjectile("HitProjectile", go));
		}
		else if (type == GameObject::GO_ARCHER)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateIdleArcher("IdleArcher", go));
			go->sm->AddState(new StateAttackArcher("AttackArcher", go));
			go->sm->AddState(new StateShootArcher("ShootArcher", go));
			go->sm->AddState(new StateDeadArcher("DeadArcher", go));
		}
		else if (type == GameObject::GO_ARCHER2)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateIdleArcher2("IdleArcher2", go));
			go->sm->AddState(new StateAttackArcher2("AttackArcher2", go));
			go->sm->AddState(new StateShootArcher2("ShootArcher2", go));
			go->sm->AddState(new StateDeadArcher2("DeadArcher2", go));
		}
		else if (type == GameObject::GO_TANK)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateIdleTank("IdleTank", go));
			go->sm->AddState(new StateAttackTank("AttackTank", go));
			go->sm->AddState(new StateDeadTank("DeadTank", go));

			
		}
		else if (type == GameObject::GO_TANK2)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateIdleTank2("IdleTank2", go));
			go->sm->AddState(new StateAttackTank2("AttackTank2", go));
			go->sm->AddState(new StateDeadTank2("DeadTank2", go));

			
		}
	}
	return FetchGO(type);
}

void SceneMovement::Process(Message* message)
{

	MessageSpawn* messageSpawn = dynamic_cast<MessageSpawn*>(message);
	if (messageSpawn)
	{
		if (messageSpawn->go->active)
		{
			if (messageSpawn->go->type == GameObject::GO_BASE && messageSpawn->Spawn == MessageSpawn::SPAWN_TYPE::SPAWN_SOLDIER)
			{
				//for (int i = 0; i < 1; ++i) {
					GameObject* temp = FetchGO(GameObject::GO_SOLDIER);
					temp->Health = 10.f;
					temp->pos = messageSpawn->go->pos;
					temp->scale.Set(m_gridSize, m_gridSize, m_gridSize);
					temp->target = temp->pos;
					temp->team = GameObject::T_1;
					temp->reached = 0;
					bool probability = Math::RandIntMinMax(0, 1);
					if (probability)
					{
						temp->t1 = 1;
						temp->t2 = 0;
					}
					else
					{
						temp->t1 = 0;
						temp->t2 = 1;
					}
				//}
			}
			else if (messageSpawn->go->type == GameObject::GO_BASE && messageSpawn->Spawn == MessageSpawn::SPAWN_TYPE::SPAWN_ARCHER)
			{
				//for (int i = 0; i < 1; ++i) {
					GameObject* temp = FetchGO(GameObject::GO_ARCHER);
					temp->Health = 5.f; // test
					temp->pos = messageSpawn->go->pos;
					temp->scale.Set(m_gridSize, m_gridSize, m_gridSize);
					temp->target = temp->pos;
					temp->team = GameObject::T_1;
					temp->reached = 0;
					bool probability = Math::RandIntMinMax(0, 1);
					if (probability)
					{
						temp->t1 = 1;
						temp->t2 = 0;
					}
					else
					{
						temp->t1 = 0;
						temp->t2 = 1;
					}
				//}
			}
			else if (messageSpawn->go->type == GameObject::GO_BASE2 && messageSpawn->Spawn == MessageSpawn::SPAWN_TYPE::SPAWN_SOLDIER2)
			{
				//for (int i = 0; i < 1; ++i) {
					GameObject* temp = FetchGO(GameObject::GO_SOLDIER2);
					temp->Health = 10.f;
					temp->pos = messageSpawn->go->pos;
					temp->scale.Set(m_gridSize, m_gridSize, m_gridSize);
					temp->target = temp->pos;
					temp->team = GameObject::T_2;
					temp->reached = 0;
					bool probability = Math::RandIntMinMax(0, 1);
					if (probability)
					{
						temp->t1 = 1;
						temp->t2 = 0;
					}
					else
					{
						temp->t1 = 0;
						temp->t2 = 1;
					}
				//}
			}
			else if (messageSpawn->go->type == GameObject::GO_BASE2 && messageSpawn->Spawn == MessageSpawn::SPAWN_TYPE::SPAWN_ARCHER2)
			{
				//for (int i = 0; i < 1; ++i) {
					GameObject* temp = FetchGO(GameObject::GO_ARCHER2);
					temp->Health = 5.f; // test
					temp->pos = messageSpawn->go->pos;
					temp->scale.Set(m_gridSize, m_gridSize, m_gridSize);
					temp->target = temp->pos;
					temp->team = GameObject::T_2;
					temp->reached = 0;
					bool probability = Math::RandIntMinMax(0, 1);
					if (probability)
					{
						temp->t1 = 1;
						temp->t2 = 0;
					}
					else
					{
						temp->t1 = 0;
						temp->t2 = 1;
					}
				//}
			}
			else if (messageSpawn->go->type == GameObject::GO_BASE2 && messageSpawn->Spawn == MessageSpawn::SPAWN_TYPE::SPAWN_TANK2)
			{
				//for (int i = 0; i < 1; ++i) {
				GameObject* temp = FetchGO(GameObject::GO_TANK2);
				temp->Health = 35.f; // test
				temp->pos = messageSpawn->go->pos;
				temp->scale.Set(m_gridSize, m_gridSize, m_gridSize);
				temp->target = temp->pos;
				temp->team = GameObject::T_2;
				temp->reached = 0;
				bool probability = Math::RandIntMinMax(0, 1);
				if (probability)
				{
					temp->t1 = 1;
					temp->t2 = 0;
				}
				else
				{
					temp->t1 = 0;
					temp->t2 = 1;
				}
				//}
			}
			else if (messageSpawn->go->type == GameObject::GO_BASE && messageSpawn->Spawn == MessageSpawn::SPAWN_TYPE::SPAWN_TANK)
			{
				//for (int i = 0; i < 1; ++i) {
				GameObject* temp = FetchGO(GameObject::GO_TANK);
				temp->Health = 35.f;
				temp->pos = messageSpawn->go->pos;
				temp->scale.Set(m_gridSize, m_gridSize, m_gridSize);
				temp->target = temp->pos;
				temp->team = GameObject::T_1;
				temp->reached = 0;
				bool probability = Math::RandIntMinMax(0, 1);
				if (probability)
				{
					temp->t1 = 1;
					temp->t2 = 0;
				}
				else
				{
					temp->t1 = 0;
					temp->t2 = 1;
				}
				//}
			}
			else if ((messageSpawn->go->type == GameObject::GO_BASE2 || messageSpawn->go->type == GameObject::GO_ETOWER || messageSpawn->go->type == GameObject::GO_ETOWER2) && messageSpawn->Spawn == MessageSpawn::SPAWN_TYPE::SPAWN_PROJECTILE2)
			{
				//for (int i = 0; i < 2; ++i)
				{
					/*GameObject* temp = FetchGO(GameObject::GO_SOLDIER);
					temp->Health = 5.f;
					temp->pos = messageSpawn->go->pos;
					temp->scale.Set(m_gridSize, m_gridSize, m_gridSize);
					temp->target = temp->pos;*/

					// spawn projectile 
					GameObject* Projectile = FetchGO(GameObject::GO_PROJECTILE2);
					Projectile->pos = messageSpawn->go->pos;
					Projectile->scale.Set(m_gridSize, m_gridSize, m_gridSize);
					Projectile->nearest = messageSpawn->go->nearest;
				}
			}
			else if ((messageSpawn->go->type == GameObject::GO_BASE || messageSpawn->go->type == GameObject::GO_TOWER || messageSpawn->go->type == GameObject::GO_TOWER2) && messageSpawn->Spawn == MessageSpawn::SPAWN_TYPE::SPAWN_PROJECTILE)
			{
				//for (int i = 0; i < 2; ++i)
				{
					/*GameObject* temp = FetchGO(GameObject::GO_SOLDIER);
					temp->Health = 5.f;
					temp->pos = messageSpawn->go->pos;
					temp->scale.Set(m_gridSize, m_gridSize, m_gridSize);
					temp->target = temp->pos;*/

					// spawn projectile 
					GameObject* Projectile = FetchGO(GameObject::GO_PROJECTILE);
					Projectile->pos = messageSpawn->go->pos;
					Projectile->scale.Set(m_gridSize, m_gridSize, m_gridSize);
					Projectile->nearest = messageSpawn->go->nearest;
				}
			}
			else if (messageSpawn->go->type == GameObject::GO_ARCHER && messageSpawn->Spawn == MessageSpawn::SPAWN_TYPE::SPAWN_PROJECTILE)
			{
				//for (int i = 0; i < 2; ++i)
				{
					/*GameObject* temp = FetchGO(GameObject::GO_SOLDIER);
					temp->Health = 5.f;
					temp->pos = messageSpawn->go->pos;
					temp->scale.Set(m_gridSize, m_gridSize, m_gridSize);
					temp->target = temp->pos;*/

					// spawn projectile 
					GameObject* Projectile = FetchGO(GameObject::GO_PROJECTILE);
					Projectile->pos = messageSpawn->go->pos;
					Projectile->scale.Set(m_gridSize, m_gridSize, m_gridSize);
					Projectile->nearest = messageSpawn->go->nearest;
				}
			}
			else if (messageSpawn->go->type == GameObject::GO_ARCHER2 && messageSpawn->Spawn == MessageSpawn::SPAWN_TYPE::SPAWN_PROJECTILE2)
			{
				//for (int i = 0; i < 2; ++i)
				{
					/*GameObject* temp = FetchGO(GameObject::GO_SOLDIER);
					temp->Health = 5.f;
					temp->pos = messageSpawn->go->pos;
					temp->scale.Set(m_gridSize, m_gridSize, m_gridSize);
					temp->target = temp->pos;*/

					// spawn projectile 
					GameObject* Projectile = FetchGO(GameObject::GO_PROJECTILE2);
					Projectile->pos = messageSpawn->go->pos;
					Projectile->scale.Set(m_gridSize, m_gridSize, m_gridSize);
					Projectile->nearest = messageSpawn->go->nearest;
				}
			}
		}
		delete message;
		return;
	}
	MessageWRU* messageWRU = dynamic_cast<MessageWRU*>(message);

	if (messageWRU)
	{
		float nearestdistance = (5* m_gridSize) * 5 * m_gridSize;
		float highestenergy = INT_MIN;
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;

			if (go->active)
			{
				if (messageWRU->type == MessageWRU::NEAREST_FULLFISH && messageWRU->go->active)
				{
					if (go->type == GameObject::GO_FISH)
					{
						if (go->sm->GetCurrentState() == "tooFull" || go->sm->GetCurrentState() == "Full")
						{
							float distancesquared = (go->pos - messageWRU->go->pos).LengthSquared();
							if (distancesquared < nearestdistance)
							{
								//std::cout << "nearest fish received" << std::endl;
								nearestdistance = distancesquared;
								messageWRU->go->nearest = go;
								//return;
							}
						}
					}
				}
				else if (messageWRU->type == MessageWRU::HIGHEST_ENERGYFISH)
				{
					if (go->type == GameObject::GO_FISH)
					{
						float distancesquared = (go->pos - messageWRU->go->pos).LengthSquared();
						if (go->energy > highestenergy)
						{
							std::cout << "highest energy fish received" << std::endl;
							highestenergy = go->energy;
							messageWRU->go->nearest = go;
							//return;
						}
					}

				}
				else if (messageWRU->type == MessageWRU::NEAREST_FISHFOOD)
				{
					if (go->type == GameObject::GO_FISHFOOD)
					{
						float distancesquared = (go->pos - messageWRU->go->pos).LengthSquared();
						if (distancesquared < nearestdistance)
						{
							std::cout << "nearest food received" << std::endl;
							nearestdistance = distancesquared;
							messageWRU->go->nearest = go;
							//return;
						}
					}
				}
				else if (messageWRU->type == MessageWRU::NEAREST_SHARK)
				{
					if (go->type == GameObject::GO_SHARK)
					{
						float distancesquared = (go->pos - messageWRU->go->pos).LengthSquared();
							if (distancesquared < nearestdistance)
						{
							std::cout << "nearest shark received" << std::endl;
							nearestdistance = distancesquared;
							messageWRU->go->nearest = go;
							//return;
						}
					}
					//std::cout << "nearest shark received" << std::endl;
				}
				else if (messageWRU->type == MessageWRU::NEAREST_ENEMY)
				{

					if (go->type == GameObject::GO_BASE2 || go->type == GameObject::GO_ARCHER2 || go->type == GameObject::GO_SOLDIER2 || go->type == GameObject::GO_TANK2 || go->type == GameObject::GO_TOWER || go->type == GameObject::GO_TOWER2 || go->type == GameObject::GO_ETOWER || go->type == GameObject::GO_ETOWER2)
					{
						float distancesquared = (go->pos - messageWRU->go->pos).LengthSquared();
						//if (distancesquared < INT_MAX)
						//float RANGE = 5 * SceneData::GetInstance()->getGridSize();

						{
							if (distancesquared <= messageWRU->threshold * messageWRU->threshold && go->Health > 0 && messageWRU->go->type != GameObject::GO_TANK && go->type != GameObject::GO_TOWER && go->type != GameObject::GO_TOWER2)
							{
								messageWRU->go->nearest = go;
							}
							else if (messageWRU->go->reached == 0 && messageWRU->go->t1 && messageWRU->go->type != GameObject::GO_TOWER && messageWRU->go->type != GameObject::GO_TOWER2 && messageWRU->go->type != GameObject::GO_BASE)
							{
								if (go->type == GameObject::GO_TOWER)
									//qqqqqqqqstd::cout << "It worked";
								if (go == BTower01)
									messageWRU->go->nearest = go;
							}
							else if (messageWRU->go->reached == 0 && messageWRU->go->t2 && messageWRU->go->type != GameObject::GO_TOWER && messageWRU->go->type != GameObject::GO_TOWER2 && messageWRU->go->type != GameObject::GO_BASE)
							{
								if (go == BTower02)
									messageWRU->go->nearest = go;
							}
							else if (go->type == GameObject::GO_ETOWER && go->Health >= 0 && messageWRU->go->t1 && messageWRU->go->type != GameObject::GO_BASE && messageWRU->go->type != GameObject::GO_TOWER && messageWRU->go->type != GameObject::GO_TOWER2 && go->active && go->Health > 0 && messageWRU->go->reached)
							{
								messageWRU->go->nearest = go;
							}
							else if (go->type == GameObject::GO_ETOWER2 && go->Health >= 0 && messageWRU->go->t2 && messageWRU->go->type != GameObject::GO_BASE && messageWRU->go->type != GameObject::GO_TOWER && messageWRU->go->type != GameObject::GO_TOWER2 && go->active && go->Health > 0 && messageWRU->go->reached)
							{
								messageWRU->go->nearest = go;
							}
							else if (go->type == GameObject::GO_BASE2 && messageWRU->go->type != GameObject::GO_BASE && messageWRU->go->type != GameObject::GO_TOWER && messageWRU->go->type != GameObject::GO_TOWER2 && go->active && go->Health > 0 )
							{
								messageWRU->go->nearest = go;
							}
						}
					}
				}
				else if (messageWRU->type == MessageWRU::NEAREST_ENEMY_BASE2)
				{
				float distancesquared = (go->pos - messageWRU->go->pos).LengthSquared();
				
					if ((go->type == GameObject::GO_ARCHER || go->type == GameObject::GO_SOLDIER || go->type == GameObject::GO_BASE || go->type == GameObject::GO_TANK || go->type == GameObject::GO_ETOWER || go->type == GameObject::GO_ETOWER2 || go->type == GameObject::GO_TOWER || go->type == GameObject::GO_TOWER2) && go->active && go->Health >= 0)
					{
						float RANGE = 5 * SceneData::GetInstance()->getGridSize();
						 distancesquared = (go->pos - messageWRU->go->pos).LengthSquared();



						//if (distancesquared < INT_MAX )
						{
						//	nearestdistance = distancesquared;
							if (distancesquared <= (messageWRU->threshold * messageWRU->threshold) && go->Health > 0 && messageWRU->go->type != GameObject::GO_TANK2 && go != ETower01 && go != ETower02) {
								messageWRU->go->nearest = go;
							}
							else if (go->Health > 0 && go == ETower01 && messageWRU->go->reached == 0 && messageWRU->go->t1 && messageWRU->go->type != GameObject::GO_ETOWER && messageWRU->go->type != GameObject::GO_ETOWER2 && messageWRU->go->type != GameObject::GO_BASE2)
							{
								//if (go == ETower01)
									messageWRU->go->nearest = go;
								// if go->t1 && t1 != dead >> t1 
								//>> else if go is t2 and t2 != dead >> t2 else attack base
								//messageWRU->go->nearest = go;
							}
							else if (go->Health > 0 && go == ETower02 && messageWRU->go->reached == 0 && messageWRU->go->t2 && messageWRU->go->type != GameObject::GO_ETOWER && messageWRU->go->type != GameObject::GO_ETOWER2 && messageWRU->go->type != GameObject::GO_BASE2)
							{
								//if (go == ETower02)
									messageWRU->go->nearest = go;
							}
							else if (go->type == GameObject::GO_TOWER && go->Health >= 0 && messageWRU->go->t1 && messageWRU->go->type != GameObject::GO_BASE2 && messageWRU->go->type != GameObject::GO_ETOWER && messageWRU->go->type != GameObject::GO_ETOWER2 && go->active && go->Health > 0)
							{

								if (go == BTower01 && ETower01->Health <= 0)
									messageWRU->go->nearest = go;
								else if (messageWRU->go->reached && ETower01->Health > 0)
									messageWRU->go->nearest = go;
							}
							else if (go->type == GameObject::GO_TOWER2 && go->Health >= 0 && messageWRU->go->t2 && messageWRU->go->type != GameObject::GO_BASE2 && messageWRU->go->type != GameObject::GO_ETOWER && messageWRU->go->type != GameObject::GO_ETOWER2 && go->active && go->Health > 0 )
							{
								if (go == BTower02 && ETower02->Health <= 0)
									messageWRU->go->nearest = go;
								else if (messageWRU->go->reached && ETower02->Health > 0)
									messageWRU->go->nearest = go;
							}
							else if (go->type == GameObject::GO_BASE && messageWRU->go->type != GameObject::GO_BASE2 && messageWRU->go->type != GameObject::GO_ETOWER && messageWRU->go->type != GameObject::GO_ETOWER2 && go->active && go->Health > 0 )
							{
								messageWRU->go->nearest = go;
							}
							
						}
					}
				}
			}
		}
		
		return;
	}

}

void SceneMovement::renderLine(GameObject* go)
{
	if (go->nearest && go->nearest->active)
	{
		Vector3 dir = go->nearest->pos - go->pos;
		Vector3 pos = go->pos + (.5f * dir);
		float length = 0.5f * dir.Length();
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y,0);
		modelStack.Rotate(Math::RadianToDegree(atan2(dir.y, dir.x)), 0, 0, 1);
		modelStack.Scale(length, length, 1);
		if (go->team == GameObject::T_1)
			RenderMesh(meshList[GEO_LINE], false);
		 else if(go->team == GameObject::T_2)
			RenderMesh(meshList[GEO_LINE2], false);
		modelStack.PopMatrix();
	}
}

bool SceneMovement::Handle(Message* message)
{
	m_MessageQueue.push(message);
	return true;
	//static const float SHARK_DIST = 10.f * m_gridSize;
	//static const float FOOD_DIST = 20.f * m_gridSize;
	//for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	//{
	//	GameObject* go = (GameObject*)*it;
	//	if (!go->active)
	//		continue;
	//	if (go->type == GameObject::GO_FISH)
	//	{
	//		//go->nearest = NULL;
	//		float nearestDistance = FLT_MAX;
	//		for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
	//		{
	//			GameObject* go2 = (GameObject*)*it2;
	//			if (!go2->active)
	//				continue;
	//			if (go2->type == GameObject::GO_SHARK)
	//			{
	//				float distance = (go->pos - go2->pos).Length();
	//				if (distance < m_gridSize)
	//				{
	//					go->energy = -1;
	//				}
	//				else if (distance < SHARK_DIST && distance < nearestDistance && go->sm->GetCurrentState() == "Full")
	//				{
	//					nearestDistance = distance;
	//					go->nearest = go2;
	//				}
	//			}
	//			else if (go2->type == GameObject::GO_FISHFOOD)
	//			{
	//				float distance = (go->pos - go2->pos).Length();
	//				if (distance < m_gridSize)
	//				{
	//					go->energy += 2.5f;
	//					go2->active = false;
	//				}
	//				else if (distance < FOOD_DIST && distance < nearestDistance && go->sm->GetCurrentState() == "Hungry")
	//				{
	//					nearestDistance = distance;
	//					go->nearest = go2;
	//				}
	//			}
	//		}
	//	}
	//	else if (go->type == GameObject::GO_SHARK)
	//	{
	//		go->nearest = NULL;
	//		float nearestDistance = FLT_MAX;
	//		float highestEnergy = FLT_MIN;
	//		for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
	//		{
	//			GameObject* go2 = (GameObject*)*it2;
	//			if (!go2->active)
	//				continue;
	//			if (go2->type == GameObject::GO_FISH)
	//			{
	//				if (go->sm->GetCurrentState() == "Naughty")
	//				{
	//					float distance = (go->pos - go2->pos).Length();
	//					if (distance < nearestDistance && (go2->sm->GetCurrentState() == "TooFull" || go2->sm->GetCurrentState() == "Full"))
	//					{
	//						nearestDistance = distance;
	//						go->nearest = go2;
	//					}
	//				}
	//				if (go->sm->GetCurrentState() == "Crazy")
	//				{
	//					if (go2->energy > highestEnergy)
	//					{
	//						highestEnergy = go2->energy;
	//						go->nearest = go2;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

void SceneMovement::Update(double dt)
{
	if (!m_MessageQueue.empty())
	{
		Process(m_MessageQueue.front());
		 m_MessageQueue.pop();
	}
	SceneBase::Update(dt);

	static const float BALL_SPEED = 5.f;
	static const float HOUR_SPEED = 1.f;

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
	if(Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
		SceneData::GetInstance()->setSpeed(Math::Max(0.f, SceneData::GetInstance()->getSpeed() - 0.1f));
	}
	if(Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_speed += 0.1f;
		SceneData::GetInstance()->setSpeed(SceneData::GetInstance()->getSpeed() + .1f);
	}
	if (Application::IsKeyPressed('R'))
	{

		for (auto go : m_goList)
		{
			//if (go->active && (go->type != GameObject::GO_BASE && go->type != GameObject::GO_BASE2 && go->type != GameObject::GO_TOWER && GameObject::GO_TOWER2))
			{
				
					go->active = 0;

					//m_goList.pop_back();
				
			}
		}

		while (!m_MessageQueue.empty())
		{
			delete m_MessageQueue.front();
			m_MessageQueue.pop();
		}
		//if (Base->active == 0)
		{
			Base->active = 1;
			Base->nearest = nullptr;
			Base->Health = 150;
		}
		//else if (Base2->active == 0)
		{
			Base2->active = 1;
			Base2->Health = 150;
			Base2->nearest = nullptr;
		}

		{
			BTower01->active = 1;
			BTower01->nearest = nullptr;
			BTower01->Health = 100;
		}

		{
			BTower02->active = 1;
			BTower02->nearest = nullptr;
			BTower02->Health = 100;
		}

		{
			ETower01->active = 1;
			ETower01->nearest = nullptr;
			ETower01->Health = 100;
		}

		{
			ETower02->active = 1;
			ETower02->nearest = nullptr;
			ETower02->Health = 100;
		}
	}

	m_hourOfTheDay += HOUR_SPEED * static_cast<float>(dt) * m_speed;
	if (m_hourOfTheDay >= 24.f)
		m_hourOfTheDay = 0;

	
	
	if (Base->active == 0 || Base2->active == 0)
		return;
		


	//Input Section
	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}
	static bool bSpaceState = false;
	if (!bSpaceState && Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = true;
		GameObject *go = FetchGO(GameObject::GO_FISH);
		go->scale.Set(m_gridSize, m_gridSize, m_gridSize);
		go->pos.Set(m_gridOffset + Math::RandIntMinMax(0, m_noGrid - 1) * m_gridSize, m_gridOffset + Math::RandIntMinMax(0, m_noGrid - 1) * m_gridSize, 0);
		go->target = go->pos;
		go->steps = 0;
		go->energy = 8.f;
		go->nearest = NULL;
		go->sm->SetNextState("Full");
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
	}
	static bool bVState = false;
	if (!bVState && Application::IsKeyPressed('V'))
	{
		bVState = true;
		GameObject *go = FetchGO(GameObject::GO_FISHFOOD);
		go->scale.Set(m_gridSize, m_gridSize, m_gridSize);
		go->pos.Set(m_gridOffset + Math::RandIntMinMax(0, m_noGrid - 1) * m_gridSize, m_gridOffset + Math::RandIntMinMax(0, m_noGrid - 1) * m_gridSize, 0);
		go->target = go->pos;
		go->moveSpeed = 1.f;
	}
	else if (bVState && !Application::IsKeyPressed('V'))
	{
		bVState = false;
	}
	static bool bBState = false;
	if (!bBState && Application::IsKeyPressed('B'))
	{
		bBState = true;
		GameObject *go = FetchGO(GameObject::GO_SHARK);
		go->scale.Set(m_gridSize, m_gridSize, m_gridSize);
		go->pos.Set(m_gridOffset + Math::RandIntMinMax(0, m_noGrid - 1) * m_gridSize, m_gridOffset + Math::RandIntMinMax(0, m_noGrid - 1) * m_gridSize, 0);
		go->target = go->pos;
	}
	else if (bBState && !Application::IsKeyPressed('B'))
	{
		bBState = false;
	}

	//StateMachine
	for (int i = 0; i != m_goList.size(); ++i)
	{
		
		GameObject *go = (GameObject *)m_goList[i];
		if (!go->active)
			continue;
		if (go->sm && go->active) {
			//std::cout << go->pos;
			go->sm->Update(dt);
		}
	}

	//External triggers
	
	static const float SHARK_DIST = 10.f * m_gridSize;
	static const float FOOD_DIST = 20.f * m_gridSize;
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (!go->active)
			continue;
		if (go->type == GameObject::GO_FISH)
		{
			//go->nearest = NULL;
			float nearestDistance = FLT_MAX;
			for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
			{
				GameObject* go2 = (GameObject*)*it2;
				if (!go2->active)
					continue;
				
				if (go2->type == GameObject::GO_FISHFOOD)
				{
					float distance = (go->pos - go2->pos).Length();
					if (distance < m_gridSize)
					{
						go->energy += 2.5f;
						go2->active = false;
						//--m_objectCount;
					}
				}
			}
		}
	}

	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (!go->active)
			continue;
		if (go->type == GameObject::GO_SHARK)
		{
			//go->nearest = NULL;
			float nearestDistance = FLT_MAX;
			for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
			{
				GameObject* go2 = (GameObject*)*it2;
				if (!go2->active)
					continue;

				if (go2->type == GameObject::GO_FISH)
				{
					float distance = (go->pos - go2->pos).Length();
					if (distance < m_gridSize)
					{
						go2->energy = 0;
						
					}
				}
			}
		}
	}
	//Movement Section
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
			continue;
		Vector3 dir = go->target - go->pos;
		if (dir.Length() < go->moveSpeed * dt * m_speed)
		{
			//GO->pos reach target
			go->pos = go->target;
			if (go->type != GameObject::GO_PROJECTILE && go->type!= GameObject::GO_PROJECTILE2)
			{
				float random = Math::RandFloatMinMax(0.f, 1.f);
				if (random < 0.25f && go->moveRight)
					go->target = go->pos + Vector3(m_gridSize, 0, 0);
				else if (random < 0.5f && go->moveLeft)
					go->target = go->pos + Vector3(-m_gridSize, 0, 0);
				else if (random < 0.75f && go->moveUp)
					go->target = go->pos + Vector3(0, m_gridSize, 0);
				else if (go->moveDown)
					go->target = go->pos + Vector3(0, -m_gridSize, 0);
				if (go->target.x < 0 || go->target.x > m_noGrid* m_gridSize || go->target.y < 0 || go->target.y > m_noGrid * m_gridSize)
					go->target = go->pos;
			}
		}
		else if(go->type != GameObject::GO_BASE && go->type != GameObject::GO_BASE2 && go->type != GameObject::GO_TOWER && go->type != GameObject::GO_TOWER2 && go->type != GameObject::GO_ETOWER && go->type != GameObject::GO_ETOWER2)
		{
			try
			{
				dir.Normalize();
				go->pos += dir * go->moveSpeed * static_cast<float>(dt) * m_speed;
			}
			catch (DivideByZero)
			{
			}
		}
	}

	//Counting objects
	int objectCount = 0;
	int fishCount = 0;
	m_numGO[GameObject::GO_FISH] = m_numGO[GameObject::GO_SHARK] = m_numGO[GameObject::GO_FISHFOOD] = 0;
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
	
		GameObject *go = (GameObject *)*it;
		if (!go->active)
			continue;
		if (cPostOffice->Send("GameObject", new MessageCheckActive()))
			++objectCount;
		else if (cPostOffice->Send("GameObject", new MessageCheckActive()) == 0)
			--objectCount;
		++m_numGO[go->type];
		if (go->type == GameObject::GO_FISH)
			++fishCount;
	}
	SceneData::GetInstance()->SetObjectCount(objectCount);
	SceneData::GetInstance()->SetFishCount(fishCount);
	SceneData::GetInstance()->setGridOffset(m_gridOffset);
	SceneData::GetInstance()->setGridSize(m_gridSize);

}


void SceneMovement::RenderGO(GameObject *go)
{
	renderLine(go);
	std::ostringstream ss;
	switch (go->type)
	{
	case GameObject::GO_BALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);

		ss << go->id;
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FISH:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, zOffset);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (go->sm)
		{
			if (go->sm->GetCurrentState() == "TooFull")
				RenderMesh(meshList[GEO_TOOFULL], false);
			else if (go->sm->GetCurrentState() == "Full")
				RenderMesh(meshList[GEO_FULL], false);
			else if (go->sm->GetCurrentState() == "Hungry")
				RenderMesh(meshList[GEO_HUNGRY], false);
			else
				RenderMesh(meshList[GEO_DEAD], false);
		}

		ss.precision(3);
		ss << go->energy;
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		modelStack.Translate(0, -m_gridSize / 4, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SHARK:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, zOffset);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);

		if (go->sm)
		{
			if (go->sm->GetCurrentState() == "Crazy")
				RenderMesh(meshList[GEO_CRAZY], false);
			else if (go->sm->GetCurrentState() == "Happy")
				RenderMesh(meshList[GEO_HAPPY], false);
			else
				RenderMesh(meshList[GEO_SHARK], false);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FISHFOOD:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, zOffset);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_FISHFOOD], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BASE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BASE], false);
		ss.precision(3);
		ss << go->Health;
		modelStack.Translate(0,-1,0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_TOWER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BASE], false);
		ss.precision(3);
		ss << go->Health;
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_TOWER2:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BASE], false);
		ss.precision(3);
		ss << go->Health;
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ETOWER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BASE2], false);
		ss.precision(3);
		ss << go->Health;
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ETOWER2:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BASE2], false);
		ss.precision(3);
		ss << go->Health;
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0));
		modelStack.PopMatrix();
		break;

	case GameObject::GO_BASE2:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BASE2], false);
		ss.precision(3);
		ss << go->Health;
		modelStack.Translate(0, 1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0));
		modelStack.PopMatrix();
		break;
		break;

	case GameObject::GO_SOLDIER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SOLDIER], false);
		ss.precision(3);
		ss << go->Health;
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
		ss.str("");
		ss << go->sm->GetCurrentState();
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SOLDIER2:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SOLDIER2], false);
		ss.precision(3);
		ss << go->Health;
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
		ss.str("");
		ss << go->sm->GetCurrentState();
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PROJECTILE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PROJECTILE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PROJECTILE2:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PROJECTILE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ARCHER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ARCHER], false);
		ss.precision(3);
		ss << go->Health;
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
		ss.str("");
		ss << go->sm->GetCurrentState();
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ARCHER2:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ARCHER2], false);
		ss.precision(3);
		ss << go->Health;
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
		ss.str("");
		ss << go->sm->GetCurrentState();
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_TANK:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_TANK], false);
		ss.precision(3);
		ss << go->Health;
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
		ss.str("");
		ss << go->sm->GetCurrentState();
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_TANK2:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_TANK2], false);
		ss.precision(3);
		ss << go->Health;
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
		ss.str("");
		ss << go->sm->GetCurrentState();
		modelStack.Translate(0, -1, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
		modelStack.PopMatrix();
		break;
	}

}

void SceneMovement::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	
	RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(m_worldHeight * 0.5f, m_worldHeight * 0.5f, -1.f);
	modelStack.Scale(m_worldHeight, m_worldHeight, m_worldHeight);
	RenderMesh(meshList[GEO_BG], false);
	modelStack.PopMatrix();

	zOffset = 0;
	if (Base->active == 0 || Base2->active == 0)
	{
		if (Base2->active == 0) {
			std::ostringstream ss;
			ss.precision(3);
			ss << "RED WINS" << '\n';
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 12, 0, 25);
		}
		else
		{
			std::ostringstream ss;
			ss.precision(3);
			ss << "GREEN WINS" << '\n';
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 12, 0, 25);
		}
		return;
	}
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			zOffset += 0.001f;
			RenderGO(go);
		}
	}

	//On screen text
	
	std::ostringstream ss;
	ss.precision(3);
	ss << "Speed:" << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 6);
	
	ss.str("");
	ss.precision(5);
	ss << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 3);

	ss.str("");
	ss << "Obj Count:" << SceneData::GetInstance()->GetObjectCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	ss.str("");
	ss << "Fishes:" << m_numGO[GameObject::GO_FISH];
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 18);

	ss.str("");
	ss << "Shark:" << m_numGO[GameObject::GO_SHARK];
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 15);

	ss.str("");
	ss << "Number of Message: " << m_MessageQueue.size();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 12);
	
	//RenderTextOnScreen(meshList[GEO_TEXT], "Aquarium", Color(0, 1, 0), 3, 50, 0);
}

void SceneMovement::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	while (!m_MessageQueue.empty())
	{
		delete m_MessageQueue.front();
		m_MessageQueue.pop();
	}
	if(m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
