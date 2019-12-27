#ifndef __J1ENTITY_MANAGER__
#define __J1ENTITY_MANAGER__

#include "j1Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "Entity.h"
#include "Entity_player.h"
#include "Entity_coin.h"
#include "j1Collisions.h"
#include "j1Scene.h"


class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();


	// Module typical functions --------------------------------------

	void Init();
	
	// Called before render is available
	bool Awake(pugi::xml_node&);
	
	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void OnCollision(Collider*, Collider*);

	bool Save(pugi::xml_node&node) const;

	bool Load(pugi::xml_node&node);

	// Module typical functions --------------------------------------

	// Specific functions --------------------------------------------

	void CreateEntity(ENTITY_TYPE type, p2Point<int> pos);

	void DestroyEntity(int index);

	void DrawEnts(float dt);

	Entity_coin* SearchTheCoin(iPoint pos);

	// Specific functions --------------------------------------------

	void Createlvl(LVL current);

	Entity_Player* my_player = nullptr;

	void InactiveAll();

	void SetCoinPos(int i, p2Point<int> pos);

	void StartAllCoins();
	void Start_2_all_coins();

	SDL_Texture* Coin_Texture;

private:

	p2List<Entity*> Entities;   //List of entities.
	p2List<Entity_coin*> Coins;
	p2Point<int> player_pos;
	
};

#endif
