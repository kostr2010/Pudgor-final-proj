#pragma once

#include "../component/components/container.hpp"
#include "../component/components/health.hpp"
#include "../component/components/kick.hpp"
#include "../component/components/movement.hpp"
#include "../component/components/position.hpp"
#include "../component/components/terrain.hpp"
#include "../monitor/monitor.hpp"
#include "../utils/vec2.hpp"

// warrior
const int HP_WARRIOR    = 10;
const int STEPS_WARRIOR = 2;
const int DMG_WARRIOR   = 3;
const int CAP_WARRIOR   = 5;

// chest
const int HP_CHEST  = 100;
const int CAP_CHEST = 16;

// dummy
const int HP_DUMMY = 1e6; // kek

// ====================
// EntityFactory
// static class that provides functions for simple object creation

class EntityFactory {
public:
  static EntityId SpawnWarrior(Monitor* monitor, Vec2 position) {
    EntityId warrior = monitor->AddEntity();

    ComponentHealth    health    = {.hp_max = HP_WARRIOR, .hp_cur = HP_WARRIOR};
    ComponentMovement  move      = {.steps_max = STEPS_WARRIOR, .steps_cur = STEPS_WARRIOR};
    ComponentPosition  pos       = {.pos = position};
    ComponentKick      kick      = {.damage_amount = DMG_WARRIOR};
    ComponentContainer container = {.subentities = {}, .capacity = CAP_WARRIOR};

    monitor->AttachComponent(pos, warrior);
    monitor->AttachComponent(health, warrior);
    monitor->AttachComponent(move, warrior);
    monitor->AttachComponent(kick, warrior);
    monitor->AttachComponent(container, warrior);

    return warrior;
  }

  static EntityId SpawnMovec(Monitor* monitor, Vec2 position) {
    EntityId movec = monitor->AddEntity();

    ComponentMovement movement = {.steps_max = 2, .steps_cur = 2};
    ComponentPosition pos      = {.pos = position};

    monitor->AttachComponent(movement, movec);
    monitor->AttachComponent(pos, movec);

    return movec;
  }

  static EntityId SpawnDummy(Monitor* monitor, Vec2 position) {
    EntityId dummy = monitor->AddEntity();

    ComponentHealth   health = {.hp_max = HP_DUMMY, .hp_cur = HP_DUMMY};
    ComponentPosition pos    = {.pos = position};

    monitor->AttachComponent(health, dummy);
    monitor->AttachComponent(pos, dummy);

    return dummy;
  }

  static EntityId SpawnChest(Monitor* monitor, Vec2 position) {
    EntityId chest = monitor->AddEntity();

    ComponentPosition  pos       = {.pos = position};
    ComponentHealth    health    = {.hp_max = HP_CHEST, .hp_cur = HP_CHEST};
    ComponentContainer container = {.subentities = {}, .capacity = CAP_CHEST};

    monitor->AttachComponent(pos, chest);
    monitor->AttachComponent(health, chest);
    monitor->AttachComponent(container, chest);

    return chest;
  }

  static EntityId SpawnTileFloorUsual(Monitor* monitor, Vec2 position) {
    EntityId tile = monitor->AddEntity();

    ComponentPosition pos     = {.pos = position};
    ComponentTerrain  terrain = {.step_cost = 1, .walkable = true};

    monitor->AttachComponent(pos, tile);
    monitor->AttachComponent(terrain, tile);

    return tile;
  }

  static EntityId SpawnTileFloorViscous(Monitor* monitor, Vec2 position) {
    EntityId tile = monitor->AddEntity();

    ComponentPosition pos     = {.pos = position};
    ComponentTerrain  terrain = {.step_cost = 2, .walkable = true};

    monitor->AttachComponent(pos, tile);
    monitor->AttachComponent(terrain, tile);

    return tile;
  }

  static EntityId SpawnTileWall(Monitor* monitor, Vec2 position) {
    EntityId tile = monitor->AddEntity();

    ComponentPosition pos     = {.pos = position};
    ComponentTerrain  terrain = {.walkable = false};

    monitor->AttachComponent(pos, tile);
    monitor->AttachComponent(terrain, tile);

    return tile;
  }
};