/**
 * @file Primitives.h
 * @brief Definicje podstawowych klas geometrycznych 2D
 */
#ifndef PRIMITIVES_H
#define PRIMITIVES_H

 /**
  * @brief Klasa reprezentuj¹ca punkt w przestrzeni 2D
  */
class Point2D {
private:
    float x; ///< Wspó³rzêdna X punktu
    float y; ///< Wspó³rzêdna Y punktu

public:
    /**
     * @brief Konstruktor domyœlny, tworzy punkt (0,0)
     */
    Point2D();
    /**
     * @brief Konstruktor z parametrami
     * @param x Wspó³rzêdna X punktu
     * @param y Wspó³rzêdna Y punktu
     */
    Point2D(float x, float y);

    /**
     * @brief Pobiera wspó³rzêdn¹ X punktu
     * @return Wartoœæ wspó³rzêdnej X
     */
    float getX() const;
    /**
     * @brief Pobiera wspó³rzêdn¹ Y punktu
     * @return Wartoœæ wspó³rzêdnej Y
     */
    float getY() const;

    /**
     * @brief Ustawia wspó³rzêdn¹ X punktu
     * @param newX Nowa wartoœæ wspó³rzêdnej X
     */
    void setX(float newX);
    /**
     * @brief Ustawia wspó³rzêdn¹ Y punktu
     * @param newY Nowa wartoœæ wspó³rzêdnej Y
     */
    void setY(float newY);
    /**
     * @brief Ustawia obie wspó³rzêdne punktu
     * @param newX Nowa wartoœæ wspó³rzêdnej X
     * @param newY Nowa wartoœæ wspó³rzêdnej Y
     */
    void setPosition(float newX, float newY);


    /**
     * @brief Przesuwa punkt o zadany wektor
     * @param dx Przesuniêcie w osi X
     * @param dy Przesuniêcie w osi Y
     */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje po³o¿enie punktu wzglêdem zadanego centrum
     * @param sx Wspó³czynnik skalowania w osi X
     * @param sy Wspó³czynnik skalowania w osi Y
     * @param center Punkt wzglêdem którego nastêpuje skalowanie
     */
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    /**
     * @brief Obraca punkt o zadany k¹t wzglêdem centrum
     * @param angle K¹t obrotu w radianach
     * @param center Punkt wzglêdem którego nastêpuje obrót
     */
    void rotate(float angle, const Point2D& center = Point2D(0, 0));

    /**
     * @brief Ogranicza wspó³rzêdne punktu do zadanego zakresu
     * @param minX Minimalna wartoœæ X
     * @param minY Minimalna wartoœæ Y
     * @param maxX Maksymalna wartoœæ X
     * @param maxY Maksymalna wartoœæ Y
     */
    void clamp(float minX, float minY, float maxX, float maxY);
};

/**
 * @brief Klasa reprezentuj¹ca odcinek w przestrzeni 2D
 */
class LineSegment {
private:
    Point2D start;  ///< Punkt pocz¹tkowy odcinka
    Point2D end;    ///< Punkt koñcowy odcinka

public:
    /**
     * @brief Konstruktor domyœlny, tworzy odcinek miêdzy punktami (0,0)
     */
    LineSegment();
    /**
     * @brief Konstruktor z punktami koñcowymi
     * @param start Punkt pocz¹tkowy
     * @param end Punkt koñcowy
     */
    LineSegment(const Point2D& start, const Point2D& end);
    /**
     * @brief Konstruktor ze wspó³rzêdnymi punktów koñcowych
     * @param x1 Wspó³rzêdna X punktu pocz¹tkowego
     * @param y1 Wspó³rzêdna Y punktu pocz¹tkowego
     * @param x2 Wspó³rzêdna X punktu koñcowego
     * @param y2 Wspó³rzêdna Y punktu koñcowego
     */
    LineSegment(float x1, float y1, float x2, float y2);


