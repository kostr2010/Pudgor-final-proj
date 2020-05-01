#pragma once

#include "../../utils/vec2.hpp"
#include <iostream>

struct ComponentPosition {
  Vec2 pos{};
};

// void to_json(json& j, const ComponentPosition& p) {
//   // j = json{{"hp_max", p.hp_max}, {"hp_cur", p.hp_cur}};
// }

// void from_json(const json& j, ComponentPosition& p) {
//   // j.at("hp_max").get_to(p.hp_max);
//   // j.at("hp_cur").get_to(p.hp_cur);
// }

std::ostream& operator<<(std::ostream& os, ComponentPosition pos) {
  return os << "pos: " << pos.pos;
}