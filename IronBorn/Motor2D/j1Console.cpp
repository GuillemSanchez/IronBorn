#include "j1Console.h"
#include "j1App.h"
#include "j1Input.h"
#include "Ui_input_text.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "Ui_ntext.h"
#include "j1Scene.h"

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
	}
	return true;
}

bool j1Console::PostUpdate()
{
	// The ol' switcherino

	return true;
}

bool j1Console::CleanUp()
{
	return true;
}

void j1Console::Open_c()
{
	//InputText = App->gui->CreateGuiElement(Types::inputText, 0, 200, { 488, 569, 344, 61 }, nullptr, this);
	//InputText->follow = true;
	//InputText->GetInputText()->SetSingleFocus();
	/*ConsoleText[0] = App->gui->CreateGuiElement(Types::text, 30, -30, { 0, 0, 0, 0 }, InputText, this, "- ");
	for (int i = 1; i < MAXTEXT; i++) {
		ConsoleText[i] = App->gui->CreateGuiElement(Types::text, 0, -i * 20, { 0, 0, 0, 0 }, ConsoleText[0], this, "- ");
	}*/

	/*for (int i = 0; i < logs.count(); i++)
	{
		Ui_ntext* helper = App->gui->CreatenText({ 0,10 * i }, logs[i],10);
		text_logs.add(helper);
	}*/
	int h = 0;
	for (int i = logs.count() - 1; i >= 0; i--)
	{
		Ui_ntext* helper = App->gui->CreatenText({ 0,(-15 * h)+300 }, logs[i], 12);
		text_logs.add(helper);
		h++;
	}

}

void j1Console::Close_c()
{
	for (int i = 0; i < logs.count(); i++)
	{
		text_logs[i]->CleanUp();
		//text_logs[i]->to_delete = true;
	}
	text_logs.clear();
}

void j1Console::Console_write_log(p2SString LOG)
{
	// If i redirect all the logs from p2Log.cpp the game crashes :D The code is done and i have to do engine so i wont print ALL the Logs, only the important ones.
	logs.add(LOG);
}
