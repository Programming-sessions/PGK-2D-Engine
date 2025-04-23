/**
 * @file Primitives.h
 * @brief Definicje podstawowych klas geometrycznych 2D
 */
#ifndef PRIMITIVES_H
#define PRIMITIVES_H

 /**
  * @brief Klasa reprezentuj�ca punkt w przestrzeni 2D
  */
class Point2D {
private:
    float x; ///< Wsp�rz�dna X punktu
    float y; ///< Wsp�rz�dna Y punktu

public:
    /**
     * @brief Konstruktor domy�lny, tworzy punkt (0,0)
     */
    Point2D();
    /**
     * @brief Konstruktor z parametrami
     * @param x Wsp�rz�dna X punktu
     * @param y Wsp�rz�dna Y punktu
     */
    Point2D(float x, float y);

    /**
     * @brief Pobiera wsp�rz�dn� X punktu
     * @return Warto�� wsp�rz�dnej X
     */
    float getX() const;
    /**
     * @brief Pobiera wsp�rz�dn� Y punktu
     * @return Warto�� wsp�rz�dnej Y
     */
    float getY() const;

    /**
     * @brief Ustawia wsp�rz�dn� X punktu
     * @param newX Nowa warto�� wsp�rz�dnej X
     */
    void setX(float newX);
    /**
     * @brief Ustawia wsp�rz�dn� Y punktu
     * @param newY Nowa warto�� wsp�rz�dnej Y
     */
    void setY(float newY);
    /**
     * @brief Ustawia obie wsp�rz�dne punktu
     * @param newX Nowa warto�� wsp�rz�dnej X
     * @param newY Nowa warto�� wsp�rz�dnej Y
     */
    void setPosition(float newX, float newY);


    /**
     * @brief Przesuwa punkt o zadany wektor
     * @param dx Przesuni�cie w osi X
     * @param dy Przesuni�cie w osi Y
     */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje po�o�enie punktu wzgl�dem zadanego centrum
     * @param sx Wsp�czynnik skalowania w osi X
     * @param sy Wsp�czynnik skalowania w osi Y
     * @param center Punkt wzgl�dem kt�rego nast�puje skalowanie
     */
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    /**
     * @brief Obraca punkt o zadany k�t wzgl�dem centrum
     * @param angle K�t obrotu w radianach
     * @param center Punkt wzgl�dem kt�rego nast�puje obr�t
     */
    void rotate(float angle, const Point2D& center = Point2D(0, 0));

    /**
     * @brief Ogranicza wsp�rz�dne punktu do zadanego zakresu
     * @param minX Minimalna warto�� X
     * @param minY Minimalna warto�� Y
     * @param maxX Maksymalna warto�� X
     * @param maxY Maksymalna warto�� Y
     */
    void clamp(float minX, float minY, float maxX, float maxY);
};

/**
 * @brief Klasa reprezentuj�ca odcinek w przestrzeni 2D
 */
class LineSegment {
private:
    Point2D start;  ///< Punkt pocz�tkowy odcinka
    Point2D end;    ///< Punkt ko�cowy odcinka

public:
    /**
     * @brief Konstruktor domy�lny, tworzy odcinek mi�dzy punktami (0,0)
     */
    LineSegment();
    /**
     * @brief Konstruktor z punktami ko�cowymi
     * @param start Punkt pocz�tkowy
     * @param end Punkt ko�cowy
     */
    LineSegment(const Point2D& start, const Point2D& end);
    /**
     * @brief Konstruktor ze wsp�rz�dnymi punkt�w ko�cowych
     * @param x1 Wsp�rz�dna X punktu pocz�tkowego
     * @param y1 Wsp�rz�dna Y punktu pocz�tkowego
     * @param x2 Wsp�rz�dna X punktu ko�cowego
     * @param y2 Wsp�rz�dna Y punktu ko�cowego
     */
    LineSegment(float x1, float y1, float x2, float y2);


