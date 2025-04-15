// TextureManager.cpp
#include "TextureManager.h"
#include <allegro5/allegro_image.h>

TextureManager* TextureManager::instance = nullptr;

TextureManager::TextureManager() {
    // Inicjalizacja dodatku do obs³ugi obrazów
    if (!al_init_image_addon()) {
        logger.error("Failed to initialize image addon!");
    }
}

TextureManager::~TextureManager() {
    unloadAllTextures();
}

TextureManager* TextureManager::getInstance() {
    if (instance == nullptr) {
        instance = new TextureManager();
    }
    return instance;
}

void TextureManager::releaseInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

ALLEGRO_BITMAP* TextureManager::loadTexture(const std::string& path) {
    // SprawdŸ czy tekstura ju¿ jest za³adowana
    if (isTextureLoaded(path)) {
        return textures[path];
    }

    // Za³aduj now¹ teksturê
    ALLEGRO_BITMAP* texture = al_load_bitmap(path.c_str());

    if (texture == nullptr) {
        logger.error("Failed to load texture: " + path);
        return nullptr;
    }

    // Zapisz teksturê w mapie
    textures[path] = texture;
    logger.info("Texture loaded: " + path);

    return texture;
}

ALLEGRO_BITMAP* TextureManager::getTexture(const std::string& path) {
    if (!isTextureLoaded(path)) {
        return loadTexture(path);
    }
    return textures[path];
}

bool TextureManager::isTextureLoaded(const std::string& path) const {
    return textures.find(path) != textures.end();
}

void TextureManager::unloadTexture(const std::string& path) {
    auto it = textures.find(path);
    if (it != textures.end()) {
        al_destroy_bitmap(it->second);
        textures.erase(it);
        logger.info("Texture unloaded: " + path);
    }
}

void TextureManager::unloadAllTextures() {
    for (auto& pair : textures) {
        al_destroy_bitmap(pair.second);
        logger.info("Texture unloaded: " + pair.first);
    }
    textures.clear();
}

bool TextureManager::getTextureDimensions(const std::string& path, int& width, int& height) {
    ALLEGRO_BITMAP* texture = getTexture(path);
    if (texture == nullptr) {
        return false;
    }

    width = al_get_bitmap_width(texture);
    height = al_get_bitmap_height(texture);
    return true;
}
