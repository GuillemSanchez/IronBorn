#include "j1EntityManager.h"

j1EntityManager::j1EntityManager()
{
	name.create("Entity Manager");
}

j1EntityManager::~j1EntityManager()
{
}

void j1EntityManager::Init()
{
}

bool j1EntityManager::Awake(pugi::xml_node &)
{
	return false;
}

bool j1EntityManager::Start()
{
	return false;
}

bool j1EntityManager::PreUpdate()
{
	return false;
}

bool j1EntityManager::Update(float dt)
{
	return false;
}

bool j1EntityManager::PostUpdate()
{
	return false;
}

bool j1EntityManager::CleanUp()
{
	return false;
}

void j1EntityManager::CreateEntity(Entity_type type, p2Point<int> pos)
{




}

void j1EntityManager::DestroyEntity(int index)
{
}