    /**
	* @brief Pobiera punkt pocz�tkowy odcinka
	* @return Punkt pocz�tkowy
    */
    Point2D getStart() const;
	/**
	* @brief Pobiera punkt ko�cowy odcinka
	* @return Punkt ko�cowy
    */
    Point2D getEnd() const;

    /**
	* @brief Ustawia punkt pocz�tkowy odcinka
	* @param point Nowy punkt pocz�tkowy
    */
    void setStart(const Point2D& point);
    /**
    * @brief Ustawia punkt ko�cowy odcinka
	* @param point Nowy punkt ko�cowy
    */
    void setEnd(const Point2D& point);
    /**
	* @brief Ustawia punkt pocz�tkowy na podstawie wsp�rz�dnych
	* @param x Wsp�rz�dna X punktu pocz�tkowego
	* @param y Wsp�rz�dna Y punktu pocz�tkowego
    */
    void setStart(float x, float y);
    /**
    * @brief Ustawia punkt ko�cowy na podstawie wsp�rz�dnych
    * @param x Wsp�rz�dna X punktu ko�cowego
    * @param y Wsp�rz�dna Y punktu ko�cowego
    */
    void setEnd(float x, float y);
	/**
	* @brief Ustawia odcinek na podstawie dw�ch punkt�w
	* @param newStart Nowy punkt pocz�tkowy
	* @param newEnd Nowy punkt ko�cowy
	*/
    void setLine(const Point2D& newStart, const Point2D& newEnd);
    /**
	* @brief Ustawia odcinek na podstawie wsp�rz�dnych
	* @param x1 Wsp�rz�dna X punktu pocz�tkowego
	* @param y1 Wsp�rz�dna Y punktu pocz�tkowego
	* @param x2 Wsp�rz�dna X punktu ko�cowego
	* @param y2 Wsp�rz�dna Y punktu ko�cowego
	*/
    void setLine(float x1, float y1, float x2, float y2);

    /**
     * @brief Przesuwa punkt o zadany wektor
     * @param dx Przesuni�cie w osi X
     * @param dy Przesuni�cie w osi Y
     */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje po�o�enie punktu wzgl�dem zadanego centrum
     * @param sx Wsp�czynnik skalowania w osi X
     * @param sy Wsp�czynnik skalowania w osi Y
     * @param center Punkt wzgl�dem kt�rego nast�puje skalowanie
     */
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    /**
     * @brief Obraca punkt o zadany k�t wzgl�dem centrum
     * @param angle K�t obrotu w radianach
     * @param center Punkt wzgl�dem kt�rego nast�puje obr�t
     */
    void rotate(float angle, const Point2D& center = Point2D(0, 0));
};

/**
 * @brief Klasa reprezentuj�ca tr�jk�t w przestrzeni 2D
 */
