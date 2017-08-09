#include <sys/stat.h>
#include <sys/types.h>
#include<iostream>
#include<string>
#include <iomanip> //時間を取得するため
#include <sstream> //値を簡単に文字列にするため
#include<math.h>
#include <memory>
#include <random>
#include <vector>
#include <numeric>
#include <chrono>
#include <stdio.h>
#include <fstream>
#include "particle.hpp"
#include "particleFactory.hpp"
#include "energy.hpp"
#include "output.hpp"

using namespace std;
using namespace Eigen;
using ptrParticle = unique_ptr<Particle>;
using Particles = vector<ptrParticle>;

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

double minimumPowDistance(Eigen::Vector2d const & p1, Eigen::Vector2d const & p2, double box_size)
{
    double xdiff = p1.x() - p2.x();
    double ydiff = p1.y() - p2.y();
    if (fabs(xdiff) > box_size / 2.0)
    {
        if (xdiff > 0) xdiff -= box_size;
        else xdiff += box_size;
    }

    if (fabs(ydiff) > box_size / 2.0)
    {
        if (ydiff > 0) ydiff -= box_size;
        else ydiff += box_size;
    }
    return xdiff * xdiff + ydiff * ydiff;
}

bool isOverlap(Particles const & particles, Particle const & particle, int const & index, double box_size)
{
    for( int i = 0; i < particles.size(); ++i )
    {
        if (i==index) continue;
        if( minimumPowDistance(particles[i] -> position, particle.position, box_size) < 1.0 ) return true;
    }
    return false;
}


