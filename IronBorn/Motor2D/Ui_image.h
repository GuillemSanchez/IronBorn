#ifndef __UI_IMAGE_H__
#define __UI_IMAGE_H__

#include "Ui_element.h"
#include "j1Render.h"


class Ui_image : public Ui_element
{
public:
	Ui_image(UI_type type, p2Point<int> pos, bool inter, SDL_Rect at_pos);
	void CleanUp();
	void Draw();

public:

};


#endif // !__UI_IMAGE_H__

