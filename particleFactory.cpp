#include <random>
#include "particleFactory.hpp"

ParticleFactory::ParticleFactory(double box_size, double delr, double deltheta) : _box_size(box_size), _delr(delr), _deltheta(deltheta){
}

Particles ParticleFactory::initializeParticles(int particleNumber)
{
    std::random_device rand_dev;
    std::mt19937 mt(rand_dev())
    std::uniform_real_distribution<double> posMake(0.0,_box_size);
    std::uniform_real_distribution<double> angleMake(0.0,360.0);

    Particles particles(particleNumber);

    for(aut itr=particles.begin(); itr != particles.end() ++itr)
    {
        Vector2d pos(posMake(mt),pasMake(mt));
        double parAng = angleMake(mt);
        double magAng = angleMake(mt);
        std::unique_ptr<Particle> ptr(new Particle(pos,parAng,magAng));
        *itr = std::move(ptr);
    }
    return particles;
}

Particles ParticleFactory::moveParticles(Particles const& particles)
{
    std::random_device rand_dev;
    std::mt19937 mt(rand_dev())
    std::uniform_real_distribution<double> diffrMaker(-_delr,_delr);
    std::uniform_real_distribution<double> diffthetaMaker(-_deltheta,_deltheta);
    Particles newParticles;
    for(itr = auto particles.begin(), itr != particle.end(), ++itr)
    {
        Vector2d diffr(diffrMaker(mt),diffrMaker(mt));
        Vector2d pos = (*itr)->position() + diffr;
        pos = adjustPostion(pos);
        double difftheta = diffthetaMaker(mt);
        double parAng = (*itr)->particleAngle() + difftheta;
        parAng = adjustAngle(parAng);
        double magAng = (*itr)->magnetizationAngle();
        std::unique_ptr<Particle> ptr(new Particle(pos,parAng,magAng));
        newParticles.push_back(std::move(ptr));
    }
    return newParticles;
}

Vector2d ParticleFactory::adjustPosition(Vector2d original_position)
{
    double x,y,o_x=original_position[0],o_y=original_position[1];
    if(o_x < 0) x = _box_size+o_x;
    else if(o_x >= _box_size) x = o_x-_box_size;
    else x=o_x;
    if(o_y < 0) y = _box_size+o_y;
    else if(o_y >= _box_size) y = o_y-_box_size;
    else y=o_y;
    return Vector2d(x,y);
}

double ParticleFactory::adjustAngle(double original_angle)
{
    if( original_angle<=0 ) return 360.0+original_angle;
    else if( original_angle>360 ) return original_angle-360.0;
    return original_angle;
}
