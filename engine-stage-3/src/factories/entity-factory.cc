#include "entity-factory.hpp"
#include "ability-factory.hpp"

//====================
// CONSTANTS

//====================
// heroes

const int WARRIOR_BASE_HP   = 10;
const int WARRIOR_BASE_DMG  = 4;
const int WARRIOR_BASE_CAP  = 10;
const int WARRIOR_BASE_HACK = 1;
const int WARRIOR_BASE_MOV  = 6;

//====================
// enemies

//====================
// containers

const int CHEST_BASE_CAP = 20;
const int CHEST_BASE_HP  = 100;

//====================
// items

//====================
// misc

const int TILE_BASE_CAP         = 5;
const int TILE_BASE_WALKABILITY = 0;

//====================
// METHODS IMPLEMENTATION

//====================
// entity factory

//====================
// heroes

Entity EntityFactory::CreateWarrior(Entity* parent) {
    Ability ability_die            = AbilityFactory::CreateAbilityDie(WARRIOR_BASE_HP, WARRIOR_BASE_HP);
    Ability ability_kick           = AbilityFactory::CreateAbilityKick(WARRIOR_BASE_DMG);
    Ability ability_pick           = AbilityFactory::CreateAbilityPick();
    Ability ability_loot           = AbilityFactory::CreateAbilityLoot();
    Ability ability_contain        = AbilityFactory::CreateAbilityContain(WARRIOR_BASE_CAP);
    Ability ability_hack           = AbilityFactory::CreateAbilityHack(WARRIOR_BASE_HACK);
    Ability ability_move           = AbilityFactory::CreateAbilityMove(WARRIOR_BASE_MOV);
    Ability ability_consume        = AbilityFactory::CreateAbilityCanConsume();
    Ability ability_is_transparent = AbilityFactory::CreateAbilityIsTransparent();

    return Entity(
        EntityKind::Warrior,
        parent,
        {ability_die, ability_kick, ability_pick, ability_loot, ability_contain, ability_hack, ability_move, ability_consume, ability_is_transparent},
        {});
}

//====================
// enemies

/*
Entity EntityFactory::CreateMimic(Entity* parent) {
    return;
}
*/

//====================
// containers

Entity EntityFactory::CreateChest(Entity* parent, size_t init_lock_lvl, const std::vector<Entity>& subentites /*= {}*/) {
    Ability ability_contain        = AbilityFactory::CreateAbilityContain(CHEST_BASE_CAP);
    Ability ability_die            = AbilityFactory::CreateAbilityDie(CHEST_BASE_HP, CHEST_BASE_HP);
    Ability ability_be_locked      = AbilityFactory::CreateAbilityBeLocked(init_lock_lvl);
    Ability ability_is_transparent = AbilityFactory::CreateAbilityIsTransparent();

    return Entity(EntityKind::Chest, parent, {ability_die, ability_contain, ability_is_transparent}, subentites);
}

//====================
// items

Entity EntityFactory::CreateFood(Entity* parent) {
    Ability ability_be_picked      = AbilityFactory::CreateAbilityBePicked();
    Ability ability_is_consumable  = AbilityFactory::CreateAbilityIsConsumable();
    Ability ability_is_transparent = AbilityFactory::CreateAbilityIsTransparent();
    Ability ability_is_walkable    = AbilityFactory::CreateAbilityIsWalkable(0);

    return Entity(EntityKind::Food, parent, {ability_be_picked, ability_is_consumable, ability_is_transparent, ability_is_walkable});
}

//====================
// misc

Entity EntityFactory::CreateTile(Entity* parent, int pos_x, int pos_y, int walkability_level) {
    Ability ability_contain        = AbilityFactory::CreateAbilityContain(TILE_BASE_CAP);
    Ability ability_is_transparent = AbilityFactory::CreateAbilityIsTransparent();
    Ability ability_is_walkable    = AbilityFactory::CreateAbilityIsWalkable(0);
    Ability ability_is_positioned  = AbilityFactory::CreateAbilityIsPositioned(pos_x, pos_y);

    return Entity(EntityKind::Tile, parent, {ability_contain, ability_is_transparent, ability_is_walkable, ability_is_positioned});
}

Entity EntityFactory::CreateMap(size_t width, size_t height) { // FIXME поверхностное копирование, дед на прошлом занятии говорил
    Entity world_map = _CreateMapShell(width, height);

    _InitMap(world_map, width, height);

    return world_map;
}

Entity EntityFactory::_CreateMapShell(size_t width, size_t height) {
    Ability ability_be_map  = Ability(AbilityKind::IsMap, {{AbilityState::MapWidth, width}, {AbilityState::MapHeight, height}});
    Ability ability_contain = Ability(AbilityKind::CanContain, {{AbilityState::ContainCapacity, width * height + 1}});

    Entity world_map(EntityKind::Map, nullptr, {ability_be_map, ability_contain});

    return world_map;
}

void EntityFactory::_InitMap(Entity& map_shell, size_t width, size_t height) {
    std::vector<Entity> tiles;

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            tiles.push_back(CreateTile(&map_shell, x, y, 0));

    map_shell.subentities_ = tiles;

    return;
}

// I odn't really understand how data works in this function, and it doesn't work
// Entity& EntityFactory::CreateMap(size_t size_x, size_t size_y) {
//     Ability ability_contain = AbilityFactory::CreateAbilityContain(size_x * size_y);

//     Entity world_map(EntityKind::Map, nullptr, {ability_contain});

//     for (int y = 0; y < size_y; y++)
//         for (int x = 0; x < size_y; x++)
//             world_map.AddSubentity({CreateTile(&world_map, x, y, 0)});

//     return world_map;
// }