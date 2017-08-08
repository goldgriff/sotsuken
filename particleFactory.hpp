#pragma once

#include <memory>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "particle.hpp"


using ptrParticle = std::unique_ptr<Particle>;
using Particles = std::vector<std::unique_ptr<Particle>>;
using namespace Eigen;
using namespace std;
class ParticleFactory
{

    private:
        const double _box_size;
        const double _delr;
        const double _deltheta;
    public:
        ParticleFactory(double box_size, double delr, double deltheta);
        double minimumPowDistance(Eigen::Vector2d const & p1, Eigen::Vector2d const & p2);
        bool isOverlap(Vector2d const & pos, vector<Vector2d> const & positions);
        Particles initializeParticles(int particleNumber);
        ptrParticle moveParticle(Particle const& particle);
        Vector2d adjustPosition(Vector2d original_position);
        double adjustAngle(double original_angle);
};
