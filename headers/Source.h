#ifndef SOURCE
#define SOURCE

#include "Shape.h"
#include "RadioNuclide.h"

class Source {
    public:
        Source(Shape *shape, RadioNuclide *radioNuclide);
        void update(double dt); // Simulation step
        
        // Getters
        Shape * getShape();
        RadioNuclide * getMaterial();

    private:
        Shape* shape;
        RadioNuclide* radioNuclide;
        double x, y, z;
        bool compton;
};

#endif