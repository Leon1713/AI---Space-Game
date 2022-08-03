#include "Maze.h"
#include <iostream>
#include "MyMath.h"
#include "Application.h"
#include <stack>
#include <queue>
#include <vector>	

Maze::Maze()
{
}

Maze::~Maze()
{
}

void Maze::Generate(unsigned key, unsigned size, MazePt start) // num of empty cell == (size * size) * (1-wallLoad)
{
	m_size = size;
	std::vector<MazePt> stack;
	std::vector<bool> visited;
	stack.push_back(start);
	m_grid.clear();
	if (size == 0)
		return;
	start.x = Math::Clamp(start.x, 0, (int)size - 1);
	start.y = Math::Clamp(start.y, 0, (int)size - 1);
	//wallLoad = Math::Clamp(wallLoad, 0.f, 0.8f);
	unsigned total = size * size;
	visited.resize(total);
	std::fill(visited.begin(), visited.end(), false);
	m_grid.resize(total);
	std::fill(m_grid.begin(), m_grid.end(), TILE_EMPTY);
	unsigned startId = start.y * size + start.x;
	srand(key);
	/*for (int i = 0; i < (int)total * wallLoad;) 
	{
		unsigned chosen = rand() % total;
		if (chosen == startId)
			continue;
		if (m_grid[chosen] == TILE_EMPTY)
		{
			m_grid[chosen] = TILE_WALL;
			++i;
		}
	}*/
	std::cout << "Maze " << key << std::endl;
	/*for (int row = (int)size - 1; row >= 0; --row)
	{
		for (int col = 0; col < (int)size; ++col)
		{
			if (m_grid[row * size + col] == TILE_WALL)
				std::cout << "1 ";
			else
				std::cout << "0 ";
		}
		std::cout << std::endl;
	}*/
	for (int row = (int)size - 1; row >= 0; --row) // set all wall.
	{
		for (int col = 0; col < (int)size ; ++col)
		{
			m_grid[row * size + col] = TILE_WALL;
		}
	}

	// BFS maze -----------------

	while (!stack.empty())
	{
		std::vector<MazePt> neighbours;
		MazePt tempPart = stack.back();
		if (tempPart.x % 2 != 0 || tempPart.y % 2 != 0)
		{
			continue;
		}
		stack.pop_back();
		visited[tempPart.y * size + tempPart.x] = true; 
		m_grid[tempPart.y * size + tempPart.x] = TILE_EMPTY;
		int rand = Math::RandIntMinMax(0, 3); // 0 up 1 right 2 bot 3 left
		MazePt next;
		// get neighbours
		for (int i = 0; i < 2; ++i)
		{
			for (int z = -1; z < 2; ++z)
			{
				if (i == 0)
				{
					if (z != 0)
					{
						next.Set(tempPart.x + (z * 2) , tempPart.y);
						if (next != tempPart && See(next) != TILE_ERROR)
						{
							if (visited[next.y * size + next.x] == false)
							{
								neighbours.push_back(next);	
							}
						}
					}
				}
				else
				{
					if (z != 0)
					{
						next.Set(tempPart.x, tempPart.y + (z * 2));
						if (next != tempPart && See(next) != TILE_ERROR)
						{
							if (visited[next.y * size + next.x] == false)
							{
								neighbours.push_back(next);
							}
						}
					}
				}
			}
		}

		int randIndex;
		if (neighbours.size() != 0) {
			randIndex = Math::RandInt() % neighbours.size();
		}
		for (int i = 0; i < neighbours.size(); ++i)
		{
			// visit all neighbours
			int index = neighbours[i].y * size + neighbours[i].x;
			if (visited[index] == false)
			{
				visited[index] = true;
				m_grid[index] = TILE_EMPTY;
				// connect nodes
				int connectionIndex = ((neighbours[i].y + tempPart.y) / 2) * size + ((neighbours[i].x + tempPart.x) / 2);
				m_grid[connectionIndex] = TILE_EMPTY;
				
				if (i != randIndex) {
					stack.push_back(neighbours[i]);
				}
			}
		}
		if (!neighbours.empty()) {
			stack.push_back(neighbours[randIndex]);
		}
		neighbours.clear();
		//system("pause");
	}


	//for (int row = (int)size - 1; row >= 0; --row)
	//{

	//	for (int col = 0; col < (int)size; ++col)
	//	{
	//		if (col % 2 != 0 || row % 2 != 0)
	//		{
	//			continue;
	//		}



	//		//int direction = Math::RandIntMinMax(1, 2); // 1 is North, 2 is West
	//		////direction = 2;
	//		//if (direction == 1)
	//		//{
	//		//	if (row < (int)size - 1)
	//		//	{
	//		//		m_grid[row * size + col] = TILE_EMPTY;
	//		//		m_grid[(row + 1) * size + col] = TILE_EMPTY;
	//		//	}
	//		//	else
	//		//	{
	//		//		m_grid[row * size + col] = TILE_EMPTY;
	//		//		if (col - 1 != -1)
	//		//		{
	//		//			m_grid[row * size + (col - 1)] = TILE_EMPTY;
	//		//		}
	//		//	}
	//		//}
	//		//else if (direction == 2)
	//		//{
	//		//	if (col > 0)
	//		//	{
	//		//		m_grid[row * size + col] = TILE_EMPTY;
	//		//		m_grid[row * size + (col - 1)] = TILE_EMPTY;
	//		//	}
	//		//	else
	//		//	{
	//		//		m_grid[row * size + col] = TILE_EMPTY;
	//		//		std::cout << '(' << col << ',' << row << ")\n";
	//		//		if (row < (int)size - 1)
	//		//		{
	//		//			m_grid[(row + 1) * size + col] = TILE_EMPTY;
	//		//		}
	//		//	}
	//		//}

	//	}
	//}

	for (int row = (int)size - 1; row >= 0; --row)
	{
		for (int col = 0; col < (int)size; ++col)
		{
			if (m_grid[row * size + col] == TILE_WALL)
				std::cout << "1 ";
			else
				std::cout << "0 ";
		}
		std::cout << std::endl;
	}


	m_key = key;
	//m_size = size;
	m_numMove = 0;
}

