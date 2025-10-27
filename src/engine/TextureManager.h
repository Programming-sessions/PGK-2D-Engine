/**
 * @file TextureManager.h
 * @brief Menedżer tekstur implementujący wzorzec Singleton
 *
 * Klasa zarządza ładowaniem, przechowywaniem i zwalnianiem tekstur.
 * Zapobiega wielokrotnemu ładowaniu tych samych tekstur i zapewnia
 * centralne miejsce zarządzania zasobami graficznymi.
 */

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <allegro5/allegro.h>
#include <string>
#include <map>
#include "Logger.h"

 /**
  * @brief Klasa zarządzająca teksturami w aplikacji
  *
  * TextureManager implementuje wzorzec Singleton i odpowiada za:
  * - Ładowanie tekstur z plików
  * - Buforowanie załadowanych tekstur
  * - Zarządzanie pamięcią tekstur
  * - Dostarczanie tekstur do innych komponentów
  */
class TextureManager {
private:
    static TextureManager* instance;    ///< Instancja Singletona

    /** @brief Mapa przechowująca załadowane tekstury (ścieżka -> bitmapa) */
    std::map<std::string, ALLEGRO_BITMAP*> textures;

    /**
     * @brief Prywatny konstruktor (wzorzec Singleton)
     *
     * Inicjalizuje dodatek do obsługi obrazów Allegro.
     */
    TextureManager();

    /**
     * @brief Prywatny destruktor
     *
     * Zwalnia wszystkie załadowane tekstury.
     */
    ~TextureManager();

    // Zablokowanie kopiowania (Singleton)
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

public:
    /**
     * @brief Pobiera instancję menedżera tekstur
     * @return Wskaźnik na jedyną instancję klasy
     *
     * Tworzy instancję przy pierwszym wywołaniu.
     */
    static TextureManager* getInstance();

    /**
     * @brief Zwalnia instancję menedżera tekstur
     *
     * Należy wywołać przed zakończeniem programu.
     * Zwalnia wszystkie załadowane tekstury.
     */
    static void releaseInstance();

    /**
     * @brief Ładuje teksturę z pliku
     * @param path Ścieżka do pliku tekstury
     * @return Wskaźnik na załadowaną bitmapę lub nullptr w przypadku błędu
     *
     * Jeśli tekstura była już wcześniej załadowana, zwraca istniejącą instancję.
     */
    ALLEGRO_BITMAP* loadTexture(const std::string& path);

    /**
     * @brief Pobiera załadowaną teksturę
     * @param path Ścieżka do pliku tekstury
     * @return Wskaźnik na bitmapę lub nullptr jeśli tekstura nie jest załadowana
     *
     * Jeśli tekstura nie jest załadowana, próbuje ją załadować.
     */
    ALLEGRO_BITMAP* getTexture(const std::string& path);

    /**
     * @brief Sprawdza czy tekstura jest już załadowana
     * @param path Ścieżka do pliku tekstury
     * @return true jeśli tekstura jest załadowana
     */
    bool isTextureLoaded(const std::string& path) const;

    /**
     * @brief Zwalnia pojedynczą teksturę
     * @param path Ścieżka do pliku tekstury
     *
     * Usuwa teksturę z pamięci i z mapy tekstur.
     */
    void unloadTexture(const std::string& path);

    /**
     * @brief Zwalnia wszystkie załadowane tekstury
     *
     * Czyści całą pamięć zajmowaną przez tekstury.
     */
    void unloadAllTextures();

    /**
     * @brief Pobiera wymiary tekstury
     * @param path Ścieżka do pliku tekstury
     * @param[out] width Szerokość tekstury w pikselach
     * @param[out] height Wysokość tekstury w pikselach
     * @return true jeśli udało się pobrać wymiary
     *
     * Jeśli tekstura nie jest załadowana, próbuje ją załadować.
     */
    bool getTextureDimensions(const std::string& path, int& width, int& height);
};

#endif // TEXTURE_MANAGER_H
