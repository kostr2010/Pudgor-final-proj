#pragma once

#include <iostream>

struct ComponentVisual {
  char sprite;
};

// void to_json(json& j, const ComponentVisual& p) {
//   // j = json{{"hp_max", p.hp_max}, {"hp_cur", p.hp_cur}};
// }

// void from_json(const json& j, ComponentVisual& p) {
//   // j.at("hp_max").get_to(p.hp_max);
//   // j.at("hp_cur").get_to(p.hp_cur);
// }

std::ostream& operator<<(std::ostream& os, ComponentVisual vis_comp) {
  return os << "sprite: " << vis_comp.sprite;
}