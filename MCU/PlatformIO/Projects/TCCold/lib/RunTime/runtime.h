#ifndef RunTime_H
#define RunTime_H

class Time{
    public:
        Time();           //      constructor
        float Update();    //     calculate time change and return dt
        float dt();       // [s]  return time change

    private:
        float dt1;       // [s]  time change
        float dt0;       // [s]  old time change
        float start;     // [ms] time when a new cycle starts
        // bool changed;   //      true when it got a valid update
};

#endif