#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "Ui_element.h"

struct _TTF_Font;


class Ui_ntext : public Ui_element
{
public:

	Ui_ntext(UI_type type, p2Point<int> pos, bool inter, p2SString text, int font_size = 20, SDL_Color _color = { 255,255,255,255 });
	void CleanUp();
	void Draw();
	void ChangeText(p2SString to_change);

private:

	p2SString my_text;
	SDL_Texture* text_tex;
	SDL_Color my_color;
	_TTF_Font* my_font;

};

#endif // !__UI_TEXT_H__

