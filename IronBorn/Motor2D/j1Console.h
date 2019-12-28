#ifndef __j1CONSOLE_H__
#define __j1CONSOLE_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2DynArray.h"



struct _TTF_Font;
class Ui_input_text;
class Ui_ntext;

enum COMS
{
	God_Mode,
	quit,
	FPS,
	map,
	list,


	max
};


class j1Console : public j1Module
{
public:

	j1Console();
	~j1Console();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();



	COMS ReturnCommand(const char*);

	void ExecuteCommand(COMS);

	void Open_c();

	void Close_c();

	void Console_write_log(p2SString LOG);

private:
	Ui_input_text* my_input;
	bool console_opened;
	p2List<p2SString> logs;
	p2List<Ui_ntext*> text_logs;
	p2List<p2SString> comands;
	int comands_act = 0;


};

#endif