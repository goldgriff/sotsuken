#pragma once

#include<memory>
#include<vector>
#include<string>

class Particle;
class Energy;

using Particles = std::vector<std::unique_ptr<Particle>>;
using namespace std;
using namespace Eigen;

class Output
{
    private:
        int outputCount = 0;
    public:
        void outputParticles(std::string const & filename, Particles const & particles);
        void outputEnergy(std::string const & filename,int const& count, vector<double> const& energies, MatrixXd const&  magneticEnergies, MatrixXd const& stericEnergies); 

        void outputEnergyInitialize(std::string const & filename);

};
