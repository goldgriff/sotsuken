#include <sys/stat.h>
#include <sys/types.h>
#include<iostream>
#include<string>
#include <iomanip> //時間を取得するため
#include <sstream> //値を簡単に文字列にするため
#include<math.h>
#include <memory>
#include <random>
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
 
	//現在日時を取得する
	time_t t = time(nullptr);
	 
	//形式を変換する    
	const tm* lt = localtime(&t);
	 
	//sに独自フォーマットになるように連結していく
	std::stringstream s;
	s<<"20";
	s<<lt->tm_year-100; //100を引くことで20xxのxxの部分になる
	s<<"-";
	s<<lt->tm_mon+1; //月を0からカウントしているため
	s<<"-";
	s<<lt->tm_mday; //そのまま
	s<<lt->tm_hour;
	s<<lt->tm_min;
	s<<lt->tm_sec;
	 
	//result = "2015-5-19" 
	std::string result = s.str();
 

    string directoryname = "outputdata/"+result;
	struct stat st;
    if(stat(directoryname.c_str(), &st) != 0){
         mkdir(directoryname.c_str(), 0777);
    }
	
	
    Output *op = new Output();

    double circleRadius = 0.5;
    double box_size = 30;
    double delr = 0.5;
    double deltheta = 10;
    double magnetCutoff = 8;
    double xi = 20;
    double lambdap = 7;


    ParticleFactory *pf = new ParticleFactory(box_size,delr,deltheta, circleRadius);


    auto particles = pf->initializeParticles(64);


    Energy energy(particles,0,magnetCutoff,box_size,xi,lambdap);



    string particlefilename = directoryname + "/particle" + to_string(0) +   ".csv";
    string energyfilename = directoryname + "/energy" +   ".csv";
    op->outputParticles(particlefilename,particles);
    op->outputEnergyInitialize(energyfilename);
    op->outputEnergy(energyfilename,0,energy);


    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> randomprobgenerator(0.0,1.0);
    cout << "ok" << endl;
    for(int i=1;i<=50000;i++)
    {
        if( i% 1000 == 0)
        {
            particlefilename = directoryname + "/particle" + to_string(i)+".csv";
            op->outputParticles(particlefilename,particles);
            op->outputEnergy(energyfilename,i,energy);
        }


        auto tmpparticles = pf->moveParticles(particles);
        Energy tmpenergy(tmpparticles,0,magnetCutoff,box_size,xi,lambdap);

        if(energy.getEnergy() > tmpenergy.getEnergy())
        {
            energy = tmpenergy;
			particles = tmpparticles;
            //particles = pf->copyParticles(tmpparticles);
        } else 
        {
            double probab = exp(-(tmpenergy.getEnergy() - energy.getEnergy()));
            double randprob = randomprobgenerator(mt);
            if(probab > randprob)
            {
                energy = tmpenergy;
				particles = tmpparticles;
                //particles = pf->copyParticles(tmpparticles);
            }
        }
        //op->outputParticles(filename,particles);
        //op -> outputEnergy(filenameee,energyaa);



    }
    delete pf;
    delete op;
    return 0;
}
