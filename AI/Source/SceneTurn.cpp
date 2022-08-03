#include "SceneTurn.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "PostOffice.h"
#include "State.h"
#include "ConcreteMessages.h"
#include "SceneData.h"
#include "StatePlayer.h"
#include "StateEnemy.h"
#include "TurnManager.h"
#include "StateHealer.h"
#include <time.h>
SceneTurn::SceneTurn()
{
}

SceneTurn::~SceneTurn()
{
}

void SceneTurn::Init()
{
	SceneBase::Init();
	cPostOffice = PostOffice::GetInstance();
	cPostOffice->Register("sceneTurn", this);
	SceneData::GetInstance()->setLives(1);
	
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	SceneData::GetInstance()->setWorldHeight(m_worldHeight);
	SceneData::GetInstance()->setWorldWidth(m_worldWidth);

	//Physics code here
	m_speed = 1.f;
	SceneData::GetInstance()->setSpeed(m_speed);
	Math::InitRNG();

	m_noGrid = 31;
	m_gridSize = m_worldHeight / m_noGrid;
	m_gridOffset = m_gridSize / 2;

	m_turn = 0;
	SceneData::GetInstance()->setTurn(m_turn);
	m_start.Set(0, 0);
	m_mazeKey = 0;
	m_wallLoad = 0.3f;
	m_maze.Generate(m_mazeKey, m_noGrid, m_start); //Generate new maze
	m_myGrid.resize(m_noGrid * m_noGrid);
	m_visited.resize(m_noGrid * m_noGrid);
	m_previous.resize(m_noGrid * m_noGrid);
	std::fill(m_myGrid.begin(), m_myGrid.end(), Maze::TILE_FOG);
	std::fill(m_visited.begin(), m_visited.end(), false);
	m_myGrid[m_start.y * m_noGrid + m_start.x] = Maze::TILE_EMPTY;
	timer = 0;
	visible.resize(m_noGrid * m_noGrid);
	std::fill(visible.begin(), visible.end(), false);
	//DFS(m_start);
	SceneData::GetInstance()->setGridOffset(m_gridOffset);
	SceneData::GetInstance()->setGridSize(m_gridSize);
	SceneData::GetInstance()->setNoGrid(m_noGrid);
	
	player = FetchGO(GameObject::GO_PLAYER);
	MazePt randStart;
	//srand(m_mazeKey);
	do {
		randStart.Set(Math::RandIntMinMax(0, m_noGrid - 1), Math::RandIntMinMax(0, m_noGrid - 1));
	} while (m_maze.See(randStart) != Maze::TILE_EMPTY);
	
	player->curr = randStart;
	player->pos.Set(m_gridOffset, m_gridOffset, 1);
	player->grid.resize(m_noGrid * m_noGrid);
	player->energy = 6;
	player->range = 2;
	player->visited.resize(m_noGrid * m_noGrid);
	std::fill(player->grid.begin(), player->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
	std::fill(player->visited.begin(), player->visited.end(), false);
	player->stack.push_back(player->curr);
	player->grid[player->curr.y * m_noGrid + player->curr.x] = Maze::TILE_EMPTY;

	for (int i = 0; i < 3; ++i)
	{
		GameObject* Enemy = FetchGO(GameObject::GO_MONSTER);
		do {
			randStart.Set(Math::RandIntMinMax(0, m_noGrid - 1), Math::RandIntMinMax(0, m_noGrid - 1));
		}
		while (m_maze.See(randStart) != Maze::TILE_EMPTY || abs(randStart.x - player->curr.x) + abs(randStart.y - player->curr.y) < 10);
		Enemy->curr = randStart;

		Enemy->pos.Set(m_gridOffset, m_gridOffset, 1);
		Enemy->grid.resize(m_noGrid * m_noGrid);
		Enemy->energy = 4;
		Enemy->range = 2;
		Enemy->visited.resize(m_noGrid * m_noGrid);
		//std::fill(Enemy->grid.begin(), Enemy->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
		std::fill(Enemy->visited.begin(), Enemy->visited.end(), false);
		Enemy->stack.push_back(Enemy->curr);
		Enemy->grid[Enemy->curr.y * m_noGrid + Enemy->curr.x] = Maze::TILE_EMPTY;
	}

	GameObject* Healer = FetchGO(GameObject::GO_HEALER);
	do {
		randStart.Set(Math::RandIntMinMax(0, m_noGrid - 1), Math::RandIntMinMax(0, m_noGrid - 1));
	} while (m_maze.See(randStart) != Maze::TILE_EMPTY || abs(randStart.x - player->curr.x) + abs(randStart.y - player->curr.y) > 4);
	Healer->curr = randStart;
	Healer->pos.Set(m_gridOffset, m_gridOffset, 1);
	Healer->grid.resize(m_noGrid * m_noGrid);
	Healer->energy = 4;
	Healer->range = 2;
	Healer->visited.resize(m_noGrid * m_noGrid);
	//std::fill(Enemy->grid.begin(), Enemy->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
	std::fill(Healer->visited.begin(), Healer->visited.end(), false);
	Healer->stack.push_back(Healer->curr);
	Healer->grid[Healer->curr.y * m_noGrid + Healer->curr.x] = Maze::TILE_EMPTY;
	
	do {
		randStart.Set(Math::RandIntMinMax(0, m_noGrid - 1), Math::RandIntMinMax(0, m_noGrid - 1));
	} while (m_maze.See(randStart) != Maze::TILE_EMPTY || abs(randStart.x - player->curr.x) + abs(randStart.y - player->curr.y) < 13);
	exit = FetchGO(GameObject::GO_EXIT);
	exit->curr = randStart;
	exit->pos.Set(m_gridOffset, m_gridOffset, 1);
	exit->grid.resize(m_noGrid * m_noGrid);
	exit->energy = 4;
	exit->range = 2;
	exit->visited.resize(m_noGrid * m_noGrid);
	//std::fill(Enemy->grid.begin(), Enemy->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
	std::fill(exit->visited.begin(), exit->visited.end(), false);
	exit->stack.push_back(exit->curr);
	exit->grid[exit->curr.y * m_noGrid + exit->curr.x] = Maze::TILE_EMPTY;

	for (auto go : m_goList)
	{
		if (go->active && (go->type == GameObject::GO_PLAYER || go->type == GameObject::GO_MONSTER || go->type == GameObject::GO_HEALER))
			TurnManager::GetInstance()->Add(go);
	}
	
	/*for (int e = 0; e < 3; ++e) {
	GameObject* Enemy = FetchGO(GameObject::GO_MONSTER);
		do {
			randStart.Set(Math::RandIntMinMax(0, m_noGrid - 1), Math::RandIntMinMax(0, m_noGrid - 1));
		} while (m_maze.See(randStart) != Maze::TILE_EMPTY && (abs(Enemy->curr.x - player->curr.x) + abs(Enemy->curr.y - player->curr.y)) >= 10);
		Enemy->curr = randStart;
		Enemy->pos.Set(m_gridOffset, m_gridOffset, 1);
		Enemy->grid.resize(m_noGrid * m_noGrid);
		Enemy->energy = 4;
		Enemy->range = 2;
		Enemy->visited.resize(m_noGrid * m_noGrid);
		std::fill(Enemy->grid.begin(), Enemy->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
		std::fill(Enemy->visited.begin(), Enemy->visited.end(), false);
	}*/
}

GameObject* SceneTurn::FetchGO(GameObject::GAMEOBJECT_TYPE type)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (!go->active && go->type == type)
		{
			go->active = true;
			++m_objectCount;
			return go;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(type);
		m_goList.push_back(go);
		if (type == GameObject::GO_PLAYER)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StatePlayerIdle("PlayerIdle", go));
			go->sm->AddState(new StatePlayerMovement("PlayerMovement", go));
			go->sm->AddState(new StatePlayerAttack("PlayerAttack", go));
			go->sm->AddState(new StatePlayerDead("PlayerDead", go));
		}
		else if (type == GameObject::GO_MONSTER)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateEnemyWander("monsterWander", go));
			go->sm->AddState(new StateEnemyHunt("monsterHunt", go));
		}
		else if (type == GameObject::GO_HEALER)
		{
			go->sm = new StateMachine();
			go->sm->AddState(new StateHealerHeal("healerHeal", go));
			go->sm->AddState(new StateHealerWander("healerWander", go));
		}
	}
	return FetchGO(type);
}

