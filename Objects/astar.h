#ifndef ASTAR_H
#define ASTAR_H

#include "base.h"
#include "object.h"

#include <map>
#include <cmath>
#include <algorithm>

template <typename node>
class AStar : public base
{
public:
    AStar(std::vector<node> &_curMap) : curMap(_curMap) {}

    std::map<node, node> comesFrom;
    std::vector<node> openSet;
    std::map<node, double> guessScore;
    std::vector<node> curMap;

    bool debug = false;
    std::vector<node> reconstruct_path(node tar, node star)
    {
        std::vector<node> result = {};
        node current = tar;
        while (comesFrom.find(current) != comesFrom.end())
        {
            current = comesFrom[current];
            if (current == star)
                continue;
            result.push_back(current);
        }
        return result;
    }

    double cost(node start, node goal)
    {
        int dX = start->position.x - goal->position.x;
        int dY = start->position.y - goal->position.y;
        return std::sqrt(dX * dX + dY * dY);
    }

    node bestOpen()
    {
        double lowestScore = 1e6;
        node lowestNode = nullptr;
        for (auto nd : openSet)
        {
            if (guessScore.find(nd) == guessScore.end())
            {
                std::cout << "Cannot find node from open set in guessScore. This should not happen!" << std::endl;
                continue;
            }
            if (guessScore[nd] < lowestScore)
            {
                lowestScore = guessScore[nd];
                lowestNode = nd;
            }
        }
        return lowestNode;
    }

    std::vector<node> find_path(node start, node goal, std::vector<node>& collObjects)
    {

        std::vector<node> closeSet = {};
        openSet = {start};

        curMap.push_back(goal);

        comesFrom.clear();
        std::map<node, double> bestScore;
        bestScore[start] = 0;
        guessScore[start] = cost(start, goal);
        for (auto tile : curMap)
        {
            guessScore[tile] = 1e6;
            bestScore[tile] = 1e6;
        }

        node current;

        while (!openSet.empty())
        {
            if (debug)
                std::cout << openSet.size() << std::endl;
            current = bestOpen();
            if (current == goal)
            {
                curMap.pop_back();
                return reconstruct_path(goal, start);
            }
            closeSet.push_back(current);
            openSet.erase(std::remove(openSet.begin(), openSet.end(), current), openSet.end());

            for (auto neighbor : curMap)
            {
                if (debug)
                    std::cout << "here2" << std::endl;
                if (neighbor == current || cost(neighbor, current) > TILESIZEPHYSICS * 1.1)
                    continue;
                if (debug)
                    std::cout << "here4" << std::endl;

                bool collides = false;
                for (auto obj : collObjects)
                {
                    if (obj == goal || obj == start)
                        continue;
                    if (SDL_HasIntersection(&neighbor->position, &obj->position))
                    {
                        collides = true;
                        break;
                    }
                }
                if (collides)
                    continue;
                if (debug)
                    std::cout << "here" << std::endl;

                double tentScore = bestScore[current] + cost(current, neighbor);
                if (tentScore < bestScore[neighbor])
                {
                    comesFrom[neighbor] = current;
                    bestScore[neighbor] = tentScore;
                    if (debug)
                        std::cout << "here3" << std::endl;
                    guessScore[neighbor] = bestScore[neighbor] + cost(neighbor, goal);
                    if (debug)
                        std::cout << "here4" << std::endl;
                    if (std::find(closeSet.begin(), closeSet.end(), neighbor) == closeSet.end())
                    {
                        openSet.push_back(neighbor);
                    }
                }
            }
        }
        curMap.pop_back();
        return {};
    }
};

#endif // ASTAR_H