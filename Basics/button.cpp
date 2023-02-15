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
        if (mouseX >= viewPort.x + screenPos.x && mouseX <= viewPort.x + screenPos.x + screenPos.w && mouseY >= viewPort.y + screenPos.y && mouseY <= viewPort.y + screenPos.y + screenPos.h && e.button.button == SDL_BUTTON_LEFT)
        {
            //Set mouse over sprite
            switch (e.type)
            {
            case SDL_MOUSEMOTION:
                state = HOVER;
                break;

            case SDL_MOUSEBUTTONDOWN:
                state = CLICK;
                break;

            case SDL_MOUSEBUTTONUP:
                state = UNCLICK;
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
    if (text)
        text->render_image(window, &screenPos);
}

bool button::set_image(Window &window, std::string imagePath, std::string title, SDL_Color color)
{
    image = std::make_shared<IMG_wrapper>();
    if (!image->load_media(window, imagePath.c_str()))
        return false;
    if (title != "")
    {
        text = std::make_shared<IMG_wrapper>();
        if (!text->load_text(window, title, color, screenPos.h))
            return false;
    }
    return true;
}
