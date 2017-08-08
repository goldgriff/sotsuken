#include "particle.hpp"

using namespace Eigen;

Particle::Particle(Vector2d pos, double parAng, double magAng)
{
    position = pos;
    particleAngle = parAng;
    magnetizationAngle = magAng;
}
