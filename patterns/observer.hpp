/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <functional> // std::function
#include <map>        // std::map
#include <vector>     // std::vector

/*
    Observer templated by TEvent.
    Users subscribe to events and get notified when those events are triggered
*/

template <typename TEvent>
class Observer
{
public:
    Observer();
    ~Observer();

    Observer(const Observer& other);
    Observer& operator=(const Observer& other);

    void subscribe(const TEvent& event, const std::function<void()>& lambda);
    void notify(const TEvent& event);

private:
    // A map that associates each event with a list of subscriber functions (lambdas)
    // '[](){ ... }' or lambdas are anonymous functions that can be stored and called later
    std::map<TEvent, std::vector<std::function<void()> > > _subscribers;
};

#include "observer.tpp"
    // * Note: Compiler needs to know the complete definition of template at the point of instantiation */
#endif
