#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../../libs/sole/sole.hpp"
#include "../constants/enums.hpp"

//====================
// CLASSES DECLARATION

//====================
// ability

class Ability {
  public:
    Ability();
    Ability(const AbilityKind& kind, const std::map<AbilityState, int>& init_state);

    int  GetStateValue(const AbilityState& state_name);
    void SetStateValue(const AbilityState& state_name, const int new_value);

    friend bool          operator==(const Ability& left, const Ability& right);
    friend std::ostream& operator<<(std::ostream& stream, Ability& ability);

    AbilityKind kind_;

  private:
    std::map<AbilityState, int> state_;

  protected:
};

//====================
// entity

class Entity {
  public:
    Entity(const EntityKind& kind, Entity* parent, const std::vector<Ability>& abilities = {}, const std::vector<Entity>& subentities = {});

    AbilityResult Apply(const AbilityKind& kind, Entity& target);
    int           GetSubEntitiesCount() const;
    void          AddSubentity(const std::vector<Entity>& items);
    void          RemoveSubentity(const size_t index);
    Entity*       GetSubentity(const size_t index);                                 // redo as Entity&
    Entity*       GetParentTile() const;                                            // redo as Entity&
    int           CheckIfHas(const Entity& target);                                 // -1 if not found, else index
    int           CheckIfInRange(const Entity& target, const AbilityKind& ability); // -1 if not in range, else index of tile

    friend std::ostream& operator<<(std::ostream& stream, Entity& entity);
    friend bool          operator==(const Entity& entity1, const Entity& entity2);

    sole::uuid                     id_;
    std::map<AbilityKind, Ability> abilities_;
    std::vector<Entity>            subentities_;
    Entity*                        parent_;
    EntityKind                     kind_;

  private:
  protected:
};
