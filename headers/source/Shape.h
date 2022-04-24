#ifndef SHAPE
#define SHAPE

class Shape {
    public:
        
        // Shape's counstructor
        Shape(double, double, double);

        // Virtual Destructor
        virtual ~Shape();
        
        // Virtual clone
        virtual Shape* clone() = 0;

        // Checks if a point is inside our shape
        virtual bool isInside(double, double, double) = 0;

        // Samples a point
        virtual double* sample() = 0;

        // Copy constructor
        Shape(const Shape&);


    protected:
        // Default constructor private to avoid unwanted initialization
        Shape();

        // Shape's position
        double x_ = 0;
        double y_ = 0;
        double z_ = 0;
};

#endif