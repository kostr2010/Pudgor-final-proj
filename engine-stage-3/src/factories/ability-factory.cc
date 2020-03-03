#include "ability-factory.hpp"

//====================
// CONSTANTS

const int RANGE_PICK = 0;
const int RANGE_LOOT = 1;
const int RANGE_KICK = 1;
const int RANGE_HACK = 1;

//====================
// METHODS IMPLEMENTATION

//====================
// ability factory

Ability AbilityFactory::CreateAbilityDie(const size_t hp_max, const size_t hp_current) {
    Ability ability_die = Ability(AbilityKind::CanDie, {{AbilityState::HpMax, hp_max}, {AbilityState::HpCurrent, hp_current}});

    return ability_die;
}

Ability AbilityFactory::CreateAbilityContain(const size_t capacity) {
    Ability ability_contain = Ability(AbilityKind::CanContain, {{AbilityState::ContainCapacity, capacity}});

    return ability_contain;
}

Ability AbilityFactory::CreateAbilityBePicked() {
    Ability ability_be_picked = Ability(AbilityKind::CanBePicked, {});

    return ability_be_picked;
}

Ability AbilityFactory::CreateAbilityMove(const size_t movability) {
    Ability ability_move = Ability(AbilityKind::CanMove, {{AbilityState::Movability, movability}});

    return ability_move;
}

Ability AbilityFactory::CreateAbilityPick() {
    Ability ability_pick = Ability(AbilityKind::CanPick, {{AbilityState::Range, RANGE_PICK}});

    return ability_pick;
}

Ability AbilityFactory::CreateAbilityLoot() {
    Ability ability_pick = Ability(AbilityKind::CanLoot, {{AbilityState::SpotToLoot, 0}, {AbilityState::Range, RANGE_LOOT}});

    return ability_pick;
}

Ability AbilityFactory::CreateAbilityKick(const size_t damage) {
    Ability ability_kick = Ability(AbilityKind::CanKick, {{AbilityState::DamageAmount, damage}, {AbilityState::Range, RANGE_KICK}});

    return ability_kick;
}

Ability AbilityFactory::CreateAbilityHack(const size_t hack_level) {
    Ability ability_hack = Ability(AbilityKind::CanHack, {{AbilityState::HackLevel, hack_level}, {AbilityState::Range, RANGE_HACK}});

    return ability_hack;
}

Ability AbilityFactory::CreateAbilityBeLocked(const size_t lock_level) {
    Ability ability_be_locked = Ability(AbilityKind::CanBeLocked, {{AbilityState::LockLevel, lock_level}});

    return ability_be_locked;
}

Ability AbilityFactory::CreateAbilityIsMap(const size_t width, const size_t height) {
    Ability ability_be_map = Ability(AbilityKind::IsMap, {{AbilityState::MapWidth, width}, {AbilityState::MapHeight, height}});

    return ability_be_map;
}

Ability AbilityFactory::CreateAbilityIsTransparent() {
    Ability ability_is_transparent = Ability(AbilityKind::IsTransparent, {});

    return ability_is_transparent;
}

Ability AbilityFactory::CreateAbilityIsWalkable(const size_t walkability_level) {
    Ability ability_is_walkable = Ability(AbilityKind::IsWalkable, {{AbilityState::WalkabilityLevel, walkability_level}});

    return ability_is_walkable;
}

Ability AbilityFactory::CreateAbilityIsPositioned(const int pos_x, const int pos_y) {
    Ability ablility_is_positioned = Ability(AbilityKind::IsPositioned, {{AbilityState::PositionX, pos_x}, {AbilityState::PositionY, pos_y}});

    return ablility_is_positioned;
}

Ability AbilityFactory::CreateAbilityCanConsume() {
    Ability ability_consume = Ability(AbilityKind::CanConsume, {});

    return ability_consume;
}

Ability AbilityFactory::CreateAbilityIsConsumable() {
    Ability ability_is_consumable = Ability(AbilityKind::IsConsumable, {});

    return ability_is_consumable;
}
