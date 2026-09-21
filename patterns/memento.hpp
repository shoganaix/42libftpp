/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memento.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMENTO_HPP
#define MEMENTO_HPP

#include "../structs/data_buffer.hpp"

/*
    Stores current state of an object

    Must be inherited by the "saveable" class, which implements (private) methods:
        - void _saveToSnapshot(Memento::Snapshot& snapshot) const;
        - void _loadFromSnapshot(Memento::Snapshot& snapshot);
*/

class Memento
{
    public:
        class Snapshot 
        // Snapshot is the saved file. Provides serialization and deserialization 
        {
            public:
                Snapshot();
                ~Snapshot();

                Snapshot(const Snapshot& other);
                Snapshot& operator=(const Snapshot& other);

                template <typename TType>Snapshot& operator<<(const TType& data);
                template <typename TType>Snapshot& operator>>(TType& data);

            private:
                DataBuffer _data;
            };

        Memento();
        virtual ~Memento();
        // Virtual to ensure proper cleanup of derived classes

        Snapshot save() const;
        void load(const Snapshot& state);
        // Public interface for saving and loading state

    private:
        virtual void _saveToSnapshot(Snapshot& snapshot) const = 0;
        virtual void _loadFromSnapshot(Snapshot& snapshot) = 0;
        // Pure virtual methods to be implemented by derived classes
        // Memento is an abstract base class, cannot know details of derived classes
            // so it relies on vitual methods to delegate the actual saving and loading
};

// Implementation of template Snapshot::operator <<
template <typename TType>Memento::Snapshot& Memento::Snapshot::operator<<(const TType& data)
{
    _data << data;
    return *this;
}

// Implementation of template Snapshot::operator >>
template <typename TType>Memento::Snapshot& Memento::Snapshot::operator>>(TType& data)
{
    _data >> data;
    return *this;
}

#endif
