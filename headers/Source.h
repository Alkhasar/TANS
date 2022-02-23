#ifndef SOURCE
#define SOURCE

#include "Shape.h"
#include "RadioNuclide.h"
#include "../headers/Detector.h"

class Source {
    public:
        // Constructor & Destructor
        Source(Shape *, RadioNuclide *);
        Source(const Source&);
        ~Source();

        // Assignment operator
        Shape& operator=(const Source&);



        void update(double, int, Detector**); // Simulation step
        
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