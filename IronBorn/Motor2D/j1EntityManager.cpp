#include "j1EntityManager.h"
#include "Entity_coin.h"
#include "j1Textures.h"
#include "Entity.h"
#include "p2List.h"
#include "p2Defs.h"


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
	CreateEntity(COIN, iPoint(80, 30));
	CreateEntity(COIN, iPoint(90, 30));
	CreateEntity(COIN, iPoint(100, 30));
	CreateEntity(COIN, iPoint(120, 30));
	CreateEntity(COIN, iPoint(130, 30));
	CreateEntity(COIN, iPoint(140, 30));
	CreateEntity(COIN, iPoint(150, 30));
	CreateEntity(COIN, iPoint(160, 30));
	CreateEntity(COIN, iPoint(170, 30));
	CreateEntity(COIN, iPoint(180, 30));


	//Here we awake the entities.
	for (int i = 0; i < Entities.count(); i++)
	{
		Entities[i]->Awake(node);
	}
	return true;
}

bool j1EntityManager::Start()
{
	Coin_Texture = App->tex->Load("Assets/MonedaD.png");
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

		if (helper->active)
		{
			helper->OnCollision(coll1, coll2);
		}
		
	}
		
}

bool j1EntityManager::Save(pugi::xml_node &node) const
{
	bool ret;
	if (Entities.count() != 0)
	{
		for (int i = 0; i < Entities.count(); i++)
		{
			if (Entities[i] != nullptr)
				ret = Entities[i]->Save(node);
		}
	}
	
	return true;
}

bool j1EntityManager::Load(pugi::xml_node &node)
{
	//for (int i = 0; i < Entities.count(); i++)
	//{
	//	Entities[i]->Load(node);
	//}
	return true;
}

void j1EntityManager::CreateEntity(ENTITY_TYPE type, p2Point<int> pos)
{
	

	int current_index = Entities.count(); //We will have index to control ours entities
	int coins_index = Coins.count();
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
	
		ent = new Entity_coin(type, pos, coins_index);
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

	/*for (int i = 0; i < Coins.count(); i++)
	{
		Coins[i]->active = true;
	}*/
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

void j1EntityManager::SetCoinPos(int i, p2Point<int> pos)
{
	if (i < Coins.count())
	{
		if (Coins[i]->active)
			Coins[i]->position = pos;
	}
}

void j1EntityManager::StartAllCoins()
{

	for (int i = 0; i < Coins.count(); i++)
	{
		
		Coins[i]->CleanUp();
		if ((Coins[i]->collected_1 == true && App->scene->current == LVL_1) || (Coins[i]->collected_2 == true && App->scene->current == LVL_2))
		{
			Coins[i]->active = false;
		}
		else
		{
			Coins[i]->active = true;
		}
	}
}

void j1EntityManager::Start_2_all_coins()
{
	for (int i = 0; i < Coins.count(); i++)
	{
		Coins[i]->Start();
	}
}
