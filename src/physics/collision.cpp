#include "core.h"
#include "environment.h"
#include "collision.h"
#include "app.h"
#include "view.h"

#include <cmath>
#include <limits>
#include <utility>
#include <vector>
#include <list>
#include <optional>
#include <memory>


// Boardphase
std::vector<std::shared_ptr<Body>>& boardphase(Environment& env) {
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
            return std::nullopt;
        }
        else 
        // Check overlap to find MTV
        {
            double o = max1-min2;
            if (std::isnan(overlap) ||  o < overlap) {
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

    injectDebugEffect(std::make_shared<PointEffect>(e1.v1, Blue));
    injectDebugEffect(std::make_shared<PointEffect>(e1.v2, Blue));
    injectDebugEffect(std::make_shared<PointEffect>(e2.v1, Blue));
    injectDebugEffect(std::make_shared<PointEffect>(e2.v2, Blue));

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


    // Select Point with higher overlap
    Vec2 p = inc[1];
    if (inc[0] * norm > inc[1] * norm) 
    {
        p = inc[0];
    }
    injectDebugEffect(std::make_shared<PointEffect>(p, Green));

    return p; 
}


// Resolve a collision between two bodies.
// -> Impulse resolution
void Body::resolve(Body& b1, Body& b2, const Collision& collision, const double dt) {

    const Vec2& n = collision.norm;
    const Vec2& p = collision.contactPoint;

    // Collision Tangent
    const Vec2 t = Vec2(-collision.norm.y, collision.norm.x);

    // Calculate Radius
    const Vec2 r1 = p - b1.getPos();
    const Vec2 r2 = p - b2.getPos();

    injectDebugEffect(std::make_shared<VectorEffect>(p, n * 20, Red));
    injectDebugEffect(std::make_shared<VectorEffect>(p, t * 20, Blue));
    injectDebugEffect(std::make_shared<VectorEffect>(b1.getPos(), r1, Green));
    injectDebugEffect(std::make_shared<VectorEffect>(b2.getPos(), r2, Green));


    //std::cout << "n:\t" << n << "  p:\t" << p << "  r:\t" << r1 << ", " << r2 << std::endl;

    // Find projected relative Velocity
    const Vec2 vR = b1.velo + Vec2( -r1.y * b1.angVelo, r1.x * b1.angVelo ) -
                    b2.velo - Vec2( -r2.y * b2.angVelo, r2.x * b2.angVelo );

    const double vRn = vR * n;
    const double vRt = vR * t;


    const double e = 1 + 0.0001;

    double Jn, Jt;
    {
        const double b1IntertiaFactor = ((r1 * r1) - (r1*n)*(r1*n)) * b1.invInertia;
        const double b2IntertiaFactor = ((r2 * r2) - (r2*n)*(r2*n)) * b2.invInertia;

        Jn = e * vRn / ( b1.invMass + b2.invMass + b1IntertiaFactor + b2IntertiaFactor );
    }
    {
        const double b1IntertiaFactor = ((r1 * r1) - (r1*t)*(r1*t)) * b1.invInertia;
        const double b2IntertiaFactor = ((r2 * r2) - (r2*t)*(r2*t)) * b2.invInertia;
        const double friction = std::sqrt(b1.friction * b2.friction);
     
        Jt = friction * e * vRt / ( b1.invMass + b2.invMass + b1IntertiaFactor + b2IntertiaFactor );
    }
     
    // Apply Impulse
    const Vec2 impulse = n * Jn;

    b1.velo = b1.velo - impulse * b1.invMass;
    b2.velo = b2.velo + impulse * b2.invMass;

    b1.angVelo -= b1.invInertia * (Vec2(-r1.y, r1.x) * impulse);
    b2.angVelo += b2.invInertia * (Vec2(-r2.y, r2.x) * impulse);


    // Apply Tangent Impulse
    const Vec2 impulseT = t * Jt;

    b1.velo = b1.velo - impulseT * b1.invMass;
    b2.velo = b2.velo + impulseT * b2.invMass;

    b1.angVelo -= b1.invInertia * (Vec2(-r1.y, r1.x) * impulseT);
    b2.angVelo += b2.invInertia * (Vec2(-r2.y, r2.x) * impulseT);


    // (Sink Prevention) Positional Correction, Linear Projection
    const double percent = 0.2;     // usually 20% to 80% 
    const double slop    = 0.01;    // usually 0.01 to 0.1 
    double correction = percent * std::max( collision.depth - slop, (double) 0 ) / (b1.invMass + b2.invMass);
    Vec2 correctionV = collision.norm * correction;

    b1.pos = b1.pos - correctionV * b1.invMass;
    b2.pos = b2.pos + correctionV * b2.invMass;
}


// Checks for collisions and resolves accordingly.
void Environment::collide(const double dt) {
        
    // Prune with boardphase
    auto& bodyList = boardphase(*this);

    // Find collisions
    for (int i=0; i<bodyList.size(); i++) 
    {
        for (int j=i+1; j<bodyList.size(); j++) 
        {
            // Check for collision
            auto opt = detectCollisionSAT(*bodyList[i], *bodyList[j]);
            if (!opt) continue;
            auto [overlap, separationAxis] = *opt;
            auto contactP = findContactPoints(*bodyList[i], *bodyList[j], separationAxis);
            Collision collision (separationAxis, overlap, contactP);

            // Resolve collision
            Body::resolve(*bodyList[i], *bodyList[j], collision, dt);
        }
    }
}


