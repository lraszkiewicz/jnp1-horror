#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include <iostream>
#include <string>
#include <tuple>
#include <utility>

#include "monster.h"

using std::enable_if_t;
using std::is_arithmetic;

template <typename M, typename U, U t0, U t1, typename... C>
class SmallTown {
    static_assert(0 <= t0 && t0 <= t1, "SmallTown: incorrect time parameters.");

public:
    template<typename V = U,
             typename = enable_if_t<is_arithmetic<U>::value, V>>
    SmallTown(M monster, C... citizens)
            : _monster(monster), _citizens(citizens...) {
        _aliveCitizens = 0;
        countAliveCitizens(std::make_index_sequence<sizeof...(C)>());
    }

    std::tuple<std::string, typename M::valueType, size_t> getStatus() const {
        return std::make_tuple(
            monsterType(_monster),
            _monster.getHealth(),
            _aliveCitizens
        );
    }

    void tick(U timeStep) {
        if (_monster.getHealth() == static_cast<typename M::valueType>(0)) {
            if (_aliveCitizens > 0)
                std::cout << "CITIZENS WON\n";
            else
                std::cout << "DRAW\n";
        } else if (_aliveCitizens == 0) {
            std::cout << "MONSTER WON\n";
        } else if (_currentTime == 1 || _currentTime == 2) { // TODO: if (_currentTime is a Fibonacci number)
            attackCitizens(std::make_index_sequence<sizeof...(C)>());
        }
        _currentTime = (_currentTime + timeStep) % t1;
    }

private:
    M _monster;
    U _currentTime = t0;
    std::tuple<C...> _citizens;
    size_t _aliveCitizens;
    // size_t _aliveCitizens = std::tuple_size<std::tuple<C...>>::value;

    template<typename T>
    void increaseAliveCitizens(T citizen) {
        if (citizen.getHealth() > 0)
            _aliveCitizens++;
    }

    template<typename T, typename... Args>
    void increaseAliveCitizens(T citizen, Args... args) {
        increaseAliveCitizens(citizen);
        increaseAliveCitizens(args...);
    }

    template<size_t... Is>
    void countAliveCitizens(std::index_sequence<Is...>) {
        _aliveCitizens = 0;
        increaseAliveCitizens(std::get<Is>(_citizens)...);
    }

    template<typename T>
    void attackCitizen(T citizen) {
        attack(_monster, citizen);
    }

    template<typename T, typename... Args>
    void attackCitizen(T citizen, Args... args) {
        attackCitizen(citizen);
        attackCitizen(args...);
    }

    template<size_t... Is>
    void attackCitizens(std::index_sequence<Is...>) {
        attackCitizen(std::get<Is>(_citizens)...);
    }

    template<typename T, monster>
    static std::string monsterType(Monster<T, monster::ZOMBIE> const& m) {
        return "Unknown monster";
    }

    template<typename T>
    static std::string monsterType(Monster<T, monster::ZOMBIE> const& m) {
        return "Zombie";
    }

    template<typename T>
    static std::string monsterType(Monster<T, monster::VAMPIRE> const& m) {
        return "Vampire";
    }

    template<typename T>
    static std::string monsterType(Monster<T, monster::MUMMY> const& m) {
        return "Mummy";
    }
};

#endif  // SMALLTOWN_H
