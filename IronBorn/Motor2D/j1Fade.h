#ifndef __J1FADE_H__
#define __J1FADE_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1Fade : public j1Module
{
public:
	j1Fade();
	~j1Fade();

	bool Start();
	bool Update(float dt);
	bool FadeToBlack(float time = 1.0f);
	bool IsFading() const;

	bool finished;
	bool mid;

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
};

#endif //__MODULEFADETOBLACK_H__
