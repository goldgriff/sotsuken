#include<iostream>
#include<string>
#include "particle.hpp"
#include "particleFactory.hpp"
#include "energy.hpp"
#include "output.hpp"

using namespace std;

/////////////////////////////////
#include <sys/time.h>

struct __bench__ {
  double start;
  __bench__(int dummy) {
    start = sec();
  }
  ~__bench__() {
    fprintf(stderr, ": %.6f sec\n", sec() - start);
  }
  double sec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
  }
  operator bool() { return false; }
};

#define benchmark(...) \
  if (__bench__ __b__ = __bench__(fprintf(stderr, __VA_ARGS__))); else
//////////////////////////////////////////////////////////////


int main()
{
    Output *op = new Output();

    double circleRadius = 0.5;
    double box_size = 108;
    double delr = 0.3;
    double deltheta = 5.0;
    ParticleFactory *pf = new ParticleFactory(box_size,delr,deltheta, circleRadius);
    auto a = pf->initializeParticles(108);
    Energy energy(a,0,8,box_size,1,1);
    string filenamep = "outputdata/" + to_string(0) + "particle.csv";
    string filenamee = "outputdata/" + to_string(0) + "energy.csv";
    //op->outputParticles(filenamep,a);
    //op->outputEnergy(filenamee,energy);


    for(int i=1;i<30000;i++)
    {

        benchmark("%d",i)
        {
        string filename = "outputdata/" + to_string(i) + "particle.csv";
        string filenameee = "outputdata/" + to_string(i) + "energy.csv";
        a = pf->moveParticles(a);
        Energy energyaa(a,0,8,box_size,1,1);
        //op->outputParticles(filename,a);
        //op -> outputEnergy(filenameee,energyaa);
        } 
    }
    delete pf;
    delete op;
    return 0;
}
