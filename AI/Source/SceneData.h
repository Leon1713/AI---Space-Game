#ifndef SCENE_DATA
#define SCENE_DATA

#include "SingletonTemplate.h"

class SceneData : public Singleton<SceneData>
{
	friend Singleton<SceneData>;
public:
	int GetObjectCount();
	void SetObjectCount(int objectCount);
	int GetFishCount(); // to edit later
	void SetFishCount(int fishCount);
	float getGridSize();
	int getNoGrid();
	void setNoGrid(int n);
	void setGridSize(float GridSize);
	float getGridOffset();
	void setGridOffset(float GridOffset);
	float getSpeed();
	float getWorldHeight();
	void setWorldHeight(float h);
	float getWorldWidth();
	void setWorldWidth(float w);
	void setSpeed(float m_speed);
	void setTurn(int t);
	void setLives(int i);
	int getLives();
	int getTurn();
	

private:
	SceneData();
	~SceneData();
	int m_objectCount;
	int m_fishCount;
	int noGrid;
	float m_gridSize;
	int Life;
	float m_gridOffset;
	float worldHeight;
	float worldWidth;
	bool GameEnd;
	float m_speed;
	int m_turn;

};

#endif
