#pragma once

#include <algorithm>
#include <vector>

#include "../../component/components/container.hpp"
#include "../system.hpp"

class SystemContainer : public System {
public:
  SystemContainer(Monitor* monitor) : System(monitor) {
  }

  ResponseCode Pick(EntityId entity_origin, EntityId entity_target) {
    // TODO remove position component from target

    ComponentContainer* origin_comp_contain =
        monitor_->GetComponent<ComponentContainer>(entity_origin);

    origin_comp_contain->subentities.push_back(entity_target);

    return ResponseCode::Success;
  }

  ResponseCode Drop(EntityId entity_origin, EntityId entity_target) {
    // TODO add position component to target

    ComponentContainer* origin_comp_contain =
        monitor_->GetComponent<ComponentContainer>(entity_origin);

    auto position = std::find(origin_comp_contain->subentities.begin(),
                              origin_comp_contain->subentities.end(),
                              entity_target);

    if (position == origin_comp_contain->subentities.end()) {
      LOG_LVL_SYSTEM_FAILURE(SystemContainer,
                             "entity " << entity_origin << " tries to drop an item "
                                       << entity_target << "it does not have");
      return ResponseCode::Failure;
    }

    origin_comp_contain->subentities.erase(position);

    return ResponseCode::Success;
  }

  ResponseCode Transfer(EntityId entity_from, EntityId entity_whom, EntityId entity_to) {
    ResponseCode code = Drop(entity_from, entity_whom);
    if (code != ResponseCode::Success)
      return code;

    return Pick(entity_to, entity_whom);
  }

  std::vector<ComponentType> GetRequiredComponentTypes() override {
    return {monitor_->RegisterComponent<ComponentContainer>()};
  }

  void RegisterDependentSystems() override {
  }
};