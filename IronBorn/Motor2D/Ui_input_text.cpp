#include "j1App.h"
#include "Ui_input_text.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"

Ui_input_text::Ui_input_text(UI_type type, p2Point<int> pos, bool inter, p2SString text, int font_size, SDL_Color _color)
{
	my_type = type;
	my_pos = pos;
	interactable = inter;
	sample_text = text;
	this->font_size = font_size;
	my_color = _color;

	my_font = App->font->Load(DEFAULT_FONT, font_size);

	text_tex = App->font->Print(sample_text.GetString(), my_color, my_font);
}

Ui_input_text::~Ui_input_text()
{
}

void Ui_input_text::Draw()
{
	
	if (focus)
	{
		App->render->DrawQuad({ my_pos.x + _widht,my_pos.y,1,_height }, 255, 255, 255, 255, true, false);
		App->render->Blit(new_text, my_pos.x, my_pos.y, NULL, 0);
	}
	else
	{
		App->render->Blit(text_tex, my_pos.x, my_pos.y, NULL, 0);
	}
	
}

void Ui_input_text::PostUpdate()
{
	if (focus)
	{
		App->input->SetTextInput(true,this); 
		App->font->CalcSize(my_text.GetString(), _widht, _height, my_font);
		inter_zone = { my_pos.x,my_pos.y,_widht,_height };
		App->tex->UnLoad(new_text);
		new_text = App->font->Print(my_text.GetString(), my_color, my_font);
	}
	else
	{
		App->font->CalcSize(sample_text.GetString(), _widht, _height, my_font);
		inter_zone = { my_pos.x,my_pos.y,_widht,_height };
		App->input->SetTextInput(false,this);
	}
}

void Ui_input_text::CleanUp()
{
	to_delete = true;
	App->tex->UnLoad(new_text);
}
