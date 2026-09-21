/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.tpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

template <typename TType>
TType* Singleton<TType>::_instance = NULL;

template <typename TType>
Singleton<TType>::Singleton()
{
}

template <typename TType>
Singleton<TType>::~Singleton()
{
}

/*
    Returns the managed instance (NULL when not instantiated yet).
*/
template <typename TType>
TType* Singleton<TType>::instance()
{
    return _instance;
}

/*
    Builds the managed instance forwarding constructor arguments.
    Throws when the instance is already set.
*/
template <typename TType>
template <typename... TArgs>
void Singleton<TType>::instantiate(TArgs&&... p_args)
{
    if (_instance != NULL)
        throw std::runtime_error("Singleton: instance already set");
    _instance = new TType(std::forward<TArgs>(p_args)...);
}

/*
    Extra helper (not in the subject, allowed): releases the instance
    so tests stay leak-free. Safe to call with no active instance.
*/
template <typename TType>
void Singleton<TType>::destroy()
{
    delete _instance;
    _instance = NULL;
}
