#ifndef __ENTITY_COIN_H__
#define __ENTITY_COIN_H__

#include "j1App.h"
#include "Entity.h"
#include "p2Point.h"
#include "Animation.h"


struct SDL_Texture;
struct Collider;

class Entity_coin : public Entity
{ 
public:
	Entity_coin(ENTITY_TYPE type, p2Point<int> pos, int index);
	~Entity_coin();

	// We dont need a lot of thinks to make the coin, we only wanna see it and we only wanna be able to collide with it.
	// Also the mechanic with coins is that we will never delete completly them, we will make them disapear and then when we swap maps we will recreate them.
	bool Awake(pugi::xml_node& config);

	void Draw(float dt);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(Collider* coin, Collider* colli);
	

public:

	bool LoadAnimation(pugi::xml_node& config, Animation& anim);

	bool LoadAllAnimations(pugi::xml_node& anima);


	Animation *current = nullptr;
	Animation IDLE;
	SDL_Texture* coin_idle = nullptr;
	Collider* coin_collider;
	p2SString coin_sound;

	uint coin_clinck;

	p2Point<int> position;
	p2Point<int> coin_w_h;
	p2Point<int> oculted_pos;
	
	bool collected_1 = false;
	bool collected_2 = false;
};






#endif // !__ENTIYU_COIN_H__

