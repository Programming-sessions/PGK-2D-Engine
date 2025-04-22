/**
 * @file Logger.h
 * @brief System logowania zdarzeñ i b³êdów
 *
 * Klasa Logger zapewnia mechanizm logowania komunikatów o ró¿nych poziomach
 * wa¿noœci. Umo¿liwia zapis do pliku oraz opcjonalne wyœwietlanie na konsoli.
 * Automatycznie dodaje znaczniki czasowe do komunikatów.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

 /**
  * @brief Poziomy wa¿noœci komunikatów logowania
  */
enum class LogLevel {
    INFO,      ///< Informacje ogólne
    WARNING,   ///< Ostrze¿enia, które nie przerywaj¹ dzia³ania programu
    ERROR,     ///< B³êdy, które mog¹ wp³ywaæ na dzia³anie programu
    FATAL      ///< Krytyczne b³êdy wymuszaj¹ce zakoñczenie programu
};

/**
 * @brief Klasa zarz¹dzaj¹ca systemem logowania
 *
 * Logger umo¿liwia:
 * - Zapis komunikatów do pliku
 * - Opcjonalne wyœwietlanie na konsoli
 * - Kategoryzacjê komunikatów wed³ug poziomów wa¿noœci
 * - Automatyczne dodawanie znaczników czasowych
 */
class Logger {
private:
    std::ofstream logFile;     ///< Strumieñ pliku logu
    bool consoleOutput;        ///< Flaga w³¹czaj¹ca wyœwietlanie na konsoli
    std::string logFilePath;   ///< Œcie¿ka do pliku logu

public:
    /**
     * @brief Konstruktor domyœlny
     *
     * Domyœlnie w³¹cza wyœwietlanie na konsoli.
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
     * @param filePath Œcie¿ka do pliku logu
     * @param outputToConsole Czy wyœwietlaæ komunikaty na konsoli
     * @return true jeœli inicjalizacja siê powiod³a, false w przeciwnym razie
     *
     * Tworzy lub otwiera plik logu w trybie dopisywania.
     * W przypadku b³êdu otwarcia pliku zwraca false.
     */
    bool init(const std::string& filePath, bool outputToConsole = true);

    /**
     * @brief Zapisuje komunikat o okreœlonym poziomie wa¿noœci
     * @param level Poziom wa¿noœci komunikatu
     * @param message Treœæ komunikatu
     *
     * Dodaje znacznik czasowy i zapisuje komunikat do pliku.
     * Jeœli w³¹czone jest wyœwietlanie na konsoli:
     * - ERROR i FATAL s¹ kierowane do cerr
     * - INFO i WARNING s¹ kierowane do cout
     */
    void log(LogLevel level, const std::string& message);

    /**
     * @brief Zapisuje komunikat informacyjny
     * @param message Treœæ komunikatu
     */
    void info(const std::string& message);

    /**
     * @brief Zapisuje ostrze¿enie
     * @param message Treœæ ostrze¿enia
     */
    void warning(const std::string& message);

    /**
     * @brief Zapisuje informacjê o b³êdzie
     * @param message Opis b³êdu
     */
    void error(const std::string& message);

    /**
     * @brief Zapisuje informacjê o b³êdzie krytycznym
     * @param message Opis b³êdu krytycznego
     */
    void fatal(const std::string& message);

    /**
     * @brief Zamyka plik logu
     *
     * Nale¿y wywo³aæ przed zakoñczeniem programu,
     * choæ destruktor równie¿ zadba o zamkniêcie pliku.
     */
    void close();
};

#endif // LOGGER_H
