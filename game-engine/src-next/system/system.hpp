#pragma once

#include <fstream>
#include <iostream>
#include <set>

#include "../entity/entity.hpp"
#include "../monitor/monitor.hpp"
#include "../utils/log.hpp"
#include "../utils/response.hpp"

// "A system is any functionality that iterates upon a list of entities with a certain signature of
// components."

// ====================
// System
// base class for each system - functionality that operate upon spacific set of entities based on
// their components set

class Monitor;

class System {
public:
  std::set<EntityId> entities_{};

  System(Monitor* monitor) {
    monitor_ = monitor;
  }

  virtual std::vector<ComponentType> GetRequiredComponentTypes() = 0;
  virtual void                       RegisterDependentSystems()  = 0;

protected:
  Monitor* monitor_;
};