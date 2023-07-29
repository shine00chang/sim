#include "core.h"
#include "environment.h"
#include "collision.h"
#include "app.h"

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

double distOfFace(const Vec2& v1, const Vec2& v2) {
    return -v1 * orthogonalTowards(v1-v2, -v1);
}

// Calculates the support point of a polygon for a given direction.
// -> Iteratively search for vertex with the max projection onto the given direction.
Vec2 support (const Body& b, const Vec2& d) {
    const auto& p = b.getPointsLocal();

    int out = 0;
    int best = 0;
    for (int i=0; i<p.size(); i++) 
        if (d * p[i] > best) {
            best = d * p[i];
            out = i;
        }
    return p[out];
}

// Calculates the support point for the Minkowski difference between two bodies.
// -> Subtract the support points from the two bodies.
Vec2 CSOsupport (const Body& b1, const Body& b2, const Vec2& d) {
    auto s1 = support(b1,  d);
    auto s2 = support(b2, -d);

    return (b1.getPos() + s1) - (b2.getPos() + s2);
}

std::tuple<int, double, Vec2, Vec2> findClosestEdge (const std::vector<Vec2>& pts) {
    double minDist = 0;
    int min = 0;

    for (int i=0; i<pts.size(); i++) {
        const Vec2& a = pts[i == 0 ? pts.size() - 1 : i-1];
        const Vec2& b = pts[i];

        double dist = orthogonalTowards(a-b, a) * a;
        if (i == 0 || dist < minDist) {
            min = i;
            minDist = dist;
        }
        //std::cout << "candidate: " << a << ", " << b << "\tdist: " << dist << std::endl;
    }

    return std::make_tuple(min-1, minDist, pts[min == 0 ? pts.size()-1 : min-1], pts[min]);
}

