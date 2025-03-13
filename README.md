# README.txt - Silnik Gry 2D

## Wprowadzenie

Ten projekt zawiera prosty silnik gry 2D stworzony na potrzeby zajęć z Grafiki Komputerowej na Politechnice Świętokrzyskiej. Silnik implementuje podstawowe funkcjonalności niezbędne do tworzenia i uruchamiania gier 2D, takie jak główna pętla gry, obsługa urządzeń wejścia oraz system logowania.

## Komponenty silnika

Silnik składa się z następujących głównych klas:

- [Engine](PGK-2D-Engine/Engine.md) - główna klasa silnika odpowiedzialna za inicjalizację biblioteki graficznej, obsługę głównej pętli gry oraz urządzeń wejścia
- [Logger](PGK-2D-Engine/Logger.md) - system logowania informacji, ostrzeżeń i błędów

## Wymagania systemowe

- C++ 11 lub nowszy
- Biblioteka Allegro 5.x
- Kompilator wspierający standard C++11 (g++, Visual C++)

## Szybki start

1. Dołącz pliki nagłówkowe i źródłowe silnika do swojego projektu
2. Zainicjuj silnik w swoim programie głównym:

```cpp
#include "Engine.h"

int main() {
    Engine* engine = Engine::getInstance();
    
    engine->setScreenSize(800, 600);
    engine->setTitle("Moja Gra");
    
    if (!engine->init()) {
        return -1;
    }
    
    engine->startGameLoop(update, render);
    engine->shutdown();
    
    return 0;
}
```

## Struktura projektu



## Planowane rozszerzenia



## Autorzy

- Rafał Ścibor
- Dawid Makuch
- Marek Nowacki

---

© 2024 | Politechnika Świętokrzyska w Kielcach | Wydział Elektrotechniki, Automatyki i Informatyki 
