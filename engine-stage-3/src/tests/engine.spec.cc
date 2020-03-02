#define CATCH_CONFIG_MAIN

#include "../../libs/catch2/catch2.hpp"
#include "../factories/ability-factory.hpp"
#include "../factories/entity-factory.hpp"
#include "../main-classes/main-classes.hpp"

#include <tuple>

TEST_CASE("Map behaviour", "[map]") {
    Entity world_map = EntityFactory::CreateMapShell();

    SECTION("map initialization") {
        EntityFactory::InitMap(&world_map, 10, 10);
    }
}

TEST_CASE("Entity behaviour", "[entity]") {
    Entity world_map = EntityFactory::CreateMapShell();
    EntityFactory::InitMap(&world_map, 10, 10);

    SECTION("initialization") {
        Entity en(Warrior, world_map.GetSubentity(0));
    }

    // SECTION("apply") {
    //     Entity en1(Test, world_map.GetSubentity(0));
    // }
}

TEST_CASE("Ability behaviour", "[ability]") {
    auto setup = []() -> std::tuple<Ability, Ability> {
        Ability ab1 = AbilityFactory::CreateAbilityDie(10, 9);
        Ability ab2 = AbilityFactory::CreateAbilityKick(4);

        return {ab1, ab2};
    };

    SECTION("get state value") {
        auto [ab1, ab2] = setup();

        REQUIRE(ab1.GetStateValue(HpMax) == 10);
        REQUIRE(ab1.GetStateValue(HpCurrent) == 9);

        REQUIRE(ab2.GetStateValue(DamageAmount) == 4);
    }

    SECTION("set state value") {
        auto [ab1, ab2] = setup();

        ab1.SetStateValue(HpMax, 3);
        ab1.SetStateValue(HpCurrent, 2);

        REQUIRE(ab1.GetStateValue(HpMax) == 3);
        REQUIRE(ab1.GetStateValue(HpCurrent) == 2);
    }
}