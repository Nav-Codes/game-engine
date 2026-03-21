//
// Created by navjo on 2/18/2026.
//

#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP
#include <memory>
#include <vector>

#include "Component.hpp"
#include "Entity.hpp"

using namespace std;

//state system: deciding which clip to use
//check if the animation has been switched
//playback system: advances the animation

//if u have multiple animations, there would be a lot of if checks just checking what this animation thingy is for
//need to make more generic system that does not use hard coded strings in the update method
//maybe define animation callbacks for each type of thing (ie. player anim callback, projectile anim callback)?
//and then call them here, making it more general if animations do different things for different entities
//but where would we define the callbacks? ie. need to ensure readability and easy extension type shit
//will also need to consider which xml file they will use bc different animations will use different things
//can return values that help the rest of the update function to run when considering different xml files


//create emit and subscribe functions here
//when creating entities with sprites, define their animation behaviour
//then subscribe that behaviour to a method caller
//emit will happen every frame...
//would need an anim info component?

class AnimationSystem {
private:
    // string oldAnim;
public:
    void update(const vector<unique_ptr<Entity>>& entities, float dt) { //this contains all the entities in the game
        for (auto& e : entities) {
            if (e->hasComponent<Animation>()) {
                auto& anim = e->getComponent<Animation>();

                string newClip;
                if (anim.animCallback != nullptr) {
                    newClip = anim.animCallback(*e);
                }

                //put this  if statement into a new function and make it return new clip
                //so that the stuff under it can handle whatever it needs to handle

                //check if the animation has switched
                //if the chosen clip is different from the current one, switch to new clip, reset time and frame index
                if (anim.clips.contains(newClip) && newClip != anim.currentClip) {
                    anim.currentClip = newClip;
                    anim.time = 0.0f;
                    anim.currentFrame = 0;
                }

                //playback system: advance the animation
                float animFrameSpeed = anim.speed; //how long each 'animation frame' should last
                auto clip = anim.clips[anim.currentClip]; //retrieve the frame data from the current clip
                //advance time
                anim.time += dt; // every 'game loop frame' we are going to add the accumulated time

                //once enough time has passed
                if (anim.time >= animFrameSpeed) {
                    anim.time -= animFrameSpeed; //subtract animFrameSpeed (eg. 0.1f) so the extra time isn't lost

                    size_t totalAnimationFrames = clip.frameIndices.size();
                    //advance to the next animation frame (currentAnimationFrame + 1)
                    //wrap around % so it loops when reaching the end of the clip
                    anim.currentFrame = (anim.currentFrame+1) % totalAnimationFrames;
                }
            }
        }
    }
};

#endif //ANIMATIONSYSTEM_HPP
