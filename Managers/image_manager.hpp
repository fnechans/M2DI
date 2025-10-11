#include "SDL_wrapper/IMG_wrapper.h"

class ImageManager
{
public:
    ImageManager(Window &window) : window(&window) {}
    void add_image(const std::string &name, const std::string &imagePath)
    {
        if (images.count(name)>0)
            throw std::runtime_error("Image of name "+name+" already "
                "exists in the manager.");
        images.emplace(name, std::make_unique<IMG_wrapper>());
        if (!images[name]->load_media(*window, imagePath.c_str()))
            throw std::runtime_error("Failed to load image " + name);
    }
    IMG_wrapper * add_text(const std::string &name, const std::string &text, SDL_Color color, uint size, uint width)
    {
        if (texts.count(name)>0)
            throw std::runtime_error("Text of name "+name+" already "
                "exists in the manager.");
        texts.emplace(name, std::make_unique<IMG_wrapper>());
        if (!texts[name]->load_text(*window, text, color, size, width))
            throw std::runtime_error("Failed to load text " + name);
        return texts[name].get();
    }

    IMG_wrapper *get_image(const std::string &name) { return images.at(name).get(); }
    IMG_wrapper *get_text(const std::string &name) { return texts.at(name).get(); }
    void delete_image(const std::string &name) { images.erase(name); }
    void delete_text(const std::string &name) { texts.erase(name); }
    void delete_texts() { texts.clear(); }

private:
    std::map<std::string, std::unique_ptr<IMG_wrapper>> images;
    std::map<std::string, std::unique_ptr<IMG_wrapper>> texts;
    Window *window;
};