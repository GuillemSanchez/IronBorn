#include "Entity_coin.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2Log.h"


Entity_coin::Entity_coin(ENTITY_TYPE type, p2Point<int> pos, int index) : Entity(type, pos, index)
{
	my_type = type;
	position = pos;
	my_index = index;
}

Entity_coin::~Entity_coin()
{
}

bool Entity_coin::Awake(pugi::xml_node & config)
{
	pugi::xml_node coin_node = config.child("entity_coin").child("animations");

	LoadAllAnimations(coin_node);
	return true;
}

void Entity_coin::Draw(float dt)
{
	current = &IDLE;
	App->render->Blit(coin_idle, position.x, position.y, &current->GetCurrentFrame(dt), 1);
}

bool Entity_coin::Start()
{
	coin_idle = App->tex->Load("Assets/MonedaD.png");
	return true;
}

bool Entity_coin::Update(float dt)
{
	//Draw(dt);
	return true;
}

bool Entity_coin::CleanUp()
{
	return true;
}

void Entity_coin::OnCollision(Collider * coin, Collider * colli)
{
}

bool Entity_coin::LoadAnimation(pugi::xml_node & config, Animation & anim)
{

	for (; config; config = config.next_sibling("frame")) {
		SDL_Rect frame_rect;
		frame_rect.x = config.attribute("x").as_int();
		frame_rect.y = config.attribute("y").as_int();
		frame_rect.w = config.attribute("width").as_int();
		frame_rect.h = config.attribute("height").as_int();
		anim.PushBack(frame_rect);
	};
	
	return true;
}

bool Entity_coin::LoadAllAnimations(pugi::xml_node & anima)
{
	LoadAnimation(anima.child("idle").child("frame"),IDLE);
	IDLE.loop = anima.child("idle").attribute("loop").as_bool();
	IDLE.speed = anima.child("idle").attribute("speed").as_float();

	
	return false;
}
