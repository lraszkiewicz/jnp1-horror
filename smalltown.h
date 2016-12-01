#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include "monster.h"

using std::enable_if_t;
using std::is_arithmetic;
using std::integral_constant;

namespace SmallTownH {

    template <typename U, size_t N>
    struct Fib {
        constexpr Fib() : fib() {
            fib[0] = static_cast<U>(0);
            fib[1] = static_cast<U>(1);
            for (size_t i = 2; i <= N; ++i)
                fib[i] = fib[i - 1] + fib[i - 2];
        }
        U fib[N + 1];
    };

    template <typename U, size_t N>
    class FibClass {
    public:
        static constexpr Fib<U, N> fibStruct = SmallTownH::Fib<U, N>();

        // Proof that the Fibonacci numbers are computed in compile time
        // (proves only when maxFibIndex() >= 6, so that fib[6] exists).
        // C++14 or newer only, not supported in C++11.
        static_assert(N < 6 || fibStruct.fib[6] == 8,
                      "Fibonacci numbers error.");
    };

    template <typename U, size_t N>
    constexpr Fib<U, N> FibClass<U, N>::fibStruct;
}

template <typename M, typename U, U t0, U t1, typename... C>
class SmallTown {
    static_assert(static_cast<U>(0) <= t0 && t0 <= t1,
                  "SmallTown: incorrect time parameters.");

public:
    template <typename V = U,
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
        } else if (std::binary_search(fibs_begin, fibs_end, _currentTime)) {
            attackCitizens(std::make_index_sequence<sizeof...(C)>());
        }
        _currentTime = (_currentTime + timeStep) % (t1 + static_cast<U>(1));
    }

private:
    M _monster;
    U _currentTime = t0;
    std::tuple<C...> _citizens;
    size_t _aliveCitizens;

    template <typename T>
    void increaseAliveCitizens(T citizen) {
        if (citizen.getHealth() > 0)
            ++_aliveCitizens;
    }

    template <typename T, typename... Args>
    void increaseAliveCitizens(T citizen, Args... args) {
        increaseAliveCitizens(citizen);
        increaseAliveCitizens(args...);
    }

    template <size_t... Is>
    void countAliveCitizens(std::index_sequence<Is...>) {
        _aliveCitizens = 0;
        increaseAliveCitizens(std::get<Is>(_citizens)...);
    }

    template <typename T>
    void attackCitizen(T citizen) {
        attack(_monster, citizen);
    }

    template <typename T, typename... Args>
    void attackCitizen(T citizen, Args... args) {
        attackCitizen(citizen);
        attackCitizen(args...);
    }

    template <size_t... Is>
    void attackCitizens(std::index_sequence<Is...>) {
        attackCitizen(std::get<Is>(_citizens)...);
    }

    template <typename T, monster>
    static std::string monsterType(Monster<T, monster::ZOMBIE> const& m) {
        return "Unknown monster";
    }

    template <typename T>
    static std::string monsterType(Monster<T, monster::ZOMBIE> const& m) {
        return "Zombie";
    }

    template <typename T>
    static std::string monsterType(Monster<T, monster::VAMPIRE> const& m) {
        return "Vampire";
    }

    template <typename T>
    static std::string monsterType(Monster<T, monster::MUMMY> const& m) {
        return "Mummy";
    }

    // value1 == fib(i - 1), value2 == fib(i - 2)
    static constexpr size_t maxFibIndexRec(size_t i, U value1, U value2) {
        return
            (value1 + value2 < value1 // integer overflow
             || value1 + value2 > t1) // exceeded maximum value
            ? i - 1
            : maxFibIndexRec(i + 1, value1 + value2, value1);
    }

    static constexpr size_t maxFibIndex() {
        return maxFibIndexRec(2, static_cast<U>(1), static_cast<U>(0));
    }

    static constexpr auto fibs_begin =
        SmallTownH::FibClass<U, maxFibIndex()>::fibStruct.fib + 1;
        // + 1 because we don't treat 0 as a Fibonacci number
    static constexpr auto fibs_end = fibs_begin + maxFibIndex();

};

#endif  // SMALLTOWN_H
