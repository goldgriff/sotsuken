#include<iostream>
#include<string>
#include "particle.hpp"
#include "particleFactory.hpp"
#include "energy.hpp"
#include "output.hpp"

using namespace std;


int main()
{
    Output *op = new Output();

    double circleRadius = 0.5;
    double box_size = 30.0;
    double delr = 0.3;
    double deltheta = 5.0;
    ParticleFactory *pf = new ParticleFactory(box_size,delr,deltheta, circleRadius);
    auto a = pf->initializeParticles(2);
    Energy energy(a,0,100,box_size,1,1);
    string filenamep = "outputdata/" + to_string(0) + "particle.csv";
    string filenamee = "outputdata/" + to_string(0) + "energy.csv";
    op->outputParticles(filenamep,a);
    op->outputEnergy(filenamee,energy);


    for(int i=1;i<31;i++)
    {
        string filename = "outputdata/" + to_string(i) + "particle.csv";
        string filenameee = "outputdata/" + to_string(i) + "energy.csv";
        a = pf->moveParticles(a);
        Energy energyaa(a,0,100,box_size,1,1);
        op->outputParticles(filename,a);
        op -> outputEnergy(filenameee,energyaa);

    }
    delete pf;
    delete op;
    return 0;
}
