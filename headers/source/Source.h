#ifndef SOURCE
#define SOURCE

// Project includes
#include "Shape.h"
#include "RadioNuclide.h"

class Source {
    public:
        // Constructor & Destructor
        Source(Shape *, RadioNuclide *);
        Source(const Source&);
        ~Source();

        // Assignment operator
        Shape& operator=(const Source&);

        int timeStepDecays(double);
        double* samplePosition();
        double** sampleAngles();

        // Getters
        Shape * getShape();
        RadioNuclide * getRadioNuclide();

    private:
        Shape* shape;
        RadioNuclide* radioNuclide;
        double x, y, z;
        bool compton;
        double oldEvaluationTime = 0;
};

#endif