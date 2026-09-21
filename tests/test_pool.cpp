/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pool.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 17:39:26 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/21 18:08:33 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_pool.hpp"

#include <iostream>
#include <string>
#include <utility>
#include "../structs/pool.hpp"

class Enemy
{
public:
    Enemy(const std::string& name, int health): _name(name), _health(health)
    {
        std::cout
            << "[Enemy] Constructor: " << _name << "(health: " << _health << ")" << std::endl;
    }

    ~Enemy()
    {
        std::cout
            << "[Enemy] Destructor: "<< _name << std::endl;
    }

    void attack() const
    {
        std::cout << _name << " attacks!" << std::endl;
    }

    void show() const
    {
        std::cout
            << "Enemy: "<< _name<< " | Health: "<< _health << std::endl;
    }

private:
    std::string _name;
    int _health;
};


void testPool()
{
    std::cout << "\n--- POOL TEST ---\n" << std::endl;

    Pool<Enemy> pool;

    std::cout << "Resizing pool to 2 slots..." << std::endl;
    pool.resize(2);

    std::cout << "\nAcquiring first enemy..." << std::endl;

    {
        auto enemy1 = pool.acquire("Alien", 100);

        enemy1->show();
        enemy1->attack();

        std::cout << "\nAcquiring second enemy..." << std::endl;

        auto enemy2 = pool.acquire("Zombie", 200);

        enemy2->show();
        enemy2->attack();

        std::cout << "\nTrying to acquire a third enemy..." << std::endl;

        try
        {
            auto enemy3 = pool.acquire("Robot", 300);
            enemy3->show();
        }
        catch (const std::exception& e)
        {
            std::cout
                << "Expected exception: "
                << e.what()
                << std::endl;
        }
        std::cout << "\nLeaving inner scope..." << std::endl;
    }

    std::cout
        << "\nBoth slots should now be free."
        << std::endl;

    std::cout
        << "Acquiring another enemy to reuse a slot..."
        << std::endl;

    {
        auto enemy = pool.acquire("Reused Enemy", 500);
        enemy->show();
    }

    std::cout << "\nTesting move constructor..." << std::endl;

    {
        auto enemy1 = pool.acquire("Movable Enemy", 600);

        auto enemy2 = std::move(enemy1);

        enemy2->show();
    }

    std::cout << "\nTesting resize..." << std::endl;

    pool.resize(4);

    {
        auto enemy1 = pool.acquire("A", 10);
        auto enemy2 = pool.acquire("B", 20);
        auto enemy3 = pool.acquire("C", 30);

        enemy1->show();
        enemy2->show();
        enemy3->show();

        std::cout
            << "\nTrying to resize below active object count..."
            << std::endl;

        try
        {
            pool.resize(2);
        }
        catch (const std::exception& e)
        {
            std::cout
                << "Expected exception: "
                << e.what()
                << std::endl;
        }
    }

    std::cout << "\nShrinking pool to 2 slots..." << std::endl;

    pool.resize(2);

    std::cout << "\n--- END POOL TEST ---\n" << std::endl;
}
/*-
----------------------RESIZING-----------------------
    Pool<Enemy> pool
        │
        ▼
        resize(2)
        │
        ├─────────────┐
        ▼             ▼
        [FREE]         [FREE]

        │ acquire()
        ▼
        [Alien]        [FREE]
        ▲
        │
        enemy1

        │ acquire()
        ▼
        [Alien]       [Zombie]
        ▲             ▲
        │             │
        enemy1         enemy2

        │ acquire()
        ▼
        NO FREE SLOT
        │
        ▼
        exception


    enemy1/enemy2 leave the scope
            │
            ▼
        ~Object()
            │
            ▼
        release()
            │
            ▼
        [FREE]         [FREE]

-------------------MOVE CONSTRUCTOR---------------------
    pool.acquire("Movable Enemy", 600) (enemy1)
        │
        ▼
        enemy1
        ┌──────────┐
        │ _pool  ──┼────► Pool
        │ _object ─┼────► Movable Enemy
        └──────────┘


    auto enemy2 = std::move(enemy1)
        │
        ▼

        enemy1                 enemy2
        ┌──────────┐           ┌──────────┐
        │ _pool  ──┼──┐        │ _pool  ──┼────► Pool
        │ _object ─┼──┼──┐     │ _object ─┼────► Movable Enemy
        └──────────┘  │  │     └──────────┘
                    │  │
                    ▼  ▼
                nullptr

    enemy1 leaves the scope    enemy2 leaves the scope
        │                        │
        ▼                        ▼
        ~Object()                ~Object()
        │                        │
        ▼                        ▼
        nothing(nullptr)         release()
                                 │
                                 ▼
                                 [Movable Enemy] → [FREE]

-------------------MOVE ASSIGNMENT---------------------
        enemy1                   enemy2
            │                        │
            ▼                        ▼
    [Move Source]          [Move Destination]
        USED                     USED

    enemy2 = std::move(enemy1)
            │
    (enemy2 already has a pointer to an active object)
            │
            ▼
            release()
            │
            ▼
        [Move Destination]
            │
            ▼
            release()
            │
            ▼
            [FREE]


    enemy2 takes ownership of enemy1's pointers

        enemy1                   enemy2
            │                       │
        nullptr                     ▼
                                [Move Source]

            enemy2 leves the scope
                │
                ▼
                ~Object()
                │
                ▼
                release()
                │
                ▼
            [FREE] [FREE]

-----------------------RESIZING------------------------

                    Pool<Enemy>
                         │
                         ▼
                         ▼
                     resize(4)
                         │
                         ▼
             [FREE][FREE][FREE][FREE]
                         │
                  acquire A/B/C
                         │
                         ▼
                 [A] [B] [C] [FREE]
                         │
                     resize(2)
                         │
                         ▼
                3 active > 2 slots
                         │
                         ▼
                     EXCEPTION
                         │
                         ▼
                [A] [B] [C] [FREE]
                         │
                    scope ends
                         │
                         ▼
                     release
                         │
                         ▼
            [FREE][FREE][FREE][FREE]
                         │
                    resize(2)
                         │
                         ▼
                  [FREE][FREE]
*/