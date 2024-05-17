#pragma once

#include <thread>
#include <vector>
#include "entity.hpp"

#define MAX_ENTITIES_PER_SCENE 1000
#define CHUNK_SIZE 25

namespace Core {

class Scene {
private:
    std::vector<Core::AbstractEntity*> entities;
    i32 entityCounter = 0;
    u32 max_layer;

public:
    bool loading;
    std::thread loadThread;
    Camera2D camera;
    Scene();

    void addEntity(Core::AbstractEntity* entity);
    virtual void update(f32 delta);
    virtual void draw();
    virtual void loadResources(){loading = false;};
    inline i32 getEntitiesCount() { return entityCounter; }
    inline Camera2D& getCamera() { return camera; }
    inline void setCamera(Camera2D cam) { camera = cam; }

    ~Scene();
};

}
