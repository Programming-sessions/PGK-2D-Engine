/**
 * @file Primitives.h
 * @brief Definicje podstawowych klas geometrycznych 2D
 */
#ifndef PRIMITIVES_H
#define PRIMITIVES_H

 /**
  * @brief Klasa reprezentująca punkt w przestrzeni 2D
  */
class Point2D {
private:
    float x; ///< Współrzędna X punktu
    float y; ///< Współrzędna Y punktu

public:
    /**
     * @brief Konstruktor domyślny, tworzy punkt (0,0)
     */
    Point2D();
    /**
     * @brief Konstruktor z parametrami
     * @param x Współrzędna X punktu
     * @param y Współrzędna Y punktu
     */
    Point2D(float x, float y);

    /**
     * @brief Pobiera współrzędną X punktu
     * @return Wartość współrzędnej X
     */
    float getX() const;
    /**
     * @brief Pobiera współrzędną Y punktu
     * @return Wartość współrzędnej Y
     */
    float getY() const;

    /**
     * @brief Ustawia współrzędną X punktu
     * @param newX Nowa wartość współrzędnej X
     */
    void setX(float newX);
    /**
     * @brief Ustawia współrzędną Y punktu
     * @param newY Nowa wartość współrzędnej Y
     */
    void setY(float newY);
    /**
     * @brief Ustawia obie współrzędne punktu
     * @param newX Nowa wartość współrzędnej X
     * @param newY Nowa wartość współrzędnej Y
     */
    void setPosition(float newX, float newY);


    /**
     * @brief Przesuwa punkt o zadany wektor
     * @param dx Przesunięcie w osi X
     * @param dy Przesunięcie w osi Y
     */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje położenie punktu względem zadanego centrum
     * @param sx Współczynnik skalowania w osi X
     * @param sy Współczynnik skalowania w osi Y
     * @param center Punkt względem którego następuje skalowanie
     */
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    /**
     * @brief Obraca punkt o zadany kąt względem centrum
     * @param angle Kąt obrotu w radianach
     * @param center Punkt względem którego następuje obrót
     */
    void rotate(float angle, const Point2D& center = Point2D(0, 0));

    /**
     * @brief Ogranicza współrzędne punktu do zadanego zakresu
     * @param minX Minimalna wartość X
     * @param minY Minimalna wartość Y
     * @param maxX Maksymalna wartość X
     * @param maxY Maksymalna wartość Y
     */
    void clamp(float minX, float minY, float maxX, float maxY);
};

/**
 * @brief Klasa reprezentująca odcinek w przestrzeni 2D
 */
class LineSegment {
private:
    Point2D start;  ///< Punkt początkowy odcinka
    Point2D end;    ///< Punkt końcowy odcinka

public:
    /**
     * @brief Konstruktor domyślny, tworzy odcinek między punktami (0,0)
     */
    LineSegment();
    /**
     * @brief Konstruktor z punktami końcowymi
     * @param start Punkt początkowy
     * @param end Punkt końcowy
     */
    LineSegment(const Point2D& start, const Point2D& end);
    /**
     * @brief Konstruktor ze współrzędnymi punktów końcowych
     * @param x1 Współrzędna X punktu początkowego
     * @param y1 Współrzędna Y punktu początkowego
     * @param x2 Współrzędna X punktu końcowego
     * @param y2 Współrzędna Y punktu końcowego
     */
    LineSegment(float x1, float y1, float x2, float y2);


    /**
	* @brief Pobiera punkt początkowy odcinka
	* @return Punkt początkowy
    */
    Point2D getStart() const;
	/**
	* @brief Pobiera punkt końcowy odcinka
	* @return Punkt końcowy
    */
    Point2D getEnd() const;

