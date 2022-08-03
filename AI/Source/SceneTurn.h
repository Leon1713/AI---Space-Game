#ifndef SCENE_TURN_H
#define SCENE_TURN_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Maze.h"
#include <queue>
#include "PostOffice.h"

class SceneTurn : public SceneBase , public ObjectBase
{
public:
	SceneTurn();
	~SceneTurn();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);
	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);

	void DFS(MazePt curr);
	bool BFS(MazePt start, MazePt end);
	static bool BFSLimit(GameObject* go, MazePt end, int limit, Maze* maze, int moveLimit);
	static std::vector<MazePt> BFSLimitVisibility(GameObject* go, int limit, Maze* maze, bool walls, int moveLimit);
	virtual bool Handle(Message* message);
	void Process(Message* message);

protected:
	bool lightOn;
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;
	int m_noGrid;
	GameObject* player;
	GameObject* exit;
	float m_gridSize;
	PostOffice* cPostOffice;
	float m_gridOffset;
	int m_turn;
	float timer;
	//virtual bool Handle(Message* message);


	Maze m_maze;
	MazePt m_start;
	MazePt m_end;

	void DFSOnce(GameObject* go);
	std::vector<bool> visible;
	std::vector<Maze::TILE_CONTENT> m_myGrid; //read maze and store here
	std::vector<bool> m_visited; //visited set for DFS/BFS
	std::queue<MazePt> m_queue; //queue for BFS
	std::vector<MazePt> m_previous; //to store previous tile
	std::vector<MazePt> m_shortestPath;  //to store shortest path
	std::vector<MazePt> turnTile;
	std::queue<Message*> m_MessageQueue;
	unsigned m_mazeKey;
	float m_wallLoad;
};

#endif