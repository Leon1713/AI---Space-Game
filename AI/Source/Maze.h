#ifndef MAZE_H
#define MAZE_H

#include <vector>

struct MazePt
{
	int x, y;
	MazePt* prev;
	MazePt* next;
	MazePt(int _x = 0, int _y = 0) : x(_x), y(_y) {}
	void Set(int _x = 0, int _y = 0) {x = _x; y = _y;}
	bool operator==(MazePt compared);
	bool operator!=(MazePt compared);
};

class Maze
{
public:
	enum TILE_CONTENT
	{
		TILE_WALL = -1,
		TILE_FOG = 0,
		TILE_EMPTY = 1,
		TILE_ERROR
	};
	enum DIRECTION
	{
		DIR_UP,
		DIR_DOWN,
		DIR_LEFT,
		DIR_RIGHT,
	};
	Maze();
	~Maze();
	void Generate(unsigned key, unsigned size, MazePt start);
	bool Move(DIRECTION direction);
	unsigned GetKey();
	unsigned GetSize();
	MazePt GetCurr();
	//TILE_CONTENT See(MazePt Tile);
	int GetNumMove();
	void SetCurr(MazePt newCurr);
	void SetNumMove(int num);
	std::vector<TILE_CONTENT> m_grid;
	TILE_CONTENT See(MazePt tile);
private:
	//Do not make these public
	unsigned m_key;
	unsigned m_size;
	MazePt m_curr;
	int m_numMove;
};

#endif