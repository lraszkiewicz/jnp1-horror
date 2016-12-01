#ifndef MONSTER_H
#define MONSTER_H

#include <algorithm>
#include <cassert>
#include <type_traits>

using std::enable_if_t;
using std::is_arithmetic;

enum class monster {
    ZOMBIE, VAMPIRE, MUMMY
};

template <typename T, monster>
class Monster {
public:
    typedef T valueType;
    template <typename U = T,
              typename = enable_if_t<is_arithmetic<T>::value, U>>
    Monster(T health, T attack_power)
            : _health(health), _attack_power(attack_power) {}
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
    victim.takeDamage(monster.getAttackPower());
    monster.takeDamage(victim.getAttackPower());
}

#endif  // MONSTER_H
