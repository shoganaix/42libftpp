/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_observer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_observer.hpp"

#include <iostream>
#include <string>
#include "../patterns/observer.hpp"

void testObserver()
{
    std::cout << "\n--- OBSERVER TEST ---\n" << std::endl;

    Observer<std::string> observer;
    int counter = 0;

    observer.subscribe("click", [&counter]() {
        counter += 1;
        std::cout << "click listener 1 (counter=" << counter << ")" << std::endl;
    });
    observer.subscribe("click", [&counter]() {
        counter += 10;
        std::cout << "click listener 2 (counter=" << counter << ")" << std::endl;
    });
    observer.subscribe("hover", []() {
        std::cout << "hover listener" << std::endl;
    });

    std::cout << "Notifying click..." << std::endl;
    observer.notify("click");

    std::cout << "Notifying hover..." << std::endl;
    observer.notify("hover");

    std::cout << "Notifying unknown event (no-op expected)..." << std::endl;
    observer.notify("unknown");

    std::cout << "Final counter: " << counter << " (expected 11)" << std::endl;

    std::cout << "\n--- END OBSERVER TEST ---\n" << std::endl;
}
