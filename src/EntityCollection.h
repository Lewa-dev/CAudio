#pragma once
#include <queue>
#include "DynamicArray.h"
#include <assert.h>

template<typename T>
class EntityCollection
{
    std::queue<unsigned int> freeIDs;
    Engine::DynamicArray<T*> elements;// TODO: replace raw pointers with managed pointers. (Performance should not be a concern in this case)

    unsigned int currentID;

  public:
    EntityCollection<T>();

    unsigned int addElement(T* element);
    bool hasElement(unsigned int id);
    T* getElement(unsigned int id);
    void removeElement(unsigned int id);

    size_t getInternalSize();
};

template<typename T>
inline unsigned int EntityCollection<T>::addElement(T* element)
{
    unsigned int nextID = 0;
    if (freeIDs.size() > 0)
    {
        nextID = freeIDs.front();
        freeIDs.pop();

        elements[nextID] = element;
    }
    else
    {
        if (currentID >= elements.size())
        {
            unsigned int newSize = elements.size() * 2;
            if (newSize < 2)
                newSize = 2;

            if (currentID < newSize)
                newSize++;

            unsigned int currentS = elements.size();
            elements.rezise(newSize);

            unsigned int newS = elements.size();
            for (unsigned int i = currentS; i < newS; i++)
            {
                elements[i] = nullptr;
            }
        }
        assert(currentID < elements.size());
        elements[currentID] = element;

        nextID = currentID;

        currentID++;
    }
    return nextID;
}

template<typename T>
inline bool EntityCollection<T>::hasElement(unsigned int id)
{
    if (elements[id] != nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template<typename T>
inline T* EntityCollection<T>::getElement(unsigned int id)
{
    return elements[id];
}

template<typename T>
inline void EntityCollection<T>::removeElement(unsigned int id)
{
    elements[id] = nullptr;
    freeIDs.push(id);
}

template<typename T>
inline size_t EntityCollection<T>::getInternalSize()
{
    return elements.size();
}

template<typename T>
inline EntityCollection<T>::EntityCollection()
{
    this->elements = Engine::DynamicArray<T*>(2);
    currentID = 1;
    for (size_t i = 0; i < elements.size(); i++)
    {
        elements[i] = nullptr;
    }
}
