#include "particle.hpp"

using namespace Eigen;

Particle::Particle(Vector2d pos, double parAng, double magAng)
{
    _position = pos;
    _particleAngle = parAng;
    _magnetizationAngle = magAng;
}

Vector2d Particle::position() const
{
    return _position;
}

double Particle::particleAngle() const
{
    return _particleAngle;
}

double Particle::magnetizationAngle() const
{
    return _magnetizationAngle;
}

