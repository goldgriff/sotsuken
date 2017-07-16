#include "particle.hpp"

using namespace Eigen;

Particle::Particle(Vector2d pos, double parAng, double magAng,double circleRadius)
{
    _position = pos;
    _particleAngle = parAng;
    _magnetizationAngle = magAng;
    _circleRadius = circleRadius;
}

Vector2d Particle::position()
{
    return _position;
}

double Particle::particleAngle()
{
    return _particleAngle;
}

double Particle::magnetizationAngle()
{
    return _magnetizationAngle;
}

double Particle::circleRadius()
{
    return _circleRadius;
}

