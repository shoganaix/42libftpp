/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_singleton.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_singleton.hpp"

#include <iostream>
#include "../patterns/singleton.hpp"

class Config : public Singleton<Config>
{
    friend class Singleton<Config>;
public:
    int getValue() const
    {
        return _value;
    }

private:
    Config(int value)
        : _value(value)
    {
    }

    int _value;
};

void testSingleton()
{
    std::cout << "\n--- SINGLETON TEST ---\n" << std::endl;

    std::cout << "Instantiating with 42..." << std::endl;
    Singleton<Config>::instantiate(42);

    std::cout << "Value: " << Singleton<Config>::instance()->getValue() << std::endl;
    std::cout << "Same pointer twice: "
        << (Singleton<Config>::instance() == Singleton<Config>::instance() ? "yes" : "no")
        << std::endl;

    std::cout << "\nTrying second instantiate (exception expected)..." << std::endl;
    try
    {
        Singleton<Config>::instantiate(7);
        std::cout << "Unexpected: no exception" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Expected exception: " << e.what() << std::endl;
    }

    Singleton<Config>::destroy();

    std::cout << "\n--- END SINGLETON TEST ---\n" << std::endl;
}
