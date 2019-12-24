#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Entity_player.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "Animation.h"
#include "j1Input.h"
#include "j1Collisions.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1EntityManager.h"




Entity_Player::Entity_Player(ENTITY_TYPE type, p2Point<int> pos, int index) : Entity(type, pos, index)
{
	i_position = pos;
	App->manager->my_player = this;
}

Entity_Player::~Entity_Player()
{

}

bool Entity_Player::Awake(pugi::xml_node & config)
{
	//we wanna have a node to the player
	pugi::xml_node player_node = config.child("entity_player").child("player");

	gravity = player_node.child("gravity").attribute("value").as_float();
	max_y_speed = player_node.child("max_y_speed").attribute("value").as_int();
	max_x_speed = player_node.child("max_x_speed").attribute("value").as_int();
	pl_w_h.x = player_node.child("player_collider").attribute("width").as_int();
	pl_w_h.y = player_node.child("player_collider").attribute("height").as_int();
	collider_dislocation = player_node.child("collider_frame").attribute("value").as_int();
	negative_acc.x = player_node.child("negative_acc").attribute("value").as_float();
	negative_acc.y = player_node.child("negative_acc").attribute("value").as_float();
	jump_power = player_node.child("max_speed_jump").attribute("value").as_float();

	rolling_velocity = player_node.child("rolling_velocity").attribute("value").as_int();
	player_velocity.x = player_node.child("player_velocity").attribute("x").as_float();
	player_velocity.y = player_node.child("player_velocity").attribute("y").as_float();

	fx_jump.create(player_node.child("fx_jump").attribute("value").as_string());
	fx_landing.create(player_node.child("fx_landing").attribute("value").as_string());


	pugi::xml_node animations = player_node.child("animations");

	LoadAllAnimations(animations);

	return true;
}

bool Entity_Player::Start()
{
	//player position.
	position.x = i_position.x;
	position.y = i_position.y;


	//Restart all variables if we die.
	acceleration.x = 0;
	acceleration.y = 0;

	velocity.x = 0;
	velocity.y = 0;

	stack.x = 0;
	stack.y = 0;
	// -----------------------------

	jump_fx = App->audio->LoadFx(fx_jump.GetString());
	landing_fx = App->audio->LoadFx(fx_landing.GetString());

	//guillem todo: Load this from xml. 

	player_idle = App->tex->Load("textures/knight/noBKG_KnightIdle_strip.png");
	player_run = App->tex->Load("textures/knight/noBKG_KnightRun_strip.png");
	player_jump_fall = App->tex->Load("textures/knight/noBKG_KnightJumpAndFall_strip.png");
	player_rolling = App->tex->Load("textures/knight/noBKG_KnightRoll_strip.png");

	//Collider ---------------------------
	SDL_Rect collo;
	collo.x = position.x;
	collo.y = position.y;
	collo.w = pl_w_h.x;
	collo.h = pl_w_h.y;
	player_collider = App->collisions->AddCollider(collo, COLLIDER_PLAYER, App->manager);
	//Collider ----------------------------

	//Scene logic -------------------------
	App->scene->alive = true;


	current = &IDLE;
	current_texture = player_idle;
	return true;
}

