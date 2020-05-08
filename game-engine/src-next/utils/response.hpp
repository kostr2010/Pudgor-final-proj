#pragma once

enum class ResponseCode {
  Success, // successfully done
  Failure, // code error
};

#define HAS_COMPONENT(entity, component) monitor_->HasComponent<component>(entity)
#define HAS_NO_COMPONENT(entity, component) monitor_->HasNoComponent<component>(entity)

#define REQUIRE_COMPONENT(system, component, entity)                                               \
  if (monitor_->HasNoComponent<component>(entity)) {                                               \
    LOG_LVL_SYSTEM_FAILURE(system, entity << " has no component " << typeid(component).name());    \
    return ResponseCode::Failure;                                                                  \
  }
