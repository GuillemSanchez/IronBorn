#include "j1App.h"
#include "Ui_button.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2Defs.h"

Ui_button::Ui_button(UI_type type, p2Point<int> pos, bool inter, p2SString centered_text, j1Module * listener, int font_size, SDL_Color _color)
{
	my_type = type;
	my_pos = pos;
	interactable = inter;
	Centered_text = centered_text;


	AddListener(listener);

	//on_hover_tex = BUTTON_HOVER; //gcambiar cuando pases a tu programa tendras que cambiar esto.
	//normal = BUTTON_NORMAL;
	//on_pressed_tex = BUTTON_PRESSED;

	inter_zone.x = my_pos.x;
	inter_zone.y = my_pos.y;

	inter_zone.w = normal.w;
	inter_zone.h = normal.h;

	CalculateCenter();

	font = App->font->Load(DEFAULT_FONT, font_size);
	text_color = _color;

	Text_texture = App->font->Print(Centered_text.GetString(),text_color,font);

	
}

Ui_button::~Ui_button()
{

}

void Ui_button::Draw()
{
	if (mouse_ev == ST_HOVER)
	{
		App->render->Blit(atlas_tex, my_pos.x, my_pos.y, &on_hover_tex, 0);
	}

	if (mouse_ev == ST_PRESSED || mouse_ev == ST_CONTINUE)
	{
		App->render->Blit(atlas_tex, my_pos.x, my_pos.y, &on_pressed_tex, 0);
	}

	if (mouse_ev == ST_NOTHING || mouse_ev == ST_UP)
	{
		App->render->Blit(atlas_tex, my_pos.x, my_pos.y, &normal, 0);
	}

	App->render->Blit(Text_texture, text_pos.x, text_pos.y, NULL, 0);
}

void Ui_button::ChangeText(p2SString new_text)
{
	Centered_text = new_text;

	App->tex->UnLoad(Text_texture);

	Text_texture = App->font->Print(Centered_text.GetString());
	CalculateCenter();
}

void Ui_button::CleanUp()
{
	App->tex->UnLoad(Text_texture);
	listeners.clear();
}


void Ui_button::CalculateCenter()
{
	int text_width;
	int text_height;

	App->font->CalcSize(Centered_text.GetString(), text_width, text_height, font);

	text_pos.x = my_pos.x + (normal.w / 2) - (text_width / 2);
	text_pos.y = my_pos.y + (normal.h / 2) - (text_height);

}
