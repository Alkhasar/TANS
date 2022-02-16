#ifndef SHAPE
#define SHAPE

class Shape {
    public:

        // Constructor
        // Shape(bool (*fx)(double, double, double));

        virtual bool isInside(double, double, double) = 0;
        virtual double * sample() = 0;
        void print();

        // Getters
        double getX();
        double getY();
        double getZ();

        // Setters
        void setX(double);
        void setY(double);
        void setZ(double);

    private:

        double X = 0;
        double Y = 0;
        double Z = 0;

        //bool (*_fx)(double, double, double);


};

#endif