    /**
	* @brief Pobiera punkt pocz¹tkowy odcinka
	* @return Punkt pocz¹tkowy
    */
    Point2D getStart() const;
	/**
	* @brief Pobiera punkt koñcowy odcinka
	* @return Punkt koñcowy
    */
    Point2D getEnd() const;

    /**
	* @brief Ustawia punkt pocz¹tkowy odcinka
	* @param point Nowy punkt pocz¹tkowy
    */
    void setStart(const Point2D& point);
    /**
    * @brief Ustawia punkt koñcowy odcinka
	* @param point Nowy punkt koñcowy
    */
    void setEnd(const Point2D& point);
    /**
	* @brief Ustawia punkt pocz¹tkowy na podstawie wspó³rzêdnych
	* @param x Wspó³rzêdna X punktu pocz¹tkowego
	* @param y Wspó³rzêdna Y punktu pocz¹tkowego
    */
    void setStart(float x, float y);
    /**
    * @brief Ustawia punkt koñcowy na podstawie wspó³rzêdnych
    * @param x Wspó³rzêdna X punktu koñcowego
    * @param y Wspó³rzêdna Y punktu koñcowego
    */
    void setEnd(float x, float y);
	/**
	* @brief Ustawia odcinek na podstawie dwóch punktów
	* @param newStart Nowy punkt pocz¹tkowy
	* @param newEnd Nowy punkt koñcowy
	*/
    void setLine(const Point2D& newStart, const Point2D& newEnd);
    /**
	* @brief Ustawia odcinek na podstawie wspó³rzêdnych
	* @param x1 Wspó³rzêdna X punktu pocz¹tkowego
	* @param y1 Wspó³rzêdna Y punktu pocz¹tkowego
	* @param x2 Wspó³rzêdna X punktu koñcowego
	* @param y2 Wspó³rzêdna Y punktu koñcowego
	*/
    void setLine(float x1, float y1, float x2, float y2);

    /**
     * @brief Przesuwa punkt o zadany wektor
     * @param dx Przesuniêcie w osi X
     * @param dy Przesuniêcie w osi Y
     */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje po³o¿enie punktu wzglêdem zadanego centrum
     * @param sx Wspó³czynnik skalowania w osi X
     * @param sy Wspó³czynnik skalowania w osi Y
     * @param center Punkt wzglêdem którego nastêpuje skalowanie
     */
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    /**
     * @brief Obraca punkt o zadany k¹t wzglêdem centrum
     * @param angle K¹t obrotu w radianach
     * @param center Punkt wzglêdem którego nastêpuje obrót
     */
    void rotate(float angle, const Point2D& center = Point2D(0, 0));
};

/**
 * @brief Klasa reprezentuj¹ca trójk¹t w przestrzeni 2D
 */
