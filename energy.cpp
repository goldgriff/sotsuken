#include <math.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "energy.hpp"
#include "particle.hpp"

#define PI 3.14159265
using namespace Eigen;


ParticleForCalc::ParticleForCalc(const std::unique_ptr<Particle> &particle)
{
    double cr = particle -> circleRadius();
    double pa = particle -> particleAngle();
    double ma = particle -> magnetizationAngle();
    auto pos = particle -> position();
    double angle1 = pa/180.0*PI;
    double angle2 = (pa+120)/180.0*PI;
    double angle3 = (pa+240)/180.0*PI;

    Eigen::Vector2d addVector2(cos(angle2),sin(angle2));
    Eigen::Vector2d addVector3(cos(angle3),sin(angle3));
    Eigen::Vector2d addVector1(cos(angle1),sin(angle1));

    particleAngle = pa;
    magnetizationAngle = ma;
    positions[0] = pos + addVector1 * 4 * cr; 
    positions[1] = pos + addVector2 * 4 * cr;
    positions[2] = pos + addVector3 * 4 * cr;
    positions[3] = pos + addVector1 * 2 * cr;
    positions[4] = pos + addVector2 * 2 * cr;
    positions[5] = pos + addVector3 * 2 * cr;
    positions[6] = particle -> position();
}






//first: distance
//second: p2nohou flag(0:+0 1:+(box,0) 2:+(0,box) 3:+ (box,box) 4:-1 5:-2 6:-3
std::pair<double,int> Energy::minimumDistance( Eigen::Vector2d p1, Eigen::Vector2d p2, double box_size)
{
    std::array<double,7> distanceArray;
    Eigen::Vector2d x(box_size,0);
    Eigen::Vector2d y(0,box_size);
    Eigen::Vector2d xy(box_size,box_size);
    distanceArray[0] = (p1-p2).norm();
    distanceArray[1] = (p1-p2-x).norm();
    distanceArray[2] = (p1-p2-y).norm();
    distanceArray[3] = (p1-p2-xy).norm();
    distanceArray[4] = (p1-p2+x).norm();
    distanceArray[5] = (p1-p2+y).norm();
    distanceArray[6] = (p1-p2+xy).norm();
    auto minItr = std::min_element(distanceArray.begin(), distanceArray.end());
    int index = static_cast<int>(std::distance(distanceArray.begin(),minItr));
    return std::make_pair(*minItr,index);
}



Energy::Energy(Particles const &particles, double stericEnergyCutOff,double magneticEnergyCutOff,double box_size, double xi, double paramaterLambda)
{
    double stericEnergy = 0;
    double magneticEnergyBetweenParticle = 0;
    double magneticEnergyBetweenMagneticField = 0;

    //ParticleForCalc make
    std::vector<std::unique_ptr<ParticleForCalc>> setOfParticleForCalc;

    for( auto itr = particles.begin(); itr != particles.end(); ++itr)
    {
        std::unique_ptr<ParticleForCalc> tempPtr(new ParticleForCalc(*itr));
        setOfParticleForCalc.push_back(move(tempPtr));
    }

    // caluculate with ParticleForCalc
    for( auto itr = setOfParticleForCalc.begin(); itr != setOfParticleForCalc.end(); ++itr)
    {
        Vector2d pos1 = (*itr)->positions[6];
        for( auto itr2 = itr+1; itr2 != setOfParticleForCalc.end(); ++itr2)
        {
            Vector2d pos2 = (*itr2)->positions[6];
            double distance = (minimumDistance(pos1,pos2,box_size)).first;
            if(distance < stericEnergyCutOff) stericEnergy += calcStericEnergy(*(*itr),*(*itr2));
            if(distance < magneticEnergyCutOff) magneticEnergyBetweenParticle += calcMagneticEnergy(*(*itr),*(*itr2),box_size ,  paramaterLambda);
        }
    }

    //jiba
    for (auto itr = particles.begin(); itr != particles.end(); ++itr)
    {
        magneticEnergyBetweenMagneticField += calcMagneticEnergyBetweenMagneticField(*(*itr),  xi);
    }


    _stericEnergy = stericEnergy;
    _magneticEnergyBetweenParticle = magneticEnergyBetweenParticle;
    _magneticEnergyBetweenMagneticField = magneticEnergyBetweenMagneticField;
    _energy = stericEnergy + magneticEnergyBetweenParticle + magneticEnergyBetweenMagneticField;
}



    
double Energy::calcStericEnergy(ParticleForCalc const &p1, ParticleForCalc const &p2)
{
    return 0;
}





double Energy::calcMagneticEnergy(ParticleForCalc const &p1, ParticleForCalc const &_p2, double box_size, double paramaterLambda)
{
    Eigen::Vector2d x(box_size,0);
    Eigen::Vector2d y(0,box_size);
    Eigen::Vector2d xy(box_size,box_size);
    Eigen::Vector2d addVector(0,0);

    ParticleForCalc p2 = _p2;
    int distFlag = ( minimumDistance(p1.positions[6],p2.positions[6], box_size)).second;

    switch (distFlag)
    {
        case 0:
            break;
        case 1:
            addVector = x;
            break;
        case 2:
            addVector = y;
            break;
        case 3:
            addVector = xy;
            break;
        case 4:
            addVector =   -x;
            break;
        case 5:
            addVector =   -y;
            break;
        case 6:
            addVector =   -xy;
            break;
    }
    
    for(auto itr = p2.positions.begin(); itr != p2.positions.end(); ++itr)
    {
        *itr += addVector;
    }

    double energy = 0;

    for(auto itr = p1.positions.begin(); itr != p1.positions.end(); ++itr)
    {
        for(auto itr2 = p2.positions.begin(); itr2 != p2.positions.end(); ++itr2)
        {
            energy += calcMagneticEnergyBetweenPortion(*itr,*itr2,p1.particleAngle+p1.magnetizationAngle,p2.particleAngle+p2.magnetizationAngle, paramaterLambda);
        }
    }
    return energy;
}




double Energy::calcMagneticEnergyBetweenPortion(Eigen::Vector2d const &p1, Eigen::Vector2d const & p2, double  const &angle1, double const &angle2, double paramaterLambda)
{
    Eigen::Vector2d rVec = p1-p2;
    Eigen::Vector2d t = rVec.normalized(); 
    double r = rVec.norm();
    double radAngle1 = angle1 / 180.0 * PI;
    double radAngle2 = angle2 / 180.0 * PI;
    Eigen::Vector2d n1(cos(radAngle1),sin(radAngle1));
    Eigen::Vector2d n2(cos(radAngle2),sin(radAngle2));
    return paramaterLambda / (r*r*r) * (n1.dot(n2)-3*(n1.dot(t))*(n2.dot(t)));
}




double Energy::calcMagneticEnergyBetweenMagneticField(Particle const & particle, double xi)
{
    double radAngle = (particle.particleAngle() + particle.magnetizationAngle()) / 180.0 * PI;
    //7tubunn
    return -sin(radAngle) * xi * 7;
}





double Energy::getEnergy() const
{
    return _energy;
}

double Energy::getStericEnergy() const
{
    return _stericEnergy;
}

double Energy::getMagneticEnergyBetweenParticle() const
{
    return _magneticEnergyBetweenParticle;
}

double Energy::getMagneticEnergyBetweenMagneticField() const
{
    return _magneticEnergyBetweenMagneticField;
}
