#include "IMG_wrapper.h"

typedef IMG_wrapper IMGW;

IMGW IMGW::defaultImage;

void IMGW::clear()
{
    SDL_DestroyTexture(image);
    image = nullptr;
    width = -1;
    height = -1;

    //Free global font
    TTF_CloseFont(gFont);
    gFont = nullptr;
}

void IMGW::set_color(Uint8 red, Uint8 green, Uint8 blue)
{
    //Modulate texture
    SDL_SetTextureColorMod(image, red, green, blue);
}

bool IMGW::load_media(Window &window, const char *path)
{
    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path);
    if (loadedSurface == nullptr)
    {
        printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
        loadedSurface = IMG_Load("data/placeholder.png");
    }
    // deletion is handled by surf_to_texture function
    return surf_to_texture(window.sdlRenderer, loadedSurface);
}

bool IMGW::load_text(Window &window, std::string text, SDL_Color color, uint textSize, uint textWidth)
{
    // Open the font
    // TODO: Configurable font
    // gFont = TTF_OpenFont("data/lazy.ttf", textSize);
    gFont = TTF_OpenFont("data/HighlandGothicFLF.ttf", textSize);
    if (gFont == nullptr)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    //Load image at specified path
    //SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, text.c_str(), color );
    //SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(gFont, text.c_str(), color, textWidth);
    SDL_Surface *textSurface = TTF_RenderText_Blended(gFont, text.c_str(), color); //, textWidth);

    if (textSurface == nullptr)
    {
        printf("Unable to generate text %s! SDL Error: %s\n", text.c_str(), SDL_GetError());
        return false;
    }
    // deletion is handled by surf_to_texture function
    return surf_to_texture(window.sdlRenderer, textSurface);
}

bool IMGW::surf_to_texture(SDL_Renderer *renderer, SDL_Surface *loadedSurface)
{
    // TMP assumes valid input... check?
    clear();

    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

    //Create texture from surface pixels
    image = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (image == nullptr)
    {
        printf("Unable to create texture! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    width = loadedSurface->w;
    height = loadedSurface->h;

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);

    return true;
}

void IMGW::render_image(Window &window, int x, int y, SDL_Rect *clip, double angle)
{
    if (image == nullptr) load_media(window, "data/placeholder.png");
    //Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, width, height};

    //Set clip rendering dimensions
    if (clip != nullptr)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    render_image(window, &renderQuad, clip, angle);
}

void IMGW::render_image(Window &window, SDL_Rect *renderQuad, SDL_Rect *clip, double angle, SDL_RendererFlip flip)
{
    if (image == nullptr) load_media(window, "data/placeholder.png");
    // Check clip size compared to width, height
    if (clip != nullptr)
    {
        if(clip->x + clip->w > width)
            clip = nullptr;
        else if(clip->y + clip->h > height)
            clip = nullptr;
    }
    
    //Render texture to screen
    SDL_RenderCopyEx(window.sdlRenderer, image, clip, renderQuad, angle, nullptr, flip);
}