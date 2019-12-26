#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"


#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes

class Ui_element;
class Ui_image;
class Ui_ntext;
class Ui_button;
class Ui_slidder;
class Ui_input_text;



// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	void Defocus();

	Ui_image* CreateImage(p2Point<int> pos, SDL_Rect rect);
	Ui_ntext* CreatenText(p2Point<int> pos, p2SString text, int font_size = 20, SDL_Color _color = { 255,255,255,255 }); 
	Ui_button* CreateButton(p2Point<int>pos, p2SString text, j1Module* mod, int font_size = 20, SDL_Color _color = { 255,255,255,255 });
	Ui_slidder* CreateSlidder(p2Point<int>pos, j1Module* mod, bool horizontal);
	Ui_input_text* CreateInputText(p2Point<int>pos, p2SString text, j1Module* mod, int font_size = 20, SDL_Color _color = { 255,255,255,255 });

	const SDL_Texture* GetAtlas() const;

public:

	// SDL_Rects to know what numbers we need to get the correct result.
	SDL_Rect botton_n;
	SDL_Rect botton_h;
	SDL_Rect botton_p;
	SDL_Rect slidder_bar;
	SDL_Rect slidder_n;
	SDL_Rect slidder_h;
	SDL_Rect slidder_p;
	SDL_Rect image_fo;
	SDL_Rect image_fo_2 = {44,954,1900,1023};


private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;

	p2List<Ui_element*> UI_elements;
};

#endif // __j1GUI_H__