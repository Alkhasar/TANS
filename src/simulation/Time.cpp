// External Libs
#include "../../libs/loguru/loguru.hpp"

// Project includes
#include "../../headers/simulation/Time.h"

/**
 * @brief Returns a static time instance -> created only once
 * 
 * @return the time instance address
 */
Time& Time::getInstance(double t1 = 0.0, double t2 = 0.0, double t3 = 0.0){
    // Guarantees Destruction
    static Time instance{t1, t2, t3};
    return instance;
}

/**
 * @brief Construct the Time singleton to keep track of time
 * across the program
 * 
 * @param t1 pointer to the simulation starting time
 * @param t2 pointer to the simulation step
 * @param t3 pointer to the maximum simulation Time
 */
Time::Time(double t1, double t2,  double t3) {
    
    LOG_SCOPE_F(INFO, "TIME");
    LOG_F(WARNING, "Time Instantiated, should appear only once!");
    LOG_F(INFO, "Constructing at %p", (void*) this);

    // Initialising time values
    simulationTime_     = t1;
    LOG_F(INFO, "Simulation time: %lfs", simulationTime_);

    simulationStep_     = t2;
    LOG_F(INFO, "Simulation step: %lfs", simulationStep_);

    maxSimulationTime_  = t3;
    LOG_F(INFO, "Maximum Simulation duration: %lfs", maxSimulationTime_);

    // EVENTUAL SOLUTION: https://stackoverflow.com/questions/21395459/how-to-pass-argument-in-a-singleton
    if ((t1 == 0.0 && t2 == 0.0 & t3 == 0.0) || (t1 < 0.0 || t2 <= 0.0 || t3 < t2 || t3 < t1))
        throw std::runtime_error{ "Time not inizialised correctly!" };
}

/**
 * @brief updates simulationTime by adding time step and checking if we reached
 * maxSimulationTime
 * 
 * @return true if the simulation should keep running
 * @return false if we reached max simulation time
 */
bool Time::update(){
    simulationTime_ += simulationStep_;
    return (simulationTime_ <= maxSimulationTime_);
}


/*
                        /\
                        ||
                        ||
                        ||
                        ||                                               ~-----~
                        ||                                            /===--  ---~~~
                        ||                   ;'                 /==~- --   -    ---~~~
                        ||                (/ ('              /=----         ~~_  --(  '
                        ||             ' / ;'             /=----               \__~
     '                ~==_=~          '('             ~-~~      ~~~~        ~~~--\~'
     \\                (c_\_        .i.             /~--    ~~~--   -~     (     '
      `\               (}| /       / : \           / ~~------~     ~~\   (
      \ '               ||/ \      |===|          /~/             ~~~ \ \(
      ``~\              ~~\  )~.~_ >._.< _~-~     |`_          ~~-~     )\
       '-~                 {  /  ) \___/ (   \   |` ` _       ~~         '
       \ -~\                -<__/  -   -  L~ -;   \\    \ _ _/
       `` ~~=\                  {    :    }\ ,\    ||   _ :(
        \  ~~=\__                \ _/ \_ /  )  } _//   ( `|'
        ``    , ~\--~=\           \     /  / _/ / '    (   '
         \`    } ~ ~~ -~=\   _~_  / \ / \ )^ ( // :_  / '
         |    ,          _~-'   '~~__-_  / - |/     \ (
          \  ,_--_     _/              \_'---', -~ .   \
           )/      /\ / /\   ,~,         \__ _}     \_  "~_
           ,      { ( _ )'} ~ - \_    ~\  (-:-)       "\   ~ 
                  /'' ''  )~ \~_ ~\   )->  \ :|    _,       " 
                 (\  _/)''} | \~_ ~  /~(   | :)   /          }
                <``  >;,,/  )= \~__ {{{ '  \ =(  ,   ,       ;
               {o_o }_/     |v  '~__  _    )-v|  "  :       ,"
               {/"\_)       {_/'  \~__ ~\_ \\_} '  {        /~\
               ,/!          '_/    '~__ _-~ \_' :  '      ,"  ~ 
              (''`                  /,'~___~    | /     ,"  \ ~' 
             '/, )                 (-)  '~____~";     ,"     , }
           /,')                    / \         /  ,~-"       '~'
       (  ''/                     / ( '       /  /          '~'
    ~ ~  ,, /) ,                 (/( \)      ( -)          /~'
  (  ~~ )`  ~}                   '  \)'     _/ /           ~'
 { |) /`,--.(  }'                    '     (  /          /~'
(` ~ ( c|~~| `}   )                        '/:\         ,'
 ~ )/``) )) '|),                          (/ | \)                     
  (` (-~(( `~`'  )                        ' (/ '
   `~'    )'`')                              '
     ` ``
*/