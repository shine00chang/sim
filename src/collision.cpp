#include "core.h"
#include "environment.h"
#include "collision.h"
#include "app.h"

#include <cmath>
#include <limits>
#include <utility>
#include <vector>
#include <list>
#include <optional>


// Boardphase
std::vector<Body>& boardphase(Environment& env) {
    // TODO
    return env.getBodiesMut();
}

// Given two vectors, find a normalized vector orthogonal to v1, in the direction of v2.
Vec2 orthogonalTowards(const Vec2& _v1, const Vec2& _v2) {

    // if v1 and v2 lie on the same vector, you cannot select an orthogonal vector towards v2.
    if (_v1.norm() == _v2.norm() || _v1.norm() == -_v2.norm()) {
        assert(false);
    }

    Vec3 v1 (_v1.x, _v1.y, 0);
    Vec3 v2 (_v2.x, _v2.y, 0);

    Vec3 o = v1 ^ v2 ^ v1;
    if (o.z != 0) {
        std::cout << "Error :" << v1 << ", " << v2 << "\tout: " << o << std::endl;
        assert(false);
    }

    return Vec2(o.x, o.y).normalize();
}



std::pair<double, double> getMinMax(const Body& body, const Vec2& axis) {
    if (!axis.isNorm()) {
        std::cout << axis << std::endl;
        assert(false);
    }

    auto min = std::nan("0");
    auto max = std::nan("0");

    for (const Vec2& p : body.getPointsGlobal())
    {
        double v = p * axis;

        if (std::isnan(min) || v < min) min = v;
        if (std::isnan(max) || v > max) max = v;
    }
    return std::make_pair(min, max);
}


std::optional<std::pair<double, Vec2>> detectCollisionSAT(const Body& b1, const Body& b2) {

    // Find normals
    std::vector<Vec2> norms;
    {
        const auto& v = b1.getPointsLocal();
        for (int i=0; i<v.size(); i++) {
            Vec2 e = (v[i] - v[i == 0 ? v.size()-1 : i-1]).normalize();
            norms.push_back(Vec2(-e.y, e.x));
        }
    }{
        const auto& v = b2.getPointsLocal();
        for (int i=0; i<v.size(); i++) {
            Vec2 e = (v[i] - v[i == 0 ? v.size()-1 : i-1]).normalize();
            norms.push_back(Vec2(-e.y, e.x));
        }
    }


    double overlap = std::nan("0");
    Vec2 separationNorm(norms[0]);
    // For each normal
    for (const Vec2& axis: norms)
    {
        // Get min & max projection
        auto [min1, max1] = getMinMax(b1, axis);
        auto [min2, max2] = getMinMax(b2, axis);

        // If no overlap
        if (max1 < min2 || max2 < min1) 
        {
            //std::cout << "no overlap\n";
            return std::nullopt;
        }
        else 
        // Check overlap to find MTV
        {
            double o = max1-min2;
            //std::cout << "axis: " << axis << "\t[" << max1 << ",  " << min1 << "]\t[" << max2 << ",  " << min2 << "]\toverlap: " << o << std::endl;
            if (std::isnan(overlap) ||  o < overlap) {
                //std::cout << "min: " << o << std::endl;
                overlap = o;
                separationNorm = axis;
            }
        }
    }

    return std::make_optional(std::make_pair(overlap, separationNorm));
}


struct Edge {
    Vec2 e;
    Vec2 n;
    Vec2 v1;
    Vec2 v2;
    Edge (const Vec2& v1, const Vec2& v2) : e((v1-v2).normalize()), n(Vec2(e.y, -e.x).normalize()), v1(v1), v2(v2) {};
};



Edge findClippingEdge (const Body& b, const Vec2& norm) {
    const auto& p = b.getPointsGlobal();

    int out = 0;
    int best = 0;
    for (int i=0; i<p.size(); i++) 
        if (norm * p[i] > best) {
            best = norm * p[i];
            out = i;
        }
    Vec2 v1 = p[out];
    Edge e1 (v1, p[out ? out-1 : p.size()-1]);
    Edge e2 (v1, p[out == p.size()-1 ? 0 : out+1]);
    if (std::abs(e1.n * norm) > std::abs(e2.n * norm)) 
        return e1;
    else 
        return e2;
}


int clip (Vec2 out[2], Vec2 a, Vec2 b, double o, Vec2 norm) {
    double da = a * norm - o;
    double db = b * norm - o;
    int i = 0;

    // DEBUG
    // std::cout << "clipping seg\t" << a << ":" << da << ", " << b << ":" << db << "\tagainst plane " << o << "\t" << norm << std::endl;
    


    // Past clipping plane
    if (da >= 0) {
        out[i++] = a;
        //std::cout << "a past\n";
    }
    if (db >= 0) { 
        out[i++] = b;
        //std::cout << "b past\n";
    }

    // Opposing sides of plane
    if (da * db < 0) 
    {
        // std::cout << "opposing sides. ";
        // assert(i == 1);

        double r = da / (da - db);
        Vec2 v = (b - a) * r;
        out[i++] = v + a;

        // std::cout << "interpolated to:\t" << out[i-1] << std::endl;
    }

    // Both behind
    else if (da < 0 && db < 0) 
    {
        // std::cout << "both behind. ";
        double r = da / (da - db);
        Vec2 v = (b - a) * -r;
        out[i++] = v + a;
        out[i++] = v + a;

        // std::cout << "interpolated to:\t" << out[i-1] << std::endl;
    }
    assert(i == 2);
    return i;
}


