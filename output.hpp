#pragma once

#include<memory>
#include<vector>
#include<string>

class Particle;
class Energy;

using Particles = std::vector<std::unique_ptr<Particle>>;


class Output
{
    private:
        int outputCount = 0;
    public:
        void outputParticles(std::string const & filename, Particles const & particles);
        void outputEnergy(std::string const & filename,int const& count, Energy const & energy);

        void outputEnergyInitialize(std::string const & filename);

};
