#ifndef CITIZEN_H
#define CITIZEN_H

#include <algorithm>
#include <cassert>
#include <type_traits>

using std::enable_if_t;
using std::is_arithmetic;

enum class citizen {
    ADULT, TEENAGER, SHERIFF
};

template <typename T, citizen C>
class Citizen {
public:
    template<typename U = T, typename =
            enable_if_t<(C == citizen::TEENAGER ||
                         C == citizen::ADULT) &&
                         is_arithmetic<T>::value, U>>
    Citizen(T health, T age) : _health(health), _age(age) {
        if (C == citizen::TEENAGER)
            assert(static_cast<T>(11) <= age && age <= static_cast<T>(17));
        else
            assert(static_cast<T>(18) <= age && age <= static_cast<T>(100));
    }
    template<typename U = T, typename =
            enable_if_t<C == citizen::SHERIFF && is_arithmetic<T>::value, U>>
    Citizen(T health, T age, T attack_power)
            : _health(health), _age(age), _attack_power(attack_power) {
        assert(static_cast<T>(18) <= age && age <= static_cast<T>(100));
    }
    T getHealth() const { return _health; }
    T getAge() const { return _age; }
    template<typename U = T, typename = enable_if_t<C == citizen::SHERIFF, U>>
    T getAttackPower() { return _attack_power; }
    void takeDamage(T damage) {
        _health = std::max(_health - damage, static_cast<T>(0));
    }
private:
    T _health, _age, _attack_power;
};

template <typename T>
using Adult = Citizen<T, citizen::ADULT>;
template <typename T>
using Teenager = Citizen<T, citizen::TEENAGER>;
template <typename T>
using Sheriff = Citizen<T, citizen::SHERIFF>;

#endif  // CITIZEN_H