class Triangle {
private:
    Point2D p1;    ///< Pierwszy wierzcho�ek tr�jk�ta
    Point2D p2;    ///< Drugi wierzcho�ek tr�jk�ta
    Point2D p3;    ///< Trzeci wierzcho�ek tr�jk�ta
    bool filled;   ///< Flaga wype�nienia tr�jk�ta

public:
    /**
    * @brief Konstruktor domy�lny, tworzy tr�jk�t w punkcie (0,0)
    */
    Triangle();
    /**
     * @brief Konstruktor z punktami i wype�nieniem
     * @param p1 Pierwszy wierzcho�ek
     * @param p2 Drugi wierzcho�ek
     * @param p3 Trzeci wierzcho�ek
     * @param filled Flaga wype�nienia
     */
    Triangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled = false);
	/**
	 * @brief Konstruktor z wsp�rz�dnymi punkt�w i wype�nieniem
	 * @param x1 Wsp�rz�dna X pierwszego wierzcho�ka
	 * @param y1 Wsp�rz�dna Y pierwszego wierzcho�ka
	 * @param x2 Wsp�rz�dna X drugiego wierzcho�ka
	 * @param y2 Wsp�rz�dna Y drugiego wierzcho�ka
	 * @param x3 Wsp�rz�dna X trzeciego wierzcho�ka
	 * @param y3 Wsp�rz�dna Y trzeciego wierzcho�ka
	 * @param filled Flaga wype�nienia
	 */
    Triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled = false);

    /**
	* @brief Pobiera pierwszy wierzcho�ek tr�jk�ta
	* @return Pierwszy wierzcho�ek
	*/
    Point2D getP1() const;
	/**
	* @brief Pobiera drugi wierzcho�ek tr�jk�ta
	* @return Drugi wierzcho�ek
    */
    Point2D getP2() const;
	/**
	* @brief Pobiera trzeci wierzcho�ek tr�jk�ta
	* @return Trzeci wierzcho�ek
	*/
    Point2D getP3() const;
	/**
	* @brief Pobiera flag� wype�nienia tr�jk�ta
	* @return Flaga wype�nienia
	*/
    bool isFilled() const;

    /**
     * @brief Ustawia pierwszy wierzcho�ek tr�jk�ta
     * @param point Nowy pierwszy wierzcho�ek
     */
    void setP1(const Point2D& point);
    /**
     * @brief Ustawia drugi wierzcho�ek tr�jk�ta
     * @param point Nowy drugi wierzcho�ek
     */
    void setP2(const Point2D& point);
    /**
     * @brief Ustawia trzeci wierzcho�ek tr�jk�ta
     * @param point Nowy trzeci wierzcho�ek
     */
    void setP3(const Point2D& point);
    /**
     * @brief Ustawia wszystkie wierzcho�ki tr�jk�ta
     * @param p1 Nowy pierwszy wierzcho�ek
     * @param p2 Nowy drugi wierzcho�ek
     * @param p3 Nowy trzeci wierzcho�ek
     */
    void setPoints(const Point2D& p1, const Point2D& p2, const Point2D& p3);
    /**
     * @brief Ustawia wszystkie wierzcho�ki tr�jk�ta na podstawie wsp�rz�dnych
     * @param x1 Wsp�rz�dna X pierwszego wierzcho�ka
     * @param y1 Wsp�rz�dna Y pierwszego wierzcho�ka
     * @param x2 Wsp�rz�dna X drugiego wierzcho�ka
     * @param y2 Wsp�rz�dna Y drugiego wierzcho�ka
     * @param x3 Wsp�rz�dna X trzeciego wierzcho�ka
     * @param y3 Wsp�rz�dna Y trzeciego wierzcho�ka
     */
    void setPoints(float x1, float y1, float x2, float y2, float x3, float y3);
    /**
     * @brief Ustawia flag� wype�nienia tr�jk�ta
     * @param fill Nowa warto�� flagi wype�nienia
     */
    void setFilled(bool fill);

    /**
      * @brief Przesuwa tr�jk�t o zadany wektor
      * @param dx Przesuni�cie w osi X
      * @param dy Przesuni�cie w osi Y
      */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje tr�jk�t wzgl�dem zadanego centrum
     * @param sx Wsp�czynnik skalowania w osi X
     * @param sy Wsp�czynnik skalowania w osi Y
     * @param center Punkt wzgl�dem kt�rego nast�puje skalowanie
     */
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    /**
     * @brief Obraca tr�jk�t o zadany k�t wzgl�dem centrum
     * @param angle K�t obrotu w radianach
     * @param center Punkt wzgl�dem kt�rego nast�puje obr�t
     */
    void rotate(float angle, const Point2D& center = Point2D(0, 0));
};

/**
 * @brief Klasa reprezentuj�ca prostok�t w przestrzeni 2D
 */
