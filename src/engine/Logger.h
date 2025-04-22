/**
 * @file Logger.h
 * @brief System logowania zdarze� i b��d�w
 *
 * Klasa Logger zapewnia mechanizm logowania komunikat�w o r�nych poziomach
 * wa�no�ci. Umo�liwia zapis do pliku oraz opcjonalne wy�wietlanie na konsoli.
 * Automatycznie dodaje znaczniki czasowe do komunikat�w.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

 /**
  * @brief Poziomy wa�no�ci komunikat�w logowania
  */
enum class LogLevel {
    INFO,      ///< Informacje og�lne
    WARNING,   ///< Ostrze�enia, kt�re nie przerywaj� dzia�ania programu
    ERROR,     ///< B��dy, kt�re mog� wp�ywa� na dzia�anie programu
    FATAL      ///< Krytyczne b��dy wymuszaj�ce zako�czenie programu
};

/**
 * @brief Klasa zarz�dzaj�ca systemem logowania
 *
 * Logger umo�liwia:
 * - Zapis komunikat�w do pliku
 * - Opcjonalne wy�wietlanie na konsoli
 * - Kategoryzacj� komunikat�w wed�ug poziom�w wa�no�ci
 * - Automatyczne dodawanie znacznik�w czasowych
 */
class Logger {
private:
    std::ofstream logFile;     ///< Strumie� pliku logu
    bool consoleOutput;        ///< Flaga w��czaj�ca wy�wietlanie na konsoli
    std::string logFilePath;   ///< �cie�ka do pliku logu

public:
    /**
     * @brief Konstruktor domy�lny
     *
     * Domy�lnie w��cza wy�wietlanie na konsoli.
     */
    Logger();

    /**
     * @brief Destruktor
     *
     * Automatycznie zamyka plik logu.
     */
    ~Logger();

    /**
     * @brief Inicjalizuje system logowania
     * @param filePath �cie�ka do pliku logu
     * @param outputToConsole Czy wy�wietla� komunikaty na konsoli
     * @return true je�li inicjalizacja si� powiod�a, false w przeciwnym razie
     *
     * Tworzy lub otwiera plik logu w trybie dopisywania.
     * W przypadku b��du otwarcia pliku zwraca false.
     */
    bool init(const std::string& filePath, bool outputToConsole = true);

    /**
     * @brief Zapisuje komunikat o okre�lonym poziomie wa�no�ci
     * @param level Poziom wa�no�ci komunikatu
     * @param message Tre�� komunikatu
     *
     * Dodaje znacznik czasowy i zapisuje komunikat do pliku.
     * Je�li w��czone jest wy�wietlanie na konsoli:
     * - ERROR i FATAL s� kierowane do cerr
     * - INFO i WARNING s� kierowane do cout
     */
    void log(LogLevel level, const std::string& message);

    /**
     * @brief Zapisuje komunikat informacyjny
     * @param message Tre�� komunikatu
     */
    void info(const std::string& message);

    /**
     * @brief Zapisuje ostrze�enie
     * @param message Tre�� ostrze�enia
     */
    void warning(const std::string& message);

    /**
     * @brief Zapisuje informacj� o b��dzie
     * @param message Opis b��du
     */
    void error(const std::string& message);

    /**
     * @brief Zapisuje informacj� o b��dzie krytycznym
     * @param message Opis b��du krytycznego
     */
    void fatal(const std::string& message);

    /**
     * @brief Zamyka plik logu
     *
     * Nale�y wywo�a� przed zako�czeniem programu,
     * cho� destruktor r�wnie� zadba o zamkni�cie pliku.
     */
    void close();
};

#endif // LOGGER_H
