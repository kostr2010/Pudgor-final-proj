#pragma once

#include "../factories/ability-factory.hpp"
#include "./enums.hpp"
#include <functional>
#include <map>

class Entity;
extern std::map<AbilityKind, std::function<AbilityResult(Entity& origin, Entity& target)>> dict_ability_dispatcher;
// FIXME: extern is buuuuuuuuuuutt style
