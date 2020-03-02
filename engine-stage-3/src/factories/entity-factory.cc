#include "entity-factory.hpp"
#include "ability-factory.hpp"

#include "../main-classes/main-classes.hpp"

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
        {},
        parent->GetCoordinates());
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

Entity EntityFactory::CreateChest(Entity* parent, size_t init_lock_lvl) {
    Ability ability_contain        = AbilityFactory::CreateAbilityContain(CHEST_BASE_CAP);
    Ability ability_die            = AbilityFactory::CreateAbilityDie(CHEST_BASE_HP, CHEST_BASE_HP);
    Ability ability_be_locked      = AbilityFactory::CreateAbilityBeLocked(init_lock_lvl);
    Ability ability_is_transparent = AbilityFactory::CreateAbilityIsTransparent();

    return Entity(EntityKind::Chest, parent, {ability_die, ability_contain, ability_is_transparent}, {}, parent->GetCoordinates());
}

//====================
// items

Entity EntityFactory::CreateFood(Entity* parent) {
    Ability ability_be_picked      = AbilityFactory::CreateAbilityBePicked();
    Ability ability_is_consumable  = AbilityFactory::CreateAbilityIsConsumable();
    Ability ability_is_transparent = AbilityFactory::CreateAbilityIsTransparent();
    Ability ability_is_walkable    = AbilityFactory::CreateAbilityIsWalkable(0);

    return Entity(EntityKind::Food,
                  parent,
                  {ability_be_picked, ability_is_consumable, ability_is_transparent, ability_is_walkable},
                  {},
                  parent->GetCoordinates());
}

//====================
// misc

Entity EntityFactory::CreateTile(Entity* parent, Coordinates pos, int walkable_level) {
    Ability ability_contain        = AbilityFactory::CreateAbilityContain(TILE_BASE_CAP);
    Ability ability_is_transparent = AbilityFactory::CreateAbilityIsTransparent();
    Ability ability_is_walkable    = AbilityFactory::CreateAbilityIsWalkable(0);

    return Entity(EntityKind::Tile, parent, {ability_contain, ability_is_transparent, ability_is_walkable}, {}, pos);
}

Entity EntityFactory::CreateMapShell() {
    Entity world_map(EntityKind::Map, nullptr);

    return world_map;
}

Entity EntityFactory::InitMap(Entity* mapShell, size_t width, size_t height) {
    std::vector<Entity> tiles;
    tiles.reserve(height * width);

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            // tiles[y * width + x] = CreateTile(mapShell, {x, y}); // ???????????????
            tiles.push_back(CreateTile(mapShell, {x, y}, 0));

    tiles.shrink_to_fit(); // needed?

    mapShell->subentities_ = tiles;

    return *mapShell;
}
