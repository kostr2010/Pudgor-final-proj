#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

#include "../constants/dict-ability-dispatcher.hpp"
#include "main-classes.hpp"

//====================
// METHODS IMPLEMENTATION

//====================
// ability

Ability::Ability() {
    return;
}

Ability::Ability(const AbilityKind& kind, const std::map<AbilityState, int>& init_state) {
    state_ = init_state;
    kind_  = kind;
}

int Ability::GetStateValue(const AbilityState& state_name) { // operator[]  conflicts with const because returns lvalue? aka type&
    return state_[state_name];
}

void Ability::SetStateValue(const AbilityState& state_name, const int new_value) {
    state_[state_name] = new_value;
}

bool operator==(const Ability& left, const Ability& right) {
    return left.state_ == right.state_;
}

std::ostream& operator<<(std::ostream& stream, Ability& ability) {
    switch (ability.kind_) {
    case AbilityKind::CanDie:
        stream << "CanDie: HpMax - " << ability.GetStateValue(AbilityState::HpMax) << ", HpCurrent - "
               << ability.GetStateValue(AbilityState::HpCurrent) << "\n";
        break;
    case AbilityKind::CanContain:
        stream << "CanContain: ContainCapacity - " << ability.GetStateValue(AbilityState::ContainCapacity) << "\n";
        break;
    case AbilityKind::CanBePicked:
        stream << "CanBePicked"
               << "\n";
        break;
    case AbilityKind::CanMove:
        stream << "CanMove: Movability - " << ability.GetStateValue(AbilityState::Movability) << "\n";
        break;
    case AbilityKind::CanPick:
        stream << "CanPick"
               << "\n";
        break;
    case AbilityKind::CanLoot:
        stream << "CanLoot: SpotToLoot - " << ability.GetStateValue(AbilityState::SpotToLoot) << "\n";
        break;
    case AbilityKind::CanKick:
        stream << "CanKick: DamageAmount - " << ability.GetStateValue(AbilityState::DamageAmount) << "\n";
        break;
    case AbilityKind::CanHack:
        stream << "CanHack: HackLevel - " << ability.GetStateValue(AbilityState::HackLevel) << "\n";
        break;
    case AbilityKind::CanBeLocked:
        stream << "CanBeLocked: LockLevel - " << ability.GetStateValue(AbilityState::LockLevel) << "\n";
        break;
    case AbilityKind::IsMap:
        stream << "IsMap: MapWidth - " << ability.GetStateValue(AbilityState::MapWidth) << ", MapHeight - "
               << ability.GetStateValue(AbilityState::MapHeight) << "\n";
        break;
    case AbilityKind::IsTransparent:
        stream << "IsTransparent"
               << "\n";
        break;
    case AbilityKind::IsWalkable:
        stream << "IsWalkable: Walkability - " << ability.GetStateValue(AbilityState::WalkabilityLevel) << "\n";
        break;
    case AbilityKind::IsPositioned:
        stream << "IsPositioned: PositionX - " << ability.GetStateValue(AbilityState::PositionX) << ", PositionY - "
               << ability.GetStateValue(AbilityState::PositionY) << "\n";
        break;
    case AbilityKind::CanConsume:
        stream << "CanConsume"
               << "\n";
        break;
    case AbilityKind::IsConsumable:
        stream << "IsConsumable"
               << "\n";
        break;
    default:
        stream << "unregistered ability\n";
    }

    return stream;
}

//====================
// entity

Entity::Entity(const EntityKind& kind, Entity* parent, const std::vector<Ability>& abilities /*= {}*/,
               const std::vector<Entity>& subentities /*= {}*/) {
    subentities_ = subentities;
    parent_      = parent;
    kind_        = kind;

    id_ = sole::uuid1(); // unique id for the entity

    if (parent != nullptr)
        parent->subentities_.push_back(*this); // check if works

    for (const auto& ability : abilities)
        abilities_[ability.kind_] = ability;
}

AbilityResult Entity::Apply(const AbilityKind& kind, Entity& target) {
    if (abilities_.count(kind) != 0)
        return dict_ability_dispatcher[kind](*this, target);
    else
        return AbilityResult::OriginDoesntHaveAbility;
}

