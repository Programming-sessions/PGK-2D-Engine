/**
 * @file TextureManager.h
 * @brief Mened�er tekstur implementuj�cy wzorzec Singleton
 *
 * Klasa zarz�dza �adowaniem, przechowywaniem i zwalnianiem tekstur.
 * Zapobiega wielokrotnemu �adowaniu tych samych tekstur i zapewnia
 * centralne miejsce zarz�dzania zasobami graficznymi.
 */

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <allegro5/allegro.h>
#include <string>
#include <map>
#include "Logger.h"

 /**
  * @brief Klasa zarz�dzaj�ca teksturami w aplikacji
  *
  * TextureManager implementuje wzorzec Singleton i odpowiada za:
  * - �adowanie tekstur z plik�w
  * - Buforowanie za�adowanych tekstur
  * - Zarz�dzanie pami�ci� tekstur
  * - Dostarczanie tekstur do innych komponent�w
  */
class TextureManager {
private:
    static TextureManager* instance;    ///< Instancja Singletona

    /** @brief Mapa przechowuj�ca za�adowane tekstury (�cie�ka -> bitmapa) */
    std::map<std::string, ALLEGRO_BITMAP*> textures;

    Logger logger;    ///< Logger do rejestrowania operacji i b��d�w

    /**
     * @brief Prywatny konstruktor (wzorzec Singleton)
     *
     * Inicjalizuje dodatek do obs�ugi obraz�w Allegro.
     */
    TextureManager();

    /**
     * @brief Prywatny destruktor
     *
     * Zwalnia wszystkie za�adowane tekstury.
     */
    ~TextureManager();

    // Zablokowanie kopiowania (Singleton)
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

public:
    /**
     * @brief Pobiera instancj� mened�era tekstur
     * @return Wska�nik na jedyn� instancj� klasy
     *
     * Tworzy instancj� przy pierwszym wywo�aniu.
     */
    static TextureManager* getInstance();

    /**
     * @brief Zwalnia instancj� mened�era tekstur
     *
     * Nale�y wywo�a� przed zako�czeniem programu.
     * Zwalnia wszystkie za�adowane tekstury.
     */
    static void releaseInstance();

    /**
     * @brief �aduje tekstur� z pliku
     * @param path �cie�ka do pliku tekstury
     * @return Wska�nik na za�adowan� bitmap� lub nullptr w przypadku b��du
     *
     * Je�li tekstura by�a ju� wcze�niej za�adowana, zwraca istniej�c� instancj�.
     */
    ALLEGRO_BITMAP* loadTexture(const std::string& path);

    /**
     * @brief Pobiera za�adowan� tekstur�
     * @param path �cie�ka do pliku tekstury
     * @return Wska�nik na bitmap� lub nullptr je�li tekstura nie jest za�adowana
     *
     * Je�li tekstura nie jest za�adowana, pr�buje j� za�adowa�.
     */
    ALLEGRO_BITMAP* getTexture(const std::string& path);

    /**
     * @brief Sprawdza czy tekstura jest ju� za�adowana
     * @param path �cie�ka do pliku tekstury
     * @return true je�li tekstura jest za�adowana
     */
    bool isTextureLoaded(const std::string& path) const;

    /**
     * @brief Zwalnia pojedyncz� tekstur�
     * @param path �cie�ka do pliku tekstury
     *
     * Usuwa tekstur� z pami�ci i z mapy tekstur.
     */
    void unloadTexture(const std::string& path);

    /**
     * @brief Zwalnia wszystkie za�adowane tekstury
     *
     * Czy�ci ca�� pami�� zajmowan� przez tekstury.
     */
    void unloadAllTextures();

    /**
     * @brief Pobiera wymiary tekstury
     * @param path �cie�ka do pliku tekstury
     * @param[out] width Szeroko�� tekstury w pikselach
     * @param[out] height Wysoko�� tekstury w pikselach
     * @return true je�li uda�o si� pobra� wymiary
     *
     * Je�li tekstura nie jest za�adowana, pr�buje j� za�adowa�.
     */
    bool getTextureDimensions(const std::string& path, int& width, int& height);
};

#endif // TEXTURE_MANAGER_H
