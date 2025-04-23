#include "Animation.h"

Animation::Animation(const std::string& name, bool looping)
    : name(name)
    , totalDuration(0.0f)
    , isLooping(looping)
    , currentFrameIndex(0)
    , currentFrameTime(0.0f)
    , isPlaying(false)
{
}

void Animation::addFrame(int x, int y, int width, int height, float duration) {
    // U�ywamy emplace_back zamiast push_back dla lepszej wydajno�ci
    // Konstruuje obiekt AnimationFrame bezpo�rednio w wektorze, unikaj�c kopii
    frames.emplace_back(x, y, width, height, duration);
    totalDuration += duration;  // Aktualizacja ca�kowitego czasu trwania animacji
}

const AnimationFrame& Animation::getCurrentFrame() const {
    // Zabezpieczenie przed dost�pem do pustego wektora
    // Zwraca domy�ln� klatk� je�li wektor jest pusty
    if (frames.empty()) {
        static AnimationFrame defaultFrame(0, 0, 0, 0, 0.0f);
        return defaultFrame;
    }
    return frames[currentFrameIndex];
}

void Animation::play() {
    isPlaying = true;
}

void Animation::pause() {
    isPlaying = false;
}

void Animation::stop() {
    isPlaying = false;
    reset();
}

void Animation::reset() {
    currentFrameIndex = 0;
    currentFrameTime = 0.0f;
}

void Animation::update(float deltaTime) {
    if (!isPlaying || frames.empty()) return;

    currentFrameTime += deltaTime;

    while (currentFrameTime >= frames[currentFrameIndex].duration) {
        // Odejmujemy czas aktualnej klatki i przechodzimy do nast�pnej
        currentFrameTime -= frames[currentFrameIndex].duration;
        currentFrameIndex++;

        // Logika zap�tlania animacji
        if (currentFrameIndex >= frames.size()) {
            if (isLooping) {
                currentFrameIndex = 0;  // Wracamy na pocz�tek animacji
            }
            else {
                currentFrameIndex = frames.size() - 1;  // Zostajemy na ostatniej klatce
                isPlaying = false;
                break;  // Przerywamy p�tl�, bo animacja si� sko�czy�a
            }
        }
    }
}

bool Animation::isFinished() const {
    return !isLooping && !isPlaying && currentFrameIndex == frames.size() - 1;
}

std::string Animation::getName() const {
    return name;
}

bool Animation::getIsLooping() const {
    return isLooping;
}

bool Animation::getIsPlaying() const {
    return isPlaying;
}

void Animation::setLooping(bool loop) {
    isLooping = loop;
}
