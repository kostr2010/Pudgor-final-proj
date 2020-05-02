#pragma once

#include "../component.hpp"
#include <iostream>

// #include "../../../libs/json/json.hpp"
// using json = nlohmann::json;

struct ComponentHealth {
  int hp_max, hp_cur;
};

// void to_json(json& j, const ComponentHealth& p) {
//   j = json{{"hp_max", p.hp_max}, {"hp_cur", p.hp_cur}};
// }

// void from_json(const json& j, ComponentHealth& p) {
//   j.at("hp_max").get_to(p.hp_max);
//   j.at("hp_cur").get_to(p.hp_cur);
// }

std::ostream& operator<<(std::ostream& os, ComponentHealth comp_health) {
  return os << "max: " << comp_health.hp_max << " cur: " << comp_health.hp_cur;
}