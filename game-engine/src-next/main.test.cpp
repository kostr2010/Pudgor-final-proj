#define CATCH_CONFIG_MAIN
#include "../libs/catch2/catch2.hpp"

// utils
#include "./monitor/monitor.hpp"
#include "./utils/dsl.hpp"
#include "./utils/log.hpp"
#include "./utils/response.hpp"

// systems
#include "./system/systems/systemContainer.hpp"
#include "./system/systems/systemHealth.hpp"
#include "./system/systems/systemKick.hpp"
#include "./system/systems/systemMovement.hpp"
#include "./system/systems/systemTerrain.hpp"

// components
#include "./component/components/health.hpp"
#include "./component/components/movement.hpp"

#include "./factories/factories.hpp"

// TEST_CASE("Serialization") {
//   ComponentHealth h1{.hp_max = 13, .hp_cur = 25};

//   json j = h1;

//   ComponentHealth h2 = j.get<ComponentHealth>();

//   REQUIRE(h2.hp_max == 13);
//   REQUIRE(h2.hp_cur == 25);

//   // std::ofstream file;
//   // file.open("../saves/test.json", std::ios_base::trunc);
// }

// TEST_CASE("Serialize component pack") {
//   ComponentPack<ComponentHealth> pack{};
//   pack.AddEntity(1, {.hp_max = 10, .hp_cur = 8});
//   pack.AddEntity(3, {.hp_max = 20, .hp_cur = 20});

//   json j = pack.Serialize();

//   ComponentPack<ComponentHealth> pack2{};
//   pack2.Deserialize(j);
// }

TEST_CASE("Kick") {
  // std::cout << "*\n" << std::endl;
  Monitor monitor{};
  // std::cout << "*\n" << std::endl;
  ComponentType comp_health_id  = monitor.RegisterComponent<ComponentHealth>();
  ComponentType comp_kick_id    = monitor.RegisterComponent<ComponentKick>();
  ComponentType comp_contain_id = monitor.RegisterComponent<ComponentContainer>();
  // std::cout << "*\n" << std::endl;
  monitor.RegisterSystem<SystemHealth>();
  SystemKick*      sys_kick = monitor.RegisterSystem<SystemKick>();
  SystemContainer* sys_cont = monitor.RegisterSystem<SystemContainer>();
  // std::cout << "*\n" << std::endl;
  EntityId ch1 = monitor.AddEntity();
  EntityId ch2 = monitor.AddEntity();
  // std::cout << "*\n" << std::endl;
  ComponentHealth ch1_health = {.hp_max = 10, .hp_cur = 10};
  ComponentHealth ch2_health = {.hp_max = 25, .hp_cur = 25};
  // std::cout << "*\n" << std::endl;
  ComponentContainer chest_inventory = {};

  ComponentKick ch1_kick = {.damage_amount = 3};
  // std::cout << "*\n" << std::endl;
  monitor.AttachComponent(ch1_health, ch1);
  monitor.AttachComponent(ch2_health, ch2);
  // std::cout << "*\n" << std::endl;
  monitor.AttachComponent(ch1_kick, ch1);
  // std::cout << "*\n" << std::endl;
  sys_kick->Kick(ch1, ch2);
  // std::cout << "*\n" << std::endl;
  ComponentHealth* ch1_health_after = monitor.GetComponent<ComponentHealth>(ch1);
  ComponentHealth* ch2_health_after = monitor.GetComponent<ComponentHealth>(ch2);
  // std::cout << "*\n" << std::endl;
  REQUIRE(ch1_health_after->hp_cur == 10);
  REQUIRE(ch2_health_after->hp_cur == 22);
  // std::cout << "*\n" << std::endl;
}

