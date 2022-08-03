#include "SceneQueen.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneQueen::SceneQueen()
{
}

SceneQueen::~SceneQueen()
{
}

void SceneQueen::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_noGrid = 8;
	m_gridSize = m_worldHeight / m_noGrid;
	m_gridOffset = m_gridSize / 2;
	m_objectCount = 0;
	numDiagonal = (m_noGrid + m_noGrid) - 1;
	midPoint = (numDiagonal / 2) + 1;
	m_queen.resize(8,0); // store position of queen for each rows.
	m_numSolutions = 0;
	DFS(1);
	std::cout << "Num solutions: " << m_numSolutions << std::endl;

	m_speed = 1.f;

	Math::InitRNG();

}

void SceneQueen::Update(double dt)
{
	SceneBase::Update(dt);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_speed += 0.1f;
	}
	if (Application::IsKeyPressed('R'))
	{
		//Exercise: Implement Reset button
	}

	//Input Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}
	static bool bSpaceState = false;
	if (!bSpaceState && Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = true;
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
	}
	
}

void SceneQueen::Render()
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
	RenderMesh(meshList[GEO_CHESSBOARD], false);
	modelStack.PopMatrix();

	//Print queens
	for (int i = 0; i < m_noGrid; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate((m_queen[i] * m_gridSize) - m_gridOffset, ((i + 1) * m_gridSize) - m_gridOffset, 0);
		modelStack.Scale(m_gridSize, m_gridSize, m_gridSize);
		if(m_queen[i] != 0)
		RenderMesh(meshList[GEO_QUEEN], false);
		modelStack.PopMatrix();

	}

	//On screen text
	std::ostringstream ss;
	//ss << "Something" << something;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 6);

	ss.str("");
	//ss << "Something" << something;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 3);

	ss.str("");
	//ss << "Something" << something;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	RenderTextOnScreen(meshList[GEO_TEXT], "8 Queens", Color(0, 1, 0), 3, 50, 0);
}

void SceneQueen::Exit()
{
	SceneBase::Exit();
}

void SceneQueen::PrintSolution()
{
	std::cout << "Solution: ";
	for (int i = 0; i < m_queen.size(); ++i)
	{
		if (i != m_queen.size() - 1)
		{
			std::cout << m_queen[i] << ", ";
		}
		else
		{
			std::cout << m_queen[i] << '\n';
		}
	}
}

bool SceneQueen::CheckQueen()
{
	 // one diagonal (top left -> bot right)
	int rowIndex;
	bool setTrue;
	int colIndex;
	int zCount = 0;
	int items = 0;
	int check = 0;
	bool isCleared = false;
	// check diagonals
	for (int i = 1; i <= numDiagonal; ++i)
	{
		std::vector<int> Storage;
		if (i <= midPoint)
		{
			items++;
			for (int item = 0; item < items; ++item)
			{
				rowIndex = (i - item) - 1; 
				colIndex = item;
				if (m_queen[rowIndex] == colIndex + 1)
				{
					Storage.push_back(m_queen[rowIndex]);
				}
				
			}
		}
		else
		{
			--items;
			for (int j = 0; j < items; ++j)
			{
				rowIndex = (m_noGrid - 1)-j;
				colIndex = (i - m_noGrid) + j;
				if(m_queen[rowIndex] == colIndex + 1)
				Storage.push_back(m_queen[rowIndex]);
			}
		}

		if (Storage.size() > 1)
		{
			return false;
		}
		else
			setTrue = true;
	}





	// **********************************
		items = 0;


	for (int i = 1; i <= numDiagonal; ++i)
	{
		std::vector<int> Storage;
		if (i <= midPoint)
		{
			items++;
			for (int item = 0; item < items; ++item)
			{
				rowIndex = (i - item) - 1;
				colIndex = 7 - item;
				if (m_queen[rowIndex] == colIndex + 1)
				{
					Storage.push_back(m_queen[rowIndex]);
				}

			}
		}
		else
		{
			--items;
			for (int j = 0; j < items; ++j)
			{
				rowIndex = (m_noGrid - 1) - j;
				colIndex = 7 - ((i - m_noGrid) + j);
				if(m_queen[rowIndex] == colIndex + 1)
				Storage.push_back(m_queen[rowIndex]);
			}
		}

		for (int z = 0; z < Storage.size(); ++z)
		{
			if (Storage[z] > 0)
			{
				
				if (Storage.size() > 1)
					return false;
				else
					setTrue = true;
				
			}
		}
	}

	items = 0;

	for (int col = 0; col < m_noGrid; ++col) // loop through collums imaginary lines
	{
		
		std::vector<int> Storage; // store vertical queens
		int itemsToCheck = m_noGrid; // 8 
		for (int row = 0; row < m_noGrid; ++row) // loop through rows
		{
			if (m_queen[row] == col + 1) // check whether they line up
			{
				Storage.push_back(m_queen[row]); // store
			}
		}
		if (Storage.size() > 1)// if there more than 1 queen horizontally line up return false
			return false;
		else
			setTrue = true;
	}
	
	
	return setTrue; // if no queen line up return true
}

void SceneQueen::DFS(int row)
{
		for (int col = 0; col < m_noGrid; ++col)
		{
			m_queen[row - 1] = col + 1;
			if (CheckQueen())
			{
				Application::GetInstance().iterate();
				if (row - 1 != m_noGrid - 1)
				{
					DFS(row + 1);
					m_queen[row] = 0;
				}
				else
				{
					++m_numSolutions;
					PrintSolution();
					Sleep(1000);
				}
			}
			else
			{
				m_queen[row - 1] = 0;
			}
		}
		
		return;
	
}
