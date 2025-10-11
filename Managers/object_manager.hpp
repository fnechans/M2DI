#ifndef OBJ_MGR_H
#define OBJ_MGR_H

#include "Basics/object.h"
#include "Tools/tools.h"
#include "Tools/base.h"

#include <map>
#include <string>

template <class objType>
class Object_manager
{
public:
    Object_manager(){}

    objType* add(const std::string& name, double x, double y)
    {
        return add(name, (uint) x*base::TILESIZEPHYSICS, (uint) y*base::TILESIZEPHYSICS);
    }

    objType* add(const std::string& name, float x, float y)
    {
        return add(name, (uint) x*base::TILESIZEPHYSICS, (uint) y*base::TILESIZEPHYSICS);
    }

    objType* add(const std::string& name, int x, int y)
    {
        return add(name, (uint) x, (uint) y);
    }

    objType* add(const std::string& name, uint x, uint y)
    {
        if(objects.count(name)>0)
            throw std::runtime_error("Object of name "+name+" already "
                "exists in the manager."); //TODO: manager name?
        // unique_ptr should take care of casting (?!)
        objects.emplace(name, std::make_unique<objType>(x, y));
        iterable.push_back(objects[name].get());
        vctObj.push_back(get_obj(name));
        return objects[name].get();
    }

    objType* operator[](const std::string& name)
    { return get(name); }

    objType* get(const std::string& name)
    {
        if(objects.count(name)==0)
            throw std::runtime_error("Object of name "+name+" does "
                "not texists in the manager.");
        return objects[name].get();
    }

    // Getter which returns the base Object class
    Object* get_obj(const std::string& name)
    {
        if(objects.count(name)==0)
            throw std::runtime_error("Object of name "+name+" does "
                "not texists in the manager.");
        return dynamic_cast<Object*>(objects[name].get());
    }

    // Get all
    std::vector<objType*>& get(){ return iterable;}
    std::vector<Object*>& get_obj(){ return vctObj;}

    void clean()
    {
        // First vectors (raw ptr) and then map (owner!!)
        // because we need to check properties!

        // This is not ideal but should not happen too often.
        // If this became a bottleneck (doubt it)
        // it would be better if functions skipped
        // dead automatically and this clean-up
        // happened only once in a while ...

        tools::remove_dead_vector<objType>(iterable);
        tools::remove_dead_vector<Object>(vctObj);
        tools::remove_dead_map_ptr<std::unique_ptr<objType>>(objects);
    }
    // Make manager iterable through
    // the iterable vector
    typename std::vector<objType*>::iterator begin() {return iterable.begin();}
    typename std::vector<objType*>::iterator end() {return iterable.end();}

private:
    std::vector<objType*> iterable; // For iterating
    std::vector<Object*> vctObj; // For iterating
    std::map<std::string, std::unique_ptr<objType>> objects;

};

#endif