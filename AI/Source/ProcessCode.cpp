else if (messageWRU->type == MessageWRU::NEAREST_ENEMY)
				{

					if (go->type == GameObject::GO_BASE2 || go->type == GameObject::GO_ARCHER2 || go->type == GameObject::GO_SOLDIER2 || go->type == GameObject::GO_TANK2 || go->type == GameObject::GO_TOWER || go->type == GameObject::GO_TOWER2 || go->type == GameObject::GO_ETOWER || go->type == GameObject::GO_ETOWER2)
					{
						float distancesquared = (go->pos - messageWRU->go->pos).LengthSquared();
						//if (distancesquared < INT_MAX)
						//float RANGE = 5 * SceneData::GetInstance()->getGridSize();

						{
							if (distancesquared <= messageWRU->threshold * messageWRU->threshold && go->Health > 0 && messageWRU->go->type != GameObject::GO_TANK && go->type != GameObject::GO_TOWER && go->type != GameObject::GO_TOWER2)
							{
								messageWRU->go->nearest = go;
							}
							else if (messageWRU->go->reached == 0 && messageWRU->go->t1 && messageWRU->go->type != GameObject::GO_TOWER && messageWRU->go->type != GameObject::GO_TOWER2 && messageWRU->go->type != GameObject::GO_BASE)
							{
								if (go->type == GameObject::GO_TOWER)
									//qqqqqqqqstd::cout << "It worked";
								if (go == BTower01)
									messageWRU->go->nearest = go;
							}
							else if (messageWRU->go->reached == 0 && messageWRU->go->t2 && messageWRU->go->type != GameObject::GO_TOWER && messageWRU->go->type != GameObject::GO_TOWER2 && messageWRU->go->type != GameObject::GO_BASE)
							{
								if (go == BTower02)
									messageWRU->go->nearest = go;
							}
							else if (go->type == GameObject::GO_ETOWER && go->Health >= 0 && messageWRU->go->t1 && messageWRU->go->type != GameObject::GO_BASE && messageWRU->go->type != GameObject::GO_TOWER && messageWRU->go->type != GameObject::GO_TOWER2 && go->active && go->Health > 0 && messageWRU->go->reached)
							{
								messageWRU->go->nearest = go;
							}
							else if (go->type == GameObject::GO_ETOWER2 && go->Health >= 0 && messageWRU->go->t2 && messageWRU->go->type != GameObject::GO_BASE && messageWRU->go->type != GameObject::GO_TOWER && messageWRU->go->type != GameObject::GO_TOWER2 && go->active && go->Health > 0 && messageWRU->go->reached)
							{
								messageWRU->go->nearest = go;
							}
							else if (go->type == GameObject::GO_BASE2 && messageWRU->go->type != GameObject::GO_BASE && messageWRU->go->type != GameObject::GO_TOWER && messageWRU->go->type != GameObject::GO_TOWER2 && go->active && go->Health > 0 )
							{
								messageWRU->go->nearest = go;
							}
						}
					}
				}