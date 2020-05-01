#pragma once

#include "../../entity/entity.hpp"

#include <iostream>
#include <vector>

struct ComponentContainer {
  // FIXME dynamic array / std::array / []check vector size
  std::vector<EntityId> subentities;
  int                   capacity;
};

// void to_json(json& j, const ComponentContainer& p) {
//   // j = json{{"hp_max", p.hp_max}, {"hp_cur", p.hp_cur}};
// }

// void from_json(const json& j, ComponentContainer& p) {
//   // j.at("hp_max").get_to(p.hp_max);
//   // j.at("hp_cur").get_to(p.hp_cur);
// }

std::ostream& operator<<(std::ostream& os, ComponentContainer comp_container) {
  os << "subentities: ";
  for (const auto& entity : comp_container.subentities) {
    os << entity << " ";
  }

  return os;
}