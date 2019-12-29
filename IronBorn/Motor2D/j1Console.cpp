#include "j1Console.h"
#include "j1App.h"
#include "j1Input.h"
#include "Ui_input_text.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "Ui_ntext.h"
#include "j1Scene.h"
#include "j1EntityManager.h"

j1Console::j1Console()
{
	name.create("console");
}

j1Console::~j1Console()
{
}

bool j1Console::Awake()
{
	Console_write_log("Hey everyone, here are only somes of the logs pls read line 96 in j1Consoles to understand why.");
	return true;
}

bool j1Console::Start()
{
	return true;
}

bool j1Console::PreUpdate()
{
	return true;
}

bool j1Console::Update(float dt)
{

	// The ol' switcherino
	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN && App->scene->in_game)
	{
		if (console_opened == false)
		{
			console_opened = true;
			Open_c();
		}
		else
		{
			console_opened = false;
			Close_c();
		}
	}

	if (console_opened)
	{
		App->render->DrawQuad({ 0,0, (int)App->win->width/2, (int)App->win->height/2 }, 0, 0, 0, 255, true, false);
		if (App->input->scrolling_up)
		{
	
			for (int i = text_logs.count() - 1; i >= 0; i--)
			{
				text_logs[i]->my_pos.y -= 10;
				
			}
		}
		if (App->input->scrolling_down)
		{
		
			int helper = text_logs.count()-1;
			if (text_logs[helper]->my_pos.y < 300)
			{
				for (int i = text_logs.count() - 1; i >= 0; i--)
				{
					text_logs[i]->my_pos.y += 10;
					
				}
			}
			else
			{
				int h = 0;
				for (int i = text_logs.count() - 1; i >= 0; i--)
				{
					text_logs[i]->my_pos.y = (-15 * h) + 300;
					h++;
				}
			}
		}


		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			COMS comand = ReturnCommand(my_input->my_text.GetString());

			ExecuteCommand(comand);

			Console_write_log(my_input->my_text.GetString());
			comands.add(my_input->my_text.GetString());
			Close_c();
			Open_c();
			
		}
		// Controlers to console lol;
		if (comands.count() > 0)
		{
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
			{
				comands_act -= 1;
				if (comands_act <= 0)
				{
					comands_act = 0;
				}
				my_input->my_text = comands[comands_act];
			}

			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
			{
				comands_act += 1;
				if (comands_act >= comands.count()-1)
				{
					comands_act = comands.count()-1;
				}
				my_input->my_text = comands[comands_act];
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
		{

			my_input->my_text.Cut(my_input->my_text.Length() - 2, my_input->my_text.Length());
			LOG("haciendo cosas");
		}
	}
	return true;
}

bool j1Console::PostUpdate()
{


	return true;
}

bool j1Console::CleanUp()
{
	return true;
}

COMS j1Console::ReturnCommand(const char *hh)
{
	COMS com = max;
	
	p2SString tt = hh;
	p2SString nn = hh;

	for (int i = 1; i <= tt.Length(); i++)
	{
		
		if (hh[i] == '0' || hh[i] == '1' || hh[i] == '2' || hh[i] == '3' || hh[i] == '4' || hh[i] == '5' || hh[i] == '6' || hh[i] == '7' || hh[i] == '8' || hh[i] == '9')
		{
			tt.Cut(i-1);

			nn.Cut(0, i-1);
		}
	}

	if (strcmp(hh, "/godmode") == 0)
	{
		com = God_Mode;
	}
	if (strcmp(hh, "/quit") == 0)
	{
		com = quit;
	}
	if (strcmp(tt.GetString(), "/FPS") == 0)
	{
		com = FPS;

		Fp = std::stoi(nn.GetString());
	}
	if (strcmp(hh, "/help") == 0)
	{
		com = list;
	}
	if (strcmp(tt.GetString(), "/map") == 0)
	{
		com = map;
		Ln = std::stoi(nn.GetString());
	}




	return com;
}

void j1Console::ExecuteCommand(COMS com)
{
	switch (com)
	{
	case God_Mode:
		App->manager->my_player->im_a_god = !App->manager->my_player->im_a_god;

		break;
	case quit:
		App->scene->wanna_quit = true;
		break;
	case FPS:
		if (Fp <= 20)
			Fp = 20;

		if (Fp >= 60)
			Fp = 60;

		App->cap = Fp;
		break;
	case map:
		if (Ln == 1)
		{
			App->scene->SwapMaps(LVL_1);
		}
		if (Ln == 2)
		{
			App->scene->SwapMaps(LVL_2);
		}
		if (Ln != 1 || Ln != 2)
		{
			Console_write_log("Wrong map number.");
		}
		break;
	case list:
		Console_write_log("List of comands:");
		Console_write_log("/godmode");
		Console_write_log("/quit");
		Console_write_log("/help");
		Console_write_log("/FPS 'number'");
		Console_write_log("/map 'number'");
		break;
	case max:
		break;
	default:
		break;
	}
}

void j1Console::Open_c()
{
	my_input = App->gui->CreateInputText({ 0,350 }, "Comand", this, 12);
	App->gui->Defocus();
	my_input->focus = true;

	int h = 0;
	for (int i = logs.count() - 1; i >= 0; i--)
	{
		Ui_ntext* helper = App->gui->CreatenText({ 0,(-15 * h)+300 }, logs[i], 12);
		text_logs.add(helper);
		h++;
	}
	comands_act = comands.count() - 1;
}

void j1Console::Close_c()
{
	my_input->CleanUp();

	for (int i = 0; i < text_logs.count(); i++)
	{
		text_logs[i]->CleanUp();
	}
	text_logs.clear();
}

void j1Console::Console_write_log(p2SString LOG)
{
	// If i redirect all the logs from p2Log.cpp the game crashes :D The code is done and i have to do engine so i wont print ALL the Logs, only the important ones.
	logs.add(LOG);
}
