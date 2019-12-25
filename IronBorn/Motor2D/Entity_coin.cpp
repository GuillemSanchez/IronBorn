#include "Entity_coin.h"


Entity_coin::Entity_coin(ENTITY_TYPE type, p2Point<int> pos, int index) : Entity(type, pos, index)
{
	my_type = type;
	position = pos;
	my_index = index;
}

Entity_coin::~Entity_coin()
{
}

bool Entity_coin::Awake(pugi::xml_node & config)
{
	return false;
}

void Entity_coin::Draw(float dt)
{
}

bool Entity_coin::Start()
{
	return false;
}

bool Entity_coin::Update(float dt)
{
	return false;
}

bool Entity_coin::CleanUp()
{
	return false;
}

void Entity_coin::OnCollision(Collider * coin, Collider * colli)
{
}
