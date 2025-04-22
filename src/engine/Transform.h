/**
* @file Transform.h
* @brief Klasa narzędziowa do wykonywania transformacji 2D na punktach
*
* Zapewnia statyczne metody do wykonywania podstawowych transformacji geometrycznych
* w przestrzeni 2D, takich jak translacja, skalowanie i rotacja.
*/

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cmath>
#include "Primitives.h"

/** @brief Stała PI używana do obliczeń związanych z rotacją */
#define PI 3.14159265358979323846

/**
* @brief Klasa zawierająca statyczne metody do transformacji geometrycznych 2D
*
* Umożliwia wykonywanie podstawowych transformacji na punktach w przestrzeni 2D:
* - Translacja (przesunięcie)
* - Skalowanie względem zadanego punktu
* - Rotacja wokół zadanego punktu
*/
class Transform {
public:
    /**
    * @brief Przesuwa punkt o zadany wektor
    * @param point Referencja do punktu, który ma zostać przesunięty
    * @param dx Przesunięcie w osi X
    * @param dy Przesunięcie w osi Y
    *
    * Modyfikuje współrzędne punktu poprzez dodanie do nich wartości dx i dy:
    * - Nowa pozycja X = stara pozycja X + dx
    * - Nowa pozycja Y = stara pozycja Y + dy
    */
    static void translate(Point2D& point, float dx, float dy);

    /**
    * @brief Skaluje punkt względem zadanego środka
    * @param point Referencja do punktu, który ma zostać przeskalowany
    * @param sx Współczynnik skalowania w osi X
    * @param sy Współczynnik skalowania w osi Y
    * @param center Punkt środka skalowania (domyślnie punkt (0,0))
    *
    * Proces skalowania:
    * 1. Przesunięcie punktu względem środka skalowania do (0,0)
    * 2. Pomnożenie współrzędnych przez współczynniki skalowania
    * 3.Przesunięcie punktu z powrotem względem środka skalowania
    */
    static void scale(Point2D& point, float sx, float sy, const Point2D& center = Point2D(0, 0));

    /**
    * @brief Obraca punkt o zadany kąt wokół zadanego środka
    * @param point Referencja do punktu, który ma zostać obrócony
    * @param angle Kąt obrotu w stopniach
    * @param center Punkt środka obrotu (domyślnie punkt (0,0))
    *
    * Proces rotacji:
    * 1. Konwersja kąta ze stopni na radiany
    * 2. Przesunięcie punktu względem środka obrotu do (0,0)
    * 3. Zastosowanie macierzy rotacji
    * 4. Przesunięcie punktu z powrotem względem środka obrotu
    *
    * Używa następujących wzorów:
    * - x' = x * cos(θ) - y * sin(θ)
    * - y' = x * sin(θ) + y * cos(θ)
    * gdzie θ to kąt w radianach
    */
    static void rotate(Point2D& point, float angle, const Point2D& center = Point2D(0, 0));
};

#endif // TRANSFORM_H
