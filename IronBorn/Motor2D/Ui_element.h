#ifndef __UI_ELEMENT_H__
#define __UI_ELEMENT_H__

#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "p2Log.h"

class j1Module;

enum Ky_St
{
	ST_PRESSED,
	ST_HOVER,
	ST_UP,
	ST_CONTINUE,


	ST_NOTHING
};

enum UI_type
{
	UTEXT,
	UIMAGE,
	UBUTTON,
	ULABEL,
	UINPUT_TEXT,
	USLIDDER,


	UUNKNOW
};

class Ui_element
{
protected:
	//vars that all have in common.
	Ky_St mouse_ev = ST_NOTHING;
	UI_type my_type = UUNKNOW;
	p2Point<int> my_pos;
	SDL_Rect inter_zone;

	//If we only have one position we will use this.
	SDL_Rect atlas_pos;


	bool interactable = false;
	p2List<j1Module*> listeners;
	
public:
	//functions:

	Ui_element() {}
	Ui_element(UI_type type, p2Point<int> pos, bool inter){}

	void Update();
	virtual void CleanUp() {} //gtodo hacer el cleanup.
	virtual void Draw(){}
	virtual void PostUpdate() {}
	Ky_St GetState();

	// This function adds a listener to the listeners list. So the module will get the information.
	void AddListener(j1Module* mod);

public:
	SDL_Texture* atlas_tex;
	//var that only one element has it true.
	bool focus = false;
};


#endif // !__UI_ELEMENT_H__





