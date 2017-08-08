#include <memory>
#include <array>
#include <Eigen/Core>
#include <Eigen/Geometry>

class Particle;

using namespace std;
using namespace Eigen;
using Particles = unique_ptr<Particle>;

SystemOfParticle
{
    private:
        //a[i][j]: distance between i,j
        array<array<double>> distanceMatrix;

        //a[i][j] me between i,j a[i][i] :me due to mf
        array<array<double>> magneticEnergyMatrix;

        array<array<double>> stericEnergyMatrix;
        //
        //a[i]: sum about i
        array<double> magneticEnergiesAboutParticle;
        array<double> stericEnergiesAboutParticle;

        double numberOfParticle = 64;
        double box_size = 64;
        double delr = 0.5;
        double deltheta = 5;
        double stericCut

    public:
        SystemOfParticle( double numberOfParticle,double box_size,double delr,double deltheta,double stericCutoff,double magnetCutoff,double xi,double parameterLambda);
        
    ParticleFactory *pf = new ParticleFactory(box_size,delr,deltheta);
