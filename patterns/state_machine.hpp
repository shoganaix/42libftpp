/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <functional>
#include <map>
#include <set>
#include <utility>

/*
    StateMachine templated by TState.
    Manages transitions between states plus one action per state.
*/

template <typename TState>
class StateMachine
{
public:
    StateMachine();
    ~StateMachine();

    StateMachine(const StateMachine& other);
    StateMachine& operator=(const StateMachine& other);

    void addState(const TState& state);
    void addTransition(const TState& startState, const TState& finalState,
        const std::function<void()>& lambda);
    void addAction(const TState& state, const std::function<void()>& lambda);
    void transitionTo(const TState& state);
    void update();

private:
    std::set<TState> _states;
    std::map<TState, std::function<void()> > _actions;
    std::map<std::pair<TState, TState>, std::function<void()> > _transitions;
    TState _currentState;
    bool _hasCurrentState;
};

#include "state_machine.tpp"

#endif
