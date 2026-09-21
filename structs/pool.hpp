/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 12:07:33 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/21 17:49:10 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POOL_HPP
#define POOL_HPP

# include <cstddef>
# include <stdexcept>
# include <utility>
# include <vector>



template <typename TType>
class Pool
{
public:

    class Object
    // * Object is responsible for monitoring borrowed objs from Pool 
        // and automatically returning them
    {
    public:
        Object();
        ~Object();

        Object(const Object&) = delete;
        Object& operator=(const Object&) = delete;

        Object(Object&& other) noexcept;
        Object& operator=(Object&& other) noexcept;
        // * "noexcept" means no exceptions will be launched
            // this means moving one to another is permitted
            // ( replacing copying behaviour)

        TType* operator->();
        const TType* operator->() const;
        //* defining "->" behaviour for const & non const

    private:
        friend class Pool<TType>;
        // * "friend" meaning this class may access my contents

        Object(Pool<TType>* pool, TType* object);
        Pool<TType>* _pool;
        TType* _object;
    };

    Pool();
    ~Pool();

    Pool(const Pool&) = delete;
    Pool& operator=(const Pool&) = delete;
    // * "= delete" means this function is forbidden
        // copying pointer into two different slots can lead to a 
        // double free error when released

    void resize(const std::size_t& numberOfObjectStored);

    template <typename... TArgs>
    // * This pool can store any class with or without arguments.
        // We use acquire() to pass these arguments of type TArgs

    Object acquire(TArgs&&... p_args);
    // * The parameter pack ("...") means it accepts any number of args


// * Every slot is a struct  with two attributes:
    //  - memory: pointer to mem (void * because type is first undefined)
    // -  used: true / false
private:

    struct Slot
    {
        void* memory;
        bool used;
    };

    std::vector<Slot> _slots;
    // * We use vector to store a contiguous collection of our slots
    // NOTE: destroying the Pool while Objects are still alive leaves them
    // dangling. Always let every Object go out of scope before the Pool.
    // release() silently ignores pointers that do not belong to the Pool.
    void release(TType* object);
};

# include "pool.tpp"

#endif