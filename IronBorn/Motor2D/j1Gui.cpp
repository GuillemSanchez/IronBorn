#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "Ui_image.h"
#include "Ui_element.h"
#include "Ui_ntext.h"
#include "Ui_button.h"
#include "Ui_slidder.h"
#include "Ui_input_text.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (int i = 0; i < UI_elements.count(); i++)
	{
		UI_elements[i]->Update();
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	for (int i = 0; i < UI_elements.count(); i++)
	{
		UI_elements[i]->CleanUp();
	}
	UI_elements.clear();

	App->tex->UnLoad(atlas);
	return true;
}

void j1Gui::Defocus()
{
	for (int i = 0; i < UI_elements.count(); i++)
	{
		UI_elements[i]->focus = false;
	}
}

Ui_image * j1Gui::CreateImage(p2Point<int> pos, SDL_Rect rect)
{
	Ui_image* resource = new Ui_image(UIMAGE,pos,false,rect);
	resource->atlas_tex = atlas;

	UI_elements.add(resource);

	return resource;
}

Ui_ntext * j1Gui::CreatenText(p2Point<int> pos, p2SString text, int font_size, SDL_Color _color)
{
	Ui_ntext* resource = new Ui_ntext(UTEXT, pos, false, text,font_size, _color);

	UI_elements.add(resource);

	return resource;
}

Ui_button * j1Gui::CreateButton(p2Point<int> pos, p2SString text, j1Module* mod, int font_size, SDL_Color _color)
{
	Ui_button* resource = new Ui_button(UBUTTON, pos, true, text, mod,font_size,_color);
	resource->atlas_tex = atlas;

	UI_elements.add(resource);

	return resource;
}

Ui_slidder * j1Gui::CreateSlidder(p2Point<int> pos, j1Module * mod, bool horizontal)
{
	Ui_slidder* resource = new Ui_slidder(USLIDDER, pos, true, mod, horizontal);
	resource->atlas_tex = atlas;

	UI_elements.add(resource);
	return  resource;
}

Ui_input_text * j1Gui::CreateInputText(p2Point<int> pos, p2SString text, j1Module * mod, int font_size, SDL_Color _color)
{
	Ui_input_text* resource = new Ui_input_text(UINPUT_TEXT, pos, true, text, font_size, _color); // gtodo falta hacer el module que lo recive.
	resource->atlas_tex = atlas;

	UI_elements.add(resource);

	return resource;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

