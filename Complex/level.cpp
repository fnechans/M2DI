#include "level.h"

Level::Level(SDL_wrapper * _wrapper) : wrapper(_wrapper), bScreen({0,0,0,0})
{
}

void Level::bake()
{
    collisionObjects.clear();
    collisionObjects.push_back(&player);
    for(auto& c : characters)
    {
        collisionObjects.push_back(&c.second);
    }
    for(auto& t : curMap.blocks) 
    {
        collisionObjects.push_back(&t);
    }
}
void Level::preprocess()
{
    screenClick = false;

    tools::remove_dead_vector<Object>(curMap.blocks);
    tools::remove_dead_map<Character>(characters);
    bake();
}

void Level::evaluate(SDL_Event& event)
{
    if( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED )
    {
        wrapper->sWidth = event.window.data1;
        wrapper->sHeight = event.window.data2;
    }
    bScreen.screenPos = curMap.gameplayScreen;
    
    player.evaluate(event);
    if(bScreen.evaluate(event, {0,0,0,0}) == bScreen.CLICK) screenClick = true;
}

void Level::move_chars()
{
    if(pause) return;

    player.move(collisionObjects);

    // TODO: char pathfinding not done here!
    if( AIclick%50==0 )
    {
        for(auto& cIt: characters)
        {
            Character* chr = &cIt.second;
            auto tmp = acko->find_path(chr ,chr->target, collisionObjects);
            if(!tmp.empty()) chr->path = tmp;
        }
    }
    for(auto& cIt: characters)
    {
        Character* chr = &cIt.second;
        chr->follow_path();
        chr->move(collisionObjects);
    }
    AIclick++;
}

void Level::plot()
{
    player.screen_position(screenRect);
    curMap.render_map(*wrapper, screenRect );
    // TODO: are all collision objects needed here?


    for(auto& cIt: characters)
    {
        Character* chr = &cIt.second;
        chr->plot_path(*wrapper,&screenRect);
        chr->plot_animation(*wrapper, &screenRect, pause);
    }
    player.plot_animation(*wrapper, nullptr, pause);
    curMap.render_minimap(*wrapper, collisionObjects);
}