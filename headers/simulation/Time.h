#ifndef TIME
#define TIME

class Time {
    public:
        // Singleton Pattern
        static Time& getInstance(double, double, double);

        // Checks if simulation should continue and updates it
        bool update();
        
        // Getters
        double getSimulationTime(){return simulationTime_;};
        double getSimulationStep(){return simulationStep_;};

        // Singleton Pattern
        Time() = delete; // Removing default constructor
        Time(Time const&) = delete; // Do not implent in singleton
        void operator=(Time const&) = delete; // Do not implent in singleton

    private:

        Time(double, double, double); // in this way we can't create more than one FileWriter

        // Current Simulation Time
        double simulationTime_ = 0.0;

        // Maximum simulation time
        double maxSimulationTime_;

        // Constant parameters
        double simulationStep_;

};      

#endif