#ifndef PLANE
#define PLANE

class Plane {
    public:
        // Constructor
        Plane(double**);

        // Destructor
        ~Plane();

        // Copy constructor
        Plane(const Plane&);

        // Checks if point lies inside boundaries
        bool isInside(double, double, double); 

        // Plane parameter
        double a_ = 0, b_ = 0, c_ = 0, d_ = 0;
        double* normal_;
        double** vertex_; 
        
    private:
        double xMax_ = 0, yMax_ = 0, zMax_ = 0;
        double xMin_ = 0, yMin_ = 0, zMin_ = 0;
};

#endif