    /**
	* @brief Ustawia punkt początkowy odcinka
	* @param point Nowy punkt początkowy
    */
    void setStart(const Point2D& point);
    /**
    * @brief Ustawia punkt końcowy odcinka
	* @param point Nowy punkt końcowy
    */
    void setEnd(const Point2D& point);
    /**
	* @brief Ustawia punkt początkowy na podstawie współrzędnych
	* @param x Współrzędna X punktu początkowego
	* @param y Współrzędna Y punktu początkowego
    */
    void setStart(float x, float y);
    /**
    * @brief Ustawia punkt końcowy na podstawie współrzędnych
    * @param x Współrzędna X punktu końcowego
    * @param y Współrzędna Y punktu końcowego
    */
    void setEnd(float x, float y);
	/**
	* @brief Ustawia odcinek na podstawie dwóch punktów
	* @param newStart Nowy punkt początkowy
	* @param newEnd Nowy punkt końcowy
	*/
    void setLine(const Point2D& newStart, const Point2D& newEnd);
    /**
	* @brief Ustawia odcinek na podstawie współrzędnych
	* @param x1 Współrzędna X punktu początkowego
	* @param y1 Współrzędna Y punktu początkowego
	* @param x2 Współrzędna X punktu końcowego
	* @param y2 Współrzędna Y punktu końcowego
	*/
    void setLine(float x1, float y1, float x2, float y2);

    /**
     * @brief Przesuwa punkt o zadany wektor
     * @param dx Przesunięcie w osi X
     * @param dy Przesunięcie w osi Y
     */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje położenie punktu względem zadanego centrum
     * @param sx Współczynnik skalowania w osi X
     * @param sy Współczynnik skalowania w osi Y
     * @param center Punkt względem którego następuje skalowanie
     */
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    /**
     * @brief Obraca punkt o zadany kąt względem centrum
     * @param angle Kąt obrotu w radianach
     * @param center Punkt względem którego następuje obrót
     */
    void rotate(float angle, const Point2D& center = Point2D(0, 0));
};

/**
 * @brief Klasa reprezentująca trójkąt w przestrzeni 2D
 */
class Triangle {
private:
    Point2D p1;    ///< Pierwszy wierzchołek trójkąta
    Point2D p2;    ///< Drugi wierzchołek trójkąta
    Point2D p3;    ///< Trzeci wierzchołek trójkąta
    bool filled;   ///< Flaga wypełnienia trójkąta

public:
    /**
    * @brief Konstruktor domyślny, tworzy trójkąt w punkcie (0,0)
    */
    Triangle();
    /**
     * @brief Konstruktor z punktami i wypełnieniem
     * @param p1 Pierwszy wierzchołek
     * @param p2 Drugi wierzchołek
     * @param p3 Trzeci wierzchołek
     * @param filled Flaga wypełnienia
     */
    Triangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled = false);
	/**
	 * @brief Konstruktor z współrzędnymi punktów i wypełnieniem
	 * @param x1 Współrzędna X pierwszego wierzchołka
	 * @param y1 Współrzędna Y pierwszego wierzchołka
	 * @param x2 Współrzędna X drugiego wierzchołka
	 * @param y2 Współrzędna Y drugiego wierzchołka
	 * @param x3 Współrzędna X trzeciego wierzchołka
	 * @param y3 Współrzędna Y trzeciego wierzchołka
	 * @param filled Flaga wypełnienia
	 */
    Triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled = false);

    /**
	* @brief Pobiera pierwszy wierzchołek trójkąta
	* @return Pierwszy wierzchołek
	*/
    Point2D getP1() const;
	/**
	* @brief Pobiera drugi wierzchołek trójkąta
	* @return Drugi wierzchołek
    */
    Point2D getP2() const;
	/**
	* @brief Pobiera trzeci wierzchołek trójkąta
	* @return Trzeci wierzchołek
	*/
    Point2D getP3() const;
	/**
	* @brief Pobiera flagę wypełnienia trójkąta
	* @return Flaga wypełnienia
	*/
    bool isFilled() const;

    /**
     * @brief Ustawia pierwszy wierzchołek trójkąta
     * @param point Nowy pierwszy wierzchołek
     */
    void setP1(const Point2D& point);
    /**
     * @brief Ustawia drugi wierzchołek trójkąta
     * @param point Nowy drugi wierzchołek
     */
    void setP2(const Point2D& point);
    /**
     * @brief Ustawia trzeci wierzchołek trójkąta
     * @param point Nowy trzeci wierzchołek
     */
    void setP3(const Point2D& point);
    /**
     * @brief Ustawia wszystkie wierzchołki trójkąta
     * @param p1 Nowy pierwszy wierzchołek
     * @param p2 Nowy drugi wierzchołek
     * @param p3 Nowy trzeci wierzchołek
     */
    void setPoints(const Point2D& p1, const Point2D& p2, const Point2D& p3);
    /**
     * @brief Ustawia wszystkie wierzchołki trójkąta na podstawie współrzędnych
     * @param x1 Współrzędna X pierwszego wierzchołka
     * @param y1 Współrzędna Y pierwszego wierzchołka
     * @param x2 Współrzędna X drugiego wierzchołka
     * @param y2 Współrzędna Y drugiego wierzchołka
     * @param x3 Współrzędna X trzeciego wierzchołka
     * @param y3 Współrzędna Y trzeciego wierzchołka
     */
    void setPoints(float x1, float y1, float x2, float y2, float x3, float y3);
    /**
     * @brief Ustawia flagę wypełnienia trójkąta
     * @param fill Nowa wartość flagi wypełnienia
     */
    void setFilled(bool fill);

    /**
      * @brief Przesuwa trójkąt o zadany wektor
      * @param dx Przesunięcie w osi X
      * @param dy Przesunięcie w osi Y
      */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje trójkąt względem zadanego centrum
     * @param sx Współczynnik skalowania w osi X
     * @param sy Współczynnik skalowania w osi Y
     * @param center Punkt względem którego następuje skalowanie
     */
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    /**
     * @brief Obraca trójkąt o zadany kąt względem centrum
     * @param angle Kąt obrotu w radianach
     * @param center Punkt względem którego następuje obrót
     */
    void rotate(float angle, const Point2D& center = Point2D(0, 0));
};