unsigned Maze::GetKey()
{
	return m_key;
}

unsigned Maze::GetSize()
{
	return m_size;
}

MazePt Maze::GetCurr()
{
	return m_curr;
}


int Maze::GetNumMove()
{
	return m_numMove;
}

void Maze::SetCurr(MazePt newCurr)
{
	/*Application::GetInstance().iterate();
	Sleep(1);*/
	++m_numMove;
	m_curr = newCurr;
}

void Maze::SetNumMove(int num)
{
	m_numMove = num;
}

Maze::TILE_CONTENT Maze::See(MazePt tile)
{
	if (tile.y > m_size - 1 || tile.y < 0 || tile.x > m_size - 1 || tile.x < 0)
		return Maze::TILE_ERROR;
	else
	return m_grid[tile.y * m_size + tile.x];
}

bool Maze::Move(DIRECTION direction)
{
	/*Application::GetInstance().iterate();
	Sleep(1);*/
	++m_numMove;
	MazePt temp = m_curr;
	switch (direction)
	{
	case DIR_LEFT:
		if (temp.x == 0)
			return false;
		temp.x -= 1;
		break;
	case DIR_RIGHT:
		if (temp.x == (int)m_size - 1)
			return false;
		temp.x += 1;
		break;
	case DIR_UP:
		if (temp.y == (int)m_size - 1)
			return false;
		temp.y += 1;
		break;
	case DIR_DOWN:
		if (temp.y == 0)
			return false;
		temp.y -= 1;
		break;
	}
	int tempId = temp.y * m_size + temp.x;
	if (m_grid[tempId] == TILE_WALL)
		return false;
	m_curr = temp;
	return true;
}

bool MazePt::operator==(const MazePt compared)
{
	if (x == compared.x && y == compared.y)
		return true;
	else
		return false;
}

bool MazePt::operator!=(const MazePt compared)
{
	if (x == compared.x && y == compared.y)
		return false;
	else
		return true;
}
