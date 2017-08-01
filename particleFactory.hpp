#pragma once

#include <memory>
#include <array>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "particle.hpp"


using ptrParticle = std::unique_ptr<Particle>
using Particles = std::array<std::unique_ptr<Particle>>;
using namespace Eigen;

class ParticleFactory
{

    private:
        const double _box_size;
        const double _delr;
        const double _deltheta;
    public:
        ParticleFactory(double box_size, double delr, double deltheta);
        Particles initializeParticles(int particleNumber);
        ptrParticle moveParticle(Particle const& particle);
        Vector2d adjustPosition(Vector2d original_position);
        double adjustAngle(double original_angle);
};
