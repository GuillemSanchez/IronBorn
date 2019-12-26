#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Player.h"
#include "j1EntityManager.h"

j1Collisions::j1Collisions()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_DEATH] = false;
	matrix[COLLIDER_WALL][COLLIDER_WIN] = false;
	matrix[COLLIDER_WALL][COLLIDER_COIN] = false;
	
	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WIN] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_COIN] = true;

	matrix[COLLIDER_DEATH][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
	matrix[COLLIDER_DEATH][COLLIDER_WIN] = false;
	matrix[COLLIDER_DEATH][COLLIDER_COIN] = false;

	matrix[COLLIDER_WIN][COLLIDER_WIN] = false;
	matrix[COLLIDER_WIN][COLLIDER_WALL] = false;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WIN][COLLIDER_DEATH] = false;
	matrix[COLLIDER_WIN][COLLIDER_COIN] = false;

	matrix[COLLIDER_COIN][COLLIDER_WIN] = false;
	matrix[COLLIDER_COIN][COLLIDER_WALL] = false;
	matrix[COLLIDER_COIN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_COIN][COLLIDER_DEATH] = false;
	matrix[COLLIDER_COIN][COLLIDER_COIN] = false;


	name.create("collisions");
}

// Destructor
j1Collisions::~j1Collisions()
{}

bool j1Collisions::Awake(pugi::xml_node & config)
{
	return true;
}

bool j1Collisions::Start()
{
	return true;
}

bool j1Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	// Calculate collisions
	Collider* c1;
	Collider* c2;


	if (!App->manager->my_player->JustLoaded && App->manager->my_player->active) {
		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			// skip empty colliders
			if (colliders[i] == nullptr)
				continue;

			c1 = colliders[i];

			// avoid checking collisions already checked
			for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
			{
				// skip empty colliders
				if (colliders[k] == nullptr)
					continue;

				c2 = colliders[k];

				if (c1->CheckCollision(c2->rect) == true)
				{
					if (matrix[c1->type][c2->type] && c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (matrix[c2->type][c1->type] && c2->callback)
						c2->callback->OnCollision(c2, c1);
				}
			}
		}
	}
	if(App->manager->my_player->active)
		App->manager->my_player->JustLoaded = false;

	return true;
}

// Called before render is available
bool j1Collisions::Update(float dt)
{

	DebugDraw();

	return true;
}

void j1Collisions::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALL: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_DEATH: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_WIN: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case COLLIDER_COIN: //Loraay
			App->render->DrawQuad(colliders[i]->rect, 69, 7, 48, alpha);
			break;
		}
	}
}

// Called before quitting
bool j1Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

void j1Collisions::CleanMap()
{


	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			if (colliders[i]->type != COLLIDER_PLAYER)
			{
				delete colliders[i];
				colliders[i] = nullptr;
			}
		}
	}

}

Collider* j1Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	bool ret = false;
	if (rect.x <= r.x + r.w) {
		if (rect.x + rect.w >= r.x) {
			if (rect.y <= r.h + r.y) {
				if (rect.y + rect.h >= r.y) {
					ret = true;
				}
			}
		}

	}
	return ret;
}