TEST_CASE("Transfer") {
  Monitor monitor{};

  ComponentType comp_contain_id = monitor.RegisterComponent<ComponentContainer>();

  SystemContainer* sys_cont = monitor.RegisterSystem<SystemContainer>();

  EntityId ch1   = monitor.AddEntity();
  EntityId chest = monitor.AddEntity();
  EntityId item  = monitor.AddEntity();

  ComponentContainer ch1_inventory   = {};
  ComponentContainer chest_inventory = {{item}};

  monitor.AttachProperty(item, Pickable);

  monitor.AttachComponent(chest_inventory, chest);
  monitor.AttachComponent(ch1_inventory, ch1);

  sys_cont->Transfer(chest, item, ch1);

  ComponentContainer* ch1_inventory_after   = monitor.GetComponent<ComponentContainer>(ch1);
  ComponentContainer* chest_inventory_after = monitor.GetComponent<ComponentContainer>(chest);

  // check if chest has no item
  REQUIRE(std::find(chest_inventory_after->subentities.begin(),
                    chest_inventory_after->subentities.end(),
                    item) == chest_inventory_after->subentities.end());

  // check if ch1 has item
  REQUIRE(std::find(ch1_inventory_after->subentities.begin(),
                    ch1_inventory_after->subentities.end(),
                    item) != ch1_inventory_after->subentities.end());
}

TEST_CASE("Move") {
  Monitor  monitor{};
  Monitor* monitor_ptr = &monitor;

  ComponentType comp_id_pos      = monitor.RegisterComponent<ComponentPosition>();
  ComponentType comp_id_movement = monitor.RegisterComponent<ComponentMovement>();
  ComponentType comp_id_terrain  = monitor.RegisterComponent<ComponentTerrain>();

  SystemMovement* sys_movement = monitor.RegisterSystem<SystemMovement>();
  monitor.RegisterSystem<SystemTerrain>();

  EntityFactory::SpawnTileFloorUsual(monitor_ptr, {0, 0});
  EntityFactory::SpawnTileFloorViscous(monitor_ptr, {1, 0});
  EntityFactory::SpawnTileFloorUsual(monitor_ptr, {0, 1});
  EntityFactory::SpawnTileWall(monitor_ptr, {1, 1});
  EntityFactory::SpawnTileFloorUsual(monitor_ptr, {2, 0});
  EntityFactory::SpawnTileFloorUsual(monitor_ptr, {2, 1});

  // map:
  // U V U
  // U W U

  EntityId ch = EntityFactory::SpawnMovec(monitor_ptr, {0, 0});
  REQUIRE(GET_STEPS_CUR(ch) == 2);
  // map:
  // * V U
  // U W U

  sys_movement->Move(ch, {1, 0});
  REQUIRE(GET_POS(ch) == Vec2{1, 0});
  REQUIRE(GET_STEPS_CUR(ch) == 1);
  // map:
  // U * U
  // U W U

  // failure because of (steps_cur == 1) < (step_cost == 2)
  auto code1 = sys_movement->Move(ch, {1, 0});
  REQUIRE(code1 == ResponseCode::Failure);
  REQUIRE(GET_POS(ch) == Vec2{1, 0});
  REQUIRE(GET_STEPS_CUR(ch) == 1);

  sys_movement->ResetCurrentSteps();
  REQUIRE(GET_STEPS_CUR(ch) == 2);

  sys_movement->Move(ch, {1, 0});
  REQUIRE(GET_POS(ch) == Vec2{2, 0});
  REQUIRE(GET_STEPS_CUR(ch) == 0);
  // map:
  // U V *
  // U W U

  // failure because of (steps_cur == 0)
  auto code2 = sys_movement->Move(ch, {0, 1});
  REQUIRE(code2 == ResponseCode::Failure);
  REQUIRE(GET_POS(ch) == Vec2{2, 0});
  REQUIRE(GET_STEPS_CUR(ch) == 0);

  sys_movement->ResetCurrentSteps();
  REQUIRE(GET_STEPS_CUR(ch) == 2);

  sys_movement->Move(ch, {0, 1});
  REQUIRE(GET_POS(ch) == Vec2{2, 1});
  REQUIRE(GET_STEPS_CUR(ch) == 1);
  // map:
  // U V U
  // U W *

  // failure because of Wall is not walkable
  auto code3 = sys_movement->Move(ch, {-1, 0});
  REQUIRE(code3 == ResponseCode::Failure);
  REQUIRE(GET_POS(ch) == Vec2{2, 1});
  REQUIRE(GET_STEPS_CUR(ch) == 1);
}