class Triangle {
private:
    Point2D p1;    ///< Pierwszy wierzcho³ek trójk¹ta
    Point2D p2;    ///< Drugi wierzcho³ek trójk¹ta
    Point2D p3;    ///< Trzeci wierzcho³ek trójk¹ta
    bool filled;   ///< Flaga wype³nienia trójk¹ta

public:
    /**
    * @brief Konstruktor domyœlny, tworzy trójk¹t w punkcie (0,0)
    */
    Triangle();
    /**
     * @brief Konstruktor z punktami i wype³nieniem
     * @param p1 Pierwszy wierzcho³ek
     * @param p2 Drugi wierzcho³ek
     * @param p3 Trzeci wierzcho³ek
     * @param filled Flaga wype³nienia
     */
    Triangle(const Point2D& p1, const Point2D& p2, const Point2D& p3, bool filled = false);
	/**
	 * @brief Konstruktor z wspó³rzêdnymi punktów i wype³nieniem
	 * @param x1 Wspó³rzêdna X pierwszego wierzcho³ka
	 * @param y1 Wspó³rzêdna Y pierwszego wierzcho³ka
	 * @param x2 Wspó³rzêdna X drugiego wierzcho³ka
	 * @param y2 Wspó³rzêdna Y drugiego wierzcho³ka
	 * @param x3 Wspó³rzêdna X trzeciego wierzcho³ka
	 * @param y3 Wspó³rzêdna Y trzeciego wierzcho³ka
	 * @param filled Flaga wype³nienia
	 */
    Triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled = false);

    /**
	* @brief Pobiera pierwszy wierzcho³ek trójk¹ta
	* @return Pierwszy wierzcho³ek
	*/
    Point2D getP1() const;
	/**
	* @brief Pobiera drugi wierzcho³ek trójk¹ta
	* @return Drugi wierzcho³ek
    */
    Point2D getP2() const;
	/**
	* @brief Pobiera trzeci wierzcho³ek trójk¹ta
	* @return Trzeci wierzcho³ek
	*/
    Point2D getP3() const;
	/**
	* @brief Pobiera flagê wype³nienia trójk¹ta
	* @return Flaga wype³nienia
	*/
    bool isFilled() const;

    /**
     * @brief Ustawia pierwszy wierzcho³ek trójk¹ta
     * @param point Nowy pierwszy wierzcho³ek
     */
    void setP1(const Point2D& point);
    /**
     * @brief Ustawia drugi wierzcho³ek trójk¹ta
     * @param point Nowy drugi wierzcho³ek
     */
    void setP2(const Point2D& point);
    /**
     * @brief Ustawia trzeci wierzcho³ek trójk¹ta
     * @param point Nowy trzeci wierzcho³ek
     */
    void setP3(const Point2D& point);
    /**
     * @brief Ustawia wszystkie wierzcho³ki trójk¹ta
     * @param p1 Nowy pierwszy wierzcho³ek
     * @param p2 Nowy drugi wierzcho³ek
     * @param p3 Nowy trzeci wierzcho³ek
     */
    void setPoints(const Point2D& p1, const Point2D& p2, const Point2D& p3);
    /**
     * @brief Ustawia wszystkie wierzcho³ki trójk¹ta na podstawie wspó³rzêdnych
     * @param x1 Wspó³rzêdna X pierwszego wierzcho³ka
     * @param y1 Wspó³rzêdna Y pierwszego wierzcho³ka
     * @param x2 Wspó³rzêdna X drugiego wierzcho³ka
     * @param y2 Wspó³rzêdna Y drugiego wierzcho³ka
     * @param x3 Wspó³rzêdna X trzeciego wierzcho³ka
     * @param y3 Wspó³rzêdna Y trzeciego wierzcho³ka
     */
    void setPoints(float x1, float y1, float x2, float y2, float x3, float y3);
    /**
     * @brief Ustawia flagê wype³nienia trójk¹ta
     * @param fill Nowa wartoœæ flagi wype³nienia
     */
    void setFilled(bool fill);

    /**
      * @brief Przesuwa trójk¹t o zadany wektor
      * @param dx Przesuniêcie w osi X
      * @param dy Przesuniêcie w osi Y
      */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje trójk¹t wzglêdem zadanego centrum
     * @param sx Wspó³czynnik skalowania w osi X
     * @param sy Wspó³czynnik skalowania w osi Y
     * @param center Punkt wzglêdem którego nastêpuje skalowanie
     */
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    /**
     * @brief Obraca trójk¹t o zadany k¹t wzglêdem centrum
     * @param angle K¹t obrotu w radianach
     * @param center Punkt wzglêdem którego nastêpuje obrót
     */
    void rotate(float angle, const Point2D& center = Point2D(0, 0));
};

/**
 * @brief Klasa reprezentuj¹ca prostok¹t w przestrzeni 2D
 */
