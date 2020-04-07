#ifndef ASTAR_H
#define ASTAR_H

#include "base.h"
#include "object.h"

#include <map>

// TODO: maybe will want to move this to template? the issue is with getting x/y...
typedef std::shared_ptr<object> node;

class astar : public base
{
public:
    astar(std::vector<node> _curMap);

    std::map<node,node> comesFrom;
    std::vector<node> openSet;
    std::map<node,double> guessScore;
    std::vector<node> curMap;

    std::vector<node> reconstruct_path( node current );
    double cost( node start, node goal );
    node bestOpen();
    std::vector<node> find_path( node start, node goal, std::vector<node> collObjects );
};

#endif // ASTAR_H
