#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "StateMachine.h"
#include "ObjectBase.h"
#include "Maze.h"
#include "NeuralNode.h"

struct GameObject : public ObjectBase
{
	enum GAMEOBJECT_TEAM
	{
		T_1 = 0,
		T_2
	};
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_PLAYER,
		GO_HEALER,
		GO_PIPE,
		GO_MONSTER,
		GO_BALL,
		GO_CROSS,
		GO_CIRCLE,
		GO_FISH,
		GO_SHARK,
		GO_FISHFOOD,
		GO_BLACK,
		GO_WHITE,
		GO_SOLDIER,
		GO_BIRD,
		GO_TANK,
		GO_ARCHER,
		GO_SOLDIER2,
		GO_TANK2,
		GO_WAYPOINT,
		GO_WAYPOINT2,
		GO_EWAYPOINT,
		GO_EWAYPOINT2,
		GO_ARCHER2,
		GO_BASE,
		GO_BASE2,
		GO_TOWER,
		GO_TOWER2,
		GO_ETOWER,
		GO_ETOWER2,
		GO_ABASE,
		GO_EBASE,
		GO_PROJECTILE,
		GO_EXIT,
		GO_PROJECTILE2,
		GO_QUEEN,
		GO_TOTAL, //must be last
	};
	std::vector<Maze::TILE_CONTENT> grid;
	std::vector<bool> visited;
	std::vector<MazePt> stack;
	std::vector<MazePt> path;
	MazePt curr;
	GAMEOBJECT_TYPE type;
	GAMEOBJECT_TEAM team;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active;
	float mass;
	Vector3 target;
	int id;
	int score;
	bool alive;
	int steps;
	float energy;
	float moveSpeed;
	float countDown;
	float Health;
	GameObject *nearest;
	bool t1;
	bool t2;
	bool turn; // if its turn base move base off energy
	bool reached;
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;
	bool endTurn;
	int range;
	StateMachine *sm;
	std::vector<MazePt> canSee;
	std::vector<GameObject*> canAttack;
	std::vector<NNode> hiddenNode;
	NNode outputNode;
	Maze* maze;
	MazePt start, end;
	bool isTurnBase;
	virtual bool Handle(Message* message);
	GameObject(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~GameObject();
};

#endif