#include "screen.h"

void Screen::evaluate()
{
    if (event.type == SDL_QUIT)
    {
        quit = true;
    }
    else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
        window->sWidth = event.window.data1;
        window->sHeight = event.window.data2;
    }
    else
    {
        if (menu->evaluate(event))
            return;
        if (level && level->evaluate(event))
            return;
        keybinds.evaluate(event);
    }
}

screen_ptr Screen::loop()
{
    if (!isInit)
        init();
    

    std::cout << "Starting new loop\n";

    // Ticks to ms conversion, round give closest integer
    Uint32 game_step_ms = std::round(1000. / TICKS_PER_SECOND);
    Uint32 render_step_ms = std::round(1000. / FRAMES_PER_SECOND);
    Uint32 next_game_step = SDL_GetTicks(); // initial value
    Uint32 next_render_step = next_game_step;
    uint nRender = 0;
    uint nEval = 0;
    Uint32 fps_step = next_game_step;
    IMG_wrapper textFPS;
    std::stringstream streamFPS;

    //tmp
    bool firstEvent = true; // force evaluate on first event

    keybinds.add_keybind("fullscreen", SDLK_F11, l_fullscreen());

    while (!quit)
    {
        Uint32 now = SDL_GetTicks();

        // Check so we don't render for no reason (unless vsync is enabled)
        if (next_render_step <= now || window->vsync())
        {
            int computer_is_too_slow_limit = 100; // max number of advances per render, adjust this according to your minimum playable fps

            // Loop until all steps are executed or computer_is_too_slow_limit is reached
            while (firstEvent || ((next_game_step <= now) && (computer_is_too_slow_limit--)))
            {
                nEval++;
                menu->reset();
                if (level) level->reset();

                // process inputs
                while (SDL_PollEvent(&event) != 0)
                {
                    evaluate();
                }
                for (auto& f : updates)
                    f();
                if (level) level->move_chars(DELTA_T);

                next_game_step += game_step_ms; // count 1 game tick done

                firstEvent = false;
            }

            next_render_step += render_step_ms; // count 1 render frame done
            nRender++;

            if (now - fps_step > 1000)
            {
                currentFPS = ((float)nRender * 1000) / (now - fps_step);
                currentTickrate = ((float)nEval * 1000) / (now - fps_step);
                // update DELTA_T to reflect the actual tick-rate
                DELTA_T = 1. / currentTickrate;
                nRender = 0;
                nEval = 0;
                fps_step = now;
            }
            window->clear();

            if (level) level->plot();

            for (auto& f : plots)
                f();

            menu->plot();

            SDL_Rect wp = {0, 0, window->sWidth, window->sHeight};
            window->viewPort(&wp);
            streamFPS.str("");
            streamFPS << "FPS: " << currentFPS << "\t Tickrate: " << currentTickrate;
            textFPS.load_text(*window, streamFPS.str(), {255, 100, 100, 255}, 16, 16 * 24);
            textFPS.render_image(*window, 0, 0);

            window->render();
            
            user_finish();
        }
        else
        {
            // we're too fast, wait a bit.
            if (true)
            { // false to burn cpu
                SDL_Delay(next_render_step - now);
            }
        }
    }


    return user_nextScreen();
}