/**
 * @brief Klasa reprezentująca prostokąt w przestrzeni 2D
 */
class Rectangle {
private:
    Point2D topLeft;    ///< Lewy górny wierzchołek prostokąta
    float width;        ///< Szerokość prostokąta
    float height;       ///< Wysokość prostokąta
    bool filled;        ///< Flaga wypełnienia prostokąta

public:
    /**
     * @brief Konstruktor domyślny, tworzy prostokąt w punkcie (0,0) o wymiarach 0x0
     */
    Rectangle();
    /**
     * @brief Konstruktor z lewym górnym wierzchołkiem, szerokością, wysokością i wypełnieniem
     * @param topLeft Lewy górny wierzchołek
     * @param width Szerokość prostokąta
     * @param height Wysokość prostokąta
     * @param filled Flaga wypełnienia
     */
    Rectangle(const Point2D& topLeft, float width, float height, bool filled = false);
    /**
     * @brief Konstruktor ze współrzędnymi lewego górnego wierzchołka, szerokością, wysokością i wypełnieniem
     * @param x Współrzędna X lewego górnego wierzchołka
     * @param y Współrzędna Y lewego górnego wierzchołka
     * @param width Szerokość prostokąta
     * @param height Wysokość prostokąta
     * @param filled Flaga wypełnienia
     */
    Rectangle(float x, float y, float width, float height, bool filled = false);

    /**
     * @brief Pobiera lewy górny wierzchołek prostokąta
     * @return Lewy górny wierzchołek
     */
    Point2D getTopLeft() const;
    /**
     * @brief Pobiera prawy górny wierzchołek prostokąta
     * @return Prawy górny wierzchołek
     */
    Point2D getTopRight() const;
    /**
     * @brief Pobiera lewy dolny wierzchołek prostokąta
     * @return Lewy dolny wierzchołek
     */
    Point2D getBottomLeft() const;
    /**
     * @brief Pobiera prawy dolny wierzchołek prostokąta
     * @return Prawy dolny wierzchołek
     */
    Point2D getBottomRight() const;
    /**
     * @brief Pobiera szerokość prostokąta
     * @return Szerokość prostokąta
     */
    float getWidth() const;
    /**
     * @brief Pobiera wysokość prostokąta
     * @return Wysokość prostokąta
     */
    float getHeight() const;
    /**
     * @brief Pobiera flagę wypełnienia prostokąta
     * @return Flaga wypełnienia
     */
    bool isFilled() const;

    /**
     * @brief Ustawia lewy górny wierzchołek prostokąta
     * @param point Nowy lewy górny wierzchołek
     */
    void setTopLeft(const Point2D& point);
    /**
     * @brief Ustawia lewy górny wierzchołek prostokąta na podstawie współrzędnych
     * @param x Nowa współrzędna X lewego górnego wierzchołka
     * @param y Nowa współrzędna Y lewego górnego wierzchołka
     */
    void setTopLeft(float x, float y);
    /**
     * @brief Ustawia szerokość i wysokość prostokąta
     * @param width Nowa szerokość prostokąta
     * @param height Nowa wysokość prostokąta
     */
    void setSize(float width, float height);
    /**
     * @brief Ustawia flagę wypełnienia prostokąta
     * @param fill Nowa wartość flagi wypełnienia
     */
    void setFilled(bool fill);


