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

	botton_n.x = conf.child("botton_n").attribute("x").as_int();
	botton_n.y = conf.child("botton_n").attribute("y").as_int();
	botton_n.w = conf.child("botton_n").attribute("w").as_int();
	botton_n.h = conf.child("botton_n").attribute("h").as_int();

	botton_h.x = conf.child("botton_h").attribute("x").as_int();
	botton_h.y = conf.child("botton_h").attribute("y").as_int();
	botton_h.w = conf.child("botton_h").attribute("w").as_int();
	botton_h.h = conf.child("botton_h").attribute("h").as_int();

	botton_p.x = conf.child("botton_p").attribute("x").as_int();
	botton_p.y = conf.child("botton_p").attribute("y").as_int();
	botton_p.w = conf.child("botton_p").attribute("w").as_int();
	botton_p.h = conf.child("botton_p").attribute("h").as_int();

	slidder_n.x = conf.child("hover_n").attribute("x").as_int();
	slidder_n.y = conf.child("hover_n").attribute("y").as_int();
	slidder_n.w = conf.child("hover_n").attribute("w").as_int();
	slidder_n.h = conf.child("hover_n").attribute("h").as_int();

	slidder_h.x = conf.child("hover_h").attribute("x").as_int();
	slidder_h.y = conf.child("hover_h").attribute("y").as_int();
	slidder_h.w = conf.child("hover_h").attribute("w").as_int();
	slidder_h.h = conf.child("hover_h").attribute("h").as_int();

	slidder_p.x = conf.child("hover_p").attribute("x").as_int();
	slidder_p.y = conf.child("hover_p").attribute("y").as_int();
	slidder_p.w = conf.child("hover_p").attribute("w").as_int();
	slidder_p.h = conf.child("hover_p").attribute("h").as_int();

	slidder_bar.x = conf.child("hover_bar").attribute("x").as_int();
	slidder_bar.y = conf.child("hover_bar").attribute("y").as_int();
	slidder_bar.w = conf.child("hover_bar").attribute("w").as_int();
	slidder_bar.h = conf.child("hover_bar").attribute("h").as_int();

	image_fo.x = conf.child("image_fo").attribute("x").as_int();
	image_fo.y = conf.child("image_fo").attribute("y").as_int();
	image_fo.w = conf.child("image_fo").attribute("w").as_int();
	image_fo.h = conf.child("image_fo").attribute("h").as_int();

	

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
		if (UI_elements[i] == nullptr)
			continue;

		UI_elements[i]->Update();

		if (UI_elements[i]->to_delete == true)
		{
			delete UI_elements[i];
			UI_elements[i] = nullptr;
		}
		
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	for (int i = 0; i < UI_elements.count(); i++)
	{
		if (UI_elements[i] == nullptr)
			continue;

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
		if (UI_elements[i] == nullptr)
			continue;
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

