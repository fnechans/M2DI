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

    int lengthFactor = 10000;

    bool set_cone(const SDL_Rect &origin, const SDL_Rect &dir, float _angle)
    {
        angle = _angle;

        // Define endpoints of the cone (perp to the sight line)
        if (angle < 0 || angle > 89)
            throw std::runtime_error("Angle <0 or >89");

        direction = tools::get_endpoint(origin, dir, lengthFactor);
        int dx = direction.x - origin.x;
        int dy = direction.y - origin.y;

        // Define one point on perpendicular line to direction
        SDL_Rect perpPoint;
        perpPoint.x = direction.x + dy;
        perpPoint.y = direction.y - dx;

        // Define distance from direction necesarry for such point to be at angle wrt origin to endpoint line
        float perpLength = tan(angle * 3.14 / 180.0) * (float) lengthFactor; // close enough

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

    // temporary helper class for evaluate point
    struct BorderPair
    {
        BorderPair() {}
        BorderPair(SDL_Rect &a) : point(a) {}
        SDL_Rect point;
        MLine line;        // line to an corner of an object
        SDL_Rect endpoint; // potential endpoint
        bool addEndpoint = true;
        Block *obst{nullptr};   // origin block fo the point (if any)
        std::vector<Block*> obsts; // for points shared by multiple blocks
        Block *endpointObst{nullptr};   // origin block of the endpoint (if any)
        float endpointDistance; //
        float distance{0.};
    };

    bool evaluate_point(BorderPair &bdrPair, Block *origin, std::vector<Block *> &obstacles)
    {
        SDL_Rect interPoint;
        auto wpLines = tools::get_lines(border);

        // If only cone vision, and point outside, skip
        // This can be found out based on whether the point is
        // closer to original direction point then the coneL* points
        // if set to the same distance
        if (doCone)
        {
            float distConeMax = tools::distance2(direction, coneL1.p2)*1.05 ; // +5% to accomodate for inprecision (we want points at exactly this distance)
            float distPoint = tools::distance2(direction, tools::get_endpoint(origin->position(), bdrPair.point, lengthFactor));
            if (distPoint > distConeMax)
                return false;
            ;
        }

        // find point intersectin with viewport (needed if vision unobstructed)
        bool inters = false;
        for (auto &wpLine : wpLines)
        {
            if (bdrPair.line.intersects(wpLine, &bdrPair.endpoint))
            {
                inters = true;
                break;
            }
        }
        if (!inters)
        {
            tools::print_rect(border);
            tools::print_rect(origin->position());
            tools::print_rect(direction);
            tools::print_rect(bdrPair.line.p1);
            tools::print_rect(bdrPair.point);

            throw std::runtime_error("Visual line does not intersect border, this should not be possible!");
            // return false;
        }
        bdrPair.endpointDistance = tools::distance2(origin->position(), bdrPair.endpoint);

        bdrPair.obsts.push_back(bdrPair.obst);

        // intersect all objects (including self)
        for (auto obst2 : obstacles)
        {
            if (obst2 == origin)
                continue;
            for (auto line2 : tools::get_lines(obst2->hitbox))
            {
                // is the point shared by multiple objects?
                if(tools::point_equal(bdrPair.point, line2.p1) ||
                    tools::point_equal(bdrPair.point, line2.p2)
                )
                    bdrPair.obsts.push_back(obst2);

                if (bdrPair.line.intersects(line2, &interPoint))
                {
                    // if intersection closer then this point is obstructed
                    if (bdrPair.distance > tools::distance2(origin->position(), interPoint))
                    {
                        return false;
                    }
                    // if intersect self  on line not containing this point
                    // then everything behind obstructed by this object
                    if (bdrPair.obst != nullptr && bdrPair.obst == obst2)
                    {

                        if (!(line2.p1.x == bdrPair.point.x && line2.p1.y == bdrPair.point.y) &&
                            !(line2.p2.x == bdrPair.point.x && line2.p2.y == bdrPair.point.y))
                        {
                            bdrPair.addEndpoint = false;
                            break;
                        }
                        continue;
                    }
                    // else find  if intersection is closer than current potential endpoint
                    float tmpDist = tools::distance2(origin->position(), interPoint);
                    if (tmpDist < bdrPair.endpointDistance)
                    {
                        bdrPair.endpointDistance = tmpDist;
                        bdrPair.endpoint = interPoint;
                        bdrPair.endpointObst = obst2;
                    }
                }
            } // loop over lines in obst2
        }     // loop over obst2
        return true;
    }

    std::vector<SDL_Rect> get_points(Block *origin, std::vector<Block *> &obstacles)
    {
        std::vector<SDL_Rect> finalPoints;

        // Divide plane for positive and negative y, makes sorting easier (I hope)
        // Corners of objects not obstructed
        std::vector<BorderPair> cornersPos;
        std::vector<BorderPair> cornersNeg;

        // On first frame it can happen that mouse position is not set up
        // it then defaults to middle of the screen and becomes equal to player
        // leading to undefined direction
        // Return empty in such case
        if (tools::point_equal(origin->position(), direction))
            return finalPoints;

        // Turn off doPlotting for each object and only turn on when in vision
        for (auto obst1 : obstacles)
        {
            if (obst1 == origin)
                continue;
            obst1->doPlot = false;
        }

        // go trough all possible obstacles
        for (auto obst1 : obstacles)
        {

            if (obst1 == origin)
                continue;
            // iterate all points on object
            for (auto point : tools::get_points(obst1->hitbox))
            {
                // define line from origin to point
                MLine line(origin->position(), point);
                line.halfInfinite = true;

                BorderPair bdrPair(point);
                bdrPair.line = line;
                bdrPair.obst = obst1;
                bdrPair.distance = tools::distance2(origin->position(), bdrPair.point);

                if (!evaluate_point(bdrPair, origin, obstacles))
                    continue;

                obst1->doPlot = true;

                // if here then point not obstructed
                if (point.y > origin->position().y)
                    cornersPos.push_back(bdrPair);
                else
                    cornersNeg.push_back(bdrPair);
            }
        }

        // Add borders of cone if applicable
        if (doCone)
        {
            BorderPair bdrPair(coneL1.p2);
            bdrPair.line = coneL1;
            bdrPair.distance = 0; // any point is further, find first unobstructed
            if (evaluate_point(bdrPair, origin, obstacles))
            {
                bdrPair.obst = bdrPair.endpointObst;
                bdrPair.point = bdrPair.endpoint;
                bdrPair.addEndpoint = false;
                if (coneL1.p2.y > origin->position().y)
                    cornersPos.push_back(bdrPair);
                else
                    cornersNeg.push_back(bdrPair);
            }

            BorderPair bdrPair2(coneL2.p2);
            bdrPair2.line = coneL2;
            bdrPair2.distance = 0;
            if (evaluate_point(bdrPair2, origin, obstacles))
            {
                bdrPair2.obst = bdrPair2.endpointObst;
                bdrPair2.point = bdrPair2.endpoint;
                bdrPair2.addEndpoint = false;
                if (coneL2.p2.y > origin->position().y)
                    cornersPos.push_back(bdrPair2);
                else
                    cornersNeg.push_back(bdrPair2);
            }
        }

        // Add corners of screen
        for(auto& point : tools::get_points(border))
        {
            BorderPair bdrPair(point);
            bdrPair.line = MLine(origin->position(), point);
            bdrPair.line.halfInfinite = true;
            bdrPair.distance = 0; // any point is further, find first unobstructed
            if (evaluate_point(bdrPair, origin, obstacles))
            {
                if(bdrPair.endpointObst) continue;
                bdrPair.obst = bdrPair.endpointObst;
                bdrPair.point = bdrPair.endpoint;
                bdrPair.addEndpoint = false;
                if (point.y > origin->position().y)
                    cornersPos.push_back(bdrPair);
                else
                    cornersNeg.push_back(bdrPair);
            }
        }

        // Make sure all points are unique
        auto it = std::unique(cornersPos.begin(), cornersPos.end(), [](BorderPair &a, BorderPair &b)
                              { return tools::point_equal(a.point, b.point); });
        cornersPos.resize(std::distance(cornersPos.begin(), it));

        it = std::unique(cornersNeg.begin(), cornersNeg.end(), [](BorderPair &a, BorderPair &b)
                         { return tools::point_equal(a.point, b.point); });
        cornersNeg.resize(std::distance(cornersNeg.begin(), it));

        // tan comparison
        auto tan_comp = [&origin](SDL_Rect & p1, SDL_Rect & p2, bool pos = true)
        {
            if( tools::point_equal(p1, p2) ) return false;
            
            bool returnValue = !pos; // switches based on hemisphere!

            float dx1 = (origin->position().x - p1.x);
            // Check for dY = 0
            float dy1 = (origin->position().y - p1.y);
            if (dy1 == 0 && dx1 > 0) // positive inf
                return returnValue;
            if (dy1 == 0 && dx1 < 0) // negative inf
                return !returnValue;

            float dx2 = (origin->position().x - p2.x);
            // Check for dY = 0, not return opposite
            float dy2 = (origin->position().y - p2.y);
            if (dy2 == 0 && dx2 > 0) // positive inf
                return !returnValue;
            if (dy2 == 0 && dx2 < 0) // negative inf
                return returnValue;

            return dx1 / dy1 > dx2 / dy2;
        };

        auto tan_comp_pos = [&origin, tan_comp](BorderPair &p1, BorderPair &p2)
        { return tan_comp(p1.point, p2.point, true); };
        auto tan_comp_neg = [&origin, tan_comp](BorderPair &p1, BorderPair &p2)
        { return tan_comp(p1.point, p2.point, false); };

        // Order points based on angle
        // in the two hemispheres
        std::sort(cornersPos.begin(), cornersPos.end(), tan_comp_pos);
        std::sort(cornersNeg.begin(), cornersNeg.end(), tan_comp_neg);

        // Create combined container
        std::vector<BorderPair> corners;
        if (doCone) // for cone order is important
        {
            // since we restrict to cone < 90 degr. to each side we can check
            // for which  origin(x +-lengthFactor) is one of the cone lines closest,
            // if positive thne positive goes first
            float dp = std::min(std::abs(origin->position().x + lengthFactor - coneL1.p2.x), std::abs(origin->position().x + lengthFactor - coneL2.p2.x));
            float dn = std::min(std::abs(origin->position().x - lengthFactor - coneL1.p2.x), std::abs(origin->position().x - lengthFactor - coneL2.p2.x));
            if (dp > dn)
            {
                cornersPos.insert(cornersPos.end(), cornersNeg.begin(), cornersNeg.end());
                corners = cornersPos;
            }
            else
            {
                cornersNeg.insert(cornersNeg.end(), cornersPos.begin(), cornersPos.end());
                corners = cornersNeg;
            }
        }
        else // if not cone order does not matter
        {
            cornersPos.insert(cornersPos.end(), cornersNeg.begin(), cornersNeg.end());
            corners = cornersPos;
        }

        // TODO: return empty for now, figure out why??
        // should be at least 2 from the cone or 4 from whole screen from its corners
        if(corners.size() < 2) return finalPoints;


        // Fill the result vector of correctly ordered points
        for (uint i = 0; i < corners.size(); ++i)
        {
            auto &current = corners[i];
            if (!current.addEndpoint)
            {
                finalPoints.push_back(current.point);
                continue;
            }

            if(current.endpointObst!=nullptr) current.endpointObst->doPlot = true; // make sure the origin object is plotted

            auto &next = (i+1==corners.size())  ? corners[0] : corners[i + 1];

            // if points are really close , just skip the next one
        /*    if (tools::distance2(current.point, next.point)<5)
            {
                i++;
                auto &next = (i+1==corners.size())  ? corners[i-1] : corners[i + 1];
            }*/

            bool matchObsts = false;
            for(auto& o : current.obsts)
            {
                if(tools::contains<Block*>(next.obsts, o)) matchObsts = true;
            }

            if((matchObsts) || tools::contains<Block*>(current.obsts, next.endpointObst))
            {
                finalPoints.push_back(current.endpoint);
                finalPoints.push_back(current.point);
            }
            else //if (tools::contains<Block*>(next.obsts,current.endpointObst))
            {
                finalPoints.push_back(current.point);
                finalPoints.push_back(current.endpoint);
            }
           /* else
            {
                finalPoints.push_back(current.endpoint);
                finalPoints.push_back(current.point);
            }*/
        }
        
        // remove duplicates again
        auto it2 = std::unique(finalPoints.begin(), finalPoints.end(), [](SDL_Rect &a, SDL_Rect &b)
                         { return tools::point_equal(a, b); });
        finalPoints.resize(std::distance(finalPoints.begin(), it2));
        // For non-cone, complete the loop
        if(!doCone) finalPoints.push_back(finalPoints[0]);

        return finalPoints;
    }

private:
    MLine coneL1, coneL2;
    SDL_Rect border;
    SDL_Rect direction;
    float angle;
    bool doCone{false};
};

#endif
