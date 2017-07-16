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
        double _circleRadius;
    public:
        Particle(Vector2d pos, double parAng, double magAng, double circleRadius);
        Vector2d position() const;
        double particleAngle() const;
        double magnetizationAngle() const;
        double circleRadius() const;
};