class Rectangle {
private:
    Point2D topLeft;    ///< Lewy g�rny wierzcho�ek prostok�ta
    float width;        ///< Szeroko�� prostok�ta
    float height;       ///< Wysoko�� prostok�ta
    bool filled;        ///< Flaga wype�nienia prostok�ta

public:
    /**
     * @brief Konstruktor domy�lny, tworzy prostok�t w punkcie (0,0) o wymiarach 0x0
     */
    Rectangle();
    /**
     * @brief Konstruktor z lewym g�rnym wierzcho�kiem, szeroko�ci�, wysoko�ci� i wype�nieniem
     * @param topLeft Lewy g�rny wierzcho�ek
     * @param width Szeroko�� prostok�ta
     * @param height Wysoko�� prostok�ta
     * @param filled Flaga wype�nienia
     */
    Rectangle(const Point2D& topLeft, float width, float height, bool filled = false);
    /**
     * @brief Konstruktor ze wsp�rz�dnymi lewego g�rnego wierzcho�ka, szeroko�ci�, wysoko�ci� i wype�nieniem
     * @param x Wsp�rz�dna X lewego g�rnego wierzcho�ka
     * @param y Wsp�rz�dna Y lewego g�rnego wierzcho�ka
     * @param width Szeroko�� prostok�ta
     * @param height Wysoko�� prostok�ta
     * @param filled Flaga wype�nienia
     */
    Rectangle(float x, float y, float width, float height, bool filled = false);

    /**
     * @brief Pobiera lewy g�rny wierzcho�ek prostok�ta
     * @return Lewy g�rny wierzcho�ek
     */
    Point2D getTopLeft() const;
    /**
     * @brief Pobiera prawy g�rny wierzcho�ek prostok�ta
     * @return Prawy g�rny wierzcho�ek
     */
    Point2D getTopRight() const;
    /**
     * @brief Pobiera lewy dolny wierzcho�ek prostok�ta
     * @return Lewy dolny wierzcho�ek
     */
    Point2D getBottomLeft() const;
    /**
     * @brief Pobiera prawy dolny wierzcho�ek prostok�ta
     * @return Prawy dolny wierzcho�ek
     */
    Point2D getBottomRight() const;
    /**
     * @brief Pobiera szeroko�� prostok�ta
     * @return Szeroko�� prostok�ta
     */
    float getWidth() const;
    /**
     * @brief Pobiera wysoko�� prostok�ta
     * @return Wysoko�� prostok�ta
     */
    float getHeight() const;
    /**
     * @brief Pobiera flag� wype�nienia prostok�ta
     * @return Flaga wype�nienia
     */
    bool isFilled() const;

    /**
     * @brief Ustawia lewy g�rny wierzcho�ek prostok�ta
     * @param point Nowy lewy g�rny wierzcho�ek
     */
    void setTopLeft(const Point2D& point);
    /**
     * @brief Ustawia lewy g�rny wierzcho�ek prostok�ta na podstawie wsp�rz�dnych
     * @param x Nowa wsp�rz�dna X lewego g�rnego wierzcho�ka
     * @param y Nowa wsp�rz�dna Y lewego g�rnego wierzcho�ka
     */
    void setTopLeft(float x, float y);
    /**
     * @brief Ustawia szeroko�� i wysoko�� prostok�ta
     * @param width Nowa szeroko�� prostok�ta
     * @param height Nowa wysoko�� prostok�ta
     */
    void setSize(float width, float height);
    /**
     * @brief Ustawia flag� wype�nienia prostok�ta
     * @param fill Nowa warto�� flagi wype�nienia
     */
    void setFilled(bool fill);


    /**
    * @brief Przesuwa prostok�t o zadany wektor
    * @param dx Przesuni�cie w osi X
    * @param dy Przesuni�cie w osi Y
    */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje prostok�t wzgl�dem zadanego centrum
     * @param sx Wsp�czynnik skalowania w osi X
     * @param sy Wsp�czynnik skalowania w osi Y
     * @param center Punkt wzgl�dem kt�rego nast�puje skalowanie
     */
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    /**
     * @brief Obraca prostok�t o zadany k�t wzgl�dem centrum
     * @param angle K�t obrotu w radianach
     * @param center Punkt wzgl�dem kt�rego nast�puje obr�t
     */
    void rotate(float angle, const Point2D& center = Point2D(0, 0));


