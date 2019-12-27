#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class Ui_image;
class Ui_ntext;
class Ui_button;
class Ui_element;
class Ui_slidder;
class Ui_input_text;

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

	void UI_listener(Ui_element* ele);

	void CreateInitalMenu();

	void CreateCreditsMenu();

	void CreateSettingsMenu();

	void DestroyInitialMenu();

	void DestroySettingsMenu();

	void DestroyCreditsmenu();

	void CreateLVL1();


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
	p2SString intro_menu;

	bool playingMusic = false;

	bool wanna_quit = false;
private:

	//UI thinks

	//Botton with wich we will start the game.
	Ui_button* play_button;

	//Botton with wich we will quit the game.
	Ui_button* quit_button;

	Ui_button* credits_button;

	Ui_button* continue_button;

	Ui_button* settings_button;

	Ui_image* image_1_menu;

	Ui_image* image_background;

	// Settings -----------------------------------
	Ui_slidder* Music_slidder;

	Ui_slidder* Sound_slidder;

	Ui_ntext* Sound_text;
	Ui_ntext* fx_text;

	// Settings -----------------------------------

	// Credits ------------------------------------
	Ui_ntext* credits_text;
	Ui_ntext* credits_text_1;
	Ui_ntext* credits_text_2;
	Ui_ntext* credits_text_3;
	Ui_ntext* credits_text_4;
	Ui_ntext* credits_author;

	Ui_button* return_button;
	// Credits ------------------------------------

	bool ini_menu = true;
	bool ini_menu_created;

	bool credits_menu = false;
	bool credits_menu_created = false;

	bool settings_menu = false;
	bool settings_menu_created = false;
public:

	int player_lives = 3;
	int player_coins = 0;
	bool alive = false;
};

#endif // __j1SCENE_H__