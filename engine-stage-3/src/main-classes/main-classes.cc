#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

// #include "../constants/constants.hpp"
#include "../constants/enums.hpp"
#include "main-classes.hpp"

//====================
// METHODS IMPLEMENTATION

//====================
// ability

Ability::Ability() {
    return;
}

Ability::Ability(AbilityKind kind, std::map<AbilityState, int> init_state) {
    state_ = init_state;
    kind_  = kind;
}

int Ability::GetStateValue(AbilityState name) {
    return state_[name];
}

void Ability::SetStateValue(AbilityState name, int new_value) {
    state_[name] = new_value;
}

bool operator==(const Ability& left, const Ability& right) {
    return left.state_ == right.state_;
}

std::ostream& operator<<(std::ostream& stream, Ability ability) {
    // stream << ability.kind_ << ": ";

    for (const auto& s : ability.state_)
        stream << s.second << " ";

    return stream;
}

//===================
// coordinates

Coordinates::Coordinates(int x, int y) {
    x_ = x;
    y_ = y;
}

int Coordinates::GetX() {
    return x_;
}

int Coordinates::GetY() {
    return y_;
}

void Coordinates::SetCoordinates(int x_new, int y_new) {
    x_ = x_new;
    y_ = y_new;
}

//====================
// entity

Entity::Entity(EntityKind kind, Entity* parent, std::vector<Ability> abilities, std::vector<Entity> subentities, Coordinates coordinates) {
    subentities_ = subentities;
    coordinates_ = coordinates_;
    parent_      = parent;
    kind_        = kind;

    id_ = sole::uuid1();

    if (parent != nullptr)
        parent->subentities_.push_back(*this); // Check if works

    for (const auto& ability : abilities)
        abilities_[ability.kind_] = ability;
}

AbilityResult Entity::Apply(AbilityKind kind, Entity& target) {
    if (abilities_.count(kind) != 0)
        return dict_ability_dispatcher[kind](*this, target);
    else
        return AbilityResult::OriginDoesntHaveAbility;
}

Entity* Entity::GetParentTile() {
    if (parent_ == nullptr)
        throw std::string("no appropriate parent");

    if (parent_->kind_ == EntityKind::Tile)
        return parent_;
    else
        return parent_->GetParentTile();
}

int Entity::GetSubEntitiesCount() {
    return subentities_.size();
}

void Entity::AddSubentity(std::vector<Entity> items) {
    if (abilities_.count(AbilityKind::CanContain) == 0)
        return;

    for (const Entity& item : items) {
        bool can_fit       = subentities_.size() < abilities_[AbilityKind::CanContain].GetStateValue(AbilityState::ContainCapacity);
        int  can_be_picked = item.abilities_.count(AbilityKind::CanBePicked);

        if (can_fit && can_be_picked != 0)
            subentities_.push_back(item);
        else if (!can_fit)
            break;
    }

    return;
}

void Entity::RemoveSubentity(size_t index) {
    if (abilities_.count(AbilityKind::CanContain) == 0)
        return;

    if (index < subentities_.size())
        subentities_.erase(subentities_.begin() + index);

    return;
}

Entity* Entity::GetSubentity(size_t index) {
    int subentities_count = GetSubEntitiesCount();

    if (index < subentities_count)
        return &(subentities_[index]);
    else
        return nullptr;
}

Coordinates Entity::GetCoordinates() {
    return coordinates_;
}

void Entity::SetCoordinates(const Coordinates& new_pos) {
    coordinates_ = new_pos;
}

std::ostream& operator<<(std::ostream& stream, Entity& entity) {
    for (const auto& ability : entity.abilities_)
        stream << "AbilityKind - " /*<< ability.first*/ << ", Ability:" << ability.second << std::endl;

    return stream;
}

bool operator==(Entity& entity1, Entity& entity2) {
    return entity1.id_ == entity2.id_;
}
