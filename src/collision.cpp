#include "core.h"
#include "environment.h"
#include "collision.h"

#include <limits>
#include <utility>
#include <vector>
#include <list>
#include <optional>


std::vector<Body>& boardphase(Environment& env) {
    // TODO
    return env.getBodiesMut();
}

std::pair<double, double> getMinMax(const Body& body, const Vec2& axis) {
    assert(axis.isNorm());

    auto min = std::numeric_limits<double>::max();
    auto max = std::numeric_limits<double>::min();

    for (const Vec2& point : body.getPoints())  
    {
        Vec2 p = body.getPos() + point;
        double v = p * axis;
        
        if (v < min) min = v;
        if (v > max) max = v;
    }
    return std::make_pair(min, max);
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
    // TODO: Assumes AABB
    double depth;
    Vec2 norm{0,0};
    Vec2 rel = b2.getPos() - b1.getPos();
    double xOverlap = (b1.getSize().x + b2.getSize().x)/2 - abs(rel.x);
    double yOverlap = (b1.getSize().y + b2.getSize().y)/2 - abs(rel.y);

    if (xOverlap > 0 && xOverlap < yOverlap) 
    {
        depth = xOverlap;
        if (rel.x > 0) norm = Vec2( 1, 0);
        else           norm = Vec2(-1, 0);
    } 
    else 
    {
        depth = yOverlap;
        if (rel.y > 0) norm = Vec2(0, 1);
        else           norm = Vec2(0,-1);
    }

    norm.normalize();
    return std::make_optional(Collision{norm, depth});
}

Vec2 orthogonalTowards(const Vec2& _v1, const Vec2& _v2) {
    Vec3 v1 (_v1.x, _v1.y, 0);
    Vec3 v2 (_v2.x, _v2.y, 0);

    Vec3 o = v1 ^ v2 ^ v1;
    if (o.z != 0) {
        assert(false);
    }

    return Vec2(o.x, o.y).normalize();
}

Vec2 support (const std::vector<Vec2>& p, const Vec2& d) {
    int out = 0;
    int best = 0;
    for (int i=0; i<p.size(); i++) 
        if (d * p[i] > best) {
            best = d * p[i];
            out = i;
        }
    return p[out];
}

Vec2 CSOsupport (const Body& b1, const Body& b2, const Vec2& d) {
    auto s1 = support(b1.getPoints(),  d);
    auto s2 = support(b2.getPoints(), -d);

    return (b1.getPos() + s1) - (b2.getPos() + s2);
}


std::optional<Collision> detectGJK(const Body& b1, const Body& b2) {
    
    Vec2 d (0, 1);
    Vec2 p1 = CSOsupport(b1, b2, d);
    d =  (-p1).normalize();
    Vec2 p2 = CSOsupport(b1, b2, d);

    while (1) {
        d = orthogonalTowards(p2-p1, -p1); 
        Vec2 p3 = CSOsupport(b1, b2, d);


        if (p3 * d < 0) {
            return std::nullopt;
        }

        if (-p3 * orthogonalTowards(p3-p1, p1-p2) > 0) {
            std::cout << "kept p1\n";
            p2 = p3;
        }
        if (-p3 * orthogonalTowards(p3-p2, p2-p1) > 0) {
            std::cout << "kept p2\n";
            p1 = p3;
        }
        else {
            return Collision{Vec2(0,0), 0};
        }
    }
}


void Body::resolve(Body& b1, Body& b2, const Collision& collision) {

    // Find projected relative Velocity
    Vec2 vR = b1.velo - b2.velo;
    double vRP = vR * collision.norm;

    // If the velocities are separating, don't resolve
    if (vRP < 0) 
        return;

    double e = 1 + 0.1;

    // Calculate impulse (I DON'T GET THIS)
    double J = e * vRP / b1.invMass + b2.invMass;
    Vec2 impulse = collision.norm * J;
    
    // Apply Impulse
    b1.velo = b1.velo - impulse * b1.invMass;
    b2.velo = b2.velo + impulse * b2.invMass;

    
    // (Sink Prevention) Positional Correction, Linear Projection
    const double percent = 0.2;     // usually 20% to 80% 
    const double slop    = 0.01;    // usually 0.01 to 0.1 
    double correction = percent * std::max( collision.depth - slop, (double) 0 ) / (b1.invMass + b2.invMass);
    Vec2 correctionV = collision.norm * correction;

    b1.pos = b1.pos - correctionV * b1.invMass;
    b2.pos = b2.pos + correctionV * b2.invMass;
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
            std::optional<Collision> opt = detectGJK(bodyList[i], bodyList[j]);
            if (!opt) continue;
            auto collision = *opt;

            // Resolve collision
            // DEBUG: Set color to red 
            bodyList[i].color = SDL_Color{255,0,0};
            bodyList[j].color = SDL_Color{255,0,0};

            // Body::resolve(bodyList[i], bodyList[j], collision);
        }
    }
}


