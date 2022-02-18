#ifndef SOURCE
#define SOURCE

#include "Shape.h"
#include "RadioNuclide.h"

class Source {
    public:
        Source(Shape *, RadioNuclide *);
        void update(double); // Simulation step
        
        // Getters
        Shape * getShape();
        RadioNuclide * getRadioNuclide();

    private:
        Shape* shape;
        RadioNuclide* radioNuclide;
        double x, y, z;
        bool compton;
};

#endif