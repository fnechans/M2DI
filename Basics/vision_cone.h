#ifndef VISIONCONE_H
#define VISIONCONE_H


#include "base.h"
#include "viewport.h"
#include "block.h"

#include <math.h>

// using class to handle intermediate variables
class VisionCone
{
public:
    VisionCone(SDL_Rect b) : border(b) {}

    int lengthFactor = 100;

    bool set_cone(const SDL_Rect &origin, const SDL_Rect &dir, float _angle)
    {
        angle = _angle;


        // Define endpoints of the cone (perp to the sight line)
        if(angle<0 || angle>89) throw std::runtime_error("Angle <0 or >89");

        direction = tools::get_endpoint(origin, dir, lengthFactor);
        int dx = direction.x - origin.x;
        int dy = direction.y - origin.y;

        // Define one point on perpendicular line to direction
        SDL_Rect perpPoint;
        perpPoint.x = direction.x + dy;
        perpPoint.y = direction.y - dx;

        // Define distance from direction necesarry for such point to be at angle wrt origin to endpoint line
        float perpLength = tan(angle*3.14/180.0)*lengthFactor; //close enough

        // Find points at correct angle from origin
        auto dir1 = tools::get_endpoint(direction, perpPoint, perpLength);
        auto dir2 = tools::get_endpoint(direction, perpPoint, -perpLength);

        // Define the cone lines (with endpoint at the same distance as direction,
        // that allows to easily compute if other points are within cone)
        coneL1 = MLine(origin, tools::get_endpoint(origin, dir1, lengthFactor));
        coneL2 = MLine(origin, tools::get_endpoint(origin, dir2, lengthFactor));


        coneL1.halfInfinite = true;
        coneL2.halfInfinite = true;

        doCone = true;

        return false;
    }

