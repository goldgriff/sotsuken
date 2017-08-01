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
using ptrParticle = unique_ptr<Particle>

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

    double box_size = 30;
    double delr = 0.5;
    double deltheta = 10;
    double magnetCutoff = 8;
    double xi = 0;
    double lambdap = 7;


    ParticleFactory *pf = new ParticleFactory(box_size,delr,deltheta);


    auto particles = pf->initializeParticles(64);


    Energy energyCalc(particles,0,magnetCutoff,box_size,xi,lambdap);

    array<double,particles.size()> energies;
    array<array<double,particles.size()>,particles.size()> magneticEnergies;
    array<array<double,particles.size()>,particles.size()> stericEnergies;



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


        for(int j = 0; j < particles.size(); j++)
        {
            ptrParticle movedParticle = pf.moveParticle(*particles[j]);
            if(isOverlap(particles,tmpParticle,j)) break;
            array<double,particles.size()> movedMagneticEnergyArray;
            array<double,particles.size()> movedStericEnergyArray;
            for(int k=0; k < particles.size(); k++)
            {
                if(j == k)
                {
                    movedMagneticEnergyArray[j] = energyCalc.calcMagneticEnergyBetweenMagneticField(*movedParticle,xi);
                    movedStericEnergyArray[j] = 0;
                    continue;
                }

                movedMagneticEnergyArray[k] = energyCalc.calcMagneticEnergyBetweenOtherParticle(*movedParticle,*particles[k],lambdap);
                movedStericEnergyArray[k] = energyCalc.calcStericEnergy2(*movedParticle,*particles[k]);

            }
            double movedEnergy = std::accumulate(movedMagneticEnergyArray.begin(),movedMagneticEnergyArray.end(), 0) + std::accumulate(movedStericEnergyArray.begin(),movedStericEnergyArray.end(),0);
            
            double diffEnergy = movedEnergy - energies[j];
            double probab = exp(-diffEnergy);
            double randprob = randomprobgenerator(mt);
            if (probab < randprob) break;
            //koushin
            for(int k=0; k< particles.size(); ++k) energies[k] += 
            magneticEnergies[j] = movedMagneticEnergyArray;
            for(int k = 0; k < magneticEnergies.size(); ++k) magneticEnergies[k][j] = movedMagneticEnergyArray[k];
            stericEnergies[j] = movedStericEnergyArray;
            for(int k = 0; k < stericEnergies.size(); ++k) stericEnergies[k][j] = movedStericEnergyArray[k];



        }


        auto tmpparticles = pf->moveParticles(particles);
        Energy tmpenergy(tmpparticles,0,magnetCutoff,box_size,xi,lambdap);

        if(energy.getEnergy() > tmpenergy.getEnergy())
        {
            energy = tmpenergy;
            particles = pf->copyParticles(tmpparticles);
        } else 
        {
            double probab = exp(-(tmpenergy.getEnergy() - energy.getEnergy()));
            double randprob = randomprobgenerator(mt);
            if(probab > randprob)
            {
                energy = tmpenergy;
                particles = pf->copyParticles(tmpparticles);
            }
        }
        //op->outputParticles(filename,particles);
        //op -> outputEnergy(filenameee,energyaa);



    }
    delete pf;
    delete op;
    return 0;
}
