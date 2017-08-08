#include <math.h>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "energy.hpp"
#include "particle.hpp"

#define PI 3.14159265
using namespace Eigen;




ParticleForCalc::ParticleForCalc(const std::unique_ptr<Particle> &particle)
{
    double pa = particle -> particleAngle;
    double ma = particle -> magnetizationAngle;
    auto pos = particle -> position;
    double angle1 = pa/180.0*PI;
    double angle2 = (pa+120)/180.0*PI;
    double angle3 = (pa+240)/180.0*PI;

    Eigen::Vector2d addVector2(cos(angle2),sin(angle2));
    Eigen::Vector2d addVector3(cos(angle3),sin(angle3));
    Eigen::Vector2d addVector1(cos(angle1),sin(angle1));

    particleAngle = pa;
    magnetizationAngle = ma;
    positions[0] = pos + addVector1 * 4 ; 
    positions[1] = pos + addVector2 * 4 ;
    positions[2] = pos + addVector3 * 4 ;
    positions[3] = pos + addVector1 * 2 ;
    positions[4] = pos + addVector2 * 2 ;
    positions[5] = pos + addVector3 * 2 ;
    positions[6] = particle -> position;
}








Energy::Energy(double stericEnergyCutOff,double magneticEnergyCutOff,double box_size, double xi, double paramaterLambda)
{
    this -> stericEnergyCutOff = stericEnergyCutOff;
    this->magneticEnergyCutOff = magneticEnergyCutOff;
    this->box_size = box_size;
    this->xi = xi;
    this->paramaterLambda = paramaterLambda;
}




    
double Energy::_calcStericEnergy(ParticleForCalc const &p1, ParticleForCalc const &p2)
{
    return 0;
}

double Energy::calcStericEnergy(Particle const &p1, Particle const &p2)
{
    return 0;
}



double Energy::calcMagneticEnergyBetweenPortion(Eigen::Vector2d const &p1, Eigen::Vector2d const & p2, double  const &angle1, double const &angle2)
{
    Eigen::Vector2d rVec = p1-p2;
    Eigen::Vector2d t = rVec.normalized(); 
    double r = rVec.norm();
    double radAngle1 = angle1 / 180.0 * PI;
    double radAngle2 = angle2 / 180.0 * PI;
    Eigen::Vector2d n1(cos(radAngle1),sin(radAngle1));
    Eigen::Vector2d n2(cos(radAngle2),sin(radAngle2));
    return this->paramaterLambda / (r*r*r) * (n1.dot(n2)-3*(n1.dot(t))*(n2.dot(t)));
}


double Energy::calcMagneticEnergyBetweenOtherParticle(Particle const & p1, Particle const & p2)
{
    double x1 = p1.position.x();
    double x2 = p2.position.x();
    double y1 = p1.position.y();
    double y2 = p2.position.y();
    double tmp;
    if(x1 > x2)
    {
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    if(y1 > y2)
    {
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }


    if(x1 < this->magneticEnergyCutOff && x2 > this->box_size - this->magneticEnergyCutOff)
    {
        x2 -= this->box_size;
    }

    if(y1 < this->magneticEnergyCutOff && y2 > this->box_size - this->magneticEnergyCutOff)
    {
        y2 -= this->box_size;
    }

    //cutof sareru
    if(pow(x2-x1,2) + pow(y2 - y1, 2) > pow(this->magneticEnergyCutOff, 2)) return 0;


    Eigen::Vector2d pos1(x1,y1);
    Eigen::Vector2d pos2(x2,y2);

    double angle1 = p1.particleAngle + p1.magnetizationAngle;
    double angle2 = p2.particleAngle + p2.magnetizationAngle;

    return calcMagneticEnergyBetweenPortion(pos1,pos2,angle1,angle2);

}


double Energy::calcMagneticEnergyBetweenMagneticField(Particle const & particle)
{
    double radAngle = (particle.particleAngle + particle.magnetizationAngle) / 180.0 * PI;
    //7tubunn
    return -sin(radAngle) * this->xi;
}




double Energy::calcEnergies( std::vector < double > &magneticEnergies, std::vector< double > &stericEnergies ,Particle  & movedParticle ,Particles  & particles, int &index )
{
    for(int k=0; k < magneticEnergies.size(); k++)
    {
        if(index == k)
        {
            magneticEnergies[k] =  calcMagneticEnergyBetweenMagneticField(movedParticle);
            stericEnergies[k] = 0;
            continue;
        }

        magneticEnergies[k] =  calcMagneticEnergyBetweenOtherParticle(movedParticle,*particles[k]);
        stericEnergies[k] =  calcStericEnergy(movedParticle,*particles[k]);

    }
}
