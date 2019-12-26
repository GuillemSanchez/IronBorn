#ifndef __UI_SLIDDER_H__
#define	__UI_SLIDDER_H__


#include "Ui_element.h"



class Ui_slidder : public Ui_element
{
public:
	Ui_slidder(UI_type type, p2Point<int> pos, bool inter, j1Module* listener, bool horizontal);
	~Ui_slidder();

	void PostUpdate();
	void Draw();

	void CalculateLimits();


public:
	float slidder_value;

private:

	SDL_Rect on_pressed_;
	SDL_Rect normal;
	SDL_Rect on_hover;

	int size_of_limits = 0;

	int min_pos;
	int max_pos;

	SDL_Rect slidder_alue;

	bool horizontal;

	p2Point<int> dot;
};






#endif // !__UI_SLIDDER_H__

