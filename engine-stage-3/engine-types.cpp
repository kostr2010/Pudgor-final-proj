#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

#include "engine-constants.h"
#include "engine-types.h"

//====================
// METHODS IMPLEMENTATION

//====================
// ability

Ability::Ability() { return; }

Ability::Ability(AbilityKind kind, std::map<AbilityState, int> init_state) {
    state_ = init_state;
    kind_  = kind;

    return;
}

int Ability::GetStateValue(AbilityState name) { return state_[name]; }

void Ability::SetStateValue(AbilityState name, int new_value) {
    state_[name] = new_value;

    return;
}

bool operator==(const Ability& left, const Ability& right) { return left.state_ == right.state_; }

std::ostream& operator<<(std::ostream& stream, Ability ability) {
    stream << ability.kind_ << ": ";

    for (const auto& s : ability.state_)
        stream << s.second << " ";

    return stream;
}

//====================
// entity

Entity::Entity() { return; }

void Entity::SetSubentities(std::vector<Entity> subentities) { subentities_ = subentities; }

void Entity::SetAbilities(std::vector<Ability> abilities) {
    for (const auto& ability : abilities)
        abilities_[ability.kind_] = ability;
}

Entity::Entity(std::vector<Entity> subentities) { SetSubentities(subentities); }

Entity::Entity(std::vector<Ability> abilities) { SetAbilities(abilities); }

Entity::Entity(std::vector<Entity> subentities, std::vector<Ability> abilities) {
    SetSubentities(subentities);
    SetAbilities(abilities);
}

void Entity::Apply(AbilityKind kind, Entity& target) {
    if (abilities_.count(kind) != 0)
        dict_ability_dispatcher[kind](*this, target);

    return;
}

int Entity::InventoryGetSize() {
    if (abilities_.count(CanContain) == 0)
        return 0;
    else
        return subentities_.size();
}

void Entity::InventoryAdd(std::vector<Entity> items) {
    if (abilities_.count(CanContain) == 0)
        return;

    for (const Entity& item : items) {
        bool can_fit       = subentities_.size() < abilities_[CanContain].GetStateValue(ContainCapacity);
        int  can_be_picked = item.abilities_.count(CanBePicked);

        if (can_fit && can_be_picked != 0)
            subentities_.push_back(item);
        else if (!can_fit)
            break;
    }

    return;
}

void Entity::InventoryRemove(size_t index) {
    if (abilities_.count(CanContain) == 0)
        return;

    if (index < subentities_.size())
        subentities_.erase(subentities_.begin() + index);

    return;
}

Entity Entity::InventoryGetSubentity(size_t index) { return (index < InventoryGetSize()) ? (subentities_[index]) : (Entity()); }

std::ostream& operator<<(std::ostream& stream, Entity entity) {
    for (const auto& ability : entity.abilities_)
        stream << "AbilityKind - " << ability.first << ", Ability:" << ability.second << std::endl;

    return stream;
}

//====================
// ability factory

Ability AbilityFactory::CreateAbilityDie(size_t hp_max, size_t hp_current) {
    Ability ability_die = Ability(CanDie, {{HpMax, hp_max}, {HpCurrent, hp_current}});

    return ability_die;
}

Ability AbilityFactory::CreateAbilityKick(size_t damage) {
    Ability ability_kick = Ability(CanKick, {{DamageAmount, damage}});

    return ability_kick;
}

Ability AbilityFactory::CreateAbilityPick() {
    Ability ability_pick = Ability(CanPick, {});

    return ability_pick;
}

Ability AbilityFactory::CreateAbilityLoot() {
    Ability ability_pick = Ability(CanLoot, {{SpotToLoot, 0}});

    return ability_pick;
}

Ability AbilityFactory::CreateAbilityBePicked() {
    Ability ability_be_picked = Ability(CanBePicked, {});

    return ability_be_picked;
}

Ability AbilityFactory::CreateAbilityContain(size_t capacity) {
    Ability ability_contain = Ability(CanContain, {{ContainCapacity, capacity}});

    return ability_contain;
}

Ability AbilityFactory::CreateAbilityMove() {
    Ability ability_move = Ability(CanMove, {});

    return ability_move;
}

Ability AbilityFactory::CreateAbilityHack(size_t hack_level) {
    Ability ability_hack = Ability(CanHack, {{HackLevel, hack_level}});

    return ability_hack;
}

Ability AbilityFactory::CreateAbilityBeLocked(size_t lock_level) {
    Ability ability_be_locked = Ability(CanBeLocked, {{LockLevel, lock_level}});

    return ability_be_locked;
}

//====================
// character factory

Entity EntityFactory::CreateWarrior() {
    Ability ability_die     = AbilityFactory::CreateAbilityDie(10, 10);
    Ability ability_kick    = AbilityFactory::CreateAbilityKick(4);
    Ability ability_pick    = AbilityFactory::CreateAbilityPick();
    Ability ability_loot    = AbilityFactory::CreateAbilityLoot();
    Ability ability_contain = AbilityFactory::CreateAbilityContain(10);
    Ability ability_hack    = AbilityFactory::CreateAbilityHack(1);
    Ability ability_move    = AbilityFactory::CreateAbilityMove();

    return Entity({ability_die, ability_kick, ability_pick, ability_loot, ability_contain, ability_hack, ability_move});
}
/*
Entity EntityFactory::CreateMage() {

}

Entity EntityFactory::CreateCustom() {

}
*/

Entity EntityFactory::CreateChest(size_t capacity, size_t init_lock_lvl) {
    Ability ability_contain   = AbilityFactory::CreateAbilityContain(capacity);
    Ability ability_die       = AbilityFactory::CreateAbilityDie(100, 100);
    Ability ability_be_locked = AbilityFactory::CreateAbilityBeLocked(init_lock_lvl);

    return Entity({ability_die, ability_contain});
}

/*
Entity EntityFactory::CreateMimic() {

}
*/

Entity EntityFactory::CreateFood() {
    Ability ability_be_picked = AbilityFactory::CreateAbilityBePicked();

    return Entity({ability_be_picked});
}

Entity EntityFactory::CreateTile() {
    // for now tile can contain up to 1 entity (may be changed)
    Ability ability_contain = AbilityFactory::CreateAbilityContain(1);

    return Entity({ability_contain});
}

Entity EntityFactory::CreateMap(size_t width, size_t height) {
    std::vector<Entity> tiles;
    tiles.reserve(height * width);

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            tiles[y * width + x] = CreateTile();

    tiles.shrink_to_fit();

    return Entity(tiles);
}