/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <cstddef>      // std::size_t
#include <stdexcept>    // std::runtime_error
#include <utility>      // std::forward, used to perfectly forward arguments to the constructor of TType

/*
    Singleton templated by TType (CRTP).
    Ensures TType has only one instance and provides access to it.
*/

template <typename TType>
class Singleton
{
public:
    static TType* instance();
    template <typename... TArgs>
    static void instantiate(TArgs&&... p_args);
    static void destroy();

protected:
    Singleton();
    ~Singleton();

private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static TType* _instance;
};

#include "singleton.tpp"

#endif