void SceneTurn::DFS(MazePt curr)
{
	m_visited[curr.y * m_noGrid + curr.x] = true;
	//UP
	if(curr.y < m_noGrid - 1)
	{
		MazePt next(curr.x, curr.y + 1);
		if (!m_visited[next.y * m_noGrid + next.x])
		{
			if (m_maze.Move(Maze::DIR_UP) == true)
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_EMPTY;
				DFS(next);
				m_maze.Move(Maze::DIR_DOWN);
			}
			else
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
			}
		}
	}
	//DOWN
	if (curr.y > 0)
	{
		MazePt next(curr.x, curr.y - 1);
		if (!m_visited[next.y * m_noGrid + next.x])
		{
			if (m_maze.Move(Maze::DIR_DOWN) == true)
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_EMPTY;
				DFS(next);
				m_maze.Move(Maze::DIR_UP);
			}
			else
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
			}
		}
	}
	//LEFT
	if (curr.x > 0)
	{
		MazePt next(curr.x - 1, curr.y);
		if (!m_visited[next.y * m_noGrid + next.x])
		{
			if (m_maze.Move(Maze::DIR_LEFT) == true)
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_EMPTY;
				DFS(next);
				m_maze.Move(Maze::DIR_RIGHT);
			}
			else
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
			}
		}
	}
	//RIGHT
	if (curr.x < m_noGrid - 1)
	{
		MazePt next(curr.x + 1, curr.y);
		if (!m_visited[next.y * m_noGrid + next.x])
		{
			if (m_maze.Move(Maze::DIR_RIGHT) == true)
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_EMPTY;
				DFS(next);
				m_maze.Move(Maze::DIR_LEFT);
			}
			else
			{
				m_myGrid[next.y * m_noGrid + next.x] = Maze::TILE_WALL;
			}
		}
	}
}

