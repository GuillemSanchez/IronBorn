#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Fade.h"
#include "j1Collisions.h"
#include "j1EntityManager.h"
#include "j1Gui.h"
#include "Ui_ntext.h"
#include "Ui_button.h"
#include "Ui_element.h"
#include "Ui_input_text.h"
#include "Ui_image.h"
#include "Ui_slidder.h"
#include "j1Console.h"


j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	App->console->Console_write_log("Loading Scene");
	bool ret = true;

	map_1.create(config.child("lvl_1").attribute("value").as_string());
	map_2.create(config.child("lvl_2").attribute("value").as_string());
	map_1_music.create(config.child("ms_lvl_1").attribute("value").as_string());
	map_2_music.create(config.child("ms_lvl_2").attribute("value").as_string());
	intro_menu.create(config.child("ms_menu").attribute("value").as_string());

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	//Gantiguo 
	/*App->map->Load(map_1.GetString());
	App->map->ChargeColliders();*/

	current = LVL_MAX;
	
	CreateInitalMenu();

	
	App->audio->PlayMusic(intro_menu.GetString(), 2);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("SCENE PREUPDATE", Profiler::Color::Black);

	if (swaping && App->fade->mid)
	{
		App->map->CleanUp();
		if (current == LVL_1)
			App->map->Load(map_1.GetString());

		if (current == LVL_2)
			App->map->Load(map_2.GetString());
		else
			LOG("Error: map is not currently avalible");

		App->map->ChargeColliders();
		swaping = false;

	}
	if (alive == false && App->fade->finished == true && App->manager->my_player->active)
	{
		App->manager->my_player->Start();
	}
	if (player_lives <= 0)
	{
		DestroyIGmenu();
		DestroyINGAMEui();
		DestroyEverythink();
		App->manager->InactiveAll();
		CreateInitalMenu();
		App->audio->PlayMusic(intro_menu.GetString(), 2);
		current = LVL_MAX;
		playingMusic = false;
		player_coins = 0;
		player_lives = 3;
	}

	if (current == LVL_1 && !playingMusic) {
		App->audio->PlayMusic(map_1_music.GetString(), 0);
		playingMusic = true;
	}


	if (current == LVL_2 && !playingMusic)
	{
		App->audio->PlayMusic(map_2_music.GetString(), 0);
		playingMusic = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT)
		App->audio->ChangeGlobalVolumen(1);

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT)
		App->audio->ChangeGlobalVolumen(-1);


	camera_out.x = 300;
	//guillem todo: Load this from tmx;
	camera_out.y = 300;
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("SCENE UPDATE", Profiler::Color::Black);

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		SwapMaps(LVL_2);

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		SwapMaps(LVL_1);
	
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		SwapMaps(current);


	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (App->cap == 60)
		{
			App->cap = 30;
		}
		else
			App->cap = 60;
	
	}

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && App->manager->my_player->active)
	{
		if (App->manager->my_player->im_a_god )
			App->manager->my_player->im_a_god = false;
		else
			App->manager->my_player->im_a_god = true;
	}

	if (App->manager->my_player->active)
		player_position = App->manager->my_player->GetPosition();

	App->render->camera.x = -player_position.x + camera_out.x;

	App->render->camera.y = -player_position.y + camera_out.y;

	if (App->render->camera.x >= 0)
		App->render->camera.x = 0;

	if (App->render->camera.y >= 0)
		App->render->camera.y = 0;

	if (App->render->camera.y - App->render->camera.h <= -App->map->data.real_height)
		App->render->camera.y = -App->map->data.real_height + App->render->camera.h;

	if (App->render->camera.x - App->render->camera.w <= -App->map->data.real_width)
		App->render->camera.x = -App->map->data.real_width + App->render->camera.w;

	

	/*if (!swaping)*/
	App->map->Draw();

	if (alive && App->manager->my_player->active)
	{
		App->manager->my_player->Draw(dt);
	}
		
	

	App->manager->DrawEnts(dt);



	//LOG("Camera x: %i Camera y: %i", App->render->camera.w, App->render->camera.y);
	////Title to position -------------------------------------------------------------------------------------------------------------------------
	//
	//int x, y;
	//App->input->GetMousePosition(x, y);
	//iPoint map_coordinates;
	//map_coordinates.x = x;
	//map_coordinates.y = y;
	//p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
	//				App->map->data.width, App->map->data.height,
	//				App->map->data.tile_width, App->map->data.tile_height,
	//				App->map->data.tilesets.count(),
	//				map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());
	//title to position--------------------------------------------------------------------------------------------------------------------------



	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("SCENE POSTUPDATE", Profiler::Color::Black);
	if (player_coins >= 10)
	{
		player_coins = 0;
		player_lives += 1;
	}
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (in_game)
		{
			if (!menu_created)
				CreateIGmenu();
		}
		else
		{
			ret = false;
		}
	}
		

	if (wanna_quit == true)
	{
		ret = false;
	}

	if (Music_slidder != nullptr)
	{
		float helper = Music_slidder->slidder_value;
		int desired = (int)(helper * 180);

		App->audio->ChangeMusicVol(desired);
	}
		

	if (Sound_slidder != nullptr)
	{
		float helper = Sound_slidder->slidder_value;
		int desired = (int)(helper * 180);
		App->audio->ChangeFxVol(desired);
	}
	
	if (player_lives_t != nullptr)
	{
		char lives[30];
		sprintf(lives, "X %i", player_lives);
		player_lives_t->ChangeText(lives);
	}

	if (player_coins_t != nullptr)
	{
		char coins[30];
		sprintf(coins,"X %i", player_coins);
		player_coins_t->ChangeText(coins);
	}

	if (score != nullptr)
	{
		score_t -= 1;
		if (score_t <= 0)
			score_t = 0;
		char scc[70];
		sprintf(scc, "Score: %i", score_t);
		score->ChangeText(scc);

	}
	if (timer != nullptr)
	{
		char ttm[70];
		sprintf(ttm, "Timer: %f", startup_time.ReadSec());
		timer->ChangeText(ttm);
		//how_much_last_time = startup_time.ReadSec();
	}



	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool j1Scene::Save(pugi::xml_node &node ) const
{

	p2SString curr;

	if (current == LVL_1)
		curr.create("LVL_1");
	else
		curr.create("LVL_2");

	pugi::xml_node sc = node.append_child("scene_player");

	sc.append_attribute("lvl_current") = curr.GetString();

	sc.append_attribute("player_lives") = player_lives;
	sc.append_attribute("player_coins") = player_coins;
	
	sc.append_attribute("score_t") = score_t;
	sc.append_attribute("timer_t") = startup_time.Read();

	return true;
}