    /**
     * @brief Sprawdza, czy dany punkt znajduje si� wewn�trz prostok�ta
     * @param point Sprawdzany punkt
     * @return true je�li punkt jest wewn�trz, false w przeciwnym razie
     */
    bool contains(const Point2D& point) const;
    /**
     * @brief Sprawdza, czy ten prostok�t przecina si� z innym prostok�tem
     * @param other Drugi prostok�t do sprawdzenia przeci�cia
     * @return true je�li prostok�ty si� przecinaj�, false w przeciwnym razie
     */
    bool intersects(const Rectangle& other) const;
};

/**
 * @brief Klasa reprezentuj�ca okr�g w przestrzeni 2D
 */
class Circle {
private:
    Point2D center; ///< �rodek okr�gu
    float radius;   ///< Promie� okr�gu
    bool filled;    ///< Flaga wype�nienia okr�gu


public:
    /**
     * @brief Konstruktor domy�lny, tworzy okr�g o �rodku w (0,0) i promieniu 0
     */
    Circle();
    /**
     * @brief Konstruktor z �rodkiem, promieniem i wype�nieniem
     * @param center �rodek okr�gu
     * @param radius Promie� okr�gu
     * @param filled Flaga wype�nienia
     */
    Circle(const Point2D& center, float radius, bool filled = false);
    /**
     * @brief Konstruktor ze wsp�rz�dnymi �rodka, promieniem i wype�nieniem
     * @param x Wsp�rz�dna X �rodka okr�gu
     * @param y Wsp�rz�dna Y �rodka okr�gu
     * @param radius Promie� okr�gu
     * @param filled Flaga wype�nienia
     */
    Circle(float x, float y, float radius, bool filled = false);

    /**
     * @brief Pobiera �rodek okr�gu
     * @return �rodek okr�gu
     */
    Point2D getCenter() const;
    /**
     * @brief Pobiera promie� okr�gu
     * @return Promie� okr�gu
     */
    float getRadius() const;
    /**
     * @brief Pobiera flag� wype�nienia okr�gu
     * @return Flaga wype�nienia
     */
    bool isFilled() const;


    /**
     * @brief Ustawia �rodek okr�gu
     * @param point Nowy �rodek okr�gu
     */
    void setCenter(const Point2D& point);
    /**
     * @brief Ustawia �rodek okr�gu na podstawie wsp�rz�dnych
     * @param x Nowa wsp�rz�dna X �rodka okr�gu
     * @param y Nowa wsp�rz�dna Y �rodka okr�gu
     */
    void setCenter(float x, float y);
    /**
     * @brief Ustawia promie� okr�gu
     * @param newRadius Nowy promie� okr�gu
     */
    void setRadius(float newRadius);
    /**
     * @brief Ustawia flag� wype�nienia okr�gu
     * @param fill Nowa warto�� flagi wype�nienia
     */
    void setFilled(bool fill);


    /**
     * @brief Przesuwa okr�g o zadany wektor
     * @param dx Przesuni�cie w osi X
     * @param dy Przesuni�cie w osi Y
     */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje okr�g
     * @param s Wsp�czynnik skalowania (jednakowy dla obu osi)
     */
    void scale(float s);  // Dla ko�a sx i sy powinny by� r�wne
    // Ko�o nie potrzebuje rotacji, bo jest symetryczne

    /**
     * @brief Sprawdza, czy dany punkt znajduje si� wewn�trz okr�gu
     * @param point Sprawdzany punkt
     * @return true je�li punkt jest wewn�trz, false w przeciwnym razie
     */
    bool contains(const Point2D& point) const;
};
#endif // PRIMITIVES_H
