#ifndef TIME
#define TIME

class Time{
    public:
        // Singleton Pattern
        static Time& getTime();
        bool update();
        double getSimulationTime();
        double getTimeStep();
        double getMaxSimualationTime();
        
    private:
        // Singleton Pattern
        Time(); // in thisway we can't create more than one FileWriter
        Time(Time const&); // Do not implent in singleton
        void operator=(Time const&); // Do not implent in singleton
        
        // Time and steps
        double simulationTime = 0;
        double simulationStep = 0;
        double maxSimulationTime = 0;

};

#endif