bool j1Scene::Load(pugi::xml_node &node )
{
	p2SString curr;

	curr.create(node.child("scene_player").attribute("lvl_current").as_string());

	player_lives = node.child("scene_player").attribute("player_lives").as_int();
	player_coins = node.child("scene_player").attribute("player_coins").as_int();
	score_t = node.child("scene_player").attribute("score_t").as_int();
	how_much_last_time = node.child("scene_player").attribute("timer_t").as_uint();



	
	p2SString LvL;
	LvL.create("LVL_1");
	if (curr == LvL)
		current = LVL_1;
	else
		current = LVL_2;

	



	InitialSwap();

	// This should work and i dont have time to see why this doesnt work correctly.
	startup_time.started_at = (startup_time.Read() - how_much_last_time);
	return true;
}

void j1Scene::InitialSwap()
{

	App->collisions->CleanMap();
	App->map->CleanUp();

	App->manager->StartAllCoins();
	App->manager->Start_2_all_coins();

	if (current == LVL_1)
		App->map->Load(map_1.GetString());

	if (current == LVL_2)
		App->map->Load(map_2.GetString());
	else
		LOG("Error: map is not currently avalible");

	App->map->ChargeColliders();
	playingMusic = false;


}

void j1Scene::DestroyEverythink()
{
	App->map->CleanUp();
	App->collisions->CleanMap();

}

