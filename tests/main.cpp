/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 17:39:08 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/21 18:49:11 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "test_pool.hpp"
#include "test_data_buffer.hpp"
#include "test_memento.hpp"
#include "test_observer.hpp"
#include "test_singleton.hpp"
#include "test_state_machine.hpp"
#include "test_thread_safe_iostream.hpp"
#include "test_threading.hpp"

int main()
{
    std::cout << "==============================" << std::endl;
    std::cout << "        LIBFTPP TESTS          " << std::endl;
    std::cout << "==============================" << std::endl;

    testPool();
    testDataBuffer();
    testMemento();
    testObserver();
    testSingleton();
    testStateMachine();
    testThreadSafeIOStream();
    testThreading();

    return 0;
}