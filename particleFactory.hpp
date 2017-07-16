#pragma once

#include <memory>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "particle.hpp"

using Particles = std::vector<std::unique_ptr<Particle>>;
using namespace Eigen;

class ParticleFactory
{

    private:
        const double _box_size;
        const double _delr;
        const double _deltheta;
        const double _circleRadius;
    public:
        ParticleFactory(double box_size, double delr, double deltheta, double circleRadius);
        Particles initializeParticles(int particleNumber);
        Particles moveParticles(Particles const& particles);
        Vector2d adjustPosition(Vector2d original_position);
        double adjustAngle(double original_angle);
};
