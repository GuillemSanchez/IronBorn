#include "Entity_coin.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Collisions.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Console.h"



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

	pugi::xml_node coin = config.child("entity_coin");

	coin_w_h.x = coin.child("coin_w_h").attribute("w").as_int();
	coin_w_h.y = coin.child("coin_w_h").attribute("h").as_int();
	oculted_pos.x = coin.child("oculted_pos").attribute("x").as_int();
	oculted_pos.y = coin.child("oculted_pos").attribute("y").as_int();
	coin_sound = coin.child("fx_collect").attribute("value").as_string();

	pugi::xml_node coin_node = config.child("entity_coin").child("animations");

	LoadAllAnimations(coin_node);
	return true;
}

void Entity_coin::Draw(float dt)
{
	current = &IDLE;
	App->render->Blit(coin_idle, position.x, position.y, &current->GetCurrentFrame(dt), 1);
	coin_collider->SetPos(position.x, position.y);
}

bool Entity_coin::Start()
{
	coin_idle = App->manager->Coin_Texture;
	coin_clinck = App->audio->LoadFx(coin_sound.GetString());

	SDL_Rect colli;
	colli.x = position.x;
	colli.y = position.y;
	colli.w = coin_w_h.x;
	colli.h = coin_w_h.y;

	coin_collider = App->collisions->AddCollider(colli, COLLIDER_COIN, App->manager);

	return true;
}

bool Entity_coin::Update(float dt)
{
	//Draw(dt);
	return true;
}

bool Entity_coin::CleanUp()
{
	coin_collider->to_delete = true;
	active = false;
	
	return true;
}

void Entity_coin::OnCollision(Collider * coin, Collider * colli)
{
	App->audio->PlayFx(coin_clinck);
	CleanUp();
	
	App->scene->player_coins += 1;
	App->scene->score_t += 1000;

	if (App->scene->current == LVL_1)
		collected_1 = true;
	if (App->scene->current == LVL_2)
		collected_2 = true;

	LOG("Player got a coin");
	
}

bool Entity_coin::Save(pugi::xml_node &node) const
{
	char I[40];

	sprintf(I, "Coin_%i", my_index);
	pugi::xml_node coin = node.append_child(I);
	pugi::xml_node pn = coin.append_child("coin_stats");

	pn.append_attribute("position_x") = position.x;
	pn.append_attribute("position_y") = position.y;


	pugi::xml_node pb = coin.append_child("coin_bools");

	pb.append_attribute("collected_1") = collected_1;
	pb.append_attribute("collected_2") = collected_2;


	return true;
}

bool Entity_coin::Load(pugi::xml_node &node)
{
	char I[40];

	sprintf(I, "Coin_%i", my_index);

	position.x = node.child(I).child("coin_stats").attribute("position_x").as_int();
	position.y = node.child(I).child("coin_stats").attribute("position_y").as_int();

	collected_1 = node.child(I).child("coin_bools").attribute("collected_1").as_bool();

	collected_2 = node.child(I).child("coin_bools").attribute("collected_2").as_bool();



	return true;
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
