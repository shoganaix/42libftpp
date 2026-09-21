/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_memento.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_memento.hpp"

#include <iostream>
#include <string>
#include "../patterns/memento.hpp"

class Player : public Memento
{
    friend class Memento;
public:
    Player()
        : _health(100), _name("Hero")
    {
    }

    Player(int health, const std::string& name)
        : _health(health), _name(name)
    {
    }

    void show() const
    {
        std::cout
            << "Player: " << _name
            << " | Health: " << _health
            << std::endl;
    }

    void damage(int amount)
    {
        _health -= amount;
    }

private:
    void _saveToSnapshot(Memento::Snapshot& snapshot) const
    {
        snapshot << _health << _name;
    }

    void _loadFromSnapshot(Memento::Snapshot& snapshot)
    {
        snapshot >> _health >> _name;
    }

    int _health;
    std::string _name;
};

void testMemento()
{
    std::cout << "\n--- MEMENTO TEST ---\n" << std::endl;

    Player player(100, "Hero");

    std::cout << "Initial state..." << std::endl;
    player.show();

    std::cout << "\nSaving checkpoint..." << std::endl;
    Memento::Snapshot checkpoint = player.save();

    std::cout << "Damaging player..." << std::endl;
    player.damage(80);
    player.show();

    std::cout << "\nRestoring checkpoint..." << std::endl;
    player.load(checkpoint);
    player.show();

    std::cout << "\nCheckpoint is reusable, damaging again..." << std::endl;
    player.damage(30);
    player.show();

    std::cout << "Restoring same checkpoint again..." << std::endl;
    player.load(checkpoint);
    player.show();

    std::cout << "\n--- END MEMENTO TEST ---\n" << std::endl;
}
