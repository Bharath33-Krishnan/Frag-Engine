#include "raylib/raylib.h"
// #include "src/test/Test.h"
#include "src/core.h"
#include "src/core/entity.hpp"
#include "src/core/graphics/graphics.hpp"
#include "src/core/graphics/lightingMaterial.hpp"

class Player : public Core::AbstractEntity {
private:
    Model model;
    ModelAnimation* anims;

    i32 animsCount;
    i32 currentFrame;
    i32 currentAnimation;
    Texture2D albedoTex;
    Texture2D normalTex;

public:
    Player(Core::Scene* scene) {

        // NOTE : Gowrish - For some reason `this` becomes NULL
        scene->addEntity(this);

        transform t = getTransform();
        t.pos = vec3(0, 0, 0);
        t.rot = vec3(0, 0, 0);
        t.scale = vec3(0, 0, 0);

        currentFrame = 0;
        currentAnimation = 0;

        this->init();
    }

    void init() {
        model = LoadModel("../res/boom.m3d");
        // Mesh cube_mesh= GenMeshCube(1.0, 1.0, 1.0);
        // model = LoadModelFromMesh(cube_mesh);
        GenMeshTangents(model.meshes);
        anims = LoadModelAnimations("../res/boom.m3d", &this->animsCount);
        albedoTex = LoadTexture("../res/Textures/Rock_albedo.png");
        normalTex = LoadTexture("../res/Textures/Rock_normal.png");

        addTexture(albedoTex, MATERIAL_MAP_ALBEDO);
        addTexture(normalTex, MATERIAL_MAP_NORMAL);
    }

    void addTexture(Texture2D& tex,i32 loc){
        model.materials[0].maps[loc].texture = tex;
    }

    void update(f32 delta) {
        currentFrame = (currentFrame + 1) % anims[currentAnimation].frameCount;
        UpdateModelAnimation(model, anims[currentAnimation], currentFrame);
    }

    void setMaterial(Core::EngineMaterial* mat){
        model.materials[0].shader = mat->getShader();
    }

    void draw() {
        // DrawPlane(Vector3Zero(), (Vector2) { 10.0, 10.0 }, WHITE);
        DrawModel(model, getTransform().pos.to_vec(), 1.0f, WHITE);
    }

    ~Player() {
        UnloadModel(model);
        UnloadModelAnimations(anims, animsCount);
        UnloadTexture(albedoTex);
        UnloadTexture(normalTex);
    }
};

class MyScene : public Core::Scene {
public:
    MyScene(Camera& cam, Core::GraphicsEngine* gfxEngine) {
        DisableCursor();
        SetTargetFPS(60);


        Core::LightingMaterial* mat1 = new Core::LightingMaterial("../res/shaders/lighting.vert.glsl","../res/shaders/lighting.frag.glsl"); 
        mat1->initMaterial(vec3(0.1,0.0,0.5),cam);
        f32 shine = .5;


        int shininessLocation = GetShaderLocation(mat1->getShader(),"shininess");
        SetShaderValue(mat1->getShader(), shininessLocation , &shine , SHADER_UNIFORM_FLOAT);


        //IMPORTANT - Register material to gfx engine
        gfxEngine->RegisterMaterial(mat1);


        Player* player = new Player(this);
        //IMPORTANT - Register entity to material
        mat1->SubscribeToMaterial(player);

        mat1->CreateLight(LIGHT_DIRECTIONAL, vec3(1.0f,2.0f,1.0f) ,vec3(0.0),WHITE);
        // mat1->CreateLight(LIGHT_POINT, vec3(2.0f,2.0f,1.0f) ,vec3(0.0),YELLOW);

        this->gfxEngine = gfxEngine;
        cam.position = vec3(1.5f).to_vec();
        cam.target = vec3().to_vec();
        cam.up = vec3(0.0f, 1.0f, 0.0f).to_vec();
        cam.fovy = 45.0f;
        cam.projection = CAMERA_PERSPECTIVE;

        setCamera(cam);
    }
};


int main(void)
{
    InitWindow(1280, 720, "Test");

    Camera cam;

    DisableCursor();

    Core::GraphicsEngine* gfxEngine = new Core::GraphicsEngine();
    
    MyScene* scene = new MyScene(cam,gfxEngine);

    Core::SceneManager::addScene(scene);

    Core::SceneManager::run();

    CloseWindow();
}
