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
        Eigen::Vector2d pos = (*itr)->position;
        double pa = (*itr) -> particleAngle;
        double ma = (*itr) -> magnetizationAngle;
        ofs << pos(0) << "," << pos(1) << "," << pa << "," << ma << endl;
    }
}


void Output::outputEnergy(std::string const & filename,int const& count, vector<double> const & energies, MatrixXd const & magneticEnergies, MatrixXd const & stericEnergies) 
{
    ofstream ofs(filename,std::ios::app);
    double stericEnergy = 0;
    double magneticEnergyBetweenParticle = 0;
    double magneticEnergyBetweenMagneticField = 0;

    magneticEnergyBetweenMagneticField += magneticEnergies.trace();
    magneticEnergyBetweenParticle += (magneticEnergies.sum() - magneticEnergyBetweenMagneticField ) / 2.0;
    stericEnergy += stericEnergies.sum() / 2.0;
    double energy = magneticEnergyBetweenMagneticField + magneticEnergyBetweenParticle + stericEnergy;

    ofs << count << ",";
    ofs << energy << ",";
    ofs << stericEnergy << ",";
    ofs << magneticEnergyBetweenParticle << ",";
    ofs << magneticEnergyBetweenMagneticField << endl;

    cout << "energies" << endl ;
    for(int i = 0; i < energies.size(); ++i)
    {
        cout << energies[i] << "  ";
    }
    cout << endl << endl;

    cout << "magneticEnergies" << endl << magneticEnergies << endl << endl;
    cout << "stericEnergies" << endl << stericEnergies << endl << endl;
}

void Output::outputEnergyInitialize(std::string const & filename)
{
    ofstream ofs(filename);
    ofs << "count,energy,stericEnergy,magneticEnergyBetweenParticle,magneticEnergyBetweenMagneticField" << endl;
}

