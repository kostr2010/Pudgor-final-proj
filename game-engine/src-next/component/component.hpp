#pragma once

#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
// #include <memory>
#include <string>
#include <typeinfo>

#include "../entity/entity.hpp"
#include "../utils/assembly.hpp"
#include "../utils/log.hpp"

// #include "../../libs/json/json.hpp"
// using json = nlohmann::json;

typedef int ComponentType;

// struct IComponent {
//   virtual json        Serialize()         = 0;
//   virtual IComponent* Deserialize(json j) = 0;
// };

// ====================
// IComponentPack
// for componentManager to be able to tell abilities that the entity was destroyed

class IComponentPack {
public:
  virtual void RemoveEntity(EntityId entity) = 0;

  virtual bool Contains(EntityId entity) const = 0;

  // virtual json Serialize() = 0;

  // virtual void Deserialize(json j) = 0;
};

// ====================
// ComponentPack
// for us to be able to store entities that belong to each component

template <typename Component_t>
class ComponentPack : public IComponentPack {
public:
  // ComponentPack()  = default;
  // ~ComponentPack() = default;

  void AddEntity(EntityId entity, Component_t component) {
    components_[entity] = component;
  }

  void RemoveEntity(EntityId entity) override {
    auto entity_index = components_.find(entity);

    assertm(entity_index != components_.end(), "entity not found");

    components_.erase(entity);
  };

  Component_t* GetComponent(EntityId entity) {
    return &(components_[entity]);
  };

  bool Contains(EntityId entity) const override {
    return components_.find(entity) != components_.end();
  }

  // json Serialize() override {
  //   json j(components_);
  //   return j;
  // }

  // void Deserialize(json j) override {
  //   components_ = j.get<std::map<EntityId, Component_t>>();
  // };

private:
  // std::array<Component_t, MAX_ENTITIES> abilities_;
  // entity to index
  // index to entity
  std::map<EntityId, Component_t> components_; // TODO: replace with Packed Array
};

// ====================
// ComponentManager
// easy ERROR with all components

class ComponentManager {
public:
  ComponentManager() {
    LOG_LVL_COMPONENT_INIT();
  }

  ~ComponentManager() {
    for (auto& pair : component_packs_) {
      delete pair.second;
    }
  }

  template <typename Component_t>
  ComponentType RegisterComponent() {
    auto id = GetComponentId<Component_t>();

    assertm(component_packs_.find(id) == component_packs_.end(),
            "the component has already been registered");

    ComponentType comp_id = next_++;
    component_types_.insert({id, comp_id});

    // mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
    // std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

    // auto pack = std::make_shared<ComponentPack<Component_t>>();

    IComponentPack* pack = new ComponentPack<Component_t>{};

    // component_packs_.insert({id, std::static_pointer_cast<IComponentPack>(pack)});
    component_packs_.insert({id, pack});

    // TODO add try / catch
    LOG_LVL_COMPONENT_ROUTINE(ComponentManager,
                              "component " << typeid(Component_t).name() << " registered as "
                                           << comp_id);

    return comp_id;
  }

  template <typename Component_t>
  ComponentType GetComponentType() { // wrapper for private function
    auto id = GetComponentId<Component_t>();

    // TODO: assert if the component exists

    return component_types_[id];
  }

  template <typename Component_t>
  void AttachComponent(EntityId entity, Component_t component) {
    GetComponentPack<Component_t>()->AddEntity(entity, component);

    // TODO add try / catch
    LOG_LVL_COMPONENT_ROUTINE(ComponentManager,
                              "component " << typeid(Component_t).name() << "(" << component << ")"
                                           << " attached to entity " << entity);
  }

  template <typename Component_t>
  void RemoveComponent(EntityId entity) {
    GetComponentPack<Component_t>()->RemoveEntity(entity);

    // TODO add try / catch
    LOG_LVL_COMPONENT_ROUTINE(ComponentManager,
                              "component " << typeid(Component_t).name() << "removed from entity "
                                           << entity);
  }

  template <typename Component_t>
  bool HasComponent(EntityId entity) {
    IComponentPack* comp_pack = GetComponentPack<Component_t>();

    assertm(comp_pack != nullptr, "unregistered component");

    return comp_pack->Contains(entity);
  }

  template <typename Component_t>
  Component_t* GetComponent(EntityId entity) {
    return GetComponentPack<Component_t>()->GetComponent(entity);
  }

  template <typename Component_t>
  bool Contains() {
    return component_types_.find(GetComponentId<Component_t>()) != component_types_.end();
  }

  // tell each component_pack that an entity has been destroyed
  // if it has an component for that entity, it will remove it
  void RemoveEntity(EntityId entity) {
    // std::cout << "    > comp man RemoveEntity " << entity << "\n";

    for (auto& pair : component_packs_) {
      // const auto pack_type = pair.first;
      const auto pack = pair.second;

      if (pack->Contains(entity))
        pack->RemoveEntity(entity);
    }

    // TODO add try / catch
    LOG_LVL_COMPONENT_ROUTINE(ComponentManager,
                              "entity " << entity << " removed from all components");
  }

  void Print() {
    for (auto& pair : component_types_) {
      auto& comp = pair.second;
      std::cout << comp << std::endl;
    }
  }

private:
  std::map<std::string, ComponentType>   component_types_{};
  std::map<std::string, IComponentPack*> component_packs_{};

  ComponentType next_{};

  template <typename Component_t>
  inline std::string GetComponentId() {
    return std::string(typeid(Component_t).name());
  }

  template <typename Component_t>
  ComponentPack<Component_t>* GetComponentPack() {
    // TODO: check if the component is registeredd

    return (ComponentPack<Component_t>*)(component_packs_[GetComponentId<Component_t>()]);
  };
};