    /**
    * @brief Przesuwa prostokąt o zadany wektor
    * @param dx Przesunięcie w osi X
    * @param dy Przesunięcie w osi Y
    */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje prostokąt względem zadanego centrum
     * @param sx Współczynnik skalowania w osi X
     * @param sy Współczynnik skalowania w osi Y
     * @param center Punkt względem którego następuje skalowanie
     */
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    /**
     * @brief Obraca prostokąt o zadany kąt względem centrum
     * @param angle Kąt obrotu w radianach
     * @param center Punkt względem którego następuje obrót
     */
    void rotate(float angle, const Point2D& center = Point2D(0, 0));


    /**
     * @brief Sprawdza, czy dany punkt znajduje się wewnątrz prostokąta
     * @param point Sprawdzany punkt
     * @return true jeśli punkt jest wewnątrz, false w przeciwnym razie
     */
    bool contains(const Point2D& point) const;
    /**
     * @brief Sprawdza, czy ten prostokąt przecina się z innym prostokątem
     * @param other Drugi prostokąt do sprawdzenia przecięcia
     * @return true jeśli prostokąty się przecinają, false w przeciwnym razie
     */
    bool intersects(const Rectangle& other) const;
};

/**
 * @brief Klasa reprezentująca okrąg w przestrzeni 2D
 */
class Circle {
private:
    Point2D center; ///< Środek okręgu
    float radius;   ///< Promień okręgu
    bool filled;    ///< Flaga wypełnienia okręgu


public:
    /**
     * @brief Konstruktor domyślny, tworzy okrąg o środku w (0,0) i promieniu 0
     */
    Circle();
    /**
     * @brief Konstruktor z środkiem, promieniem i wypełnieniem
     * @param center Środek okręgu
     * @param radius Promień okręgu
     * @param filled Flaga wypełnienia
     */
    Circle(const Point2D& center, float radius, bool filled = false);
    /**
     * @brief Konstruktor ze współrzędnymi środka, promieniem i wypełnieniem
     * @param x Współrzędna X środka okręgu
     * @param y Współrzędna Y środka okręgu
     * @param radius Promień okręgu
     * @param filled Flaga wypełnienia
     */
    Circle(float x, float y, float radius, bool filled = false);

    /**
     * @brief Pobiera środek okręgu
     * @return Środek okręgu
     */
    Point2D getCenter() const;
    /**
     * @brief Pobiera promień okręgu
     * @return Promień okręgu
     */
    float getRadius() const;
    /**
     * @brief Pobiera flagę wypełnienia okręgu
     * @return Flaga wypełnienia
     */
    bool isFilled() const;


    /**
     * @brief Ustawia środek okręgu
     * @param point Nowy środek okręgu
     */
    void setCenter(const Point2D& point);
    /**
     * @brief Ustawia środek okręgu na podstawie współrzędnych
     * @param x Nowa współrzędna X środka okręgu
     * @param y Nowa współrzędna Y środka okręgu
     */
    void setCenter(float x, float y);
    /**
     * @brief Ustawia promień okręgu
     * @param newRadius Nowy promień okręgu
     */
    void setRadius(float newRadius);
    /**
     * @brief Ustawia flagę wypełnienia okręgu
     * @param fill Nowa wartość flagi wypełnienia
     */
    void setFilled(bool fill);


    /**
     * @brief Przesuwa okrąg o zadany wektor
     * @param dx Przesunięcie w osi X
     * @param dy Przesunięcie w osi Y
     */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje okrąg
     * @param s Współczynnik skalowania (jednakowy dla obu osi)
     */
    void scale(float s);  // Dla koła sx i sy powinny być równe
    // Koło nie potrzebuje rotacji, bo jest symetryczne

    /**
     * @brief Sprawdza, czy dany punkt znajduje się wewnątrz okręgu
     * @param point Sprawdzany punkt
     * @return true jeśli punkt jest wewnątrz, false w przeciwnym razie
     */
    bool contains(const Point2D& point) const;
};
#endif // PRIMITIVES_H