bool Entity_Player::PreUpdate()
{
	//First of all we reset the acceleration.
	acceleration.x = 0;
	acceleration.y = 0;


	//Then we get the input of the cycle.  0.01F is a good acc for this game.

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		acceleration.x -= player_velocity.x;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		acceleration.x += player_velocity.x;

	if (im_a_god) {
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			acceleration.y -= player_velocity.y;

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			acceleration.y += player_velocity.y;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && jump_ava)
	{
		jump_ava = false;
		acceleration.y -= jump_power;
		App->audio->PlayFx(jump_fx, 0);

	}


	//We calculate the current velocity (we dont need to reset this one because we can stack the velocity).
	if (acceleration.x != 0) {
		velocity.x = velocity.x + acceleration.x;
	}
	else { //slow the character when acc is 0;
		if (velocity.x > 0) {
			if (!jump_ava)
				velocity.x -= negative_acc.x / 3;
			else
				velocity.x -= negative_acc.x;
			if (velocity.x < 0)
				velocity.x = 0;
		}
		if (velocity.x < 0) {
			if (!jump_ava)
				velocity.x += negative_acc.x / 3;
			else
				velocity.x += negative_acc.x;
			if (velocity.x > 0)
				velocity.x = 0;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		rolling = true;
	}



	if (!im_a_god)
		velocity.y = velocity.y + acceleration.y + gravity;
	else
		velocity.y = velocity.y + acceleration.y;


	//We check if the velocity is superior to the max velocity. And if so we set the velocity to max speed. Also we do it in both axis.
	if (velocity.x > max_x_speed || velocity.x < -max_x_speed) {
		if (velocity.x > 0)
			velocity.x = max_x_speed;
		else
			velocity.x = -max_x_speed;
	}
	// Y axis.
	if (velocity.y > max_y_speed || velocity.y < -max_y_speed) {
		if (velocity.y > 0)
			velocity.y = max_y_speed;
		else
			velocity.y = -max_y_speed;
	}


	//we dont want to include 0 in this because it would always reset.
	if (velocity.x > 0)
		orientation_right = true;
	if (velocity.x < 0)
		orientation_right = false;

	if (orientation_right && rolling)
		velocity.x = rolling_velocity;

	if (!orientation_right && rolling)
		velocity.x = -rolling_velocity;
	return true;
}

bool Entity_Player::Update(float dt)
{
	ChangetoCurrentAnimation();

	// we use stacks to work under 1 pixel :D.
	stack.x += velocity.x*dt;
	stack.y += velocity.y*dt;
	if (stack.x >= 1 || stack.x <= -1) {
		position.x += stack.x;

		stack.x = 0;
	}
	if (stack.y >= 2 || stack.y <= -2) {
		position.y += stack.y;
		stack.y = 0;
	}
	//LOG("Player position: %i %i", position.x, position.y);

	return true;
}

bool Entity_Player::PostUpdate()
{
	return true;
}

bool Entity_Player::CleanUp()
{
	//Textures --------------------------
	App->tex->UnLoad(player_idle);
	App->tex->UnLoad(player_jump_fall);
	App->tex->UnLoad(player_idle);
	App->tex->UnLoad(player_rolling);
	current_texture = nullptr;
	//Textures --------------------------

	App->scene->alive = false;
	current = nullptr;
	player_collider->to_delete = true;
	App->scene->player_lives -= 1;
	return true;
}

void Entity_Player::Draw(float dt)
{
	r_current = current->GetCurrentFrame(dt);

	if (orientation_right)
		App->render->Blit(current_texture, position.x, position.y, &r_current, 1);
	else
		App->render->Blit(current_texture, position.x, position.y, &r_current, 1, true);

	if (!rolling)
		player_collider->SetPos((position.x), (position.y));
	else
		player_collider->SetPos((position.x), (position.y) + pl_w_h.y / 4);

}

void Entity_Player::OnCollision(Collider *player, Collider *colli)
{
	bool grounded = false; //this variable will become true if we are in colision with the top or the bottom of a collider.

	//one important think is that we collide with multiple colliders so we can handle multiple collisions at once. our variable grounded is unique in every collider 
	//so we can collision with 2 colliders at the same time and get diferents grounded at the same time.

	if (!im_a_god)
	{
		if (((player->rect.y + player->rect.h) >= colli->rect.y) && ((player->rect.y + player->rect.h) <= colli->rect.y + collider_dislocation)) //player is colliding with the top of the collider
		{
			if (velocity.y > 0) // we only want to put to zero if the velocity is positive because that means that we are falling.
			{
				if (velocity.y >= max_y_speed/2)
					App->audio->PlayFx(landing_fx, 0);
				velocity.y = 0;
			}

			position.y = colli->rect.y - pl_w_h.y; // we put the player just above the collider.
			grounded = true;
			jump_ava = true;
			FALL.Reset();
			JUMP.Reset();
		}

		if ((player->rect.y < (colli->rect.y + colli->rect.h)) && (player->rect.y > (colli->rect.y + colli->rect.h - collider_dislocation))) //player is colliding with the bottom side of the collider
		{
			if (velocity.y < 0)
				velocity.y = 0;
			position.y = colli->rect.y + colli->rect.h;
			grounded = true;
		}


		if ((player->rect.x <= (colli->rect.x + colli->rect.w)) && (player->rect.x >= (colli->rect.x + colli->rect.w - collider_dislocation)) && !grounded) //player is colliding with the right sideo of the collider
		{
			if (velocity.x < 0)
				velocity.x = 0;
			position.x = colli->rect.x + colli->rect.w;
		}

		if (((player->rect.x + player->rect.w) >= colli->rect.x) && ((player->rect.x + player->rect.w) <= colli->rect.x + collider_dislocation) && !grounded) //player is colliding with the left side of the collider
		{
			if (velocity.x > 0)
				velocity.x = 0;
			position.x = colli->rect.x - pl_w_h.x;
		}
	}
}

p2Point<int> Entity_Player::GetPosition()
{
	return position;
}
//
//bool Entity_Player::Save(pugi::xml_node &node) const
//{
//
//	pugi::xml_node pn = node.append_child("player_numbers");
//
//	pn.append_attribute("position_x") = position.x;
//	pn.append_attribute("position_y") = position.y;
//	pn.append_attribute("velocity_x") = velocity.x;
//	pn.append_attribute("velocity_y") = velocity.y;
//	pn.append_attribute("acceleration_x") = acceleration.x;
//	pn.append_attribute("acceleration_y") = acceleration.y;
//	pn.append_attribute("stack_x") = stack.x;
//	pn.append_attribute("stack_y") = stack.y;
//
//
//	pugi::xml_node pb = node.append_child("player_bools");
//
//	pb.append_attribute("jump_ava") = jump_ava;
//
//	return true;
//}
//
//bool Entity_Player::Load(pugi::xml_node &node)
//{
//	position.x = node.child("player_numbers").attribute("position_x").as_int();
//	position.y = node.child("player_numbers").attribute("position_y").as_int();
//	velocity.x = node.child("player_numbers").attribute("velocity_x").as_float();
//	velocity.y = node.child("player_numbers").attribute("velocity_y").as_float();
//	acceleration.x = node.child("player_numbers").attribute("acceleration_x").as_float();
//	acceleration.y = node.child("player_numbers").attribute("acceleration_y").as_float();
//	stack.x = node.child("player_numbers").attribute("stack_x").as_float();
//	stack.x = node.child("player_numbers").attribute("stack_x").as_float();
//
//
//	jump_ava = node.child("player_bools").attribute("jump_ava").as_bool();
//
//
//	JustLoaded = true;
//	return true;
//}

bool Entity_Player::LoadAnimation(pugi::xml_node & config, Animation& anim)
{

	for (; config; config = config.next_sibling("frame")) {
		SDL_Rect frame_rect;
		frame_rect.x = config.attribute("x").as_int();
		frame_rect.y = config.attribute("y").as_int();
		frame_rect.w = config.attribute("width").as_int();
		frame_rect.h = config.attribute("height").as_int();
		anim.PushBack(frame_rect);
	}
	return true;
}

bool Entity_Player::LoadAllAnimations(pugi::xml_node & anima)
{
	LoadAnimation(anima.child("idle").child("frame"), IDLE);
	IDLE.loop = anima.child("idle").attribute("loop").as_bool();
	IDLE.speed = anima.child("idle").attribute("speed").as_float();

	LoadAnimation(anima.child("run").child("frame"), RUN);
	RUN.loop = anima.child("run").attribute("loop").as_bool();
	RUN.speed = anima.child("run").attribute("speed").as_float();

	LoadAnimation(anima.child("jump").child("frame"), JUMP);
	JUMP.loop = anima.child("jump").attribute("loop").as_bool();
	JUMP.speed = anima.child("jump").attribute("speed").as_float();

	LoadAnimation(anima.child("fall").child("frame"), FALL);
	FALL.loop = anima.child("fall").attribute("loop").as_bool();
	FALL.speed = anima.child("fall").attribute("speed").as_float();

	LoadAnimation(anima.child("roll").child("frame"), ROLLING);
	ROLLING.loop = anima.child("roll").attribute("loop").as_bool();
	ROLLING.speed = anima.child("roll").attribute("speed").as_float();


	return true;
}

void Entity_Player::ChangetoCurrentAnimation()
{
	if (velocity.x == 0 && velocity.y == 0)
	{
		current = &IDLE;
		current_texture = player_idle;
	}
	if ((velocity.x > 0 || velocity.x < 0) && velocity.y == 0)
	{
		current = &RUN;
		current_texture = player_run;
	}
	if (velocity.y < 0)
	{
		current = &JUMP;
		current_texture = player_jump_fall;
	}
	if (velocity.y > 0)
	{
		current = &FALL;
		current_texture = player_jump_fall;
	}

	if (rolling)
	{
		current = &ROLLING;
		current_texture = player_rolling;
		player_collider->rect.h = pl_w_h.y / 1.5;
		if (current->Finished())
		{
			rolling = false;
			ROLLING.Reset();
			player_collider->rect.h = pl_w_h.y;
		}

	}




}