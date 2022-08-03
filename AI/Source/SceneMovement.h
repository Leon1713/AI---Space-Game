#ifndef SCENE_MOVEMENT_H
#define SCENE_MOVEMENT_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "PostOffice.h"
#include "Message.h"
#include <queue>

class SceneMovement : public ObjectBase, public SceneBase
{
public:
	SceneMovement();
	~SceneMovement();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject* go);

	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);
	void Process(Message* message);
	void renderLine(GameObject* go);
	virtual bool Handle(Message* message);
protected:

	std::vector<GameObject*> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject* m_ghost;
	int m_objectCount;
	int m_noGrid;
	float m_gridSize;
	float m_gridOffset;
	float m_hourOfTheDay;
	int m_numGO[GameObject::GO_TOTAL];
	float zOffset;
	GameObject* Base2;
	GameObject* Base;
	GameObject* BTower01;
	GameObject* BTower02;
	GameObject* ETower01;
	GameObject* ETower02;
	/*StateMachine* SoldierSM;
	StateMachine* ArcherSM;
	StateMachine* Tank2SM;
	StateMachine* Archer2SM;
	StateMachine* Soldier2SM;
	StateMachine* Base2SM;
	StateMachine* BaseSM;
	StateMachine* TankSM;*/

	PostOffice* cPostOffice;
	std::queue<Message*> m_MessageQueue;
};

#endif