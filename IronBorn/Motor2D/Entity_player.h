#ifndef __ENTITY_PLAYER_H__
#define __ENTITY_PLAYER_H__


#include "Entity.h"
#include "p2Point.h"
#include "Animation.h"


struct SDL_Texture;
struct Collider;


class Entity_Player : public Entity
{
public:

	Entity_Player(ENTITY_TYPE type, p2Point<int> pos, int index);

	~Entity_Player();

	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void Draw(float dt);

	void OnCollision(Collider* player, Collider* colli);

	p2Point<int> GetPosition();

	bool Save(pugi::xml_node&) const;

	bool Load(pugi::xml_node&);

	bool JustLoaded = false;

	bool im_a_god = false;
private:


	bool LoadAnimation(pugi::xml_node& config, Animation& anim);

	bool LoadAllAnimations(pugi::xml_node& anima);

	void ChangetoCurrentAnimation();

	// its true when the character is looking right (derecha).
	bool orientation_right;

	p2SString player_idle_str;

	Animation *current = nullptr;
	Animation IDLE;
	Animation RUN;
	Animation JUMP;
	Animation FALL;
	Animation ROLLING;

	SDL_Texture* player_idle = nullptr;
	SDL_Texture* player_run = nullptr;
	SDL_Texture* player_jump_fall = nullptr;
	SDL_Texture* player_rolling = nullptr;
	SDL_Texture* current_texture = nullptr;
	SDL_Rect r_current;

	float gravity;
	int max_y_speed;
	int max_x_speed;
	p2Point<int> i_position;
	p2Point<int> position;
	p2Point<float> velocity;
	p2Point<float> acceleration;
	p2Point<float> stack;
	int rolling_velocity;
	p2Point<float> player_velocity;

	bool rolling = false;
	//if the jump is avalible 
	bool jump_ava = false;
	float jump_power;

	//Negative acceleration to slow the character
	p2Point<float> negative_acc;

	// width and height of the player. Collider things ---------------
	p2Point<int> pl_w_h;
	int collider_dislocation;

	Collider* player_collider;

	//Sound effects. :D
	p2SString fx_jump;
	p2SString fx_landing;
	uint jump_fx;
	uint landing_fx;
};


#endif