class Rectangle {
private:
    Point2D topLeft;    ///< Lewy górny wierzcho³ek prostok¹ta
    float width;        ///< Szerokoœæ prostok¹ta
    float height;       ///< Wysokoœæ prostok¹ta
    bool filled;        ///< Flaga wype³nienia prostok¹ta

public:
    /**
     * @brief Konstruktor domyœlny, tworzy prostok¹t w punkcie (0,0) o wymiarach 0x0
     */
    Rectangle();
    /**
     * @brief Konstruktor z lewym górnym wierzcho³kiem, szerokoœci¹, wysokoœci¹ i wype³nieniem
     * @param topLeft Lewy górny wierzcho³ek
     * @param width Szerokoœæ prostok¹ta
     * @param height Wysokoœæ prostok¹ta
     * @param filled Flaga wype³nienia
     */
    Rectangle(const Point2D& topLeft, float width, float height, bool filled = false);
    /**
     * @brief Konstruktor ze wspó³rzêdnymi lewego górnego wierzcho³ka, szerokoœci¹, wysokoœci¹ i wype³nieniem
     * @param x Wspó³rzêdna X lewego górnego wierzcho³ka
     * @param y Wspó³rzêdna Y lewego górnego wierzcho³ka
     * @param width Szerokoœæ prostok¹ta
     * @param height Wysokoœæ prostok¹ta
     * @param filled Flaga wype³nienia
     */
    Rectangle(float x, float y, float width, float height, bool filled = false);

    /**
     * @brief Pobiera lewy górny wierzcho³ek prostok¹ta
     * @return Lewy górny wierzcho³ek
     */
    Point2D getTopLeft() const;
    /**
     * @brief Pobiera prawy górny wierzcho³ek prostok¹ta
     * @return Prawy górny wierzcho³ek
     */
    Point2D getTopRight() const;
    /**
     * @brief Pobiera lewy dolny wierzcho³ek prostok¹ta
     * @return Lewy dolny wierzcho³ek
     */
    Point2D getBottomLeft() const;
    /**
     * @brief Pobiera prawy dolny wierzcho³ek prostok¹ta
     * @return Prawy dolny wierzcho³ek
     */
    Point2D getBottomRight() const;
    /**
     * @brief Pobiera szerokoœæ prostok¹ta
     * @return Szerokoœæ prostok¹ta
     */
    float getWidth() const;
    /**
     * @brief Pobiera wysokoœæ prostok¹ta
     * @return Wysokoœæ prostok¹ta
     */
    float getHeight() const;
    /**
     * @brief Pobiera flagê wype³nienia prostok¹ta
     * @return Flaga wype³nienia
     */
    bool isFilled() const;

    /**
     * @brief Ustawia lewy górny wierzcho³ek prostok¹ta
     * @param point Nowy lewy górny wierzcho³ek
     */
    void setTopLeft(const Point2D& point);
    /**
     * @brief Ustawia lewy górny wierzcho³ek prostok¹ta na podstawie wspó³rzêdnych
     * @param x Nowa wspó³rzêdna X lewego górnego wierzcho³ka
     * @param y Nowa wspó³rzêdna Y lewego górnego wierzcho³ka
     */
    void setTopLeft(float x, float y);
    /**
     * @brief Ustawia szerokoœæ i wysokoœæ prostok¹ta
     * @param width Nowa szerokoœæ prostok¹ta
     * @param height Nowa wysokoœæ prostok¹ta
     */
    void setSize(float width, float height);
    /**
     * @brief Ustawia flagê wype³nienia prostok¹ta
     * @param fill Nowa wartoœæ flagi wype³nienia
     */
    void setFilled(bool fill);


    /**
    * @brief Przesuwa prostok¹t o zadany wektor
    * @param dx Przesuniêcie w osi X
    * @param dy Przesuniêcie w osi Y
    */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje prostok¹t wzglêdem zadanego centrum
     * @param sx Wspó³czynnik skalowania w osi X
     * @param sy Wspó³czynnik skalowania w osi Y
     * @param center Punkt wzglêdem którego nastêpuje skalowanie
     */
    void scale(float sx, float sy, const Point2D& center = Point2D(0, 0));
    /**
     * @brief Obraca prostok¹t o zadany k¹t wzglêdem centrum
     * @param angle K¹t obrotu w radianach
     * @param center Punkt wzglêdem którego nastêpuje obrót
     */
    void rotate(float angle, const Point2D& center = Point2D(0, 0));


