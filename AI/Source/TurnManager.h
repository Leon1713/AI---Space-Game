#ifndef TURN_MAN_H
#define TURN_MAN_H

#include "SingletonTemplate.h"

#include <string>
#include <vector>
#include "GameObject.h"

class TurnManager : public Singleton<TurnManager>
{
	friend Singleton<TurnManager>;

public:
	void Add(GameObject* go);
	void remove(GameObject* go);
	int getTurn(GameObject* go);
	void Clear();
	void Update(double dt);

private:
	~TurnManager();
	TurnManager();
	std::vector<GameObject*> turnList;
};

#endif
