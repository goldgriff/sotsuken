#include <iostream>
#include <fstream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "output.hpp"
#include "particle.hpp"
#include "energy.hpp"

using namespace std;

void Output::outputParticles(std::string const & filename, Particles const & particles)
{
    ofstream ofs(filename);
    ofs << "x,y,particle angle,magnetization angle" << endl;

    for (auto itr = particles.begin(); itr != particles.end(); ++itr)
    {
        Eigen::Vector2d pos = (*itr)->position();
        double pa = (*itr) -> particleAngle();
        double ma = (*itr) -> magnetizationAngle();
        ofs << pos(0) << "," << pos(1) << "," << pa << "," << ma << endl;
    }
}


void Output::outputEnergy(std::string const & filename, Energy const & energy)
{
    ofstream ofs(filename);
    ofs << "energy,stericEnergy,magneticEnergyBetweenParticle,magneticEnergyBetweenMagneticField" << endl;
    ofs << energy.getEnergy() << ",";
    ofs << energy.getStericEnergy() << ",";
    ofs << energy.getMagneticEnergyBetweenParticle() << ",";
    ofs << energy.getMagneticEnergyBetweenMagneticField() << endl;
}
