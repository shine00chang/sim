#include "environment.h"
#include "collision.h"

#include <vector>


std::vector<Body> boardphase(const Environment& Env) {
    return std::vector<Body>();
}

std::vector<Collision> detect(const std::vector<Body>& bodyList) {
    return std::vector<Collision>();
}

void resolve(const std::vector<Collision>& collisions) {

}

void Environment::collide() {
    std::vector<Body> bodyList = boardphase(*this);
    auto collisions = detect(bodyList);
    resolve(collisions);
}


