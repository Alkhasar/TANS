// STD includes
#include <math.h>
#include <string>

// External includes
#include "../../libs/loguru/loguru.hpp"

// Project includes
#include "../../headers/detector/Detector.h"
#include "../../src/utils/utilities.cpp"
#include "../../headers/utils/FileWriter.h"
/**
 * @brief Construct a new Detector starting by a base detector centered in (r, 0, 0) with width w
 * and heigth h, and rotates it along the z azis by an angle omega
 * 
 * @param width detector width, direction along z
 * @param heigth detector heigth, direction along y
 * @param omega rotation angle centered on z axis
 * @param distance distance of detector center to origin
 */
Detector::Detector(double width, double heigth, double phi, double distance){
    // Changing log scope
    LOG_SCOPE_F(INFO,"DETECTOR");

    // Logging construction
    LOG_F(INFO, "Constructing at: %p", (void*) this);
    
    width_ = width; // Dimension along z
    LOG_F(INFO, "Width: %lfm", width_);

    heigth_ = heigth; // Dimension along y
    LOG_F(INFO, "Heigth: %lfm", heigth_);

    phi_ = phi;
    LOG_F(INFO, "Phi: %lfrad", phi_);

    deltaPhi_ = atan(heigth_/(2*distance_)); // Half aperture angle from center in y direction
    LOG_F(INFO, "DeltaPhi: %lfrad", deltaPhi_);

    distance_ = distance;
    LOG_F(INFO, "Distance from origin: %lfm", distance_);

    // Declaring vertex array
    double** vertex = new double*[4];

    // Assigning vertes with rotations
    vertex[0] = zAxisRotation(new double[3]{distance_, heigth_/2, width_/2}, phi_);
    vertex[1] = zAxisRotation(new double[3]{distance_, heigth_/2, -width_/2}, phi_);
    vertex[2] = zAxisRotation(new double[3]{distance_, -heigth_/2, -width_/2}, phi_);   
    vertex[3] = zAxisRotation(new double[3]{distance_, -heigth_/2, width_/2}, phi_);

    // Generating detector planar surface
    plane_ = new Plane(vertex);
}

/**
 * @brief Destroy the Detector and clear dynamic memory
 * 
 */
Detector::~Detector(){
    LOG_F(INFO, "Destroying Detector %p", (void*) this);
    delete plane_;
}


/**
 * @brief Detector copy constructor
 * 
 * @param src The source detector to copy
 */
Detector::Detector(const Detector& src):
    width_(src.width_),
    heigth_(src.heigth_),
    phi_(src.phi_),
    deltaPhi_(src.deltaPhi_),
    distance_(src.distance_)
{   
    
    LOG_F(INFO, "Copying detector %p into %p", (void*) &src, (void*) this);
    
    // Copying plane
    plane_ = new Plane(*src.plane_);
}

/**
 * @brief Adding a hit to buffer
 * 
 * @param data to add
 */
void Detector::addData(Data& data){
    data_.push_back(data);
}

/**
 * @brief Checks if the detector is paralysed by last signal
 * 
 * @param time in nanosecond
 * @return true if deltaTime > 2ns
 * @return false if deltaTime < 2ns
 */
bool Detector::paralysed(double time){
    if(time - lastHit_ > timeResolution_){
        lastHit_ = time;
        return false;
    }
     
    return true;
}


/**
 * @brief Checks if a photon emitted from P with angle omega, theta interacts
 * with this detector
 * 
 * @param P photon origin
 * @param angles photon (omega, theta)
 * @return interaction time
 */
double Detector::checkInteraction(double* P, double* angles){  

    static FileWriter& fileWriter= FileWriter::getInstance();

    // Locally referencing photon angles
    double phi      = angles[0];
    double theta    = angles[1];

    // Locally referencing photon emission point
    double xGamma   = P[0];
    double yGamma   = P[1];
    double zGamma   = P[2];

    // Calculating NORMALISED line director
    double l        = sin(theta)*cos(phi);
    double m        = sin(theta)*sin(phi);
    double n        = cos(theta);
    
    // Calculating intersection parameter
    // double dn = plane_->a_*(l + xGamma) + plane_->b_*(m + yGamma) + plane_->c_*(n + zGamma);
    double dn = plane_->a_*l + plane_->b_*m  + plane_->c_*n;
    double t = -(plane_->d_ + plane_->a_*xGamma + plane_->b_*yGamma + plane_->c_*zGamma)/dn;
    
    // Intersection Point
    double x = xGamma + l * t;
    double y = yGamma + m * t;
    double z = zGamma + n * t;

    LOG_IF_F(ERROR, x == INFINITY || y == INFINITY || z == INFINITY, "Something went to infinity : Detector.cpp");
    LOG_IF_F(ERROR, x == NAN || y == NAN || z == NAN, "Something is not a number : Detector.cpp");

    #if DEBUG_HIT == 1
        LOG_F(ERROR, "______________________________________________"); 
        LOG_F(WARNING, "l=%f m=%f n=%f", l, m, n);
        LOG_F(ERROR, "a=%f b=%f c=%f d=%f", plane_->a_, plane_->b_, plane_->c_, plane_->d_);
        LOG_F(INFO, "dn=%f", dn);
        LOG_F(WARNING, "xGamma=%f l=%f t=%f", xGamma, l, t);    
        LOG_F(WARNING, "yGamma=%f m=%f t=%f", yGamma, m, t);    
        LOG_F(WARNING, "zGamma=%f n=%f t=%f", zGamma, n, t);
        LOG_F(ERROR, "x=%f y=%f z=%f", x, y, z);
    #endif
    

    if(plane_->isInside(x, y, z)){
        // Calculating dot product parts
        double adotb = plane_->a_*l + plane_->b_*m + plane_->c_*n;
        double magA = sqrt(plane_->a_*plane_->a_+plane_->b_*plane_->b_+plane_->c_*plane_->c_);
        double magB = sqrt(l*l+m*m+n*n);

        #if DEBUG_HIT == 1
            LOG_F(INFO, "cos(theta)=%f", adotb/(magA*magB));
        #endif


        //LOG_F(ERROR, "cost(t): %.20f",A);
        if(adotb/(magA*magB) >= 0){

            // Distance between point and detector
            double distance = std::sqrt((xGamma-x)*(xGamma-x) + (yGamma-y)*(yGamma-y) + (zGamma-z)*(zGamma-z));

            // Calculating time taken in ns
            double time = distance / 0.3;

            // Checking if this detector is paralysed

            #if DEBUG_POINTS == 1
                // Saving some debug data
                std::string data = to_string(xGamma) + "," + to_string(yGamma) + "," + to_string(zGamma) + "," + to_string(x) + "," + to_string(y) + "," + to_string(z);
                fileWriter.writeData(3, data);
            #elif DEBUG_POINTS == 2
                // Saving some debug data
                std::string data = to_string(xGamma) + " " + to_string(yGamma) + " " + to_string(zGamma) + "\n" + to_string(x) + " " + to_string(y) + " " + to_string(z);
                fileWriter.writeData(3, data);
            #endif

            // Returning data
            return time; 
        }
    }
    // Returning 0
    return 0;
}   