//
// Created by navjo on 2/25/2026.
//

#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "Scene.hpp"

using namespace std;

struct SceneParams {
    const char* name;
    const char* mapPath;
    int windowWidth;
    int windowHeight;
};

class SceneManager {
    unordered_map<string, SceneParams> sceneParam;
    string pendingScene;

    void changeScene(const string& name) {
        auto it = sceneParam.find(name);
        if (it != sceneParam.end()) {
            const auto& params = it->second;
            //create scene object
            //build scene
            currentScene = make_unique<Scene>(
                params.name,
                params.mapPath,
                params.windowWidth,
                params.windowHeight);
        } else {
            cerr << "Scene " << name << " not found!" << endl;
        }
    }

public:
    unique_ptr<Scene> currentScene;
    void loadScene(const char *sceneName, const char *mapPath, int windowWidth, int windowHeight) {
        sceneParam[sceneName] = {sceneName, mapPath, windowWidth, windowHeight};
    }

    void changeSceneDeferred(const string& name) {
        pendingScene = name;
    }

    void update(const float dt, const SDL_Event &e) {
        if (currentScene) currentScene->update(dt, e);
        if (!pendingScene.empty()) {
            changeScene(pendingScene);
            pendingScene.clear();
        }
    }

    void render() const {
        if (currentScene) currentScene->render();
    }
};

#endif //SCENEMANAGER_HPP