Vec2 findContactPoints (const Body& b1, const Body& b2, const Vec2& separationAxis) {

    // Get Edges
    const Edge e1 = findClippingEdge(b1, separationAxis);
    const Edge e2 = findClippingEdge(b2,-separationAxis);

    debugPoints.push_back(std::make_pair(e1.v1, Blue));
    debugPoints.push_back(std::make_pair(e1.v2, Blue));
    debugPoints.push_back(std::make_pair(e2.v1, Blue));
    debugPoints.push_back(std::make_pair(e2.v2, Blue));

    // Identify Reference & Incident Edge 
    Edge ref = e2;
    Vec2 inc[] = { e1.v1, e1.v2 };
    Vec2 norm = orthogonalTowards(ref.e, separationAxis);

    // if e1.norm is more parallel to norm. ref -> e1
    if (std::abs(e1.n * separationAxis) > std::abs(e2.n * separationAxis)) {
        ref = e1;
        norm = orthogonalTowards(ref.e,-separationAxis);
        inc[0] = e2.v1;
        inc[1] = e2.v2;
    }
    // DEBUG
    /*
    std::cout << "norm: " << separationAxis << std::endl;
    std::cout << "ref:  " << ref.v1 << ", " << ref.v2 << std::endl;
    std::cout << "inc:  " << inc[0] << ", " << inc[1] << std::endl;
    */
    


    // Adjacent Clip 
    // v1 -- v2
    if (ref.v1 * ref.e < ref.v2 * ref.e) 
    {
        clip(inc, inc[0], inc[1], ref.v1 * ref.e, ref.e);
        clip(inc, inc[0], inc[1], ref.v2 *-ref.e,-ref.e);
    }
    // v2 -- v1
    else 
    {
        clip(inc, inc[0], inc[1], ref.v1 *-ref.e,-ref.e);
        clip(inc, inc[0], inc[1], ref.v2 * ref.e, ref.e);
    }

    // Normal Clip
    clip(inc, inc[0], inc[1], ref.v1 * norm, norm);


    debugPoints.push_back(std::make_pair(inc[0], Green));
    debugPoints.push_back(std::make_pair(inc[1], Green));


    return inc[0];
}


// Resolve a collision between two bodies.
// -> Impulse resolution
void Body::resolve(Body& b1, Body& b2, const Collision& collision) {

    const Vec2& n = collision.norm;
    const Vec2& p = collision.contactPoint;
    debugPoints.push_back(std::make_pair(p, Red));

    // Calculate Radius
    const Vec2 r1 = p - b1.getPos();
    const Vec2 r2 = p - b2.getPos();
    std::cout << "n:\t" << n << "  p:\t" << p << "  r:\t" << r1 << ", " << r2 << std::endl;

    // Find projected relative Velocity
    const Vec2 vR = b1.velo + Vec2( -r1.x * b1.angVelo, r1.y * b1.angVelo ) -
                    b2.velo - Vec2( -r1.x * b2.angVelo, r2.y * b2.angVelo );

    const double vRP = vR * n;

    std::cout << "\t" << vR << "\t" << vRP << std::endl;
    // If the velocities are separating, don't resolve
    //if (vRP < 0) 
    //    return;

    const double e = 1 + 0.0001;

    double J;
    if (n.norm() == r1.norm() || -n.norm() == r2.norm()) 
    {
        J = e * vRP / ( b1.invMass + b2.invMass);
    }
    else 
    {
        std::cout << "Inertial factors\n";
        const double b1IntertiaFactor = ((r1 * r1) - (r1*n)*(r1*n)) * b1.invInertia;
        const double b2IntertiaFactor = ((r2 * r2) - (r2*n)*(r2*n)) * b2.invInertia;

        std::cout << "Intertial F's: " << b1IntertiaFactor << " " << b2IntertiaFactor << std::endl;
        J = e * vRP / ( b1.invMass + b2.invMass + b1IntertiaFactor + b2IntertiaFactor );
    } 

    std::cout << "J :" << J << std::endl;
    const Vec2 impulse = n * J;
    
    // Apply Impulse
    b1.velo = b1.velo - impulse * b1.invMass;
    b2.velo = b2.velo + impulse * b2.invMass;

    if (!(n.norm() == r1.norm())) {
        b1.angVelo -= b1.invInertia * (r1.x * impulse.y - r1.y * impulse.x);
        b2.angVelo += b2.invInertia * (r2.x * impulse.y - r2.y * impulse.x);
    }

    
    // (Sink Prevention) Positional Correction, Linear Projection
    const double percent = 0.2;     // usually 20% to 80% 
    const double slop    = 0.01;    // usually 0.01 to 0.1 
    double correction = percent * std::max( collision.depth - slop, (double) 0 ) / (b1.invMass + b2.invMass);
    Vec2 correctionV = collision.norm * correction;

    b1.pos = b1.pos - correctionV * b1.invMass;
    b2.pos = b2.pos + correctionV * b2.invMass;
}


// Checks for collisions and resolves accordingly.
void Environment::collide() {
        
    // Prune with boardphase
    std::vector<Body>& bodyList = boardphase(*this);

    // Find collisions
    for (int i=0; i<bodyList.size(); i++) 
    {
        for (int j=i+1; j<bodyList.size(); j++) 
        {
            // Check for collision
            auto opt = detectCollisionSAT(bodyList[i], bodyList[j]);
            if (!opt) continue;
            auto [overlap, separationAxis] = *opt;
            auto contactP = findContactPoints(bodyList[i], bodyList[j], separationAxis);
            Collision collision (separationAxis, overlap, contactP);

            // Resolve collision
            // DEBUG: Set color to red 
            bodyList[i].color = SDL_Color{100,100,100};
            bodyList[j].color = SDL_Color{100,100,100};

            Body::resolve(bodyList[i], bodyList[j], collision);
        }
    }
}


