// STD includes
#include <math.h>

// External libs
#include "../../../libs/loguru/loguru.hpp"

// Project includes
#include "../../../headers/detector/geometry/Plane.h"

/**
 * @brief Construct a plane given 4 vertex 
 * 
 * @param vertex 
 */
Plane::Plane(double** vertex){
    // Changing log scope
    LOG_SCOPE_F(INFO, "PLANE");

    // Logging construction
    LOG_F(INFO, "Constructing at: %p", (void*) this);
    
    // Locally referencing the plane vertex
    vertex_ = vertex;
    for(int i = 0; i < 4; i++){
        LOG_F(INFO, "Plane Vertex: v%i=(%lf, %lf, %lf)", i, vertex_[i][0], vertex_[i][1], vertex_[i][2]);
    }

    // Getting plane parameters
    a_ = (vertex_[1][1] - vertex_[0][1]) * (vertex_[2][2] - vertex_[0][2]) -( vertex_[1][2] -vertex_[0][2]) * (vertex_[2][1] - vertex_[0][1]);
    b_ = -(vertex_[1][0] - vertex_[0][0]) * (vertex_[2][2]- vertex_[0][2]) + ( vertex_[1][2] - vertex_[0][2]) * (vertex_[2][0] - vertex_[0][0]);
    c_ = (vertex_[1][0] - vertex_[0][0]) * (vertex_[2][1] - vertex_[0][1]) - (vertex_[1][1] - vertex_[0][1]) * (vertex_[2][0] - vertex_[0][0]);
    
    // Calculating fourth parameter
    d_ = -a_*vertex_[0][0] - b_*vertex_[0][1] - c_*vertex_[0][2];

    // Checking that a, b, c != simultanously
    LOG_IF_F(ERROR, (a_ == 0 && b_==0 && c_==0), "Plane %p degenerated: a=%lf, b=%lf, c=%lf", (void*) this, a_, b_, c_);

    // Normal vector
    double len = sqrt(a_*a_+b_*b_+c_*c_);
    normal_ = new double[3]{a_/len, b_/len, c_/len};

    LOG_F(INFO, "Detector Plane parameters: a=%lf b=%lf c=%lf d=%lf)", a_, b_, c_, d_);

    // Finding maximum values
    xMax_ = std::max(std::max(vertex_[0][0], vertex_[1][0]), std::max(vertex_[2][0], vertex_[3][0]));
    yMax_ = std::max(std::max(vertex_[0][1], vertex_[1][1]), std::max(vertex_[2][1], vertex_[3][1]));
    zMax_ = std::max(std::max(vertex_[0][2], vertex_[1][2]), std::max(vertex_[2][2], vertex_[3][2]));
    
    // Finding minimum values
    xMin_ = std::min(std::min(vertex_[0][0], vertex_[1][0]), std::min(vertex_[2][0], vertex_[3][0]));
    yMin_ = std::min(std::min(vertex_[0][1], vertex_[1][1]), std::min(vertex_[2][1], vertex_[3][1]));
    zMin_ = std::min(std::min(vertex_[0][2], vertex_[1][2]), std::min(vertex_[2][2], vertex_[3][2]));
}

/**
 * @brief Destroy the Plane and disposes of vertex_
 * 
 */
Plane::~Plane(){
    LOG_F(INFO, "Destroying Plane %p", (void*) this);
    for(int i = 0; i < 4; i++){
        delete[] vertex_[i];
    }
    delete[] vertex_;
    delete[] normal_;
}

/**
 * @brief Copy constructor
 * 
 * @param src to copy
 */
Plane::Plane(const Plane& src) :
    d_(src.d_)
{      
    LOG_F(INFO, "Copying Plane %p into %p", (void*) &src, (void*) this);

    // Copying the normal vector
    normal_ = new double[3];
    for(int i = 0; i < 3; i++){
        normal_[i] = src.normal_[i];
    }

    // Copying vertex
    vertex_ = new double*[4];
    for(int i = 0; i < 4; i++){
        vertex_[i] = new double[3];
        for(int j = 0; j < 3; j++){
            vertex_[i][j] = src.vertex_[i][j];
        }
    }
}

bool Plane::isInside(double x, double y, double z){
    static double EPSILON = 1e-15;

    // Conditions to check
    bool A = (x <= xMax_ + EPSILON && x >= xMin_ - EPSILON);
    bool B = (y <= yMax_ + EPSILON && y >= yMin_ - EPSILON);
    bool C = (z <= zMax_ + EPSILON && z >= zMin_ - EPSILON);

    #if DEBUG_HIT == 1
        LOG_F(ERROR,"Conditions %i, %i, %i FINAL COND: %i", A, B, C, A&&B&&C );
        LOG_F(WARNING, "XCOND: %i %i %i", x <= xMax_, x >= xMin_);
        LOG_F(WARNING, "YCOND: %i %i %i", y <= yMax_, y >= yMin_);
        LOG_F(WARNING, "ZCOND: %i %i %i", z <= zMax_, z >= zMin_);
        LOG_F(INFO, "MINMAX: %f,%f,%f,%f,%f,%f", xMin_, xMax_, yMin_, yMax_, zMin_, zMax_);
    #endif

    return A && B && C;
}