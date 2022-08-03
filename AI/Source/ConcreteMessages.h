#ifndef CONCRETE_MESSAGE_H
#define CONCRETE_MESSAGE_H

#include "Message.h"
#include "GameObject.h"
#include "Maze.h"

struct MessageMove : public Message
{
	enum MoveType
	{
		MOVE_DFSONCE = 0,
		MOVE_BFSLIMIT,
		MOVE_BFS_LIMIT_VISIBILITY
	};
	MessageMove(GameObject* goValue, float thresholdValue, MoveType type, MazePt vend) : go(goValue),moveType(type), end(vend)  {}
	virtual ~MessageMove() {}
	GameObject* go;
	MoveType moveType;
	MazePt end;
};


struct MessageWRU : public Message
{
	enum SEARCH_TYPE
	{
		SEARCH_NONE = 0,
		NEAREST_SHARK,
		NEAREST_FISHFOOD,
		NEAREST_FULLFISH,
	 	HIGHEST_ENERGYFISH,
		NEAREST_ENEMY,
		NEAREST_BTOWER,
		NEAREST_ETOWER,
		NEAREST_ENEMY_BASE2,
		NEAREST_GRID_ENEMY,
		NEAREST_GRID_PLAYER

	};
	MessageWRU(GameObject *goValue, SEARCH_TYPE typeValue, float thresholdValue) : go(goValue), type(typeValue), threshold(thresholdValue) {}
	virtual ~MessageWRU() {}

	GameObject *go;
	SEARCH_TYPE type;
	float threshold;
};

struct MessageCheckActive : public Message
{
	
	MessageCheckActive()
	{

	}
	virtual ~MessageCheckActive() {}
};

struct MessageSpawn : public Message
{
	enum SPAWN_TYPE
	{
		SPAWN_SOLDIER = 0,
		SPAWN_ARCHER,
		SPAWN_TANK,
		SPAWN_SOLDIER2,
		SPAWN_ARCHER2,
		SPAWN_TANK2,
		SPAWN_PROJECTILE,
		SPAWN_PROJECTILE2,
		NUM_SPAWN

	};
	MessageSpawn(GameObject* goVal,SPAWN_TYPE spawnType = SPAWN_SOLDIER) : go(goVal), Spawn(spawnType) {};
	virtual ~MessageSpawn() {};
	SPAWN_TYPE Spawn;
	GameObject* go;
};


struct MessageDying : public Message
{
	MessageDying(GameObject* goVal) : go(goVal) {};
	virtual ~MessageDying() {};
	GameObject* go;
};


#endif
