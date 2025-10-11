#include "button.h"

button::button(SDL_Rect pos, double ws, double hs)
{
    screenPos = pos;

    for (int c = DEFAULT; c < COUNT; ++c)
    {
        clips[c] = {(int)(c * base::TILESIZEINPUT * ws),
                     0,
                     (int)(base::TILESIZEINPUT * ws),
                     (int)(base::TILESIZEINPUT * hs)};
    }

    // initial state
    SDL_GetMouseState(&mouseX, &mouseY);
}

button::button(SDL_Rect pos, SDL_Rect _clips[COUNT])
{
    screenPos = pos;
    for (int c = DEFAULT; c < COUNT; ++c)
    {
        clips[c] = _clips[c];
    }

    // initial state
    SDL_GetMouseState(&mouseX, &mouseY);
}

button::button(SDL_Rect pos, SDL_Rect clip)
{
    screenPos = pos;
    for (int c = DEFAULT; c < COUNT; ++c)
    {
        clips[c] = clip;
    }

    // initial state
    SDL_GetMouseState(&mouseX, &mouseY);
}

button::button(const button &b)
{
    screenPos = b.screenPos;

    for (int c = DEFAULT; c < COUNT; ++c)
    {
        clips[c] = b.clips[c];
    }

    // initial state
    SDL_GetMouseState(&mouseX, &mouseY);
}

int button::evaluate(SDL_Event &e, SDL_Rect viewPort)
{
    SDL_GetMouseState(&mouseX, &mouseY);
    //If mouse event happened
    if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
    {
        //Get mouse position
        if (mouseX >= viewPort.x + screenPos.x && mouseX <= viewPort.x + screenPos.x + screenPos.w && mouseY >= viewPort.y + screenPos.y && mouseY <= viewPort.y + screenPos.y + screenPos.h)
        {
            //Set mouse over sprite
            switch (e.type)
            {
            case SDL_MOUSEMOTION:
                state = HOVER;
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                if (e.button.button == SDL_BUTTON_LEFT)
                    state = CLICK;
                break;

            case SDL_MOUSEBUTTONUP:
                if (e.button.button == SDL_BUTTON_LEFT)
                    state = HOVER; //UNCLICK; TODO add
                break;
            }
        }
        else state = DEFAULT;
    }
    else state = DEFAULT;
    return state;
}

void button::plot(Window &window)
{
   // image->set_color(255, 0, 0);
    image->render_image(window, &screenPos, &clips[state]);
    // For text, we modify the position to match the text

    SDL_Rect new_position = screenPos;
    int gap = (screenPos.w - text->width);
    new_position.x += gap / 2;
    new_position.w -= gap;

    if (text)
        text->render_image(window, &new_position);
}
