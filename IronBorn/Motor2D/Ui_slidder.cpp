#include "j1App.h"
#include "Ui_slidder.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2Defs.h"
#include "j1Input.h"

Ui_slidder::Ui_slidder(UI_type type, p2Point<int> pos, bool inter, j1Module * listener, bool horizontal)
{
	my_type = type;
	my_pos = pos;
	interactable = inter;
	this->horizontal = horizontal;

	if (horizontal)
	{
		on_pressed_ = App->gui->slidder_p;
		normal = App->gui->slidder_n;
		slidder_alue = App->gui->slidder_bar;
		on_hover = App->gui->slidder_h;
		//LOG("he llegado");
	}
	else
	{
	/*	on_pressed_ = VERTICAL_SLIDDER_PRESSED;
		normal = VERTICAL_SLIDDER_NORMAL;
		slidder_value = VERTICAL_SLIDDER_VAR;*/

	}

	CalculateLimits();
}

Ui_slidder::~Ui_slidder()
{
}

void Ui_slidder::PostUpdate()
{

	if (mouse_ev == Ky_St::ST_CONTINUE || mouse_ev == Ky_St::ST_PRESSED)
	{
		p2Point<int> m_pos;
		App->input->GetMousePosition(m_pos.x, m_pos.y);

		if (m_pos.x >= min_pos && m_pos.x <= max_pos)
		{
			inter_zone.x = m_pos.x - normal.w/2;
			dot.x = m_pos.x;
		}
	}
	float total = max_pos - min_pos;
	float actual = max_pos - dot.x;

	float val = actual/total;
	//LOG("val: %f, max: %f, min: %f", val, total, actual);
	slidder_value = 1 - val;
	
}

void Ui_slidder::Draw()
{
	App->render->Blit(atlas_tex, my_pos.x, my_pos.y, &slidder_alue, 0);
	if (mouse_ev == ST_HOVER)
	{
		App->render->Blit(atlas_tex, inter_zone.x, inter_zone.y, &on_hover, 0);
	}

	if (mouse_ev == ST_PRESSED || mouse_ev == ST_CONTINUE)
	{
		App->render->Blit(atlas_tex, inter_zone.x, inter_zone.y, &on_pressed_, 0);
	}

	if (mouse_ev == ST_NOTHING || mouse_ev == ST_UP)
	{
		App->render->Blit(atlas_tex, inter_zone.x, inter_zone.y, &normal, 0);
	}
	
	
}

void Ui_slidder::CleanUp()
{
	to_delete = true;
}

void Ui_slidder::CalculateLimits()
{
	if (horizontal)
	{
		min_pos = (my_pos.x + size_of_limits + normal.w / 2);
		max_pos = (my_pos.x - size_of_limits + slidder_alue.w - (normal.w / 2));
	}
	else
	{

	}

	dot.x = min_pos;
	dot.y = my_pos.y;
	inter_zone.x = dot.x;
	inter_zone.y = dot.y;
	inter_zone.w = normal.w;
	inter_zone.h = normal.h;


}
