#pragma once
#include <vector>
#include <memory>
#include <array>
#include <Eigen/Core>

class Particle;

using Particles = std::vector<std::unique_ptr<Particle>>;
using Positions = std::array<Eigen::Vector2d,7>;

class ParticleForCalc
{
    public:
        ParticleForCalc(const std::unique_ptr<Particle> &particle);

        /*
         *        1
         *        +  4
         *           +            
         *               6   3   0
         *               +   +   +
         *           5
         *        2  +      
         *        +
         *
        */
        Positions positions;
        double particleAngle;
        double magnetizationAngle;

};


class Energy
{
    private:
        double _energy;
        double _stericEnergy;
        double _magneticEnergyBetweenParticle;
        double _magneticEnergyBetweenMagneticField;
    public:
        Energy(double stericEnergy,double magneticEnergyBetweenParticle,double magneticEnergyBetweenMagneticField)
        {
            _stericEnergy = stericEnergy;
            _magneticEnergyBetweenParticle = magneticEnergyBetweenParticle;
            _magneticEnergyBetweenMagneticField = magneticEnergyBetweenMagneticField;
            _energy = stericEnergy + magneticEnergyBetweenMagneticField + magneticEnergyBetweenParticle;
        }
        Energy(Particles particles, double stericEnergyCutOff,double magneticEnergyCutOff,double box_size, double xi, double paramaterLambda);
        

        double calcStericEnergy(ParticleForCalc const &p1, ParticleForCalc const &p2);
        double calcMagneticEnergy(ParticleForCalc const &p1, ParticleForCalc const &p2, double paramaterLambda);

        //first: distance
        //second: p2nohou flag(0:+0 1:+(box,0) 2:+(0,box) 3:+ (box,box) 4:-1 5:-2 6:-3
        std::pair<double,int> minimumDistance( Eigen::Vector2d p1, Eigen::Vector2d p2, double box_size);

        double calcMagneticEnergyBetweenPortion(Eigen::Vector2d const &p1, Eigen::Vector2d const & p2, double  const &angle1, double const &angle2, double paramaterLambda);

        double calcMagneticEnergyBetweenMagneticField(Particle const & particle, double xi);
};
