#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

using namespace std;

class Component;
class Entity;
class Manager;

using ComponentID = size_t;
using Group = size_t;
inline ComponentID getNewComponentTypeID() {

	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
	static_assert (is_base_of<Component, T>::value, "");
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr size_t maxComponents = 32;
constexpr size_t maxGroups = 32;

using ComponentBitSet = bitset<maxComponents>;
using GroupBitSet = bitset<maxGroups>;

using ComponentArray = array<Component*, maxComponents>;

class Component {

public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity {

private:
	Manager& manager;
	bool active = true;
	vector<unique_ptr<Component>> components;

	ComponentArray ComponentArray;
	ComponentBitSet ComponentBitSet;
	GroupBitSet GroupBitSet;

public:
	Entity(Manager& mManager) : manager(mManager) {

	}
	void update() {

		for (auto& c : components) c->update();
	}

	void draw() {

		for (auto& c : components) c->draw();
	}
	bool isActive() const { return active; }
	void destroy() { active = false; }

	bool hasGroup(Group mGroup) {
		return GroupBitSet[mGroup];
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup) {

		GroupBitSet[mGroup] = false;
	}

	template <typename T> bool hasComponent() const {
		
		return ComponentBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs) {

		T* c(new T(forward<TArgs>(mArgs)...));
		c->entity = this;
		unique_ptr<Component> uPtr{ c };
		components.emplace_back(move(uPtr));

		ComponentArray[getComponentTypeID<T>()] = c;
		ComponentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template<typename T>T& getComponent() const {

		auto ptr(ComponentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

class Manager {

private:
	vector<unique_ptr<Entity>> entities;
	array<vector<Entity*>, maxGroups> groupedEntities;
public:
	void upDate() {

		for (auto& e : entities) e->update();
	}
	void draw() {

		for (auto& e : entities) e->draw();
	}

	void refersh() {
		
		for (auto i(0u); i < maxGroups; i++) {
			auto& v(groupedEntities[i]);
			v.erase(remove_if(begin(v), end(v), [i](Entity* mEntity)
				{
					return !mEntity->isActive() || !mEntity->hasGroup(i);
				}),
				end(v));
		}
		entities.erase(remove_if(begin(entities), end(entities),
			[](const unique_ptr<Entity>& mEntity) {
			return !mEntity->isActive();
		}),
		end(entities));
	}

	void AddToGroup(Entity* mEntity, Group mGroup) {

		groupedEntities[mGroup].emplace_back(mEntity);
	}

	vector<Entity*>& getGroup(Group mGroup) {

		return groupedEntities[mGroup];
	}

	Entity& addEntity() {

		Entity* e = new Entity(*this);
		unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(move(uPtr));
		return *e;
	}
};