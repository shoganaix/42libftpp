/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.tpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msoriano <msoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 13:57:31 by msoriano          #+#    #+#             */
/*   Updated: 2026/09/21 19:28:10 by msoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
    Object():
    _pool = NULL
    _object = NULL
*/ 
template <typename TType> Pool<TType>::Object::Object(): 
_pool(nullptr), _object(nullptr)
{
}
/*
    Object(pool, object):
    _pool = pool
    _object = object
*/ 
template <typename TType> Pool<TType>::Object::Object(Pool<TType>* pool, TType* object): 
_pool(pool), _object(object)
{
}
/*
     ~Object():
*/ 
template <typename TType> Pool<TType>::Object::~Object()
{
    if (_pool != nullptr && _object != nullptr)
        _pool->release(_object);
}
/*
    Object                  other
    _pool = Pool            _pool = NULLL   
    _object = Object        _object = NULL
*/
template <typename TType> Pool<TType>::Object::Object(Object&& other) noexcept
: _pool(other._pool), _object(other._object)
{
    other._pool = nullptr;
    other._object = nullptr;
}

/*
    When two objects already exist:
    a (other)                b (this)
     _pool = Pool            _pool = Pool  
    _object = Enemy A       _object = Enemy B

    -------- RELEASE ENEMY B + COPY PONTERS ----------

    a (other)                b (this)
     _pool = NULL           _pool = Pool  
    _object = NULL          _object = Enemy A

*/
template <typename TType>typename Pool<TType>::Object&Pool<TType>::Object::operator=(Object&& other) noexcept
{
    if (this != &other)
    {
        if (_pool != nullptr && _object != nullptr)
            _pool->release(_object);

        _pool = other._pool;
        _object = other._object;

        other._pool = nullptr;
        other._object = nullptr;
    }

    return *this;
}

template <typename TType>
Pool<TType>::Pool()
{
}

/*
    Allows Pool::Object to be used like a pointer to TType
    Example:
        object->method();
*/ 
template <typename TType> TType* Pool<TType>::Object::operator->()
{
    return _object;
}

template <typename TType> const TType* Pool<TType>::Object::operator->() const
{
    return _object;
}

/*
    Finds a free slot, constructs a TType in the memory
    already reserved, and returns an obj that will manage it

        Pool
        ┌───────────────────────────────┐
        │                               │
        │ Slot                          │
        │ ┌───────────────────────────┐ │
        │ │ used = false              │ │
        │ │ memory ───────► [memory ] │ │
        │ └───────────────────────────┘ │
        │                               │
        └───────────────────────────────┘
                        │
                        │ placement new
                        ▼
        ┌───────────────────────────────┐
        │ Slot                          │
        │ ┌───────────────────────────┐ │
        │ │ used = true               │ │
        │ │ memory ───────► [ TType ] │ │
        │ └───────────────────────────┘ │
        └───────────────────────────────┘
                              ▲
                              │
                        Pool::Object
                        _object ──┘

    
1. Find a slot where "used == false"
2. Construct TType in slot.memory using placement new
3. Marks slot as occupied
4. Returns `Object(this, object)` that manages TType

If no free slot -> throws exception
*/
template <typename TType>
template <typename... TArgs>
typename Pool<TType>::Object Pool<TType>::acquire(TArgs&&... p_args)
{
    for (Slot& slot : _slots)
    {
        if (!slot.used)
        {
            TType* object = new (slot.memory)
                TType(std::forward<TArgs>(p_args)...);

            slot.used = true;

            return Object(this, object);
        }
    }

    throw std::runtime_error("Pool: no available slot");
}
/*
    Destroys TType managed by Object and makes its Slot available for reuse

        BEFORE:

        Slot
        ┌───────────────────────────┐
        │ used = true               │
        │ memory ───────► [ TType ] │
        └───────────────────────────┘
                              ▲
                              │
                         TType* object

                        object->~TType()
                           │
                           ▼

        AFTER:

        Slot
        ┌───────────────────────────┐
        │ used = false              │
        │ memory ───────► [ memory ]│
        └───────────────────────────┘

    IMPORTANT:
    TType destructor is explicitly called, but memory itself is NOT deallocated
    Memory belongs to Pool and will be reused.
*/
template <typename TType>void Pool<TType>::release(TType* object)
{
    for (Slot& slot : _slots)
    {
        if (slot.memory == static_cast<void*>(object))
        {
            object->~TType();
            slot.used = false;
            return;
        }
    }
}

/*
    Changes number of memory slots managed by pool

    GROW:
        [Slot] [Slot]
              │
              │ resize(4)
              ▼
        [Slot] [Slot] [Slot] [Slot]

        new slots allocate memory for TType but DONT construct objects

    SHRINK:
        [USED] [FREE] [USED] [FREE]
            │
            │ resize(2)
            ▼
        [USED] [USED]

        *only free slots can be removed*

    If requested size < number of currently active objects, 
    Pool cannot be safely resized, an exception is thrown
*/
template <typename TType>
void Pool<TType>::resize(const std::size_t& numberOfObjectStored)
{
    if (numberOfObjectStored > _slots.size())
    {
        while (_slots.size() < numberOfObjectStored)
        {
            void *memory = ::operator new(sizeof(TType));
            try
            {
                _slots.push_back({memory, false});
            }
            catch(...)
            {
                ::operator delete(memory);
                throw;
            }
        }
        return;
    }

    std::size_t usedCount = 0;

    for (const Slot& slot : _slots)
    {
        if (slot.used)
            ++usedCount;
    }

    if (numberOfObjectStored < usedCount)
    {
        throw std::runtime_error("Pool: cannot resize below the number of active objects");
    }

    while (_slots.size() > numberOfObjectStored)
    {
        for (auto it = _slots.end(); it != _slots.begin();)
        {
            --it;

            if (!it->used)
            {
                ::operator delete(it->memory);
                _slots.erase(it);
                break;
            }
        }
    }
}

/*
    if used slot:
        memory -> TType -> ~TType -> memory 

    either way deallocate memory using ::operator delete
*/
template <typename TType>Pool<TType>::~Pool()
{
    for (Slot& slot : _slots)
    {
        if (slot.used)
            static_cast<TType*>(slot.memory)->~TType();
        ::operator delete(slot.memory);
    }
}
