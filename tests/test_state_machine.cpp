/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_state_machine.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_state_machine.hpp"

#include <iostream>
#include <string>
#include "../patterns/state_machine.hpp"

void testStateMachine()
{
    std::cout << "\n--- STATE MACHINE TEST ---\n" << std::endl;

    StateMachine<std::string> machine;

    machine.addState("idle");
    machine.addState("run");
    machine.addAction("idle", []() {
        std::cout << "action: idle" << std::endl;
    });
    machine.addAction("run", []() {
        std::cout << "action: run" << std::endl;
    });
    machine.addTransition("idle", "run", []() {
        std::cout << "transition: idle -> run" << std::endl;
    });
    machine.addTransition("run", "idle", []() {
        std::cout << "transition: run -> idle" << std::endl;
    });

    std::cout << "update() before any state (exception expected)..." << std::endl;
    try
    {
        machine.update();
        std::cout << "Unexpected: no exception" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Expected exception: " << e.what() << std::endl;
    }

    machine.transitionTo("idle");
    machine.update();
    machine.transitionTo("run");
    machine.update();

    std::cout << "\nTransition to unknown state (exception expected)..." << std::endl;
    try
    {
        machine.transitionTo("fly");
        std::cout << "Unexpected: no exception" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Expected exception: " << e.what() << std::endl;
    }

    StateMachine<std::string> noAction;

    noAction.addState("lonely");
    noAction.transitionTo("lonely");

    std::cout << "\nupdate() without action (exception expected)..." << std::endl;
    try
    {
        noAction.update();
        std::cout << "Unexpected: no exception" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Expected exception: " << e.what() << std::endl;
    }

    std::cout << "\n--- END STATE MACHINE TEST ---\n" << std::endl;
}