    std::vector<SDL_Rect> get_points(Block* origin, std::vector<Block*> obstacles)
    {
        // Divide plane for positive and negative y, makes sorting easier (I hope)
        std::vector<SDL_Rect> endpointsPos;
        std::vector<SDL_Rect> endpointsNeg;

        SDL_Rect interPoint;

        auto wpLines = tools::get_lines(border);

        // go trough all possible obstacles
        for(auto obst1 : obstacles)
        {
            if(obst1 == origin) continue;
            // iterate all points on object
            for(auto point : tools::get_points(obst1->hitbox))
            {
                // define line from origin to point
                MLine line(origin->position(), point);
                line.halfInfinite = true;
                float distance = tools::distance2(origin->position(), point);

                // If only cone vision, and point outside, skip
                // This can be found out based on whether the point is
                // closer to original direction point then the coneL* points
                // if set to the same distance
                if(doCone)
                {
                    float distConeMax = tools::distance2(direction, coneL1.p2);
                    float distPoint = tools::distance2(direction, tools::get_endpoint(origin->position(), point, lengthFactor));
                    if(distPoint>distConeMax) continue;
                }

                // find point intersectin with viewport (needed if vision unobstructed)
                SDL_Rect potentialEndpoint;
                bool inters = false;
                for(auto& wpLine : wpLines)
                {
                    if(line.intersects(wpLine, &potentialEndpoint))
                    {
                        inters = true;
                        break;
                    }
                }
                if(!inters)
                {
                    throw std::runtime_error("Visual line does not intersect border, this should not be possible!");
                }
                float potentialDistance = tools::distance2(origin->position(), potentialEndpoint);

                bool pointFinished = false;
                bool intSelf = false;

                // intersect all objects (including self)
                for(auto obst2 : obstacles)
                {
                    if(obst2 == origin) continue;
                    for(auto line2 : tools::get_lines(obst2->hitbox))
                    {
                        if(line.intersects(line2, &interPoint))
                        {
                            // if intersection closer then this point is obstructed
                            if( distance > tools::distance2(origin->position(), interPoint))
                            {
                                pointFinished = true;
                                break;
                            }
                            // if intersect self  on line not containing this point
                            // then everything behind obstructed by this object
                            if(obst1==obst2)
                            {

                                intSelf = intSelf || (!(line2.p1.x==point.x &&line2.p1.y==point.y) &&
                                        !(line2.p2.x==point.x &&line2.p2.y==point.y));
                               //if(!intSelf && !(interPoint.x==point.x && interPoint.y==point.y)) std::cout <<  interPoint.x << " " << interPoint.y<< " " << point.x << " " << point.y << std::endl;
                                continue;
                            }
                            // else find  if intersection is closer than current potential endpoint
                            float tmpDist = tools::distance2(origin->position(), interPoint);
                            if(tmpDist < potentialDistance)
                            {
                                potentialDistance = tmpDist;
                                potentialEndpoint = interPoint;
                            }

                        }
                    } // loop over lines in obst2
                    if(pointFinished) break;

                } // loop over obst2
                if(pointFinished) continue;

                // if here then point not obstructed
                // but also possible to view behind up to potential endpoint
                // TODO: pos/neg condition should be evaluated the same for both??
                if (point.y > origin->position().y)
                    endpointsPos.push_back(point);
                else
                    endpointsNeg.push_back(point);

                if(intSelf)
                {
                    continue;
                }

                if (potentialEndpoint.y > origin->position().y)
                    endpointsPos.push_back(potentialEndpoint);
                else
                    endpointsNeg.push_back(potentialEndpoint);
            }
        }

        // reject those outside cone (line length inf intersects endpoints of cone?)
        if (doCone)
        {
            if (coneL1.p2.y > origin->position().y)
                endpointsPos.push_back(coneL1.p2);
            else
                endpointsNeg.push_back(coneL1.p2);
            if (coneL1.p2.y > origin->position().y)
                endpointsPos.push_back(coneL2.p2);
            else
                endpointsNeg.push_back(coneL2.p2);
        }

        // Order points based on angle
        // in the two hemispheres
        std::sort(endpointsPos.begin(), endpointsPos.end(),
            [&origin](SDL_Rect& p1, SDL_Rect& p2)
            {
                float dx1 = (origin->position().x-p1.x);
                // Check for dY = 0
                float dy1 = (origin->position().y-p1.y);
                if(dy1==0 && dx1>0) // positive inf
                    return true;
                if(dy1==0 && dx1<0) // negative inf
                    return false;

                float dx2 = (origin->position().x-p2.x);
                // Check for dY = 0, not return opposite
                float dy2 = (origin->position().y-p2.y);
                if(dy1==0 && dx1>0) // positive inf
                    return false;
                if(dy1==0 && dx1<0) // negative inf
                    return true;
                return dx1/dy1 > dx2/dy2;
            }
        );
        std::sort(endpointsPos.begin(), endpointsPos.end(),
            [&origin](SDL_Rect& p1, SDL_Rect& p2)
            {
                float dx1 = (origin->position().x-p1.x);
                // Check for dY = 0
                float dy1 = (origin->position().y-p1.y);
                if(dy1==0 && dx1>0) // positive inf
                    return true;
                if(dy1==0 && dx1<0) // negative inf
                    return false;

                float dx2 = (origin->position().x-p2.x);
                // Check for dY = 0, not return opposite
                float dy2 = (origin->position().y-p2.y);
                if(dy1==0 && dx1>0) // positive inf
                    return false;
                if(dy1==0 && dx1<0) // negative inf
                    return true;
                return dx1/dy1 > dx2/dy2;
            }
        );

        if(doCone) // for cone order is important
        {
            // if one vector is empty return the other
            if(endpointsNeg.size()==0) return endpointsPos;
            if(endpointsPos.size()==0) return endpointsNeg;

            // since we restrict to cone < 90 degr. to each side we can check
            // for which  origin(x +-lengthFactor) is one of the cone lines closest,
            // if positive thne positive goes first
            float dp = std::min(std::abs(origin->position().x+lengthFactor-coneL1.p2.x), std::abs(origin->position().x+lengthFactor-coneL2.p2.x));
            float dn = std::min(std::abs(origin->position().x-lengthFactor-coneL1.p2.x), std::abs(origin->position().x-lengthFactor-coneL2.p2.x));
            if(dp<dn)
            {
                endpointsPos.insert(endpointsPos.end(), endpointsNeg.begin(), endpointsNeg.end());
                return endpointsPos;
            }
            else
            {
                endpointsNeg.insert(endpointsNeg.end(), endpointsPos.begin(), endpointsPos.end());
                return endpointsNeg;
            }
        }

        // if not cone order does not matter
        endpointsPos.insert(endpointsPos.end(), endpointsNeg.begin(), endpointsNeg.end());
        return endpointsPos;
    }
private:
    MLine coneL1, coneL2;
    SDL_Rect border;
    SDL_Rect direction;
    float angle;
    bool doCone{false};
};

#endif
