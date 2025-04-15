#pragma once
// Animation.h
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>

// Struktura przechowuj¹ca dane pojedynczej klatki animacji
struct AnimationFrame {
    int x;          // Pozycja X w sprite sheet
    int y;          // Pozycja Y w sprite sheet
    int width;      // Szerokoœæ klatki
    int height;     // Wysokoœæ klatki
    float duration; // Czas trwania klatki w sekundach

    AnimationFrame(int x, int y, int width, int height, float duration)
        : x(x), y(y), width(width), height(height), duration(duration) {
    }
};

class Animation {
private:
    std::string name;                    // Nazwa animacji
    std::vector<AnimationFrame> frames;  // Klatki animacji
    float totalDuration;                 // Ca³kowity czas trwania
    bool isLooping;                      // Czy animacja siê zapêtla

    // Stan animacji
    int currentFrameIndex;               // Indeks aktualnej klatki
    float currentFrameTime;              // Czas spêdzony na aktualnej klatce
    bool isPlaying;                      // Czy animacja jest odtwarzana

public:
    Animation(const std::string& name, bool looping = true);

    // Zarz¹dzanie klatkami
    void addFrame(int x, int y, int width, int height, float duration);
    const AnimationFrame& getCurrentFrame() const;

    // Kontrola odtwarzania
    void play();
    void pause();
    void stop();
    void reset();

    // Aktualizacja stanu
    void update(float deltaTime);

    // Gettery
    bool isFinished() const;
    std::string getName() const;
    bool getIsLooping() const;
    bool getIsPlaying() const;

    // Settery
    void setLooping(bool loop);
};

#endif // ANIMATION_H


