#pragma once

#include <algorithm>
#include <bitset>
#include <iostream>
#include <set>

const int MAX_ENTITIES   = 128;
const int MAX_COMPONENTS = 32;

// bitmap to keep track of assigned components to specific entities
typedef std::bitset<MAX_COMPONENTS> Signature;

// EntityId - just numeric Id
typedef int EntityId;

// ====================
// EntityManager
// keeps track of entities and their components(via signatures)

class EntityManager {
public:
  // EntityManager()  = default;
  ~EntityManager() = default;

  EntityManager() {
    for (int i = MAX_ENTITIES; i >= 0; i--)
      available_ids_.push_back(i);
  }

  EntityId CreateEntity() {
    auto id = available_ids_.back();
    available_ids_.pop_back();

    return id;
  }

  void RemoveEntity(EntityId entity) {
    // std::cout << "    > ent man RemoveEntity " << entity << "\n";

    available_ids_.push_back(entity);
    abilities_signatures_[entity].reset();
  }

  Signature GetSignature(EntityId entity) {
    return abilities_signatures_[entity];
  }

  Signature SetSignature(EntityId entity, Signature signature) {
    return abilities_signatures_[entity] = signature;
  }

  bool CheckIfEntityExists(EntityId entity_target) {
    return std::find(available_ids_.begin(), available_ids_.end(), entity_target) ==
           available_ids_.end();
  }

private:
  std::array<Signature, MAX_ENTITIES>
                        abilities_signatures_{}; // shows whenever an component is present for an entity // list!!!!
  std::vector<EntityId> available_ids_{}; // TODO: replace "set" with smth better
  int                   entities_alive_;
};
