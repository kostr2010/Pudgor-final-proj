#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../../libs/sole/sole.hpp"
#include "../constants/constants.hpp"
#include "../constants/enums.hpp"

//====================
// CLASSES DECLARATION

//====================
// ability

class Ability {
  public:
    Ability();
    Ability(AbilityKind kind, std::map<AbilityState, int> init_state);

    int  GetStateValue(AbilityState name);
    void SetStateValue(AbilityState name, int new_value);

    friend bool          operator==(const Ability& left, const Ability& right);
    friend std::ostream& operator<<(std::ostream& stream, Ability ability);

    AbilityKind kind_;

  private:
    std::map<AbilityState, int> state_;

  protected:
};

//====================
// coordinates

class Coordinates {
  public:
    Coordinates(int x = 0, int y = 0);

    int  GetX();
    int  GetY();
    void SetCoordinates(int x_new, int y_new);

  private:
    int x_, y_;

  protected:
};

//====================
// entity

class Entity {
  public:
    Entity(EntityKind kind, Entity* parent, std::vector<Ability> abilities = {}, std::vector<Entity> subentities = {}, Coordinates coordinates = {});

    AbilityResult Apply(AbilityKind kind, Entity& target);
    int           GetSubEntitiesCount();
    void          AddSubentity(std::vector<Entity> items);
    void          RemoveSubentity(size_t index);
    Entity*       GetSubentity(size_t index);
    Coordinates   GetCoordinates();
    void          SetCoordinates(const Coordinates& new_pos);
    Entity*       GetParentTile();
    bool          CheckIfInRange(const Entity& target) const;

    friend std::ostream& operator<<(std::ostream& stream, Entity& entity);
    friend bool          operator==(Entity& entity1, Entity& entity2);

    sole::uuid id_;

    std::map<AbilityKind, Ability> abilities_;
    std::vector<Entity>            subentities_;
    Entity*                        parent_;
    EntityKind                     kind_;

  private:
    Coordinates coordinates_;

  protected:
};
