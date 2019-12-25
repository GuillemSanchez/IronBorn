#include "j1EntityManager.h"
#include "Entity_coin.h"

j1EntityManager::j1EntityManager()
{
	name.create("manager");
}

j1EntityManager::~j1EntityManager()
{
}

void j1EntityManager::Init()
{

}

bool j1EntityManager::Awake(pugi::xml_node & node)
{
	// First we wanna create the entities, then we will awake them.
	p2Point<int> player_pos;
	
	pugi::xml_node player = node.child("entity_player");
	player_pos.x = player.child("initial_pos").attribute("x").as_int();
	player_pos.y = player.child("initial_pos").attribute("y").as_int();

	CreateEntity(PLAYER, player_pos);
	CreateEntity(COIN,	iPoint(30,30));

	//Here we awake the entities.
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

void j1EntityManager::OnCollision(Collider * coll1, Collider * coll2)
{
	if (coll1->type == COLLIDER_PLAYER)
		my_player->OnCollision(coll1, coll2);

}

void j1EntityManager::CreateEntity(ENTITY_TYPE type, p2Point<int> pos)
{
	Entity* Ent = nullptr;

	int current_index = Entities.count(); //We will have index to control ours entities

	switch (type)
	{
	case PLAYER:
		Ent = new Entity_Player(type, pos, current_index);
		Ent->active = true;
		break;
	case NONE:

		break;
	case COIN:
		Ent = new Entity_coin(type, pos, current_index);
		Ent->active = true;
		break;
	default:

		break;
	}

	// Here we look if the entity have info or not
	if (Ent != nullptr)
		Entities.add(Ent);


}

void j1EntityManager::DestroyEntity(int index)
{
}

void j1EntityManager::DrawEnts(float dt)
{
	for (int i = 0; i < Entities.count(); i++)
	{
		if (Entities[i]->active)
		{
			if (Entities[i]->my_type != PLAYER)
				Entities[i]->Draw(dt);
		}
			
	}
}
