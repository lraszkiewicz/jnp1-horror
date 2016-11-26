#ifndef CITIZEN_H
#define CITIZEN_H

#include <iostream>
#include <cassert>

using namespace std;

enum citizen {
    adult, teenager, sheriff
};

template <typename T, citizen>
class Citizen;

template <typename T>
using Adult = Citizen<T, adult>;
template <typename T>
using Teenager = Citizen<T, teenager>;
template <typename T>
using Sheriff = Citizen<T, sheriff>;

template <typename T>
class Citizen<T, adult> {
public:
    Citizen<T, adult>(T health, T age) : _health(health), _age(age) {
        assert((T)18 <= age && age <= (T)100);
    }
    T getHealth() { return _health; }
    T getAge() { return _age; }
    void takeDamage(T damage) { _health = max(_health - damage, (T)0); }
private:
    T _health, _age;
};

template <typename T>
class Citizen<T, teenager> {
public:
    Citizen<T, teenager>(T health, T age) : _health(health), _age(age) {
        assert((T)11 <= age && age <= (T)17);
    }
    T getHealth() { return _health; }
    T getAge() { return _age; }
    void takeDamage(T damage) { _health = max(_health - damage, (T)0); }
private:
    T _health, _age;
};

template <typename T>
class Citizen<T, sheriff> {
public:
    Citizen<T, sheriff>(T health, T age, T attack_power)
            : _health(health), _age(age), _attack_power(attack_power) {
        assert((T)18 <= age && age <= (T)100);
    }
    T getHealth() { return _health; }
    T getAge() { return _age; }
    T getAttackPower() { return _attack_power; }
    void takeDamage(T damage) { _health = max(_health - damage, (T)0); }
private:
    T _health, _age, _attack_power;
};

#endif