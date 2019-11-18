#ifndef __J1ENTITYMANAGER__
#define __J1ENTITYMANAGER__

#include "j1Module.h"
#include "Entity.h"
#include "p2Point.h"




class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();


	// Module typical functions --------------------------------------

	void Init();
	
	// Called before render is available
	bool Awake(pugi::xml_node&);
	
	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Module typical functions --------------------------------------

	// Specific functions --------------------------------------------

	void CreateEntity(Entity_type type, p2Point<int> pos);

	void DestroyEntity(int index);

	// Specific functions --------------------------------------------




};
	












#endif
