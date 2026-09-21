/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_thread_safe_iostream.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_thread_safe_iostream.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include "../iostream/thread_safe_iostream.hpp"

static void printFromThread(const std::string& prefix, int lines)
{
    threadSafeCout.setPrefix(prefix);
    for (int i = 0; i < lines; ++i)
        threadSafeCout << "message " << i << " from " << prefix << std::endl;
}

void testThreadSafeIOStream()
{
    std::cout << "\n--- THREAD SAFE IOSTREAM TEST ---\n" << std::endl;

    /*
        TEST 1:
        Prefix + operator<< with several types in one line.
    */
    std::cout << "Testing prefix and operator<<..." << std::endl;

    threadSafeCout.setPrefix("[Main] ");
    threadSafeCout << "Hello " << 42 << " " << 3.5 << std::endl;

    /*
        TEST 2:
        Embedded newline flushes each line with its prefix.
    */
    std::cout << "\nTesting embedded newline..." << std::endl;

    threadSafeCout << "line one\nline two\n";

    /*
        TEST 3:
        prompt() and operator>> reading from redirected stdin.
    */
    std::cout << "\nTesting prompt and operator>>..." << std::endl;

    std::istringstream fakeInput("Alice 30");
    std::streambuf* savedCin = std::cin.rdbuf(fakeInput.rdbuf());

    std::string name;
    int age = 0;

    threadSafeCout.setPrefix("[Prompt] ");
    threadSafeCout.prompt("Enter your name: ", name);
    threadSafeCout << "Hello " << name << std::endl;
    threadSafeCout >> age;
    threadSafeCout << "Age: " << age << std::endl;

    std::cin.rdbuf(savedCin);

    /*
        TEST 4:
        Several threads printing at once. No line may come out mixed:
        every line must start with one of the thread prefixes.
    */
    std::cout << "\nTesting concurrent threads (4 threads x 25 lines)..." << std::endl;

    std::vector<std::thread> threads;

    threads.push_back(std::thread(printFromThread, "[T1] ", 25));
    threads.push_back(std::thread(printFromThread, "[T2] ", 25));
    threads.push_back(std::thread(printFromThread, "[T3] ", 25));
    threads.push_back(std::thread(printFromThread, "[T4] ", 25));

    for (std::vector<std::thread>::iterator it = threads.begin();
        it != threads.end(); ++it)
    {
        it->join();
    }

    threadSafeCout.setPrefix("[Main] ");
    threadSafeCout << "All threads finished" << std::endl;

    std::cout << "\n--- END THREAD SAFE IOSTREAM TEST ---\n" << std::endl;
}
