#pragma once

#include <iostream>

struct ComponentKick {
  int damage_amount;
};

// void to_json(json& j, const ComponentKick& p) {
//   // j = json{{"hp_max", p.hp_max}, {"hp_cur", p.hp_cur}};
// }

// void from_json(const json& j, ComponentKick& p) {
//   // j.at("hp_max").get_to(p.hp_max);
//   // j.at("hp_cur").get_to(p.hp_cur);
// }

std::ostream& operator<<(std::ostream& os, ComponentKick& comp_kick) {
  return os << "dmg: " << comp_kick.damage_amount;
}