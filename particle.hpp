#pragma once
#include<Eigen/Core>
#include<Eigen/Geometry>
#include <vector>

using namespace Eigen;
using namespace std;


class Particle
{

    public:
        Vector2d position;
        double particleAngle;
        double magnetizationAngle;
        Particle(Vector2d pos, double parAng, double magAng);
};
