#ifndef SOURCE
#define SOURCE

// Project includes
#include "Shape.h"
#include "RadioNuclide.h"

class Source {
    public:
        // Cosntructor
        Source(Shape *, RadioNuclide *);

        // Destructor
        ~Source();

        // Copy contructor
        Source(const Source&);

        // Assignment operator
        Shape& operator=(const Source&);

        // Methods
        int decayedNuclei(double, double);
        double* samplePosition();
        double** sampleAngles();

    private:
        Shape* shape_;
        RadioNuclide* radioNuclide_;
};

#endif