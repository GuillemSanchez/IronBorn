#include "j1App.h"
#include "Ui_element.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Audio.h"

void Ui_element::Update()
{
	// Cheking if the mouse is in the zone and what state should we stay --------------------------------------------------
	p2Point<int> mouse_position;
	App->input->GetMousePosition(mouse_position.x, mouse_position.y);

	if (interactable)
	{
		if (mouse_position.x >= inter_zone.x && mouse_position.x <= inter_zone.x + inter_zone.w && mouse_position.y >= inter_zone.y && mouse_position.y <= inter_zone.y + inter_zone.h)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				if (mouse_ev != ST_PRESSED)
				{
					App->audio->PlayFx(pressed_fx);
				}
				mouse_ev = ST_PRESSED;
				App->gui->Defocus();
				focus = true;
			}

			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				mouse_ev = ST_CONTINUE;
			}
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_IDLE)
			{
				if (mouse_ev != ST_HOVER)
				{
					App->audio->PlayFx(hover_fx);
				}
				
				mouse_ev = ST_HOVER;
			}
		}
		else
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_IDLE)
			{
				mouse_ev = ST_NOTHING;
			}
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && (mouse_ev == ST_PRESSED || mouse_ev == ST_CONTINUE))
		{
			mouse_ev = ST_UP;
		}
	}

	

	
	// Cheking if the mouse is in the zone and what state should we stay --------------------------------------------------

	for (int i = 0; i < listeners.count(); i++)
	{
		listeners[i]->UI_listener(this); 
	}

	PostUpdate();
	Draw();
}

Ky_St Ui_element::GetState()
{
	return mouse_ev;
}



void Ui_element::AddListener(j1Module * mod)
{
	listeners.add(mod);
}
