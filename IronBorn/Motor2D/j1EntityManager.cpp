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

	
	pugi::xml_node player = node.child("entity_player");
	player_pos.x = player.child("initial_pos").attribute("x").as_int();
	player_pos.y = player.child("initial_pos").attribute("y").as_int();

	CreateEntity(PLAYER, player_pos);
	CreateEntity(COIN,	iPoint(80,30));

	//Here we awake the entities.
	for (int i = 0; i < Entities.count(); i++)
	{
		Entities[i]->Awake(node);
	}
	return true;
}

bool j1EntityManager::Start()
{
	for (int i = 0; i < Entities.count(); i++)
	{
		
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

	
	if (coll1->type == COLLIDER_COIN)
	{
		iPoint help;
		help.x = coll1->rect.x;
		help.y = coll1->rect.y;
		Entity_coin* helper = SearchTheCoin(help);

		helper->OnCollision(coll1, coll2);
	}
		
}

void j1EntityManager::CreateEntity(ENTITY_TYPE type, p2Point<int> pos)
{
	

	int current_index = Entities.count(); //We will have index to control ours entities
	Entity* Ent = nullptr;
	Entity_coin* ent = nullptr;
	switch (type)
	{
	case PLAYER:
		
		Ent = new Entity_Player(type, pos, current_index);
		Ent->active = false;
		Entities.add(Ent);
		break;
	case NONE:

		break;
	case COIN:
	
		ent = new Entity_coin(type, pos, current_index);
		ent->active = false;
		Entities.add(ent);
		Coins.add(ent);
		break;
	default:

		break;
	}
		


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

Entity_coin * j1EntityManager::SearchTheCoin(iPoint pos)
{
	for (int i = 0; i < Coins.count(); i++)
	{
		if (Coins[i]->position == pos)
			return Coins[i];
	}
	return nullptr;
}

void j1EntityManager::Createlvl(LVL current)
{
	my_player->SetPos(player_pos);
	my_player->active = true;

	for (int i = 0; i < Coins.count(); i++)
	{
		Coins[i]->active = true;
	}
}

void j1EntityManager::InactiveAll()
{
	for (int i = 0; i < Entities.count(); i++)
	{
		Entities[i]->CleanUp();
		Entities[i]->active = false;
		Entities[i]->Start();
	}
}
