#include "SceneData.h"

int SceneData::GetObjectCount()
{
	return m_objectCount;
}

int SceneData::getTurn()
{
	return m_turn;
}

int SceneData::getLives()
{
	return Life;
}

void SceneData::setLives(int lives)
{
	Life = lives;
}

void SceneData::setTurn(int turn)
{
	m_turn = turn;
}
void SceneData::SetObjectCount(int objectCount)
{
	m_objectCount = objectCount;
}

int SceneData::GetFishCount()
{
	return m_fishCount;
}

float SceneData::getWorldHeight()
{
	return worldHeight;
}

void SceneData::setWorldHeight(float height)
{
	worldHeight = height;
}

void SceneData::setWorldWidth(float width)
{
	worldWidth = width;
}

float SceneData::getWorldWidth()
{
	return worldWidth;
}

void SceneData::SetFishCount(int fishCount)
{
	m_fishCount = fishCount;
}

float SceneData::getGridSize()
{
	return m_gridSize;
}

int SceneData::getNoGrid()
{
	return noGrid;
}

void SceneData::setGridSize(float GridSize)
{
	m_gridSize = GridSize;
}

void SceneData::setNoGrid(int size)
{
	noGrid = size;
}

float SceneData::getGridOffset()
{
	return m_gridOffset;
}

void SceneData::setGridOffset(float gridOffset)
{
	m_gridOffset = gridOffset;
}

float SceneData::getSpeed()
{
	return m_speed;
}

void SceneData::setSpeed(float speed)
{
	m_speed = speed;
}

SceneData::SceneData()
{
}

SceneData::~SceneData()
{
}
