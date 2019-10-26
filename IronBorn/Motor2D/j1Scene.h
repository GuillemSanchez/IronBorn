#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

enum LVL
{
	LVL_1,
	LVL_2,



	LVL_MAX
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
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

	bool Save(pugi::xml_node&) const;

	bool Load(pugi::xml_node&);
	
	void InitialSwap();
	

	void OnCollision(Collider* c1, Collider* c2);

	void SwapMaps(LVL desired);

private:
	bool swaping = false;
	LVL current = LVL_1;
	int current_lvl = 1;
	p2SString map_1;
	p2SString map_2;
	p2Point<int> player_position;
	p2Point<int> camera_out;
	p2SString map_1_music;
	p2SString map_2_music;

	bool playingMusic = false;


public:

	bool alive = false;
};

#endif // __j1SCENE_H__