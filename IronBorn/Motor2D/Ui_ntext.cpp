#include "j1App.h"
#include "Ui_ntext.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Textures.h"



Ui_ntext::Ui_ntext(UI_type type, p2Point<int> pos, bool inter, p2SString text, int font_size, SDL_Color _color)
{
	my_type = type;
	my_pos = pos;
	interactable = inter;

	my_text = text;

	my_font = App->font->Load(DEFAULT_FONT, font_size);
	my_color = _color;

		
	text_tex = App->font->Print(text.GetString(),_color,my_font);
}

void Ui_ntext::CleanUp()
{
	App->tex->UnLoad(text_tex); //fonts cause little memory leaks lol.
}

void Ui_ntext::Draw()
{
	App->render->Blit(text_tex, my_pos.x, my_pos.y, NULL, 0);
}

void Ui_ntext::ChangeText(p2SString to_change)
{
	my_text = to_change;

	App->tex->UnLoad(text_tex);

	text_tex = App->font->Print(my_text.GetString());
	
}
