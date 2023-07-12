#include "core.h"
#include "environment.h"
#include "collision.h"

#include <vector>
#include <list>
#include <optional>


std::vector<Body>& boardphase(Environment& env) {
    // TODO
    return env.getBodiesMut();
}

std::pair<double, double> getMinMax(const Body& body, const Vec2& axis) {
    // TODO: Temp: Assume AABB
    //
    if (axis.x == 0 && axis.y == 1) 
    {
        return std::make_pair(body.getPos().x, body.getPos().x + body.getSize().x);
    }

    if (axis.x == 1 && axis.y == 0) 
    {
        return std::make_pair(body.getPos().y, body.getPos().y + body.getSize().y);
    }

    assert(false);
}

std::optional<Collision> detect(const Body& b1, const Body& b2) {

    // Find normals
    std::vector<Vec2> norms;
    {
        // TODO: TEMP: assume AABB
        norms.push_back(Vec2(0, 1)); 
        norms.push_back(Vec2(1, 0)); 
    }

    // For each normal
    for (const Vec2& axis: norms) 
    {
        // Get min & max projection
        auto [min1, max1] = getMinMax(b1, axis);
        auto [min2, max2] = getMinMax(b2, axis);


        // If no overlap 
        if (max1 < min2 || max2 < min1) 
            return std::nullopt;
    }

    // No axis possible, find collision 
    // TODO: Collision Normal
    Vec2 norm{0,0};
    Vec2 rel = b2.getPos() - b1.getPos();
    double xOverlap = b1.getSize().x + b2.getSize().x - abs(rel.x);
    double yOverlap = b1.getSize().y + b2.getSize().y - abs(rel.y);

    if (xOverlap > 0 && xOverlap < yOverlap) 
    {
        if (rel.x > 0) norm = Vec2( 1, 0);
        else           norm = Vec2(-1, 0);
    } 
    else 
    {
        if (rel.y > 0) norm = Vec2(0, 1);
        else           norm = Vec2(0,-1);
    }

    return std::make_optional(Collision{norm, 0});
}

void Body::resolve(Body& b1, Body& b2, const Collision& collision) {

    // Find projected relative Velocity
    Vec2 vR = b1.velo - b2.velo;
    double vRP = vR * collision.norm;

    std::cout << vR << " * " << collision.norm << " -> " << vRP << std::endl;
    // If the velocities are separating, don't resolve
    if (vRP < 0) 
        return;

    double e = 1 + 0.1;

    // Calculate impulse (I DON'T GET THIS)
    double J = e * vRP / ( 1/b1.mass + 1/b2.mass );
    Vec2 impulse = collision.norm * J;
    
    // Apply Impulse
    Vec2 nv1 = b1.velo - impulse * (1 / b1.mass);
    Vec2 nv2 = b2.velo + impulse * (1 / b2.mass);
    std::cout << b1.velo << " -> " << nv1 << std::endl;
    std::cout << b2.velo << " -> " << nv2 << std::endl;
    b1.velo = nv1;
    b2.velo = nv2;
}

void Environment::collide() {
        
    // Prune with boardphase
    std::vector<Body>& bodyList = boardphase(*this);

    // Find collisions
    for (int i=0; i<bodyList.size(); i++) 
    {
        for (int j=i+1; j<bodyList.size(); j++) 
        {
            // Check for collision
            std::optional<Collision> opt = detect(bodyList[i], bodyList[j]);
            if (!opt) continue;
            auto collision = *opt;

            // Resolve collision
            Body::resolve(bodyList[i], bodyList[j], collision);
        }
    }
}


