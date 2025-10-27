// TextureManager.cpp
#include "TextureManager.h"
#include <allegro5/allegro_image.h>

TextureManager* TextureManager::instance = nullptr;

TextureManager::TextureManager() {
    // Inicjalizacja dodatku do obsługi obrazów
    if (!al_init_image_addon()) {
        Logger::getInstance().error("Failed to initialize image addon!");
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
    // Sprawdź czy tekstura już jest załadowana
    if (isTextureLoaded(path)) {
        return textures[path];
    }

    // Załaduj nową teksturę
    ALLEGRO_BITMAP* texture = al_load_bitmap(path.c_str());

    if (texture == nullptr) {
        Logger::getInstance().error("Failed to load texture: " + path);
        return nullptr;
    }

    // Zapisz teksturę w mapie
    textures[path] = texture;
    Logger::getInstance().info("Texture loaded: " + path);

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
        Logger::getInstance().info("Texture unloaded: " + path);
    }
}

void TextureManager::unloadAllTextures() {
    for (auto& pair : textures) {
        al_destroy_bitmap(pair.second);
        Logger::getInstance().info("Texture unloaded: " + pair.first);
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