Entity* Entity::GetParentTile() const {
    if (parent_ == nullptr)
        throw std::string("no appropriate parent");

    if (parent_->kind_ == EntityKind::Tile)
        return parent_;
    else
        return parent_->GetParentTile();
}

int Entity::GetSubEntitiesCount() const {
    return subentities_.size();
}

void Entity::AddSubentity(const std::vector<Entity>& items) {
    if (abilities_.count(AbilityKind::CanContain) == 0)
        return;

    for (const Entity& item : items) {
        bool can_fit       = subentities_.size() < abilities_[AbilityKind::CanContain].GetStateValue(AbilityState::ContainCapacity);
        bool can_be_picked = item.abilities_.count(AbilityKind::CanBePicked) != 0;

        if (can_fit && can_be_picked)
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
        return &(subentities_[index]); // & - non-const operator? when ttrying to add "const" to
    else
        return nullptr;
}

int Entity::CheckIfHas(const Entity& target) {
    // std::cout << id_ << std::endl;

    for (int i = 0; i < subentities_.size(); i++)
        if (target == subentities_[i])
            return i;

    return -1;
}

int Entity::CheckIfInRange(const Entity& target, const AbilityKind& ability) { // operator[] vs const method
    // std::cout << "target " << target.id_ << std::endl;

    Entity* parent_tile = this->GetParentTile();
    Entity* world_map   = parent_tile->parent_;

    int map_size_x = world_map->abilities_[AbilityKind::IsMap].GetStateValue(AbilityState::MapWidth);
    int map_size_y = world_map->abilities_[AbilityKind::IsMap].GetStateValue(AbilityState::MapHeight);
    int pos_x      = parent_tile->abilities_[AbilityKind::IsPositioned].GetStateValue(AbilityState::PositionX);
    int pos_y      = parent_tile->abilities_[AbilityKind::IsPositioned].GetStateValue(AbilityState::PositionY);

    Entity* parent_tile_t = target.GetParentTile();
    int     pos_x_t       = parent_tile_t->abilities_[AbilityKind::IsPositioned].GetStateValue(AbilityState::PositionX);
    int     pos_y_t       = parent_tile_t->abilities_[AbilityKind::IsPositioned].GetStateValue(AbilityState::PositionY);

    int range = abilities_[ability].GetStateValue(AbilityState::Range);

    // std::cout << "target " << pos_x_t << " " << pos_y_t << std::endl;
    // std::cout << "origin " << pos_x << " " << pos_y << "; range " << range << std::endl;

    // index of (x, y) = y * size_x + x

    for (int y = pos_y - range; y <= pos_y + range; y++) {
        for (int x = pos_x - range; x <= pos_x + range; x++) {
            if (x >= 0 && y >= 0 && x < map_size_x && y < map_size_y) {
                int res = world_map->subentities_[y * map_size_x + x].CheckIfHas(target);

                if (res != -1)
                    return res;
            }
        }
    }

    return -1;
}

std::ostream& operator<<(std::ostream& stream, Entity& entity) {
    switch (entity.kind_) {
    case (EntityKind::Warrior):
        stream << "Warrior: \n";
        break;
    case (EntityKind::Mimic):
        stream << "Mimic: \n";
        break;
    case (EntityKind::Tile):
        stream << "Tile: \n";
        break;
    case (EntityKind::Map):
        stream << "Map: \n";
        break;
    case (EntityKind::Chest):
        stream << "Chest: \n";
        break;
    case (EntityKind::Food):
        stream << "Food: \n";
        break;
    }

    stream << entity.id_ << "\n";
    /*
    stream << "Inventory: {\n";

    for (auto& item : entity.subentities_)
        stream << item;

    stream << "Inventory: }\n";
    */
    stream << "Abilities: {\n";
    for (auto& ability : entity.abilities_) {
        stream << ability.second;
    }
    stream << "Abilities: }\n";

    stream << "=======================\n";

    return stream;
}

bool operator==(const Entity& entity1, const Entity& entity2) {
    return entity1.id_ == entity2.id_;
}