// GJK Collision detection
std::optional<Collision> detectCollision(const Body& b1, const Body& b2) {

    /*
    std::cout << "points: \n";
    for (auto i : b1.getPointsLocal()) 
        std::cout << i << ", ";
    std::cout << std::endl;
    {
        std::cout << CSOsupport(b1, b2, Vec2(0,0)) << std::endl;
        std::cout << CSOsupport(b1, b2, Vec2(0,1)) << std::endl;
        std::cout << CSOsupport(b1, b2, Vec2(1,1)) << std::endl;
        std::cout << CSOsupport(b1, b2, Vec2(1,0)) << std::endl;
    }
    */

    // Find Initial points. Use a random direction, then Op1.
    Vec2 d (0, 1);
    Vec2 p1 = CSOsupport(b1, b2, d);
    d = (-p1).normalize();
    Vec2 p2 = CSOsupport(b1, b2, d);
    Vec2 p3(0, 0);

    //std::cout << "initial: " << p1 << ", " << p2 << std::endl; 

    while (1) {
        // Find third point.
        const Vec2 edge = p2 - p1;
        if (edge.norm() == p1.norm() || edge.norm() == -p1.norm()) {
            d = Vec2(-edge.y, edge.x); 
        } else {
            d = orthogonalTowards(edge, -p1); 
        }

        p3 = CSOsupport(b1, b2, d);

        //std::cout << "d: " << d << "\tp3: " << p3 << std::endl; 

        // If p3 does not pass the origin, the simplex can never include the origin.
        if (p3 * d < 0) {
            return std::nullopt;
        }

        // If origin is in the direction of face p1, p3, drop p2.
        if (-p3 * orthogonalTowards(p3-p1, p1-p2) > 0) {
            //std::cout << "kept p1\n";
            p2 = p3;
        }
        // If origin is in the direction of face p2, p3, drop p1.
        if (-p3 * orthogonalTowards(p3-p2, p2-p1) > 0) {
            //std::cout << "kept p2\n";
            p1 = p3;
        }
        // Otherwise, origin is within simplex, break & continue to EPA
        else {
            break;
        }
    }

    // EPA
    // (a, b) := Find face closest to origin
    // d := orthoTowards(ab, aO)
    // p := Support(orthoTowards(ab, aO))
    // if p is new => Insert P into polytope
    // if p is already in polytope => {
    //   norm := d
    //   depth: || closet point ||
    // }

    std::vector<Vec2> pts {p1, p2, p3};

    int i = 0;
    while (i++ < 10) {

        /*
        std::cout << "Polytope: ";
        for (auto& v : pts) 
            std::cout << v << ", ";
        std::cout << "\n";
        */
        

        // Find Edge closest to Origin
        const auto [index, dist, A, B] = findClosestEdge(pts);
        const Vec2 d = orthogonalTowards(A-B, A);
        const Vec2 P = d * ( CSOsupport(b1, b2, d) * d );

        // Debug
        //std::cout << "Selected: " << A << ", " << B << "\tindex: " << index << "\tdist: " << dist << "\td : " << d << "\tP : " << P << std::endl;

        // If support function returned a point no further from the origin
        // meaning the current edge is already the furthest edge from the origin
        // this is the closest edge on the polytope to the origin
        if ((P-A) * d < 0.001) {
        //if (P == A || P == B) {
            //std::cout << "RETURNED\td : " << d << "\tmin : " << dist << "EPA Point: " << P << std::endl;
            return Collision(d, dist);
        }
        
        pts.insert(pts.begin() + index+1, P);
    }
    assert(false);
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


Collision findContactPoints (const Body& b1, const Body& b2, Collision& col) {

    // Get Edges
    const Edge e1 = findClippingEdge(b1, col.norm);
    const Edge e2 = findClippingEdge(b2,-col.norm);

    debugPoints.push_back(std::make_pair(e1.v1, Blue));
    debugPoints.push_back(std::make_pair(e1.v2, Blue));
    debugPoints.push_back(std::make_pair(e2.v1, Blue));
    debugPoints.push_back(std::make_pair(e2.v2, Blue));

    // Identify Reference & Incident Edge 
    Edge ref = e2;
    Vec2 inc[] = { e1.v1, e1.v2 };
    Vec2 norm = orthogonalTowards(ref.e, col.norm);

    // if e1.norm is more parallel to norm. ref -> e1
    if (std::abs(e1.n * col.norm) > std::abs(e2.n * col.norm)) {
        ref = e1;
        norm = orthogonalTowards(ref.e,-col.norm);
        inc[0] = e2.v1;
        inc[1] = e2.v2;
    }
    // DEBUG
    std::cout << "norm: " << col.norm << std::endl;
    std::cout << "ref:  " << ref.v1 << ", " << ref.v2 << std::endl;
    std::cout << "inc:  " << inc[0] << ", " << inc[1] << std::endl;
    


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
    std::cout << inc[0] << inc[1] << std::endl;

    // Normal Clip
    clip(inc, inc[0], inc[1], ref.v1 * norm, norm);
    std::cout << inc[0] << inc[1] << std::endl;


    debugPoints.push_back(std::make_pair(inc[0], Green));
    debugPoints.push_back(std::make_pair(inc[1], Green));


    col.contactPoint = inc[0];
    return col;
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
    const Vec2 vR = (b1.velo + r1 * b1.angVelo) - (b2.velo + r2 * b2.angVelo);
    const double vRP = vR * n;


    // If the velocities are separating, don't resolve
    if (vRP < 0) 
        return;

    const double e = 1 + 0.1;

    // Calculate impulse (I DON'T GET THIS)
    double J;
    if (n.norm() == r1.norm() || -n.norm() == r2.norm()) 
    {
        J = e * vRP / ( b1.invMass + b2.invMass);
    }
    else 
    {
        std::cout << "Inertial factors\n";
        const double b1IntertiaFactor = orthogonalTowards(r1, n) * n * b1.invInertia;
        const double b2IntertiaFactor = orthogonalTowards(r2, n) * n * b2.invInertia;

        std::cout << "Intertial F's: " << b1IntertiaFactor << " " << b2IntertiaFactor << std::endl;
        J = e * vRP / ( b1.invMass + b2.invMass + b1IntertiaFactor + b2IntertiaFactor );
    } 

    
    const Vec2 impulse = n * J;
    
    // Apply Impulse
    b1.velo = b1.velo - impulse * b1.invMass;
    b2.velo = b2.velo + impulse * b2.invMass;

    if (!(n.norm() == r1.norm())) {
        b1.angVelo += b1.invInertia * (-orthogonalTowards(r1, n) * impulse);
        b2.angVelo += b2.invInertia * (-orthogonalTowards(r2, n) * impulse);
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
            std::optional<Collision> opt = detectCollision(bodyList[i], bodyList[j]);
            if (!opt) continue;
            auto collision = findContactPoints(bodyList[i], bodyList[j], *opt);

            // Resolve collision
            // DEBUG: Set color to red 
            bodyList[i].color = SDL_Color{100,100,100};
            bodyList[j].color = SDL_Color{100,100,100};

            Body::resolve(bodyList[i], bodyList[j], collision);
        }
    }
}