bool SceneTurn::BFS(MazePt start, MazePt end)
{
	std::fill(m_visited.begin(), m_visited.end(), false);
	while (!m_queue.empty())
		m_queue.pop();
	m_shortestPath.clear();
	m_queue.push(start);
	m_maze.SetNumMove(0);
	while (!m_queue.empty())
	{
		MazePt curr = m_queue.front();
		m_maze.SetCurr(curr);
		m_queue.pop();
		if (curr.x == end.x && curr.y == end.y)
		{
			while (!(curr.x == start.x && curr.y == start.y))
			{
				m_shortestPath.insert(m_shortestPath.begin(), curr);
				curr = m_previous[curr.y * m_noGrid + curr.x];
			}
			m_shortestPath.insert(m_shortestPath.begin(), curr);
			return true;
		}
		//UP
		if (curr.y < m_noGrid - 1)
		{
			MazePt next(curr.x, curr.y + 1);
			if (!m_visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				m_previous[next.y * m_noGrid + next.x] = curr;
				m_queue.push(next);
				m_visited[next.y * m_noGrid + next.x] = true;
			}
		}
		//DOWN
		if (curr.y > 0)
		{
			MazePt next(curr.x, curr.y - 1);
			if (!m_visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				m_previous[next.y * m_noGrid + next.x] = curr;
				m_queue.push(next);
				m_visited[next.y * m_noGrid + next.x] = true;
			}
		}
		//LEFT
		if (curr.x > 0)
		{
			MazePt next(curr.x - 1, curr.y);
			if (!m_visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				m_previous[next.y * m_noGrid + next.x] = curr;
				m_queue.push(next);
				m_visited[next.y * m_noGrid + next.x] = true;
			}
		}
		//RIGHT
		if (curr.x < m_noGrid - 1)
		{
			MazePt next(curr.x + 1, curr.y);
			if (!m_visited[next.y * m_noGrid + next.x] && m_myGrid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
			{
				m_previous[next.y * m_noGrid + next.x] = curr;
				m_queue.push(next);
				m_visited[next.y * m_noGrid + next.x] = true;
			}
		}
	}
	return false;
}

std::vector<MazePt> SceneTurn::BFSLimitVisibility(GameObject* go, int limit, Maze* m_maze, bool walls, int moveLimit)
{
	int loop = 0;
	int nearestDistance = INT_MAX;
	MazePt nearest = go->curr;
	MazePt curr;
	MazePt next;
	std::vector<MazePt> result;
	std::vector<MazePt> previous;
	std::vector<bool> visited;
	previous.resize(m_maze->GetSize() * m_maze->GetSize());
	visited.resize(m_maze->GetSize() * m_maze->GetSize());
	std::fill(visited.begin(), visited.end(), false);
	std::queue<MazePt> queue;
	queue.push(go->curr);
	if (moveLimit == 0)
		return result;
	while (!queue.empty() && loop <= limit)
	{
		++loop;
		curr = queue.front();
		queue.pop();
		// find distance and break if dist is more than move limit
		if (abs(curr.x - go->curr.x) + abs(curr.y - go->curr.y) > moveLimit)
		{
			break;
		}
		result.push_back(curr);
		if (!walls)
		{
			next = MazePt(curr.x, curr.y + 1);
			if (m_maze->See(next) != Maze::TILE_ERROR && m_maze->See(next) != Maze::TILE_WALL)
			{
				if (!visited[next.y * m_maze->GetSize() + next.x])
				{
					previous[next.y * m_maze->GetSize() + next.x] = curr;
					queue.push(next);
					//result.push_back(next);
					visited[next.y * m_maze->GetSize() + next.x] = true;
				}
			}
			// DOWN
			next = MazePt(curr.x, curr.y - 1);
			if (m_maze->See(next) != Maze::TILE_ERROR && m_maze->See(next) != Maze::TILE_WALL) {
				if (!visited[next.y * m_maze->GetSize() + next.x])
				{
					previous[next.y * m_maze->GetSize() + next.x] = curr;
					queue.push(next);
					//result.push_back(next);
					visited[next.y * m_maze->GetSize() + next.x] = true;
				}
			}
			// LEFT
			next = MazePt(curr.x - 1, curr.y);
			if (m_maze->See(next) != Maze::TILE_ERROR && m_maze->See(next) != Maze::TILE_WALL) {
				if (!visited[next.y * m_maze->GetSize() + next.x])
				{
					previous[next.y * m_maze->GetSize() + next.x] = curr;
					queue.push(next);
					//result.push_back(next);
					visited[next.y * m_maze->GetSize() + next.x] = true;
				}
			}
			// RIGHT
			next = MazePt(curr.x + 1, curr.y);
			if (m_maze->See(next) != Maze::TILE_ERROR && m_maze->See(next) != Maze::TILE_WALL) {
				if (!visited[next.y * m_maze->GetSize() + next.x])
				{
					previous[next.y * m_maze->GetSize() + next.x] = curr;
					queue.push(next);
					//result.push_back(next);
					visited[next.y * m_maze->GetSize() + next.x] = true;
				}
			}
		}
		else
		{
			// UP
			next = MazePt(curr.x, curr.y + 1);
			if (m_maze->See(next) != Maze::TILE_ERROR)
			{
				if (!visited[next.y * m_maze->GetSize() + next.x])
				{
					previous[next.y * m_maze->GetSize() + next.x] = curr;
					queue.push(next);
					result.push_back(next);
					visited[next.y * m_maze->GetSize() + next.x] = true;
				}
			}
			// DOWN
			next = MazePt(curr.x, curr.y - 1);
			if (m_maze->See(next) != Maze::TILE_ERROR) {
				if (!visited[next.y * m_maze->GetSize() + next.x])
				{
					previous[next.y * m_maze->GetSize() + next.x] = curr;
					queue.push(next);
					result.push_back(next);
					visited[next.y * m_maze->GetSize() + next.x] = true;
				}
			}
			// LEFT
			next = MazePt(curr.x - 1, curr.y);
			if (m_maze->See(next) != Maze::TILE_ERROR) {
				if (!visited[next.y * m_maze->GetSize() + next.x])
				{
					previous[next.y * m_maze->GetSize() + next.x] = curr;
					queue.push(next);
					result.push_back(next);
					visited[next.y * m_maze->GetSize() + next.x] = true;
				}
			}
			// RIGHT
			next = MazePt(curr.x + 1, curr.y);
			if (m_maze->See(next) != Maze::TILE_ERROR) {
				if (!visited[next.y * m_maze->GetSize() + next.x])
				{
					previous[next.y * m_maze->GetSize() + next.x] = curr;
					queue.push(next);
					result.push_back(next);
					visited[next.y * m_maze->GetSize() + next.x] = true;
				}
			}
		}
	}
	return result;

}


void SceneTurn::Update(double dt)
{
	const float TURN_TIME = 0.2f;
	SceneBase::Update(dt);
	static bool rStateButton = false;
	if (Application::IsKeyPressed('R') && !rStateButton)
	{
		if (m_mazeKey > 2)
			m_mazeKey = 0;
		SceneData::GetInstance()->setTurn(0);
		SceneData::GetInstance()->setLives(1);
		for (int i = 0; i < visible.size(); ++i)
		{
			visible[i] = false;
		}
		rStateButton = 1;
		for (auto go : m_goList)
		{
			go->active = false;
		}
		TurnManager::GetInstance()->Clear();
		player = FetchGO(GameObject::GO_PLAYER);
		MazePt randStart;
		do {
			randStart.Set(Math::RandIntMinMax(0, m_noGrid - 1), Math::RandIntMinMax(0, m_noGrid - 1));
		} while (m_maze.See(randStart) != Maze::TILE_EMPTY);
		player->curr = randStart;
		player->pos.Set(m_gridOffset, m_gridOffset, 1);
		player->grid.resize(m_noGrid * m_noGrid);
		player->energy = 6;
		player->range = 2;
		player->visited.resize(m_noGrid * m_noGrid);
		std::fill(player->grid.begin(), player->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
		std::fill(player->visited.begin(), player->visited.end(), false);
		player->stack.push_back(player->curr);
		player->grid[player->curr.y * m_noGrid + player->curr.x] = Maze::TILE_EMPTY;

		for (int i = 0; i < 3; ++i)
		{
			GameObject* Enemy = FetchGO(GameObject::GO_MONSTER);
			do {
				randStart.Set(Math::RandIntMinMax(0, m_noGrid - 1), Math::RandIntMinMax(0, m_noGrid - 1));
			} while (m_maze.See(randStart) != Maze::TILE_EMPTY || abs(randStart.x - player->curr.x) + abs(randStart.y - player->curr.y) < 10);
			Enemy->curr = randStart;
			Enemy->pos.Set(m_gridOffset, m_gridOffset, 1);
			Enemy->grid.resize(m_noGrid * m_noGrid);
			Enemy->energy = 4;
			Enemy->range = 2;
			Enemy->visited.resize(m_noGrid * m_noGrid);
			//std::fill(Enemy->grid.begin(), Enemy->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
			std::fill(Enemy->visited.begin(), Enemy->visited.end(), false);
			Enemy->stack.push_back(Enemy->curr);
			Enemy->grid[Enemy->curr.y * m_noGrid + Enemy->curr.x] = Maze::TILE_EMPTY;
		}

		GameObject* Healer = FetchGO(GameObject::GO_HEALER);
		do {
			randStart.Set(Math::RandIntMinMax(0, m_noGrid - 1), Math::RandIntMinMax(0, m_noGrid - 1));
		} while (m_maze.See(randStart) != Maze::TILE_EMPTY || abs(randStart.x - player->curr.x) + abs(randStart.y - player->curr.y) > 4);
		Healer->curr = randStart;
		Healer->pos.Set(m_gridOffset, m_gridOffset, 1);
		Healer->grid.resize(m_noGrid * m_noGrid);
		Healer->energy = 4;
		Healer->range = 2;
		Healer->visited.resize(m_noGrid * m_noGrid);
		//std::fill(Enemy->grid.begin(), Enemy->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
		std::fill(Healer->visited.begin(), Healer->visited.end(), false);
		Healer->stack.push_back(Healer->curr);
		Healer->grid[Healer->curr.y * m_noGrid + Healer->curr.x] = Maze::TILE_EMPTY;

		do {
			randStart.Set(Math::RandIntMinMax(0, m_noGrid - 1), Math::RandIntMinMax(0, m_noGrid - 1));
		} while (m_maze.See(randStart) != Maze::TILE_EMPTY || abs(randStart.x - player->curr.x) + abs(randStart.y - player->curr.y) < 13);
		exit = FetchGO(GameObject::GO_EXIT);
		exit->curr = randStart;
		exit->pos.Set(m_gridOffset, m_gridOffset, 1);
		exit->grid.resize(m_noGrid * m_noGrid);
		exit->energy = 4;
		exit->range = 2;
		exit->visited.resize(m_noGrid * m_noGrid);
		//std::fill(Enemy->grid.begin(), Enemy->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
		std::fill(exit->visited.begin(), exit->visited.end(), false);
		exit->stack.push_back(exit->curr);
		exit->grid[exit->curr.y * m_noGrid + exit->curr.x] = Maze::TILE_EMPTY;


		//Exercise: Implement Reset button

		for (auto go : m_goList)
		{
			if (go->active && (go->type == GameObject::GO_PLAYER || go->type == GameObject::GO_MONSTER || go->type == GameObject::GO_HEALER))
				TurnManager::GetInstance()->Add(go);
		}
	}
	else if (rStateButton && !Application::IsKeyPressed('R'))
	{
		rStateButton = 0;
	}
	if (SceneData::GetInstance()->getLives() == 0 || m_mazeKey > 2)
	{
		for (int i = 0; i < visible.size(); ++i)
		{
			visible[i] = 0;
		}
		for (auto go : m_goList)
		{
			go->active = false;
		}
		TurnManager::GetInstance()->Clear();
	}
	if (player->curr.x == exit->curr.x && player->curr.y == exit->curr.y && player->active)
	{
		if (!player->active)
		{
			return;
		}
		TurnManager::GetInstance()->Clear();
		for (auto go : m_goList)
		{
			go->active = false;
		}
		for (int i = 0; i < visible.size(); ++i)
		{
			visible[i] = false;
		}
		++m_mazeKey;
		m_maze.Generate(m_mazeKey, m_noGrid, m_start);
		m_goList.clear();
		player->active = true;

		player = FetchGO(GameObject::GO_PLAYER);

		MazePt randStart;
		srand(m_mazeKey);
		do {
			randStart.Set(Math::RandIntMinMax(0, m_noGrid - 1), Math::RandIntMinMax(0, m_noGrid - 1));
		} while (m_maze.See(randStart) != Maze::TILE_EMPTY);
		player->curr = randStart;
		player->pos.Set(m_gridOffset, m_gridOffset, 1);
		player->grid.resize(m_noGrid * m_noGrid);
		player->energy = 6;
		player->range = 2;
		player->visited.resize(m_noGrid * m_noGrid);
		std::fill(player->grid.begin(), player->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
		std::fill(player->visited.begin(), player->visited.end(), false);
		player->stack.push_back(player->curr);
		player->grid[player->curr.y * m_noGrid + player->curr.x] = Maze::TILE_EMPTY;
		
		for (int i = 0; i < 3; ++i)
		{
			GameObject* Enemy = FetchGO(GameObject::GO_MONSTER);
			do {
				randStart.Set(Math::RandIntMinMax(0, m_noGrid - 1), Math::RandIntMinMax(0, m_noGrid - 1));
			} while (m_maze.See(randStart) != Maze::TILE_EMPTY || abs(randStart.x - player->curr.x) + abs(randStart.y - player->curr.y) < 10);
			Enemy->curr = randStart;
			Enemy->pos.Set(m_gridOffset, m_gridOffset, 1);
			Enemy->grid.resize(m_noGrid * m_noGrid);
			Enemy->energy = 4;
			Enemy->range = 2;
			Enemy->visited.resize(m_noGrid * m_noGrid);
			//std::fill(Enemy->grid.begin(), Enemy->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
			std::fill(Enemy->visited.begin(), Enemy->visited.end(), false);
			Enemy->stack.push_back(Enemy->curr);
			Enemy->grid[Enemy->curr.y * m_noGrid + Enemy->curr.x] = Maze::TILE_EMPTY;
		}


		do {
			randStart.Set(Math::RandIntMinMax(0, m_noGrid - 1), Math::RandIntMinMax(0, m_noGrid - 1));
		} while (m_maze.See(randStart) != Maze::TILE_EMPTY || abs(randStart.x - player->curr.x) + abs(randStart.y - player->curr.y) < 13);
		exit = FetchGO(GameObject::GO_EXIT);
		exit->curr = randStart;
		exit->pos.Set(m_gridOffset, m_gridOffset, 1);
		exit->grid.resize(m_noGrid * m_noGrid);
		exit->energy = 4;
		exit->range = 2;
		exit->visited.resize(m_noGrid * m_noGrid);
		//std::fill(Enemy->grid.begin(), Enemy->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
		std::fill(exit->visited.begin(), exit->visited.end(), false);
		exit->stack.push_back(exit->curr);
		exit->grid[exit->curr.y * m_noGrid + exit->curr.x] = Maze::TILE_EMPTY;

		GameObject* Healer = FetchGO(GameObject::GO_HEALER);
		do {
			randStart.Set(Math::RandIntMinMax(0, m_noGrid - 1), Math::RandIntMinMax(0, m_noGrid - 1));
		} while (m_maze.See(randStart) != Maze::TILE_EMPTY || abs(randStart.x - player->curr.x) + abs(randStart.y - player->curr.y) > 4);
		Healer->curr = randStart;
		Healer->pos.Set(m_gridOffset, m_gridOffset, 1);
		Healer->grid.resize(m_noGrid * m_noGrid);
		Healer->energy = 4;
		Healer->range = 2;
		Healer->visited.resize(m_noGrid * m_noGrid);
		//std::fill(Enemy->grid.begin(), Enemy->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
		std::fill(Healer->visited.begin(), Healer->visited.end(), false);
		Healer->stack.push_back(Healer->curr);
		Healer->grid[Healer->curr.y * m_noGrid + Healer->curr.x] = Maze::TILE_EMPTY;

		for (auto go : m_goList)
		{
			if (go->active && (go->type == GameObject::GO_PLAYER || go->type == GameObject::GO_MONSTER || go->type == GameObject::GO_HEALER))
				TurnManager::GetInstance()->Add(go);
		}
		
		return;
	}
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	SceneData::GetInstance()->setWorldHeight(m_worldHeight);
	SceneData::GetInstance()->setWorldWidth(m_worldWidth);

	if (!m_MessageQueue.empty())
	{
		Process(m_MessageQueue.front());
		m_MessageQueue.pop();
	}
	for (auto go : m_goList)
	{
		TurnManager::GetInstance()->Update(dt);
		go->canSee.clear();
		if (!go->active){	
			continue;
		}
		if(go->sm)
		go->sm->Update(dt);

		go->maze = &m_maze;
		std::vector<MazePt> temp;
		if (go->type == GameObject::GO_PLAYER || go->type == GameObject::GO_HEALER)
		{
			if(go->type == GameObject::GO_HEALER)
				temp = BFSLimitVisibility(go, INT_MAX, &m_maze, 1, 4);
			else
			temp = BFSLimitVisibility(go, INT_MAX, &m_maze, 1, 4);

			go->canSee = BFSLimitVisibility(go, INT_MAX, &m_maze, 0, go->energy);
		}
		else
		{
			go->canSee = BFSLimitVisibility(go, INT_MAX, &m_maze, 0, go->energy + 5);
		}
		for (int s = 0; s < temp.size(); ++s)
		{
			visible[temp[s].y * m_noGrid + temp[s].x] = true;
		}

		/*for (int i = -1; i < 2; ++i)
		{
			for (int z = -1; z < 2; ++z)
			{
				MazePt temp;

				temp.Set(go->curr.x + i, go->curr.y + z);
				if (m_maze.See(temp) != Maze::TILE_ERROR)
				{
					go->canSee.push_back(temp);
				}
			}
		}*/
	}
	timer += m_speed * dt;
	if (timer > TURN_TIME)
	{
		//m_turn++;
		timer = 0.f;
		// move all entities.
		for (auto go : m_goList)
		{
			if (go->active) {
				//if (!go->stack.empty()) {
				//	//DFSOnce(go);
				//	//go->stack.clear();
				//}
				//else 
				if(!go->path.empty())
				{
					if (go->curr != go->path.front() && go->turn)
					{
					--go->energy;
					}
					go->curr = go->path.front();
					if (go->energy < 0)
					{
						go->energy = 0;
					}
					go->path.erase(go->path.begin());
				}
			}
		}
	}

	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
		SceneData::GetInstance()->setSpeed(Math::Max(0.f, SceneData::GetInstance()->getSpeed() - 0.1f));
	}
	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_speed += 0.1f;
		SceneData::GetInstance()->setSpeed( SceneData::GetInstance()->getSpeed() + 0.1f);
	}
	if (Application::IsKeyPressed('N'))
	{
		for (int i = 0; i < visible.size(); ++i)
		{
			visible[i] = false;
		}
	}
	if (Application::IsKeyPressed('M'))
	{
	}
	if (Application::IsKeyPressed(VK_RETURN))
	{
	}
	

	//Input Section
	static bool bLButtonState = false;
	/*if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		if (posX < m_noGrid * m_gridSize && posY < m_noGrid * m_gridSize)
		{
			m_end.Set((int)(posX / m_gridSize), (int)(posY / m_gridSize));
			for (auto go : m_goList) {
				if(go->active)
				BFSLimit(go,m_end,INT_MAX,&m_maze, 10000);
			}
			std::cout << "Path:";
			for (auto tile : m_shortestPath)
			{
				std::cout << "(" << tile.x << "," << tile.y << ")";
			}
			std::cout << std::endl;
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}*/
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		lightOn = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		lightOn = false;
		std::cout << "RBUTTON UP" << std::endl;
	}
	static bool bSpaceState = false;
	/*if (!bSpaceState && Application::IsKeyPressed(VK_SPACE))
	{
		MazePt randStart;
		do {
			randStart.Set(Math::RandIntMinMax(0, 11), Math::RandIntMinMax(0, 11));
		} while (m_maze.See(randStart) != Maze::TILE_EMPTY);
		GameObject* go = FetchGO(GameObject::GO_PLAYER);
		go->curr = randStart;
		go->pos.Set(m_gridOffset, m_gridOffset, 1);
		go->grid.resize(m_noGrid * m_noGrid);
		go->energy =4;
		go->range = 2;
		go->visited.resize(m_noGrid * m_noGrid);
		std::fill(go->grid.begin(), go->grid.end(), Maze::TILE_CONTENT::TILE_FOG);
		std::fill(go->visited.begin(), go->visited.end(), false);
		
		go->stack.push_back(go->curr);
		go->grid[go->curr.y * m_noGrid + go->curr.x] = Maze::TILE_EMPTY;
		bSpaceState = true;
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
	}*/
}

