/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memento.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memento.hpp"

Memento::Memento()
{
}

Memento::~Memento()
{
}

/*
    Builds snapshot by delegating serialization to the child class
*/
Memento::Snapshot Memento::save() const
{
    Snapshot snapshot;

    _saveToSnapshot(snapshot);
    return snapshot;
}

/*
    Restores a previously saved Snapshot.

    Note: A local copy is used because deserialization (>>) moves the
    read position, so const snapshot stays unchanged
*/
void Memento::load(const Snapshot& state)
{
    Snapshot copy(state);

    _loadFromSnapshot(copy);
}

Memento::Snapshot::Snapshot()
{
}

Memento::Snapshot::~Snapshot()
{
}

Memento::Snapshot::Snapshot(const Snapshot& other): _data(other._data)
{
}

Memento::Snapshot& Memento::Snapshot::operator=(const Snapshot& other)
{
    if (this != &other)
        _data = other._data;
    return *this;
}
