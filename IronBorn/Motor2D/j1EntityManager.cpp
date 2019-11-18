#include "j1EntityManager.h"

j1EntityManager::j1EntityManager()
{
	name.create("Entity Manager");
}

j1EntityManager::~j1EntityManager()
{
}

void j1EntityManager::Init()
{

}

bool j1EntityManager::Awake(pugi::xml_node & node)
{


	for (int i = 0; i < Entities.count(); i++)
	{
		if (Entities[i]->active)
			Entities[i]->Awake(node);
	}
	return true;
}

bool j1EntityManager::Start()
{
	for (int i = 0; i < Entities.count(); i++)
	{
		if (Entities[i]->active)
			Entities[i]->Start();
	}
	return true;
}

bool j1EntityManager::PreUpdate()
{
	for (int i = 0; i < Entities.count(); i++)
	{
		if (Entities[i]->active)
			Entities[i]->PreUpdate();
	}
	return true;
}

bool j1EntityManager::Update(float dt)
{
	for (int i = 0; i < Entities.count(); i++)
	{
		if (Entities[i]->active)
			Entities[i]->Update(dt);
	}
	return true;
}

bool j1EntityManager::PostUpdate()
{
	for (int i = 0; i < Entities.count(); i++)
	{
		if (Entities[i]->active)
			Entities[i]->PostUpdate();
	}
	return true;
}

bool j1EntityManager::CleanUp()
{
	for (int i = 0; i < Entities.count(); i++)
	{
		if (Entities[i]->active)
			Entities[i]->CleanUp();
	}
	return true;
}

void j1EntityManager::CreateEntity(ENTITY_TYPE type, p2Point<int> pos)
{
	Entity* Ent = nullptr;

	int current_index = Entities.count(); //We will have index to control ours entities

	switch (type)
	{
	case PLAYER:
		Ent = new Entity_Player(type, pos, current_index);
		break;
	case NONE:

		break;
	default:

		break;
	}

}

void j1EntityManager::DestroyEntity(int index)
{
}
