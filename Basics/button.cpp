#include "button.h"

button::button(SDL_Rect pos, int ws, int hs)
{
    screenPos = pos;

    for (int c = DEFAULT; c < COUNT; ++c)
    {
        clips[c] = {c * ws,
                     0,
                     ws,
                     hs};
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
        bool onScreen = (mouseX >= viewPort.x + screenPos.x && mouseX <= viewPort.x + screenPos.x + screenPos.w && mouseY >= viewPort.y + screenPos.y && mouseY <= viewPort.y + screenPos.y + screenPos.h);
    
        if ((state == DEFAULT || state>= UNCLICK) && e.type == SDL_MOUSEMOTION && onScreen) return state = HOVER;
        if ((state == DEFAULT || state == HOVER) && e.type == SDL_MOUSEBUTTONDOWN && onScreen) return state = CLICK;
        if (state == HOVER && !onScreen) return state = DEFAULT;
        if ((state == CLICK || state == CLICKED) && e.type == SDL_MOUSEBUTTONUP && onScreen) return state = UNCLICK_ON_SCREEN;
        if ((state == CLICK || state == CLICKED) && e.type == SDL_MOUSEBUTTONUP && !onScreen) return state = UNCLICK;

        if (state >= UNCLICK) return state = DEFAULT;
        if (state == CLICK) return state = CLICKED;
    }
    return state;
}

void button::plot(Window &window)
{
   // image->set_color(255, 0, 0);
   auto tmp_state = (state >= UNCLICK)  ? DEFAULT : (state == CLICKED) ? CLICK : state;
    image->render_image(window, &screenPos, &clips[tmp_state]);
    // For text, we modify the position to match the text

    if(!text) return;

    SDL_Rect new_position = screenPos;
    int gap = (screenPos.w - text->width);
    new_position.x += gap / 2;
    new_position.w -= gap;

    text->render_image(window, &new_position);
}
