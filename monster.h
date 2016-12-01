// Łukasz Raszkiewicz, Adam Sobecki

#ifndef MONSTER_H
#define MONSTER_H

#include <algorithm>
#include <cassert>
#include <type_traits>

#include "citizen.h"

enum class monster {
    ZOMBIE, VAMPIRE, MUMMY
};

template <typename T, monster,
          typename = std::enable_if_t<std::is_arithmetic<T>::value, T>>
class Monster {
public:
    typedef T valueType;

    template <typename U = T,
              typename = std::enable_if_t<std::is_arithmetic<T>::value, U>>
    Monster(T health, T attack_power)
            : _health(health), _attack_power(attack_power) {
        assert(health >= static_cast<T>(0)
               && attack_power >= static_cast<T>(0));
    }

    T getHealth() const { return _health; }

    T getAttackPower() const { return _attack_power; }

    void takeDamage(T damage) {
        _health = std::max(_health - damage, static_cast<T>(0));
    }

private:
    T _health, _attack_power;
};

template <typename T>
using Zombie = Monster<T, monster::ZOMBIE>;

template <typename T>
using Vampire = Monster<T, monster::VAMPIRE>;

template <typename T>
using Mummy = Monster<T, monster::MUMMY>;

template <typename T1, monster m, typename T2, citizen c>
void attack(Monster<T1, m>& monster, Citizen<T2, c>& victim) {
    victim.takeDamage(monster.getAttackPower());
}

template <typename T1, monster m, typename T2>
void attack(Monster<T1, m>& monster, Citizen<T2, citizen::SHERIFF>& victim) {
    if (victim.getHealth() > 0)
        monster.takeDamage(victim.getAttackPower());
    victim.takeDamage(monster.getAttackPower());
}

#endif  // MONSTER_H
