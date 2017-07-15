#pragma once
#include<Eigen/Core>
#include<Eigen/Geometry>


using namespace Eigen;

class Particle
{

    private:
        Vector2d _position;
        double _particleAngle;
        double _magnetizationAngle;
    public:
        Particle(Vector2d pos, double parAng, double magAng);
        Vector2d position();
        double particleAngle();
        double magnetizationAngle();
};
