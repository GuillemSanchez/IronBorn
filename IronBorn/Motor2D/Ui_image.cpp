#include "j1App.h"
#include "Ui_image.h"
#include "j1Render.h"


Ui_image::Ui_image(UI_type type, p2Point<int> pos, bool inter, SDL_Rect at_pos)
{
	my_type = type;
	my_pos = pos;
	interactable = inter;
	atlas_pos = at_pos;
}

void Ui_image::CleanUp()
{
	to_delete = true;
}

void Ui_image::Draw()
{
	App->render->Blit(atlas_tex, my_pos.x, my_pos.y, &atlas_pos, 0);
}
