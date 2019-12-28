#include "j1Console.h"
#include "j1App.h"
#include "j1Input.h"
#include "Ui_input_text.h"
#include "j1Render.h"
#include "j1Window.h"

j1Console::j1Console()
{
	name.create("console");
}

j1Console::~j1Console()
{
}

bool j1Console::Awake()
{
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


	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN)
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
}

void j1Console::Close_c()
{
}

void j1Console::Console_write_log(p2SString LOG)
{
	logs.add(LOG);
}
