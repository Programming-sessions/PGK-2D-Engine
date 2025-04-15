#pragma once
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <allegro5/allegro.h>
#include <string>
#include <map>
#include "Logger.h"

class TextureManager {
private:
    // Singleton instance
    static TextureManager* instance;

    // Mapa przechowuj�ca tekstury (�cie�ka -> bitmap)
    std::map<std::string, ALLEGRO_BITMAP*> textures;

    // Logger do obs�ugi b��d�w
    Logger logger;

    // Prywatny konstruktor (Singleton)
    TextureManager();

    // Prywatny destruktor
    ~TextureManager();

    // Zablokowanie kopiowania
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

public:
    // Pobranie instancji singletona
    static TextureManager* getInstance();

    // Zwolnienie instancji
    static void releaseInstance();

    // �adowanie tekstury
    ALLEGRO_BITMAP* loadTexture(const std::string& path);

    // Pobranie za�adowanej tekstury
    ALLEGRO_BITMAP* getTexture(const std::string& path);

    // Sprawdzenie czy tekstura jest za�adowana
    bool isTextureLoaded(const std::string& path) const;

    // Zwolnienie pojedynczej tekstury
    void unloadTexture(const std::string& path);

    // Zwolnienie wszystkich tekstur
    void unloadAllTextures();

    // Pobranie wymiar�w tekstury
    bool getTextureDimensions(const std::string& path, int& width, int& height);
};

#endif // TEXTURE_MANAGER_H

