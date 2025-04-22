/**
 * @file TextureManager.h
 * @brief Mened¿er tekstur implementuj¹cy wzorzec Singleton
 *
 * Klasa zarz¹dza ³adowaniem, przechowywaniem i zwalnianiem tekstur.
 * Zapobiega wielokrotnemu ³adowaniu tych samych tekstur i zapewnia
 * centralne miejsce zarz¹dzania zasobami graficznymi.
 */

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <allegro5/allegro.h>
#include <string>
#include <map>
#include "Logger.h"

 /**
  * @brief Klasa zarz¹dzaj¹ca teksturami w aplikacji
  *
  * TextureManager implementuje wzorzec Singleton i odpowiada za:
  * - £adowanie tekstur z plików
  * - Buforowanie za³adowanych tekstur
  * - Zarz¹dzanie pamiêci¹ tekstur
  * - Dostarczanie tekstur do innych komponentów
  */
class TextureManager {
private:
    static TextureManager* instance;    ///< Instancja Singletona

    /** @brief Mapa przechowuj¹ca za³adowane tekstury (œcie¿ka -> bitmapa) */
    std::map<std::string, ALLEGRO_BITMAP*> textures;

    Logger logger;    ///< Logger do rejestrowania operacji i b³êdów

    /**
     * @brief Prywatny konstruktor (wzorzec Singleton)
     *
     * Inicjalizuje dodatek do obs³ugi obrazów Allegro.
     */
    TextureManager();

    /**
     * @brief Prywatny destruktor
     *
     * Zwalnia wszystkie za³adowane tekstury.
     */
    ~TextureManager();

    // Zablokowanie kopiowania (Singleton)
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

public:
    /**
     * @brief Pobiera instancjê mened¿era tekstur
     * @return WskaŸnik na jedyn¹ instancjê klasy
     *
     * Tworzy instancjê przy pierwszym wywo³aniu.
     */
    static TextureManager* getInstance();

    /**
     * @brief Zwalnia instancjê mened¿era tekstur
     *
     * Nale¿y wywo³aæ przed zakoñczeniem programu.
     * Zwalnia wszystkie za³adowane tekstury.
     */
    static void releaseInstance();

    /**
     * @brief £aduje teksturê z pliku
     * @param path Œcie¿ka do pliku tekstury
     * @return WskaŸnik na za³adowan¹ bitmapê lub nullptr w przypadku b³êdu
     *
     * Jeœli tekstura by³a ju¿ wczeœniej za³adowana, zwraca istniej¹c¹ instancjê.
     */
    ALLEGRO_BITMAP* loadTexture(const std::string& path);

    /**
     * @brief Pobiera za³adowan¹ teksturê
     * @param path Œcie¿ka do pliku tekstury
     * @return WskaŸnik na bitmapê lub nullptr jeœli tekstura nie jest za³adowana
     *
     * Jeœli tekstura nie jest za³adowana, próbuje j¹ za³adowaæ.
     */
    ALLEGRO_BITMAP* getTexture(const std::string& path);

    /**
     * @brief Sprawdza czy tekstura jest ju¿ za³adowana
     * @param path Œcie¿ka do pliku tekstury
     * @return true jeœli tekstura jest za³adowana
     */
    bool isTextureLoaded(const std::string& path) const;

    /**
     * @brief Zwalnia pojedyncz¹ teksturê
     * @param path Œcie¿ka do pliku tekstury
     *
     * Usuwa teksturê z pamiêci i z mapy tekstur.
     */
    void unloadTexture(const std::string& path);

    /**
     * @brief Zwalnia wszystkie za³adowane tekstury
     *
     * Czyœci ca³¹ pamiêæ zajmowan¹ przez tekstury.
     */
    void unloadAllTextures();

    /**
     * @brief Pobiera wymiary tekstury
     * @param path Œcie¿ka do pliku tekstury
     * @param[out] width Szerokoœæ tekstury w pikselach
     * @param[out] height Wysokoœæ tekstury w pikselach
     * @return true jeœli uda³o siê pobraæ wymiary
     *
     * Jeœli tekstura nie jest za³adowana, próbuje j¹ za³adowaæ.
     */
    bool getTextureDimensions(const std::string& path, int& width, int& height);
};

#endif // TEXTURE_MANAGER_H
