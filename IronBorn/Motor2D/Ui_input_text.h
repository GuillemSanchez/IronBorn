#ifndef __UI_INPUT_TEXT_H__
#define __UI_INPUT_TEXT_H__



#include "Ui_element.h"

struct _TTF_Font;


class Ui_input_text : public  Ui_element
{
private:
	p2SString sample_text;
	SDL_Texture* text_tex;
	SDL_Color my_color;
	_TTF_Font* my_font;
	int font_size;
	int _height;
	int _widht;
	

public:
	Ui_input_text(UI_type type, p2Point<int> pos, bool inter, p2SString text, int font_size = 20, SDL_Color _color = { 255,255,255,255 });
	~Ui_input_text();

	void Draw();
	void PostUpdate();
	void CleanUp();

public:
	SDL_Texture* new_text;
	p2SString my_text;




};

#endif // !__UI_INPUT_TEXT_H__

