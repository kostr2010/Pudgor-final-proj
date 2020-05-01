#pragma once

#include "../../component/components/movement.hpp"
#include "../../component/components/position.hpp"
#include "../../component/components/terrain.hpp"
#include "../system.hpp"

#include <algorithm>

class SystemTerrain : public System {
public:
  SystemTerrain(Monitor* monitor) : System(monitor) {
  }

  ResponseCode ApplyEffects() {
    return ResponseCode::Success;
  }

  ResponseCode LeaveTile(EntityId entity) {
    LOG_LVL_SYSTEM_ROUTINE(SystemTerrain, "entity " << entity << " tries to leave the tile");

    REQUIRE_COMPONENT(SystemTerrain, ComponentMovement, entity);
    REQUIRE_COMPONENT(SystemTerrain, ComponentPosition, entity);

    ComponentPosition* comp_pos = monitor_->GetComponent<ComponentPosition>(entity);
    EntityId           tile     = GetTile(comp_pos->pos);

    ComponentMovement* comp_movement = monitor_->GetComponent<ComponentMovement>(entity);
    ComponentTerrain*  comp_terrain  = monitor_->GetComponent<ComponentTerrain>(tile);

    if (comp_movement->steps_cur < comp_terrain->step_cost) {
      LOG_LVL_SYSTEM_FAILURE(SystemTerrain,
                             "entity " << entity << " has " << comp_movement->steps_cur
                                       << "steps avaliable, but " << comp_terrain->step_cost
                                       << " are needed to leave tile");
      return ResponseCode::Failure;
    }

    comp_movement->steps_cur -= comp_terrain->step_cost;

    LOG_LVL_SYSTEM_ROUTINE(SystemTerrain, "entity " << entity << " successfully left the tile");

    return ResponseCode::Success;
  }

  ResponseCode EnterTile(EntityId entity, Vec2 position_new) {
    LOG_LVL_SYSTEM_ROUTINE(SystemTerrain,
                           "entity " << entity << " tries to enter the tile " << position_new);

    // REQUIRE_COMPONENT(SystemTerrain, ComponentMovement, entity);
    // REQUIRE_COMPONENT(SystemTerrain, ComponentPosition, entity);

    // apply effects via if-es of tile.poison, tile.fire, etc.

    // TODO: assert that position_new is not out of range
    EntityId tile = GetTile(position_new);

    ComponentTerrain* comp_terrain = monitor_->GetComponent<ComponentTerrain>(tile);
    if (!comp_terrain->walkable) {
      LOG_LVL_SYSTEM_FAILURE(SystemTerrain,
                             "destination terrain for entity " << entity << " is unwalkable");

      return ResponseCode::Failure;
    }

    LOG_LVL_SYSTEM_ROUTINE(SystemTerrain,
                           "entity " << entity << " entered new tile " << position_new);

    return ResponseCode::Success;
  }

  std::vector<ComponentType> GetRequiredComponentTypes() override {
    return {monitor_->RegisterComponent<ComponentTerrain>()};
  }

private:
  EntityId GetTile(Vec2 pos) {
    for (const auto& entity : entities_) {
      ComponentPosition* comp_pos = monitor_->GetComponent<ComponentPosition>(entity);
      if (comp_pos->pos == pos)
        return entity;
    }

    assertm(false, "pos for the tile is out of range");
  }
};