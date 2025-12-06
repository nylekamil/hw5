#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<size_t>& shifts,
    size_t day,
    size_t pos
);
// Add your implementation of schedule() and other helper functions here  

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    size_t days = avail.size(); 
    size_t workers = avail[0].size(); 

    if (dailyNeed == 0 || workers == 0) {
        return false; 
    }

    //intiliaze sched 
    sched.assign(days, std::vector<Worker_T>(dailyNeed, INVALID_ID)); 

    //store the amount of shifts each worker has worked 
    std::vector<size_t> shiftsUsed(workers, 0);

    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsUsed, 0, 0); 

}

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<size_t>& shifts,
    size_t day,
    size_t pos
)
{
    size_t days = avail.size(); 
    size_t workers = avail[0].size();

    //base case --> check to see if all days are filled 
    if (day == days) {
        return true;
    }
    
    //assign worker to day 
    for (Worker_T w = 0; w < workers; ++w) {
        //check to see if worker is free 
        if (!avail[day][w]) {
            continue; 
        }
        //check to see if worker is under the max shifts 
        if (shifts[w] >= maxShifts) {
            continue; 
        }
        //check to see if already scheduled
        if (std::find(sched[day].begin(), sched[day].end(), w) != sched[day].end()) {
            continue;
        }
        //find worker 
        sched[day][pos] = w; 
        shifts[w]++; 

        //now for next 
        size_t nextDay = day; 
        size_t nextPos = pos + 1; 
        if(nextPos == dailyNeed) {
            nextPos = 0;
            nextDay++; 
        }
        
        //for the rest 
        if (scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts, nextDay, nextPos)) {
            return true; 
        }

        //now to backtrack 
        shifts[w]--;
        sched[day][pos] = INVALID_ID; 
    }
    return false; 

}