bool SceneTurn::Handle(Message* message)
{
	m_MessageQueue.push(message);
	return true;
}

void SceneTurn::Process(Message* message)
{
	MessageMove* messageMove = dynamic_cast<MessageMove*>(message);
	if (messageMove)
	{
		if (messageMove->moveType == MessageMove::MOVE_DFSONCE)
		{
			DFSOnce(messageMove->go);
			--messageMove->go->energy;
		}
		else if (messageMove->moveType == MessageMove::MOVE_BFSLIMIT)
		{
			BFSLimit(messageMove->go, messageMove->end, INT_MAX, &m_maze, messageMove->go->energy);
			--messageMove->go->energy;

		}
	}
	MessageWRU* messageWRU = dynamic_cast<MessageWRU*>(message);
	if (messageWRU)
	{
		if (messageWRU->type == MessageWRU::NEAREST_GRID_PLAYER)
		{
			for (int i = 0; i < messageWRU->go->canSee.size(); ++i)
			{
				if (player->curr.x == messageWRU->go->canSee[i].x && player->curr.y == messageWRU->go->canSee[i].y)
				{
					messageWRU->go->nearest = player;
				}
			}
		}
	}
}

void SceneTurn::DFSOnce(GameObject* go)
{
	srand(time(NULL));
	int minusNum;
	bool directions; // true == x
	int checkBounds;
	int randIndex = -1;
	std::vector<MazePt> neighbours;
	MazePt next;
	go->stack.push_back(go->curr);
	go->visited[go->curr.y * SceneData::GetInstance()->getNoGrid() + go->curr.x] = true;
	for (int i = 0; i < 2; ++i)
	{
		for (int z = -1; z < 2; ++z)
		{
			if (i == 0)
			{
				if (z != 0)
				{
					next.Set(go->curr.x + z, go->curr.y); // push visitable neighbbours
					if (next != go->curr && m_maze.See(next) == Maze::TILE_EMPTY  )
					{
						if (go->visited[next.y * m_noGrid + next.x] == false)
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
					next.Set(go->curr.x, go->curr.y + z); // push visitable neighbbours
					if (next != go->curr && m_maze.See(next) == Maze::TILE_EMPTY)
					{
						if (go->visited[next.y * m_noGrid + next.x] == false)
						{
							neighbours.push_back(next);
						}
					}
				}
			}
		}
	}
	if (neighbours.size() > 0)
	{
		randIndex = Math::RandInt() % neighbours.size();
		go->curr = neighbours[randIndex];
		neighbours.clear();
		return;
	}
	// UP
	//for (int i = 0; i < 4; ++i)
	//{
	//	if (i < 2)
	//	{
	//		minusNum = 1;
	//	}
	//	else
	//	{
	//		minusNum = -1;
	//	}
	//	if ((i + 1) % 2 == 0)
	//	{
	//		directions = 1;
	//	}
	//	else
	//	{
	//		directions = 0;
	//	}
	//	// bounds check the next instead of curr
	//	if (directions)
	//	{
	//		next.Set(go->curr.x + minusNum, go->curr.y);
	//	}
	//	else
	//	{
	//		next.Set(go->curr.x, go->curr.y + minusNum);
	//	}
	//	next.x = Math::Clamp(next.x, 0, SceneData::GetInstance()->getNoGrid() - 1);
	//	next.y = Math::Clamp(next.y, 0, SceneData::GetInstance()->getNoGrid() - 1);
	//	if (!go->visited[next.y * SceneData::GetInstance()->getNoGrid() + next.x])
	//	{
	//		go->grid[next.y * SceneData::GetInstance()->getNoGrid() + next.x] = go->maze->See(next);
	//		if (go->grid[next.y * SceneData::GetInstance()->getNoGrid() + next.x] == Maze::TILE_EMPTY)
	//		{
	//			go->curr = next;
	//			return;
	//		}
	//	}

	// }

	//if (go->curr.y < m_noGrid - 1) // bounds check
	//{
	//    next.Set(go->curr.x, go->curr.y + 1);
	//    if (!go->visited[next.y * m_noGrid + next.x])
	//    {
	//        go->grid[next.y * m_noGrid + next.x] = m_maze.See(next);
	//        if (go->grid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
	//        {
	//            go->curr = next;
	//            return;
	//        }
	//    }
	//}
	//// DOWN
	//if (go->curr.y > 0) {
	//    next.Set(go->curr.x, go->curr.y - 1);
	//    if (!go->visited[next.y * m_noGrid + next.x])
	//    {
	//        go->grid[next.y * m_noGrid + next.x] = m_maze.See(next);
	//        if (go->grid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
	//        {
	//            go->curr = next;
	//            return;
	//        }
	//    }
	//}
	//// LEFT
	//if (go->curr.x > 0)
	//{
	//    next.Set(go->curr.x - 1, go->curr.y);
	//    if (!go->visited[next.y * m_noGrid + next.x])
	//    {
	//        go->grid[next.y * m_noGrid + next.x] = m_maze.See(next);
	//        if (go->grid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
	//        {
	//            go->curr = next;
	//            return;
	//        }
	//    }
	//}
	//// RIGHT
	//if (go->curr.x < m_noGrid - 1)
	//{
	//    next.Set(go->curr.x + 1, go->curr.y);
	//    if (!go->visited[next.y * m_noGrid + next.x])
	//    {
	//        go->grid[next.y * m_noGrid + next.x] = m_maze.See(next);
	//        if (go->grid[next.y * m_noGrid + next.x] == Maze::TILE_EMPTY)
	//        {
	//            go->curr = next;
	//            return;
	//        }
	//    }
	//}
	

	go->stack.pop_back();
	if (!go->stack.empty())
	{
		go->curr = go->stack.back();
		go->stack.pop_back();
		return;
	}
	// DFS done
}

bool SceneTurn::BFSLimit(GameObject* go, MazePt end, int limit, Maze* m_maze, int moveLimit)
{
	if (moveLimit == 0)
		return false;

	int loop = 0;
	int nearestDistance = INT_MAX;
	MazePt nearest = go->curr;
	MazePt curr;
	MazePt next;
	go->path.clear();
	std::vector<MazePt> previous;
	std::vector<bool> visited;
	previous.resize(m_maze->GetSize() * m_maze->GetSize());
	visited.resize(m_maze->GetSize() * m_maze->GetSize());
	std::fill(visited.begin(), visited.end(), false);
	std::queue<MazePt> queue;
	queue.push(go->curr);
	while (!queue.empty() && loop <= limit)
	{
		++loop;
		curr = queue.front();
		queue.pop();
		int distance = abs(end.x - curr.x) + abs(end.y - curr.y);

		if (abs(curr.x - go->curr.x) + abs(curr.y - go->curr.y) > moveLimit)
		{
			break;
		}
		if (distance <= nearestDistance)
		{
			nearestDistance = distance;
			nearest = curr;
		}
		if (curr == end)
		{
			//Shortest path to end
			while (curr != go->curr)
			{
				go->path.insert(go->path.begin(), curr);
				curr = previous[curr.y * m_maze->GetSize() + curr.x];
			}
			go->path.insert(go->path.begin(), curr);
			return true;
		}
		// UP
		next = MazePt(curr.x, curr.y + 1);
		if (m_maze->See(next) == Maze::TILE_EMPTY)
		{
			if (!visited[next.y * m_maze->GetSize() + next.x])
			{
				previous[next.y * m_maze->GetSize() + next.x] = curr;
				queue.push(next);
				visited[next.y * m_maze->GetSize() + next.x] = true;
			}
		}
		// DOWN
		next = MazePt(curr.x, curr.y - 1);
		if (m_maze->See(next) == Maze::TILE_EMPTY) {
			if (!visited[next.y * m_maze->GetSize() + next.x])
			{
				previous[next.y * m_maze->GetSize() + next.x] = curr;
				queue.push(next);
				visited[next.y * m_maze->GetSize() + next.x] = true;
			}
		}
		// LEFT
		next = MazePt(curr.x - 1, curr.y);
		if (m_maze->See(next) == Maze::TILE_EMPTY) {
			if (!visited[next.y * m_maze->GetSize() + next.x])
			{
				previous[next.y * m_maze->GetSize() + next.x] = curr;
				queue.push(next);
				visited[next.y * m_maze->GetSize() + next.x] = true;
			}
		}
		// RIGHT
		next = MazePt(curr.x + 1, curr.y);
		if (m_maze->See(next) == Maze::TILE_EMPTY) {
			if (!visited[next.y * m_maze->GetSize() + next.x])
			{
				previous[next.y * m_maze->GetSize() + next.x] = curr;
				queue.push(next);
				visited[next.y * m_maze->GetSize() + next.x] = true;
			}
		}
	}
	curr = nearest;
	while (curr != go->curr)
	{
		go->path.insert(go->path.begin(), curr);
		curr = previous[curr.y * m_maze->GetSize() + curr.x];
	}
	go->path.insert(go->path.begin(), curr);
	return false;

}



void SceneTurn::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_NONE: //Render GO_PLAYER
		break;
	case GameObject::GO_PLAYER:
		modelStack.PushMatrix();
		modelStack.Translate(m_gridOffset + m_gridSize * go->curr.x, m_gridOffset + m_gridSize * go->curr.y, 2);
		modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
		RenderMesh(meshList[GEO_QUEEN], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_MONSTER:
		modelStack.PushMatrix();
		modelStack.Translate(m_gridOffset + m_gridSize * go->curr.x, m_gridOffset + m_gridSize * go->curr.y, 2);
		modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
		RenderMesh(meshList[GEO_MONSTER], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_EXIT:
		modelStack.PushMatrix();
		modelStack.Translate(m_gridOffset + m_gridSize * go->curr.x, m_gridOffset + m_gridSize * go->curr.y, 2);
		modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
		RenderMesh(meshList[GEO_YELLOWQUAD], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_HEALER:
		modelStack.PushMatrix();
		modelStack.Translate(m_gridOffset + m_gridSize * go->curr.x, m_gridOffset + m_gridSize * go->curr.y, 2);
		modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
		RenderMesh(meshList[GEO_HEALER], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneTurn::Render()
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
	RenderMesh(meshList[GEO_WHITEQUAD], false);

	modelStack.PopMatrix();


	for (auto go : m_goList)
	{
		if (go->active)
		{
			for (int i = 0; i < go->canSee.size(); ++i)
			{
				if (!visible[go->canSee[i].y * m_noGrid + go->canSee[i].x] && !lightOn)
					continue;

				modelStack.PushMatrix();
				modelStack.Translate(m_gridOffset + m_gridSize * go->canSee[i].x, m_gridOffset + m_gridSize * go->canSee[i].y, 1); // render fog only when not in player range
				modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
				if (go->energy == 0 && go->canSee[i].x == go->curr.x && go->canSee[i].y == go->curr.y)
				{
					RenderMesh(meshList[GEO_WHITEQUAD], false);
				}
				else if (go->turn && go->canSee[i].x == go->curr.x && go->canSee[i].y == go->curr.y)
				{
					RenderMesh(meshList[GEO_BLUEQUAD], false);
				}
				else if (go->turn)
				{
					if (go->type == GameObject::GO_PLAYER || go->type == GameObject::GO_HEALER)
						RenderMesh(meshList[GEO_GREENQUAD], false);
					else
						RenderMesh(meshList[GEO_REDQUAD], false);

				}
				modelStack.PopMatrix();
			}
			if (visible[go->curr.y * m_noGrid + go->curr.x] || lightOn)
			{
				RenderGO(go);
			}
		}
	}

	//Render tiles 
	for (int row = 0; row < m_noGrid; ++row)
	{
		for (int col = 0; col < m_noGrid; ++col)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_gridOffset + m_gridSize * col, m_gridOffset + m_gridSize * row, 0); // render fog only when not in player range
			modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
			if (lightOn || visible[row * m_noGrid + col])
			{
				switch (m_maze.m_grid[row * m_noGrid + col])
				{
				case Maze::TILE_WALL:
					RenderMesh(meshList[GEO_GREYQUAD], false);
					break;
				case Maze::TILE_FOG:
					RenderMesh(meshList[GEO_BLACKQUAD], false);
					break;
				case Maze::TILE_EMPTY:
					RenderMesh(meshList[GEO_WHITEQUAD], false);
					break;
				}
			}
			else {
				RenderMesh(meshList[GEO_BLACKQUAD], false);
			}

			modelStack.PopMatrix();
		}
	}


	//Render curr point
	/*MazePt curr = m_maze.GetCurr();
	modelStack.PushMatrix();
	modelStack.Translate(m_gridOffset + m_gridSize * curr.x, m_gridOffset + m_gridSize * curr.y, 1);
	modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
	RenderMesh(meshList[GEO_QUEEN], false);
	modelStack.PopMatrix();*/
	//Render shortest path
	/*for (auto tile : m_shortestPath)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_gridOffset + m_gridSize * tile.x, m_gridOffset + m_gridSize * tile.y, 1);
		modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
		RenderMesh(meshList[GEO_QUEEN], false);
		modelStack.PopMatrix();
	}*/
	//Render m_goList
	for (auto go : m_goList)
	{
		if (!go->active)
			continue;

		if (go->type == GameObject::GO_PLAYER && go->turn)
		{
			std::ostringstream ss;
			ss.precision(3);
			ss << "Player's turn.";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 45);

		}
		else if (go->type == GameObject::GO_HEALER && go->turn)
		{
			std::ostringstream ss;
			ss.precision(3);
			ss << "Allies's turn.";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 45);

		}
		else if (go->type == GameObject::GO_MONSTER && go->turn)
		{
			std::ostringstream ss;
			ss.precision(3);
			ss << "Enemy's turn.";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 50, 45);

		}
	}
	//On screen text
	std::ostringstream ss;

	ss.str("");
	ss.precision(3);
	ss << "Lives:" << SceneData::GetInstance()->getLives();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 37);

	ss.str("");
	ss.precision(3);
	ss << "Speed:" << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 6);

	ss.str("");
	ss.precision(5);
	ss << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 3);

	ss.str("");
	ss << "Turn:" << SceneData::GetInstance()->getTurn();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	ss.str("");
	ss << "Turn Maze " << m_mazeKey;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 0);
	if (SceneData::GetInstance()->getLives() <= 0)
	{
		ss.str("");
		ss << "YOU BEEN CAUGHT!!";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 6, 0, 25);

		ss.str("");
		ss << "Press 'R' to restart.";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 8, 22);
	}
	else if (m_mazeKey > 2)
	{
		ss.str("");
		ss << "YOU WIN!!!";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 6, 7, 25);

		ss.str("");
		ss << "Press 'R' to restart.";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 8, 22);
	}
}

void SceneTurn::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}
