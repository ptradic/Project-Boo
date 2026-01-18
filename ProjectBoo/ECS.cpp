#include "ECS.h"

void Entity::addGroup(Group mGroup) {

	GroupBitSet[mGroup] = true;
	manager.AddToGroup(this, mGroup);
}