void j1Scene::OnCollision(Collider * c1, Collider * c2)
{
	if (!App->manager->my_player->im_a_god && App->manager->my_player->active)
	{
		if (c1->type == COLLIDER_DEATH)
		{
			App->manager->my_player->CleanUp();
			App->fade->FadeToBlack();
			App->scene->player_lives -= 1;
		}

	}
	if (c1->type == COLLIDER_WIN)
	{
		if (current == LVL_1)
			SwapMaps(LVL_2);
		else
			SwapMaps(LVL_1);
	}

		
}

void j1Scene::SwapMaps(LVL desired)
{
	if (App->manager->my_player->active)
		App->manager->my_player->CleanUp(); // first we eliminate the player
	App->collisions->CleanUp(); // we clean the collider array.
	App->fade->FadeToBlack(); // we fade to swap.
	swaping = true;
	playingMusic = false;

	current = desired;

	App->manager->StartAllCoins();
	App->manager->Start_2_all_coins();
}

void j1Scene::UI_listener(Ui_element * ele)
{
	if (ele == play_button && ele->GetState() == ST_PRESSED)
	{
		DestroyInitialMenu();
		CreateLVL1();
		CreateINGAMEui();
	}
	if (ele == quit_button && ele->GetState() == ST_PRESSED)
	{
		wanna_quit = true;
	}

	if (ele == continue_button && ele->GetState() == ST_PRESSED)
	{
		DestroyInitialMenu();
		CreateLVL1();
		CreateINGAMEui();
		App->LoadGame("save_game.xml");
	}
	
	if (ele == credits_button && ele->GetState() == ST_PRESSED)
	{
		DestroyInitialMenu();
		CreateCreditsMenu();
	}
	if (ele == settings_button && ele->GetState() == ST_PRESSED)
	{
		DestroyInitialMenu();
		CreateSettingsMenu();
	}
	if (ele == return_button && ele->GetState() == ST_PRESSED)
	{
		DestroyCreditsmenu();
		DestroySettingsMenu();
		CreateInitalMenu();
	}

	if (ele == resume_button && ele->GetState() == ST_PRESSED)
	{
		DestroyIGmenu();
	}
	if (ele == return_to_hub_button && ele->GetState() == ST_PRESSED)
	{
		DestroyIGmenu();
		DestroyINGAMEui();
		DestroyEverythink();
		App->manager->InactiveAll();
		CreateInitalMenu();
		App->audio->PlayMusic(intro_menu.GetString(), 2);
		current = LVL_MAX;
		playingMusic = false;
		player_coins = 0;
		player_lives = 3;

	}
}

void j1Scene::CreateInitalMenu()
{
	ini_menu = true;
	SDL_Color BLACK = { 0,0,0,255 }; // Recuerda el orden de pintado
	image_background = App->gui->CreateImage({ 0,-10 }, App->gui->image_fo_2);
	image_1_menu = App->gui->CreateImage({260,30}, App->gui->image_fo);
	play_button = App->gui->CreateButton({ 410,140 }, "Play", this, 25, BLACK);
	continue_button = App->gui->CreateButton({ 410,250 }, "Continue", this, 25, BLACK);
	settings_button = App->gui->CreateButton({ 410,360 }, "Settings", this, 25, BLACK);
	credits_button = App->gui->CreateButton({ 410, 470 }, "Credits", this, 25, BLACK);
	quit_button = App->gui->CreateButton({ 290,600 }, "Quit", this, 25, BLACK);


}

