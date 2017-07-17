#include<iostream>
#include "particle.hpp"
#include "particleFactory.hpp"
#include "energy.hpp"

using namespace std;


int main()
{
    double circleRadius = 0.5;
    double box_size = 30.0;
    double delr = 0.3;
    double deltheta = 5.0;
    ParticleFactory *pf = new ParticleFactory(box_size,delr,deltheta, circleRadius);
    auto a = pf->initializeParticles(108);
    int i = 1;
    for(auto itr=a.begin(); itr != a.end(); ++itr)
    {
        cout << "position" << i << endl;
        cout << (*itr)->position() << endl << endl ;
        ++i;
    }

    for(i=0;i<30000;i++)
    {
        if(i % 1000 == 0) cout << i << endl;
        a = pf->moveParticles(a);
    }

    delete pf;
    return 0;
}
