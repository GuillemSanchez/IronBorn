#ifndef __ENTITY_H__
#define __ENTITY_H__


#include "p2Point.h"



enum ENTITY_TYPE
{
	PLAYER,
	COIN,
	NONE
};

class Entity
{
public:

	Entity(ENTITY_TYPE type, p2Point<int> pos, int index) {
		my_type = type;
		my_index = index;
	}
	~Entity(){}


	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual void Draw(float dt)
	{

	}

	virtual void SetPos(iPoint new_pos)
	{

	}
	//variables

public:

	int my_index;
	ENTITY_TYPE my_type;
	bool active = false;
};





#endif // !__ENTITY_H__

