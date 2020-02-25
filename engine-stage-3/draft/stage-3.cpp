#include <iostream>
#include <vector>

#include "classes.h"

using namespace std;

int main() {
    CreateCharacter factory;

    Character warrior = factory.CreateWarrior();

    if (warrior.GetAbility(AbilityMove()) == Ability())
        cout << "can't move" << endl;
    else 
        cout << "can move" << endl;

    return 0;
}

