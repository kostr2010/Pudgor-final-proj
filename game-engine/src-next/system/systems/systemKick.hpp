#pragma once

#include "../../component/components/health.hpp"
#include "../../component/components/kick.hpp"
#include "../system.hpp"
#include "./systemHealth.hpp"

class SystemKick : public System {
public:
  SystemKick(Monitor* monitor) : System(monitor) {
  }

  ResponseCode Kick(EntityId entity_origin, EntityId entity_target) {
    LOG_LVL_SYSTEM_ROUTINE(SystemKick,
                           "entity " << entity_origin << " attempts to kick " << entity_target);

    REQUIRE_COMPONENT(SystemKick, ComponentKick, entity_origin);
    REQUIRE_COMPONENT(SystemKick, ComponentHealth, entity_target);

    ComponentKick* comp_kick = monitor_->GetComponent<ComponentKick>(entity_origin);

    SystemHealth* sys_health = monitor_->GetSystem<SystemHealth>();

    LOG_LVL_SYSTEM_ROUTINE(SystemKick,
                           "entity " << entity_origin << " kicks entity " << entity_target
                                     << " for " << comp_kick->damage_amount
                                     << " points of damage!");

    sys_health->ChangeCurrentHp(entity_target, -1 * comp_kick->damage_amount);

    return ResponseCode::Success;
  }

  std::vector<ComponentType> GetRequiredComponentTypes() override {
    return {monitor_->RegisterComponent<ComponentKick>()};
  }

private:
  void RegisterDependentSystems() override {
    monitor_->RegisterSystem<SystemHealth>();
  }
};
