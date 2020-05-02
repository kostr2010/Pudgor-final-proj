#pragma once

#include "../../component/components/health.hpp"
// #include "../../monitor/monitor.hpp"
#include "../system.hpp"

class SystemHealth : public System {
public:
  SystemHealth()  = default;
  ~SystemHealth() = default;

  SystemHealth(Monitor* monitor) : System(monitor) {
  }

  ResponseCode ChangeCurrentHp(EntityId entity, int delta) {
    REQUIRE_COMPONENT(SystemHealth, ComponentHealth, entity);

    ComponentHealth* comp_health = monitor_->GetComponent<ComponentHealth>(entity);

    comp_health->hp_cur += delta;

    LOG_LVL_SYSTEM_ROUTINE(SystemHealth,
                           "entity's " << entity << " hp changed from "
                                       << comp_health->hp_cur - delta << " to "
                                       << comp_health->hp_cur << std::endl);

    return ResponseCode::Success;
  }

  ResponseCode ChangeMaximumHP(EntityId entity, int delta) {
    REQUIRE_COMPONENT(SystemHealth, ComponentHealth, entity);

    ComponentHealth* comp_health = monitor_->GetComponent<ComponentHealth>(entity);

    comp_health->hp_max += delta;

    LOG_LVL_SYSTEM_ROUTINE(SystemHealth,
                           "entity's " << entity << " max hp was changed from "
                                       << comp_health->hp_max - delta << " to "
                                       << comp_health->hp_max << std::endl);

    return ResponseCode::Success;
  }

  std::vector<ComponentType> GetRequiredComponentTypes() override {
    return {monitor_->RegisterComponent<ComponentHealth>()};
  }
};