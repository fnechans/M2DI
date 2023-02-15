#ifndef CHARACTER_H
#define CHARACTER_H

#include "object.h"
#include "dmgr_inst.h"

class Dmgr_instance;

class Character : public Object
{
public:
    using Object::Object;
    Character(const Character& other) : Object(other)
    {
        dmgr_insts = other.dmgr_insts;
    }
    ////////////
    // DAMAGE //
    ////////////

    std::map<std::string, Dmgr_instance> dmgr_insts;
    bool evaluate_attack(const std::string& name, std::vector<Object*>& collObjects);
    bool evaluate_attack(const std::string& name, const SDL_Rect &origin, Object::direction dir, std::vector<Object*>& collObjects);
    bool evaluate_attack(const std::string& name, const SDL_Rect &origin, const SDL_Rect& dir, std::vector<Object*>& collObjects);
private:
};

#endif // CHARACTER_H