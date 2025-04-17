#ifndef BODY_MANAGER_H
#define BODY_MANAGER_H

#include <vector>
#include "DeadBody.h"

class BodyManager {
private:
    static BodyManager* instance;
    std::vector<DeadBody*> bodies;

    // Prywatny konstruktor i destruktor (singleton)
    BodyManager() = default;
    ~BodyManager();

    // Zablokowanie kopiowania
    BodyManager(const BodyManager&) = delete;
    BodyManager& operator=(const BodyManager&) = delete;

public:
    static BodyManager* getInstance();
    static void releaseInstance();

    void addBody(const Point2D& position, float rotation, const std::string& entityTag);
    void draw();
    void cleanup();
};

#endif
