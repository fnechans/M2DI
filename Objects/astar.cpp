#include "astar.h"

#include <vector>
#include <cmath>
#include <algorithm>

astar::astar( std::vector<node> _curMap ) : curMap(_curMap)
{

}

std::vector<node> astar::reconstruct_path( node current )
{
    std::vector<node> result = {current};
    while( comesFrom.find(current)!=comesFrom.end() )
    {
        current = comesFrom[current];
        result.push_back(current);
    }
    return result;
}

double astar::cost( node start, node goal )
{
    int dX = start->position.x - goal->position.x;
    int dY = start->position.y - goal->position.y;
    return std::sqrt(dX*dX+dY*dY);
}

node astar::bestOpen()
{
    double lowestScore = 1e6;
    node lowestNode = nullptr;
    for( auto node : openSet )
    {
        if( guessScore.find(node)==guessScore.end() )
        {
            std::cout << "Cannot find node from open set in guessScore. This should not happen!" << std::endl;
            continue;
        }
        if( guessScore[node] < lowestScore )
        {
            lowestScore = guessScore[node];
            lowestNode = node;
        }
    }
    return lowestNode;
}

std::vector<node> astar::find_path( node start, node goal, std::vector<node> collObjects )
{
    std::vector<node> closeSet = {};
    openSet = {start};

    curMap.push_back(goal);

    //comesFrom
    std::map<node,double> bestScore;
    bestScore[start] = 0;
    guessScore[start] = cost(start,goal);
    for( auto tile : curMap )
    {
        guessScore[tile] = 1e6;
        bestScore[tile] = 1e6;
    }

    node current;

    while( !openSet.empty() )
    {
        current = bestOpen();
        if( current == goal )
        {
            curMap.pop_back();
            return reconstruct_path(current);
        }
        closeSet.push_back(current);
        openSet.erase( std::find(openSet.begin(), openSet.end(), current) );

        for( auto neighbor : curMap )
        {
            if( cost(neighbor, current) > TILESIZERENDER*1.5 || neighbor == current ) continue;

            bool collides = false;
            for( auto obj : collObjects )
            {
                if( obj == goal || obj == start ) continue;
                if(  SDL_HasIntersection(&neighbor->position,&obj->position ) )
                {
                    collides = true;
                    break;
                }
            }
            if(collides) continue;

            double tentScore = bestScore[current] + cost(current,neighbor);
            if( tentScore < bestScore[neighbor] )
            {
                comesFrom[neighbor] = current;
                bestScore[neighbor] = tentScore;
                guessScore[neighbor]= bestScore[neighbor]+cost(neighbor,goal);
                if( std::find( closeSet.begin(), closeSet.end(), neighbor )==closeSet.end() )
                {
                    openSet.push_back(neighbor);
                }
            }
        }
    }
    curMap.pop_back();
    return {};
}
