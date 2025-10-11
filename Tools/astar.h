#ifndef ASTAR_H
#define ASTAR_H

#include <map>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <memory>
#include "Tools/base.h"


template <typename node>
class AStar
{
public:
    AStar(std::vector<node> &_curMap) : curMap(_curMap) {}

    std::map<node, node> comesFrom;
    std::vector<node> openSet;
    std::map<node, double> guessScore;
    std::vector<node> curMap;

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
        int dX = start->position().x - goal->position().x;
        int dY = start->position().y - goal->position().y;
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
                if (neighbor == current || cost(neighbor, current) > base::TILESIZEPHYSICS * 1.1)
                    continue;

                bool collides = false;
                for (auto obj : collObjects)
                {
                    if (obj == goal || obj == start)
                        continue;
                    if (SDL_HasIntersection(&neighbor->hitbox, &obj->hitbox))
                    {
                        collides = true;
                        break;
                    }
                }
                if (collides)
                    continue;

                double tentScore = bestScore[current] + cost(current, neighbor);
                if (tentScore < bestScore[neighbor])
                {
                    comesFrom[neighbor] = current;
                    bestScore[neighbor] = tentScore;
                    guessScore[neighbor] = bestScore[neighbor] + cost(neighbor, goal);
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

template <typename node>
class AI
{
public:
    int AIclick = 0; // how often is AI updated
    std::unique_ptr<AStar<node>> acko = nullptr;
    void init_astar(std::vector<node> nodes)
    {
        acko = std::make_unique<AStar<node>>(nodes);
    }
    bool tick(uint frequency) { return AIclick % frequency == 0; }
    void increment(){AIclick++;}
};

#endif // ASTAR_H