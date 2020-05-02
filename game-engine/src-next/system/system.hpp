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

// mortal and other called abilities will be pulled into the cache for this entity as well as for
// all of the entities near it in the component array, which are likely to be needed with the next
// entity in the list of entities.

// for (auto& entity : entities_) {
//     auto& mortal = GetComponent<Mortal>(entity);
//     mortal.health -= 1;
// }

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