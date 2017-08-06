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


void Output::outputEnergy(std::string const & filename,int const& count, double const & energy, double const & magneticEnergyBetweenMagneticField, double const & magneticEnergyBetweenParticle, double const & stericEnergy);
{
    ofstream ofs(filename,std::ios::app);
    ofs << count << ",";
    ofs << energy << ",";
    ofs << stericEnergy << ",";
    ofs << magneticEnergyBetweenParticle << ",";
    ofs << magneticEnergyBetweenMagneticField << endl;
}

void Output::outputEnergyInitialize(std::string const & filename)
{
    ofstream ofs(filename);
    ofs << "count,energy,stericEnergy,magneticEnergyBetweenParticle,magneticEnergyBetweenMagneticField" << endl;
}

