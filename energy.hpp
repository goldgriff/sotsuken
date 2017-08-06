#pragma once
#include <memory>
#include <array>
#include <Eigen/Core>

class Particle;

using Particles = std::array<std::unique_ptr<Particle>>;
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
    public:
        double stericEnergyCutOff;
        double magneticEnergyCutOff;
        double box_size;
        double xi;
        double paramaterLambda;

        Energy(double stericEnergyCutOff,double magneticEnergyCutOff,double box_size, double xi, double paramaterLambda);
        

        double _calcStericEnergy(ParticleForCalc const &p1, ParticleForCalc const &p2);
        double calcStericEnergy(Particle const &p1, Particle const &p2);


        //first: distance
        //second: p2nohou flag(0:+0 1:+(box,0) 2:+(0,box) 3:+ (box,box) 4:-1 5:-2 6:-3
        std::pair<double,int> minimumDistance( Eigen::Vector2d p1, Eigen::Vector2d p2);

        double calcMagneticEnergyBetweenOtherParticle(Particle const & p1 , Particle const & p2);

        double calcMagneticEnergyBetweenMagneticField(Particle const & particle);
        
};
