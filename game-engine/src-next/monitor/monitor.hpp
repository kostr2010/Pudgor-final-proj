#pragma once

#include <cassert>
#include <vector>

#include "../component/component.hpp"
#include "../entity/entity.hpp"
#include "../map/map.hpp"

#include "../property/properties.hpp"
#include "../property/property.hpp"

#include "../system/system.hpp"
#include "../system/systemManager.hpp"

#include "../utils/assembly.hpp"
#include "../utils/log.hpp"
#include "../utils/vec2.hpp"

// ====================
// Monitor
// global manager for all managers

class Monitor {
public:
  Monitor() {
    LOG_LVL_MONITOR_INIT();

    component_manager_ = ComponentManager{};
    // TODO add try/catch here
    LOG_LVL_MONITOR_ROUTINE("ComponentManager initialized");

    entity_manager_ = EntityManager{};
    // TODO add try/catch here
    LOG_LVL_MONITOR_ROUTINE("EntityManager initialized");

    system_manager_ = SystemManager{this};
    // TODO add try/catch here
    LOG_LVL_MONITOR_ROUTINE("SystemManager initialized");
  }
  ~Monitor() = default;

  // wrappers for managers, so user has to speak only to monitor instead of directly to each one of
  // the managers

  template <typename Component_t>
  ComponentType RegisterComponent() {
    if (component_manager_.Contains<Component_t>())
      return component_manager_.GetComponentType<Component_t>();

    ComponentType component = component_manager_.RegisterComponent<Component_t>();

    // TODO add try/catch here
    LOG_LVL_MONITOR_ROUTINE("component " << typeid(Component_t).name() << " registered with id "
                                         << component);

    return component;
  }

  template <typename Component_t>
  void AttachComponent(Component_t& component_new, EntityId entity) {
    component_manager_.AttachComponent(entity, component_new);

    Signature     signature_prev = entity_manager_.GetSignature(entity);
    ComponentType component      = component_manager_.GetComponentType<Component_t>();

    Signature signature_new = signature_prev.set(component, true);

    UpdateSignature(entity, signature_new);

    // TODO add try/catch here
    LOG_LVL_MONITOR_ROUTINE("component " << typeid(Component_t).name() << " aka " << component
                                         << " attached to entity " << entity
                                         << ". now it's signature is "
                                         << entity_manager_.GetSignature(entity));
  }

  template <typename Component_t>
  void RemoveComponent(EntityId entity) {
    component_manager_.RemoveComponent<Component_t>(entity);

    Signature     signature_prev = entity_manager_.GetSignature(entity);
    ComponentType component      = component_manager_.GetComponentType<Component_t>();

    Signature signature_new = signature_prev.set(component, false);

    UpdateSignature(entity, signature_new);

    // TODO add try/catch here
    LOG_LVL_MONITOR_ROUTINE("component " << typeid(Component_t).name() << " aka " << component
                                         << " removed from entity " << entity
                                         << ". now it's signature is "
                                         << entity_manager_.GetSignature(entity));
  }

  EntityId AddEntity() {
    EntityId entity_new = entity_manager_.CreateEntity();

    // TODO add try/catch here
    LOG_LVL_MONITOR_ROUTINE("new entity with id " << entity_new << " registered");

    return entity_new;
  }

  void RemoveEntity(EntityId entity) {
    // std::cout << "  > start removing from entity manager\n";
    entity_manager_.RemoveEntity(entity);
    // std::cout << "  > start removing from component manager\n";
    component_manager_.RemoveEntity(entity);
    // std::cout << "  > start removing from system manager\n";
    system_manager_.RemoveEntity(entity);
    // std::cout << "  > removed from all systems\n";

    // TODO add try/catch here
    LOG_LVL_MONITOR_ROUTINE("entity " << entity << "deleted from all managers");
  }

