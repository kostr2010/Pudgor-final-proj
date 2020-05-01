#pragma once

#include <iostream>

// enum class TerrainType {
//   Metal,
//   Stone,
// };

struct ComponentTerrain {
  int  step_cost;
  bool walkable;
  bool poison;
  bool fire;
  bool bless;
};

// void to_json(json& j, const ComponentTerrain& p) {
//   // j = json{{"hp_max", p.hp_max}, {"hp_cur", p.hp_cur}};
// }

// void from_json(const json& j, ComponentTerrain& p) {
//   // j.at("hp_max").get_to(p.hp_max);
//   // j.at("hp_cur").get_to(p.hp_cur);
// }

std::ostream& operator<<(std::ostream& os, ComponentTerrain terrain) {
  return os << "walkable: " << terrain.walkable << " step_cost: " << terrain.step_cost;
}