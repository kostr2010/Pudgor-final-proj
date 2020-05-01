#pragma once

#include <iostream>

struct ComponentMovement {
  int steps_max;
  int steps_cur;
};

// void to_json(json& j, const ComponentMovement& p) {
//   // j = json{{"hp_max", p.hp_max}, {"hp_cur", p.hp_cur}};
// }

// void from_json(const json& j, ComponentMovement& p) {
//   // j.at("hp_max").get_to(p.hp_max);
//   // j.at("hp_cur").get_to(p.hp_cur);
// }

std::ostream& operator<<(std::ostream& os, ComponentMovement comp_movement) {
  return os << "steps_max: " << comp_movement.steps_max << "steps_cur: " << comp_movement.steps_cur;
}