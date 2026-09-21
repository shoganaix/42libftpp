/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.tpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

template <typename TEvent>Observer<TEvent>::Observer()
{
}

template <typename TEvent>Observer<TEvent>::~Observer()
{
}

template <typename TEvent>Observer<TEvent>::Observer(const Observer& other): _subscribers(other._subscribers)
{
}

template <typename TEvent>Observer<TEvent>& Observer<TEvent>::operator=(const Observer& other)
{
    if (this != &other)
        _subscribers = other._subscribers;
    return *this;
}

/*
    Registers a lambda for a specific event
*/
template <typename TEvent>void Observer<TEvent>::subscribe(const TEvent& event, const std::function<void()>& lambda)
{
    _subscribers[event].push_back(lambda);
}

/*
    Executes all lambdas subscribed to the event
    If no lambda is subscribed to the event -> do nothing
*/
template <typename TEvent>void Observer<TEvent>::notify(const TEvent& event)
{
    typename std::map<TEvent, std::vector<std::function<void()>>>::iterator it;

    // Find the event in the map of subscribers
    it = _subscribers.find(event);
    // If the event is not found, return early
    if (it == _subscribers.end())
        return;

    // Iterate through all lambdas subscribed & call them
    for (typename std::vector<std::function<void()> >::iterator lambda = it->second.begin();
        lambda != it->second.end(); ++lambda)
    {
        (*lambda)();
    }
}
