/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_data_buffer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 17:39:28 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/21 20:20:28 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_data_buffer.hpp"

#include "../structs/data_buffer.hpp"

#include <iostream>
#include <string>


class Position
{
public:
    Position()
        : x(0), y(0)
    {
    }

    Position(int xValue, int yValue): x(xValue), y(yValue)
    {
    }

    int x;
    int y;
};


std::ostream& operator<<(std::ostream& os, const Position& position)
{
    os << position.x << ' ' << position.y;
    return os;
}


std::istream& operator>>(std::istream& is, Position& position)
{
    is >> position.x >> position.y;
    return is;
}


void testDataBuffer()
{
    std::cout << "\n--- DATA BUFFER TEST ---\n" << std::endl;

    std::cout << "Testing basic types..." << std::endl;

    DataBuffer buffer;

    int originalNumber = 42;
    std::string originalText = "Hello";

    buffer << originalNumber;
    buffer << originalText;

    int recoveredNumber = 0;
    std::string recoveredText;

    buffer >> recoveredNumber;
    buffer >> recoveredText;

    std::cout
        << "Recovered number: "
        << recoveredNumber
        << std::endl;

    std::cout
        << "Recovered text: "
        << recoveredText
        << std::endl;

    std::cout << "\nTesting custom object..." << std::endl;

    DataBuffer positionBuffer;

    Position originalPosition(10, 20);

    positionBuffer << originalPosition;

    Position recoveredPosition;

    positionBuffer >> recoveredPosition;

    std::cout
        << "Recovered position: ("
        << recoveredPosition.x
        << ", "
        << recoveredPosition.y
        << ")"
        << std::endl;

    std::cout << "\nTesting empty read..." << std::endl;

    try
    {
        int value;
        positionBuffer >> value;

        std::cout
            << "Unexpected value: "
            << value
            << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout
            << "Expected exception: "
            << e.what()
            << std::endl;
    }

    std::cout << "\nTesting clear..." << std::endl;

    DataBuffer clearBuffer;

    clearBuffer << 123;
    clearBuffer << 456;

    std::cout
        << "Size before clear: "
        << clearBuffer.size()
        << std::endl;

    clearBuffer.clear();

    std::cout
        << "Size after clear: "
        << clearBuffer.size()
        << std::endl;

    std::cout
        << "Buffer empty: "
        << (clearBuffer.empty() ? "yes" : "no")
        << std::endl;

    std::cout << "\nTesting copy..." << std::endl;

    DataBuffer original;

    original << 100;

    DataBuffer copy(original);

    int originalValue = 0;
    int copiedValue = 0;

    original >> originalValue;
    copy >> copiedValue;

    std::cout
        << "Original value: "
        << originalValue
        << std::endl;

    std::cout
        << "Copied value: "
        << copiedValue
        << std::endl;

    std::cout << "\n--- END DATA BUFFER TEST ---\n" << std::endl;
}

/*-
----------------------TEST1----------------------

    DataBuffer buffer
           │
           ▼
        [ EMPTY ]
           │ buffer << 42
           ▼
        [ "42" ]
           │ buffer << "Hello"
           ▼
       
------>[ "42" ][ "Hello" ]
            ▲
            │ _readPosition
         buffer >> recoveredNumber
            │ 
            ▼
            recoveredNumber = 42

------>[ "42" ][ "Hello" ]
                  ▲
                  │ _readPosition
            buffer >> recoveredText
                  │      
                  ▼
                recoveredText = "Hello"

----------------------TEST2----------------------

    Position
    ┌─────────────┐
    │ x = 10      │
    │ y = 20      │
    └─────────────┘
           │
           │
           │ buffer << position
           ▼
    ┌─────────────────┐
    │     "10 20"     │
    └─────────────────┘
           ▲
           │
      _readPosition
           │ buffer >> recoveredPosition
           ▼
        "10 20"
           │
           │ operator>>
           ▼
    recoveredPosition
    ┌─────────────┐
    │ x = 10      │
    │ y = 20      │
    └─────────────┘
----------------------TEST3----------------------

     [ empty buffer ]
            ▲
            │
        _readPosition
           │  empty buffer >> value
           ▼
       NO DATA AVAILABLE
           │
           ▼
        exception

----------------------TEST4----------------------

        [ "123" ][ "456" ]
           │     clearBuffer.clear();
           ▼
        size()  = 0
        empty() = true

----------------------TEST5----------------------

        [ "100" ]
            ▲
            │
      _readPosition


    DataBuffer copy(original)
                  │COPY BUFFER + READ POSITION
                  ▼

    original                       copy

    [ "100" ]                    [ "100" ]
        ▲                            ▲
        │                            │
    readPosition                readPosition
        │ original >> value          │ copy >> value
        ▼                            ▼

    originalValue = 100          copiedValue = 100


    These two Data buffers contain independent copies of the
    serialized data and maintain their own read position

*/