void j1Scene::CreateCreditsMenu()
{
	SDL_Color BLACK = { 0,0,0,255 }; // Recuerda el orden de pintado

	image_background = App->gui->CreateImage({ 0,-10 }, App->gui->image_fo_2);
	image_1_menu = App->gui->CreateImage({ 260,30 }, App->gui->image_fo);

	credits_text = App->gui->CreatenText({ 280,50 }, App->gui->credits_1, 15, BLACK);
	credits_text_1 = App->gui->CreatenText({ 280,70 }, App->gui->credits_2, 15, BLACK);
	credits_text_2 = App->gui->CreatenText({ 280,100 }, App->gui->credits_3, 15, BLACK);
	credits_text_3 = App->gui->CreatenText({ 280,290 }, App->gui->credits_4, 15, BLACK);
	credits_text_4 = App->gui->CreatenText({ 280,360 }, App->gui->credits_5, 15, BLACK);
	credits_author = App->gui->CreatenText({ 280,580 }, App->gui->credits_6, 15, BLACK);

	return_button = App->gui->CreateButton({ 530,600 }, "Return", this, 25, BLACK);
}

void j1Scene::CreateSettingsMenu()
{
	SDL_Color BLACK = { 0,0,0,255 }; // Recuerda el orden de pintado

	image_background = App->gui->CreateImage({ 0,-10 }, App->gui->image_fo_2);
	image_1_menu = App->gui->CreateImage({ 260,30 }, App->gui->image_fo);

	fx_text = App->gui->CreatenText({ 320,180 }, "Fxs:", 25, BLACK);
	Sound_slidder = App->gui->CreateSlidder({ 320,220 }, this, true);

	Sound_text = App->gui->CreatenText({ 320,280 }, "Music:", 25, BLACK);
	Music_slidder = App->gui->CreateSlidder({ 320,320 }, this, true);

	return_button = App->gui->CreateButton({ 530,600 }, "Return", this, 25, BLACK);
}

void j1Scene::DestroyInitialMenu()
{
	if (image_1_menu != nullptr)
	{
		image_1_menu->CleanUp();
		image_1_menu = nullptr;
	}

	if (play_button != nullptr)
	{
		play_button->CleanUp();
		play_button = nullptr;
	}

	if (quit_button != nullptr)
	{
		quit_button->CleanUp();
		quit_button = nullptr;
	}

	if (credits_button != nullptr)
	{
		credits_button->CleanUp();
		credits_button = nullptr;
	}

	if (settings_button != nullptr)
	{
		settings_button->CleanUp();
		settings_button = nullptr;
	}
	if (continue_button != nullptr)
	{
		continue_button->CleanUp();
		continue_button = nullptr;
	}
	if (image_background != nullptr)
	{
		image_background->CleanUp();
		image_background = nullptr;
	}
	ini_menu = false;
}

void j1Scene::DestroySettingsMenu()
{
	if (return_button != nullptr)
	{
		return_button->CleanUp();
		return_button = nullptr;
	}
	if (image_1_menu != nullptr)
	{
		image_1_menu->CleanUp();
		image_1_menu = nullptr;
	}
	if (image_background != nullptr)
	{
		image_background->CleanUp();
		image_background = nullptr;
	}
	if (Sound_slidder != nullptr)
	{
		Sound_slidder->CleanUp();
		Sound_slidder = nullptr;
	}
	if (Music_slidder != nullptr)
	{
		Music_slidder->CleanUp();
		Music_slidder = nullptr;
	}
	if (fx_text != nullptr)
	{
		fx_text->CleanUp();
		fx_text = nullptr;
	}
	if (Sound_text != nullptr)
	{
		Sound_text->CleanUp();
		Sound_text = nullptr;
	}
	
}

void j1Scene::DestroyCreditsmenu()
{
	if (image_1_menu != nullptr)
	{
		image_1_menu->CleanUp();
		image_1_menu = nullptr;
	}
	if (image_background != nullptr)
	{
		image_background->CleanUp();
		image_background = nullptr;
	}
	if (credits_text != nullptr)
	{
		credits_text->CleanUp();
		credits_text = nullptr;
	}
	if (credits_text_1 != nullptr)
	{
		credits_text_1->CleanUp();
		credits_text_1 = nullptr;
	}
	if (credits_text_2 != nullptr)
	{
		credits_text_2->CleanUp();
		credits_text_2 = nullptr;
	}
	if (credits_text_3 != nullptr)
	{
		credits_text_3->CleanUp();
		credits_text_3 = nullptr;
	}
	if (credits_text_4 != nullptr)
	{
		credits_text_4->CleanUp();
		credits_text_4 = nullptr;
	}
	if (credits_author != nullptr)
	{
		credits_author->CleanUp();
		credits_author = nullptr;
	}
	if (return_button != nullptr)
	{
		return_button->CleanUp();
		return_button = nullptr;
	}
}

