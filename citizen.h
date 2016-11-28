#ifndef CITIZEN_H
#define CITIZEN_H

#include <iostream>
#include <cassert>

using namespace std;

enum class citizen {
    ADULT, TEENAGER, SHERIFF
};

template <typename T, citizen C>
class Citizen {
    static_assert(is_arithmetic<T>::value);
public:
    template<typename U = T, typename =
        enable_if_t<C == citizen::TEENAGER || C == citizen::ADULT, U>>
    Citizen(T health, T age) : _health(health), _age(age) {
        if (C == citizen::TEENAGER)
            assert(static_cast<T>(11) <= age && age <= static_cast<T>(17));
        else
            assert(static_cast<T>(18) <= age && age <= static_cast<T>(100));
    }
    template<typename U = T, typename = enable_if_t<C == citizen::SHERIFF, U>>
    Citizen(T health, T age, T attack_power)
            : _health(health), _age(age), _attack_power(attack_power) {
        assert(static_cast<T>(18) <= age && age <= static_cast<T>(100));
    }
    T getHealth() { return _health; }
    T getAge() { return _age; }
    template<typename U = T, typename = enable_if_t<C == citizen::SHERIFF, U>>
    T getAttackPower() { return _attack_power; }
    void takeDamage(T damage) {
        _health = max(_health - damage, static_cast<T>(0));
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

#endif