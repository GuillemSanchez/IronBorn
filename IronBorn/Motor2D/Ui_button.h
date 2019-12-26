#ifndef __UI_BUTTON_H__
#define __UI_BUTTON_H__

#include "Ui_element.h"
#include "j1Module.h"



struct _TTF_Font;


class Ui_button : public Ui_element
{
protected:
	SDL_Rect on_hover_tex;
	SDL_Rect on_pressed_tex;
	SDL_Rect normal;

	p2SString Centered_text;
	SDL_Texture* Text_texture;
	p2Point<int> text_pos;
	SDL_Color text_color;
	_TTF_Font* font;

public:

	Ui_button(UI_type type, p2Point<int> pos, bool inter, p2SString centered_text, j1Module* listener, int font_size = 20, SDL_Color _color = { 255,255,255,255 });
	~Ui_button();

	void Draw();
	void ChangeText(p2SString new_text);
	void CleanUp();

	void CalculateCenter();



};





#endif // !__UI_BUTTON_H__