void j1Scene::CreateLVL1()
{
	App->manager->StartAllCoins();

	App->manager->Start_2_all_coins();
	App->map->Load(map_1.GetString());
	App->map->ChargeColliders();
	current = LVL_1;
	startup_time.Start();

	App->manager->Createlvl(current);
}

void j1Scene::CreateINGAMEui()
{
	in_game = true;

	coin_sprite = App->gui->CreateImage({ 105,20 }, App->gui->coin_sprite);
	life_sprite = App->gui->CreateImage({ 20,20 }, App->gui->hearth_sprite);
	player_coins_t = App->gui->CreatenText({ 125,14 }, "X 3", 25);
	player_lives_t = App->gui->CreatenText({ 55,14 }, "X 3", 25);
	timer = App->gui->CreatenText({ 500,14 }, "Time:", 25);
	score = App->gui->CreatenText({ 750,14 }, "Score:", 25);
}

void j1Scene::DestroyINGAMEui()
{

	if (coin_sprite != nullptr)
	{
		coin_sprite->CleanUp();
		coin_sprite = nullptr;
	}
	if (life_sprite != nullptr)
	{
		life_sprite->CleanUp();
		life_sprite = nullptr;
	}
	if (player_coins_t != nullptr)
	{
		player_coins_t->CleanUp();
		player_coins_t = nullptr;
	}
	if (player_lives_t != nullptr)
	{
		player_lives_t->CleanUp();
		player_lives_t = nullptr;
	}

	if (score != nullptr)
	{
		score->CleanUp();
		score = nullptr;
	}

	if (timer != nullptr)
	{
		timer->CleanUp();
		timer = nullptr;
	}
	in_game = false;
}

void j1Scene::CreateIGmenu()
{
	menu_created = true;
	pause = true;
	SDL_Color BLACK = { 0,0,0,255 }; // Recuerda el orden de pintado
	image_1_menu = App->gui->CreateImage({ 260,30 }, App->gui->image_fo);

	fx_text = App->gui->CreatenText({ 320,180 }, "Fxs:", 25, BLACK);
	Sound_slidder = App->gui->CreateSlidder({ 320,220 }, this, true);

	Sound_text = App->gui->CreatenText({ 320,280 }, "Music:", 25, BLACK);
	Music_slidder = App->gui->CreateSlidder({ 320,320 }, this, true);

	return_to_hub_button = App->gui->CreateButton({ 530,600 }, "Hub", this, 25, BLACK);
	resume_button = App->gui->CreateButton({ 290,600 }, "Resume", this, 25, BLACK);
}

void j1Scene::DestroyIGmenu()
{
	menu_created = false;
	if (image_1_menu != nullptr)
	{
		image_1_menu->CleanUp();
		image_1_menu = nullptr;
	}
	if (Sound_slidder != nullptr)
	{
		Sound_slidder->CleanUp();
		Sound_slidder = nullptr;
	}
	if (Music_slidder != nullptr)
	{
		Music_slidder->CleanUp();
		Music_slidder = nullptr;
	}
	if (fx_text != nullptr)
	{
		fx_text->CleanUp();
		fx_text = nullptr;
	}
	if (Sound_text != nullptr)
	{
		Sound_text->CleanUp();
		Sound_text = nullptr;
	}
	if (return_to_hub_button != nullptr)
	{
		return_to_hub_button->CleanUp();
		return_to_hub_button = nullptr;
	}
	if (resume_button != nullptr)
	{
		resume_button->CleanUp();
		resume_button = nullptr;
	}
	pause = false;
}
