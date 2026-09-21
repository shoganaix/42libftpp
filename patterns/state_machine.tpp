/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.tpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 by msoriano                  #+#    #+#             */
/*   Updated: 2026/09/21 by msoriano                 ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>

template <typename TState>
StateMachine<TState>::StateMachine()
    : _hasCurrentState(false)
{
}

template <typename TState>
StateMachine<TState>::~StateMachine()
{
}

template <typename TState>
StateMachine<TState>::StateMachine(const StateMachine& other)
    : _states(other._states),
      _actions(other._actions),
      _transitions(other._transitions),
      _currentState(other._currentState),
      _hasCurrentState(other._hasCurrentState)
{
}

template <typename TState>
StateMachine<TState>& StateMachine<TState>::operator=(const StateMachine& other)
{
    if (this != &other)
    {
        _states = other._states;
        _actions = other._actions;
        _transitions = other._transitions;
        _currentState = other._currentState;
        _hasCurrentState = other._hasCurrentState;
    }
    return *this;
}

template <typename TState>
void StateMachine<TState>::addState(const TState& state)
{
    _states.insert(state);
}

template <typename TState>
void StateMachine<TState>::addTransition(const TState& startState,
    const TState& finalState, const std::function<void()>& lambda)
{
    _states.insert(startState);
    _states.insert(finalState);
    _transitions[std::make_pair(startState, finalState)] = lambda;
}

template <typename TState>
void StateMachine<TState>::addAction(const TState& state,
    const std::function<void()>& lambda)
{
    _states.insert(state);
    _actions[state] = lambda;
}

/*
    Executes the transition to a specific state.

    The first call sets the current state (transition from "nowhere"
    is allowed as long as the target state is known). Later calls
    require a registered (current -> target) transition, otherwise
    an exception is thrown.
*/
template <typename TState>
void StateMachine<TState>::transitionTo(const TState& state)
{
    if (_states.find(state) == _states.end())
        throw std::runtime_error("StateMachine: unknown state");

    if (!_hasCurrentState)
    {
        _currentState = state;
        _hasCurrentState = true;
        return;
    }

    typename std::map<std::pair<TState, TState>, std::function<void()> >::iterator it;

    it = _transitions.find(std::make_pair(_currentState, state));
    if (it == _transitions.end())
        throw std::runtime_error("StateMachine: transition not set up");

    it->second();
    _currentState = state;
}

/*
    Executes the action registered for the current state.
    Throws when there is no current state or no action for it.
*/
template <typename TState>
void StateMachine<TState>::update()
{
    if (!_hasCurrentState)
        throw std::runtime_error("StateMachine: no current state");

    typename std::map<TState, std::function<void()> >::iterator it;

    it = _actions.find(_currentState);
    if (it == _actions.end())
        throw std::runtime_error("StateMachine: action not set up");

    it->second();
}