  template <typename System_t>
  void SetSystemSignature(Signature& signature) {
    system_manager_.SetSignature<System_t>(signature);

    // TODO add try/catch here
    // LOG_LVL_MONITOR_ROUTINE("now ");
  }

  template <typename System_t>
  void SetSystemSignature(std::vector<ComponentType> components) {
    // merge all components to one signature
    Signature signature{};
    for (auto& component : components) {
      signature.set(component, true);
    }

    // set the system's initial signature
    SetSystemSignature<System_t>(signature);
  }

  template <typename System_t>
  System_t* RegisterSystem() {
    if (system_manager_.Contains<System_t>())
      return system_manager_.GetSystem<System_t>();

    // assertm(system_manager_.Contains<System_t>(), "this system has already been registered");
    System_t* system = system_manager_.RegisterSystem<System_t>();
    system->RegisterDependentSystems();

    SetSystemSignature<System_t>(system->GetRequiredComponentTypes());

    // TODO add try/catch here
    LOG_LVL_MONITOR_ROUTINE("new system " << typeid(System_t).name() << " registered");

    return system;
  }

  template <typename System_t>
  System_t* GetSystem() {
    return system_manager_.GetSystem<System_t>();
  }

  template <typename Component_t>
  bool HasComponent(EntityId entity) {
    return component_manager_.HasComponent<Component_t>(entity);
  }

  template <typename Component_t>
  bool HasNoComponent(EntityId entity) {
    return !HasComponent<Component_t>(entity);
  }

  template <typename Component_t>
  Component_t* GetComponent(EntityId entity) {
    return component_manager_.GetComponent<Component_t>(entity);
  }

  void AttachProperty(EntityId entity, PropertyType property) {
    property_manager_.AttachProperty(entity, property);
  }

  void RemoveProperty(EntityId entity, PropertyType property) {
    property_manager_.RemoveProperty(entity, property);
  }

  bool HasProperty(EntityId entity, PropertyType property) {
    return property_manager_.HasProperty(entity, property);
  }

  bool HasNoProperty(EntityId entity, PropertyType property) {
    return property_manager_.HasNoProperty(entity, property);
  }

  bool CheckIfEntityExists(EntityId entity) {
    return entity_manager_.CheckIfEntityExists(entity);
  }

  // EntityId GetTile(Vec2 pos) {
  //   SystemTerrain* sys_terrain = GetSystem<SystemTerrain>();

  //   for (const auto& entity : sys_terrain->entities_) {
  //     ComponentPosition* comp_pos = GetComponent<ComponentPosition>(entity);
  //     if (comp_pos->pos == pos)
  //       return entity;
  //   }

  //   assertm(false, "pos for the tile is out of range");
  // }

  // Terrain CellGetTerrain(Vec2 pos) {
  //   return map_->GetTerrain(pos);
  // }

  // bool CellContainsEntity(Vec2 pos, EntityId entity) {
  //   return map_->ContainsEntity(pos, entity);
  // }

  // void CellRemoveEntity(Vec2 pos, EntityId entity) {
  //   map_->RemoveEntity(pos, entity);
  // }

  // void CellAddEntity(Vec2 pos, EntityId entity) {
  //   map_->AddEntity(pos, entity);
  // }

  // bool CellIsWalkable(Vec2 pos) {
  //   return map_->IsWalkable(pos);
  // }

private:
  void UpdateSignature(EntityId entity, Signature& signature) {
    // std::cout << "*\n" << std::endl;
    entity_manager_.SetSignature(entity, signature);
    // std::cout << "*\n" << std::endl;
    system_manager_.UpdateEntitySignature(entity, signature);
    // std::cout << "*\n" << std::endl;

    // TODO add try/catch here
    LOG_LVL_MONITOR_ROUTINE("entity's " << entity << " signature updated. now it's signature is "
                                        << signature);
  }

  // Map*             map_{};
  PropertyManager  property_manager_{};
  ComponentManager component_manager_{};
  EntityManager    entity_manager_{};
  SystemManager    system_manager_{this};
};