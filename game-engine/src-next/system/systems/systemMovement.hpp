#pragma once

#include "../../component/components/movement.hpp"
#include "../../component/components/position.hpp"
#include "../system.hpp"
#include "./systemTerrain.hpp"

#include <cmath>

class SystemMovement : public System {
public:
  SystemMovement(Monitor* monitor) : System(monitor) {
  }

  ResponseCode Move(EntityId entity, Vec2 direction) {
    if (std::abs(direction.x) + std::abs(direction.y) != 1) {
      LOG_LVL_SYSTEM_FAILURE(SystemMovement, "direction invalid " << direction);
      return ResponseCode::Failure;
    }

    REQUIRE_COMPONENT(SystemMovement, ComponentMovement, entity);
    REQUIRE_COMPONENT(SystemMovement, ComponentPosition, entity);

    SystemTerrain* sys_terrain = monitor_->GetSystem<SystemTerrain>();

    ComponentMovement* comp_movement = monitor_->GetComponent<ComponentMovement>(entity);
    ComponentPosition* comp_pos      = monitor_->GetComponent<ComponentPosition>(entity);

    int          steps_cur_before = comp_movement->steps_cur;
    ResponseCode resp_code_leave  = sys_terrain->LeaveTile(entity);

    if (resp_code_leave != ResponseCode::Success) {
      LOG_LVL_SYSTEM_FAILURE(SystemMovement, "entity " << entity << " unable to leave its tile");
      return resp_code_leave;
    }

    Vec2 position_new = comp_pos->pos + direction;
    // LOG_LVL_SYSTEM_ROUTINE(SystemMovement,
    //                        "entity " << entity << " tries to enter " << position_new
    //                                  << " (getting the tile)");

    ResponseCode resp_code_arrive = sys_terrain->EnterTile(entity, position_new);

    if (resp_code_arrive != ResponseCode::Success) {
      LOG_LVL_SYSTEM_FAILURE(SystemMovement,
                             "entity " << entity << " unable to enter new tile. returning back...");

      comp_movement->steps_cur = steps_cur_before;
      return resp_code_arrive;
    }

    comp_pos->pos = position_new;

    LOG_LVL_SYSTEM_ROUTINE(SystemMovement,
                           "entity " << entity << " stepped into direction " << direction);

    return ResponseCode::Success;
  }

  ResponseCode ResetCurrentSteps() {
    for (auto entity : entities_) {
      ComponentMovement* comp_move = monitor_->GetComponent<ComponentMovement>(entity);
      comp_move->steps_cur         = comp_move->steps_max;
    }

    LOG_LVL_SYSTEM_ROUTINE(SystemMovement, "steps for all entities were reset");

    return ResponseCode::Success;
  }

  std::vector<ComponentType> GetRequiredComponentTypes() override {
    return {monitor_->RegisterComponent<ComponentMovement>()};
  }

private:
  void RegisterDependentSystems() override {
    monitor_->RegisterSystem<SystemTerrain>();
  }
};
