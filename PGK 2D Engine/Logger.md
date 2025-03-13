# README.md - Klasa Logger

## Spis treści
- [Wprowadzenie](#wprowadzenie)
- [Funkcjonalność](#funkcjonalność)
- [Poziomy logowania](#poziomy-logowania)
- [Metody](#metody)
- [Przykład użycia](#przykład-użycia)
- [Bezpieczeństwo](#bezpieczeństwo)
- [Rozszerzenia](#możliwe-rozszerzenia)

## Wprowadzenie

Klasa `Logger` jest narzędziem służącym do rejestrowania komunikatów diagnostycznych w aplikacjach. Umożliwia zapisywanie informacji o stanie programu, ostrzeżeń i błędów do pliku oraz wyświetlanie ich w konsoli. Została zaprojektowana z myślą o prostocie użycia, jednocześnie oferując elastyczność konfiguracji.

## Funkcjonalność

- **Zapis do pliku**: Automatyczne zapisywanie komunikatów do pliku dziennika.
- **Wyświetlanie w konsoli**: Opcjonalne wyświetlanie komunikatów w oknie konsoli.
- **Datowanie wpisów**: Automatyczne dodawanie znaczników czasowych do każdego wpisu.
- **Kategoryzacja komunikatów**: Podział na cztery poziomy ważności.
- **Natychmiastowy zapis**: Komunikaty są zapisywane bezpośrednio po wywołaniu metody (flush).
- **Obsługa destruktora**: Automatyczne zamykanie pliku w przypadku zniszczenia obiektu.

## Poziomy logowania

Klasa `Logger` oferuje cztery poziomy logowania, które pozwalają na kategoryzację komunikatów według ich ważności:

| Poziom | Enum | Opis |
|--------|------|------|
| **INFO** | `LogLevel::INFO` | Standardowe informacje o działaniu programu. |
| **WARNING** | `LogLevel::WARNING` | Ostrzeżenia, które nie przerywają działania programu, ale mogą wskazywać na potencjalne problemy. |
| **ERROR** | `LogLevel::ERROR` | Błędy, które mogą wpływać na działanie programu, ale nie powodują jego zatrzymania. |
| **FATAL** | `LogLevel::FATAL` | Krytyczne błędy, które uniemożliwiają dalsze działanie programu. |

## Metody

### Konstruktor i destruktor

```cpp
Logger();
~Logger();
```

- **Konstruktor**: Tworzy nowy obiekt loggera z domyślnymi ustawieniami.
- **Destruktor**: Automatycznie zamyka plik dziennika, jeśli jest otwarty.

### Inicjalizacja

```cpp
bool init(const std::string& filePath, bool outputToConsole = true);
```

**Parametry**:
- `filePath`: Ścieżka do pliku, w którym będą zapisywane komunikaty.
- `outputToConsole`: Flaga określająca, czy komunikaty powinny być również wyświetlane w konsoli (domyślnie `true`).

**Zwraca**:
- `true`: Jeśli inicjalizacja powiodła się i plik został poprawnie otwarty.
- `false`: W przypadku niepowodzenia otwarcia pliku.

### Metody logowania

#### Ogólna metoda logowania

```cpp
void log(LogLevel level, const std::string& message);
```

**Parametry**:
- `level`: Poziom ważności komunikatu (jeden z typów `LogLevel`).
- `message`: Treść komunikatu do zapisania.

#### Specjalistyczne metody logowania

```cpp
void info(const std::string& message);    // Komunikat informacyjny
void warning(const std::string& message); // Ostrzeżenie
void error(const std::string& message);   // Błąd
void fatal(const std::string& message);   // Błąd krytyczny
```

**Parametry**:
- `message`: Treść komunikatu do zapisania.

### Zamykanie

```cpp
void close();
```

Zamyka plik dziennika. Metoda jest wywoływana automatycznie przez destruktor, ale można ją również wywołać ręcznie, jeśli plik ma zostać zamknięty przed zniszczeniem obiektu.

## Przykład użycia

```cpp
#include "Logger.h"

int main() {
    // Utworzenie i inicjalizacja loggera
    Logger logger;
    if (!logger.init("aplikacja.log")) {
        return -1;
    }
    
    // Zapis różnych typów komunikatów
    logger.info("Aplikacja uruchomiona");
    
    int liczbaUzytkownikow = 10;
    logger.info("Liczba użytkowników: " + std::to_string(liczbaUzytkownikow));
    
    if (liczbaUzytkownikow > 100) {
        logger.warning("Duża liczba użytkowników może wpływać na wydajność");
    }
    
    try {
        // Operacja, która może zakończyć się wyjątkiem
        if (/*warunek błędu*/ true) {
            throw std::runtime_error("Przykładowy błąd");
        }
    } catch (const std::exception& e) {
        logger.error("Wystąpił błąd: " + std::string(e.what()));
    }
    
    // Ręczne zamknięcie loggera (opcjonalne)
    logger.close();
    
    return 0;
}
```

## Struktura wpisu w dzienniku

Każdy wpis w dzienniku ma następujący format:
```
[POZIOM] Data Godzina - Treść komunikatu
```

Przykład:
```
[INFO] Thu Oct 10 12:34:56 2024 - Aplikacja uruchomiona
[WARNING] Thu Oct 10 12:35:01 2024 - Duża liczba użytkowników może wpływać na wydajność
[ERROR] Thu Oct 10 12:35:10 2024 - Wystąpił błąd: Przykładowy błąd
```

## Bezpieczeństwo

W implementacji klasy `Logger` należy zwrócić uwagę na kilka kwestii bezpieczeństwa:

1. **Funkcja `ctime()`**: W środowisku Visual Studio funkcja ta jest oznaczona jako przestarzała i potencjalnie niebezpieczna. Zaleca się używanie bezpieczniejszej alternatywy `ctime_s()` lub stosowanie nowszych funkcji z biblioteki `<chrono>` i `<iomanip>`.

2. **Obsługa błędów**: Klasa sprawdza, czy plik dziennika został poprawnie otwarty i informuje o tym poprzez zwracaną wartość metody `init()`.

3. **Automatyczne zamykanie pliku**: Destruktor automatycznie zamyka plik dziennika, co zabezpiecza przed pozostawieniem otwartych zasobów.

## Możliwe rozszerzenia

Klasa `Logger` może być rozszerzona o dodatkowe funkcjonalności:

1. **Rotacja plików dziennika**: Automatyczne tworzenie nowych plików po osiągnięciu określonego rozmiaru lub po upływie określonego czasu.

2. **Filtry poziomów**: Możliwość konfiguracji minimalnego poziomu komunikatów zapisywanych do pliku lub wyświetlanych w konsoli.

3. **Wsparcie dla wielu kanałów**: Możliwość kierowania komunikatów do różnych plików w zależności od ich kategorii.

4. **Formatowanie wiadomości**: Możliwość dostosowania formatu wpisów w dzienniku.

5. **Wsparcie dla wątków**: Zabezpieczenie klasy do bezpiecznego używania w środowisku wielowątkowym.

6. **Integracja z systemowymi mechanizmami logowania**: Możliwość kierowania komunikatów do systemowych dzienników (np. syslog w systemach Unix).

## Uwagi implementacyjne

Przy implementacji klasy `Logger` należy pamiętać o:

1. Obsłudze wyjątków podczas operacji na plikach.
2. Poprawnym formatowaniu daty i czasu.
3. Synchronizacji w środowisku wielowątkowym, jeśli logger ma być używany przez wiele wątków.
4. Prawidłowym zamykaniu zasobów.
5. Optymalizacji operacji zapisu, szczególnie w przypadku częstego logowania.

---

© 2025 | Opracowano na potrzeby projektu silnika 2D dla Politechniki Świętokrzyskiej. 