void input(int& numberOfParticle, double& box_size, double& delr, double& deltheta, double& magnetCutoff, double& stericEnergyCutOff, double& xi, double& lambdap, int& step, int& outputstep)
{
    ifstream ifs("input.data");
    string str;
    char tmp[30];
    
    getline(ifs,str);
    sscanf(str.data(),"%s %d",tmp,&numberOfParticle);
    cout << "numberOfParticle:" << numberOfParticle << endl;

    getline(ifs,str);
    sscanf(str.data(),"%s %lf",tmp,&box_size);
    cout << "box_size:" << box_size << endl;

    getline(ifs,str);
    sscanf(str.data(),"%s %lf",tmp,&delr);
    cout << "delr:" <<delr << endl;

    getline(ifs,str);
    sscanf(str.data(),"%s %lf",tmp,&deltheta);
    cout << "deltheta:" <<deltheta << endl;

    getline(ifs,str);
    sscanf(str.data(),"%s %lf",tmp,&magnetCutoff);
    cout << "magneticEnergyCutOff:" <<magnetCutoff << endl;

    getline(ifs,str);
    sscanf(str.data(),"%s %lf",tmp,&stericEnergyCutOff);
    cout << tmp <<":"<<stericEnergyCutOff<< endl;

    getline(ifs,str);
    sscanf(str.data(),"%s %lf",tmp,&xi);
    cout << tmp <<":"<<xi << endl;

    getline(ifs,str);
    sscanf(str.data(),"%s %lf",tmp,&lambdap);
    cout << tmp <<":"<< lambdap << endl;

    getline(ifs,str);
    sscanf(str.data(),"%s %d",tmp,&step);
    cout << tmp <<":"<< step << endl;

    getline(ifs,str);
    sscanf(str.data(),"%s %d",tmp,&outputstep);
    cout << tmp <<":"<< outputstep << endl;

    cout << endl << endl;
}



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

    int numberOfParticle,step,outputstep; 
    double box_size ,delr, deltheta , magnetCutoff, stericEnergyCutOff,xi, lambdap; 

    input(numberOfParticle,box_size,delr,deltheta,magnetCutoff,stericEnergyCutOff,xi,lambdap,step,outputstep);

    double neighborDistance = magnetCutoff + 5;
    int neighborUpdateFreq = 5;

    ParticleFactory *pf = new ParticleFactory(box_size,delr,deltheta);


    auto particles = pf->initializeParticles(numberOfParticle);



    Energy energyCalc(stericEnergyCutOff,magnetCutoff,box_size,xi,lambdap);


    vector<double> energies(numberOfParticle);
    MatrixXd magneticEnergies = MatrixXd::Zero(numberOfParticle,numberOfParticle);
    MatrixXd stericEnergies = MatrixXd::Zero(numberOfParticle,numberOfParticle);

    //saisyoni energies toka syokika
    for( int i = 0; i < numberOfParticle; ++i)
    {
        energies[i] = 0.0;

        for (int j = 0; j < numberOfParticle ; ++j)
        {
            if (i==j)
            {
                magneticEnergies(i,i) = energyCalc.calcMagneticEnergyBetweenMagneticField(*(particles[i]));
                stericEnergies(i,i) = 0;
                energies[i] += magneticEnergies(i,i);
                continue;
            }
            magneticEnergies(i,j) = energyCalc.calcMagneticEnergyBetweenOtherParticle(*(particles[i]),*(particles[j]));
            stericEnergies(i,j) = energyCalc.calcStericEnergy(*(particles[i]),*(particles[j]));

            energies[i] += magneticEnergies(i,j);
            energies[i] += stericEnergies(i,j);
        }
    }




    string particlefilename = directoryname + "/particle" + to_string(0) +   ".csv";
    string energyfilename = directoryname + "/energy" +   ".csv";
    op->outputParticles(particlefilename,particles);
    op->outputEnergyInitialize(energyfilename);
    op->outputEnergy(energyfilename,0, energies, magneticEnergies, stericEnergies); 


    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> randomprobgenerator(0.0,1.0);
    cout << "ok" << endl;




    clock_t start,end;
    double overlapt,energyt;

    for(int i=1;i<=step;i++)
    {

        if( i% outputstep == 0)
        {
            cout << i << endl;
            particlefilename = directoryname + "/particle" + to_string(i)+".csv";
            op->outputParticles(particlefilename,particles);
            op->outputEnergy(energyfilename, i , energies, magneticEnergies, stericEnergies); 
        }

        // if (i % 100  == 0)
        // {
        //     cout << i << endl;
        //     cout << "overlapt: " << overlapt << endl;
        //     cout << "energyt:  " << energyt << endl;
        //     overlapt = 0;
        //     energyt = 0;
        //
        // }


        for(int j = 0; j < numberOfParticle; j++)
        {
            vector<double> movedMagneticEnergyArray(numberOfParticle);
            vector<double> movedStericEnergyArray(numberOfParticle);
            vector<double> koushinEnergyArray(numberOfParticle);

            ptrParticle movedParticle = pf->moveParticle(*(particles[j]));





            if(isOverlap(particles,*movedParticle,j, box_size)) continue;


            
            energyCalc.calcEnergies(movedMagneticEnergyArray,movedStericEnergyArray , *movedParticle , particles , j );


            
            double movedEnergy = 0.0;
            for(int k = 0; k < numberOfParticle; ++k)
            {
                movedEnergy += movedStericEnergyArray[k] + movedMagneticEnergyArray[k];
            }
            
            double diffEnergy = movedEnergy - energies[j];

            if (diffEnergy > 0) 
            {
                double probab = exp(-diffEnergy);
                double randprob = randomprobgenerator(mt);
                if (probab < randprob) continue;
            }

            //koushin
            particles[j] = move(movedParticle);

            for(int k=0; k< numberOfParticle; ++k)
            {
                koushinEnergyArray[k] = movedMagneticEnergyArray[k]-magneticEnergies(j,k); //k,jkannnoyatu
                koushinEnergyArray[k] += movedStericEnergyArray[k] - stericEnergies(j,k);
            }

            //energies
            for ( int k = 0; k < numberOfParticle ; ++k )
            {
                if(j==k)energies[k] += diffEnergy;  
                else energies[k] += koushinEnergyArray[k];


                magneticEnergies(j,k) =  movedMagneticEnergyArray[k];
                magneticEnergies(k,j) =  movedMagneticEnergyArray[k];

                stericEnergies(j,k) =  movedStericEnergyArray[k];
                stericEnergies(k,j) =  movedStericEnergyArray[k];
            }


        }

    }
    delete pf;
    delete op;
    return 0;
}

