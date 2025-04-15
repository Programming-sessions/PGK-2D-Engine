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
    frames.emplace_back(x, y, width, height, duration);
    totalDuration += duration;
}

const AnimationFrame& Animation::getCurrentFrame() const {
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

    // Sprawdzamy czy czas obecnej klatki siê skoñczy³
    while (currentFrameTime >= frames[currentFrameIndex].duration) {
        currentFrameTime -= frames[currentFrameIndex].duration;
        currentFrameIndex++;

        // Sprawdzamy czy dotarliœmy do koñca animacji
        if (currentFrameIndex >= frames.size()) {
            if (isLooping) {
                currentFrameIndex = 0;
            }
            else {
                currentFrameIndex = frames.size() - 1;
                isPlaying = false;
                break;
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