    /**
     * @brief Sprawdza, czy dany punkt znajduje siê wewn¹trz prostok¹ta
     * @param point Sprawdzany punkt
     * @return true jeœli punkt jest wewn¹trz, false w przeciwnym razie
     */
    bool contains(const Point2D& point) const;
    /**
     * @brief Sprawdza, czy ten prostok¹t przecina siê z innym prostok¹tem
     * @param other Drugi prostok¹t do sprawdzenia przeciêcia
     * @return true jeœli prostok¹ty siê przecinaj¹, false w przeciwnym razie
     */
    bool intersects(const Rectangle& other) const;
};

/**
 * @brief Klasa reprezentuj¹ca okr¹g w przestrzeni 2D
 */
class Circle {
private:
    Point2D center; ///< Œrodek okrêgu
    float radius;   ///< Promieñ okrêgu
    bool filled;    ///< Flaga wype³nienia okrêgu


public:
    /**
     * @brief Konstruktor domyœlny, tworzy okr¹g o œrodku w (0,0) i promieniu 0
     */
    Circle();
    /**
     * @brief Konstruktor z œrodkiem, promieniem i wype³nieniem
     * @param center Œrodek okrêgu
     * @param radius Promieñ okrêgu
     * @param filled Flaga wype³nienia
     */
    Circle(const Point2D& center, float radius, bool filled = false);
    /**
     * @brief Konstruktor ze wspó³rzêdnymi œrodka, promieniem i wype³nieniem
     * @param x Wspó³rzêdna X œrodka okrêgu
     * @param y Wspó³rzêdna Y œrodka okrêgu
     * @param radius Promieñ okrêgu
     * @param filled Flaga wype³nienia
     */
    Circle(float x, float y, float radius, bool filled = false);

    /**
     * @brief Pobiera œrodek okrêgu
     * @return Œrodek okrêgu
     */
    Point2D getCenter() const;
    /**
     * @brief Pobiera promieñ okrêgu
     * @return Promieñ okrêgu
     */
    float getRadius() const;
    /**
     * @brief Pobiera flagê wype³nienia okrêgu
     * @return Flaga wype³nienia
     */
    bool isFilled() const;


    /**
     * @brief Ustawia œrodek okrêgu
     * @param point Nowy œrodek okrêgu
     */
    void setCenter(const Point2D& point);
    /**
     * @brief Ustawia œrodek okrêgu na podstawie wspó³rzêdnych
     * @param x Nowa wspó³rzêdna X œrodka okrêgu
     * @param y Nowa wspó³rzêdna Y œrodka okrêgu
     */
    void setCenter(float x, float y);
    /**
     * @brief Ustawia promieñ okrêgu
     * @param newRadius Nowy promieñ okrêgu
     */
    void setRadius(float newRadius);
    /**
     * @brief Ustawia flagê wype³nienia okrêgu
     * @param fill Nowa wartoœæ flagi wype³nienia
     */
    void setFilled(bool fill);


    /**
     * @brief Przesuwa okr¹g o zadany wektor
     * @param dx Przesuniêcie w osi X
     * @param dy Przesuniêcie w osi Y
     */
    void translate(float dx, float dy);
    /**
     * @brief Skaluje okr¹g
     * @param s Wspó³czynnik skalowania (jednakowy dla obu osi)
     */
    void scale(float s);  // Dla ko³a sx i sy powinny byæ równe
    // Ko³o nie potrzebuje rotacji, bo jest symetryczne

    /**
     * @brief Sprawdza, czy dany punkt znajduje siê wewn¹trz okrêgu
     * @param point Sprawdzany punkt
     * @return true jeœli punkt jest wewn¹trz, false w przeciwnym razie
     */
    bool contains(const Point2D& point) const;
};
#endif // PRIMITIVES_H
