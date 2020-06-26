/**
 * @author Spencer Kase Rohlfing
 * @brief This program is a timer to check how long a program is running for.
 * This starts, stops, and gets the time difference between the two.
 * @date 7/27/17
 */

/**
 * @mainpage
 * Timer cpp code fo the functions for the Timer.h file. This class includes
 * a default contstructor, which sets the timer to 0, a start function to 
 * start counting the time, stop function to top counting the time, and a
 * getElapsedTime function to find the different between start and stop time. 
 */

/**
 * @file
 */

#ifndef TIMER_CPP
#define TIMER_CPP

#include "Timer.hpp"
/**
 * Default constructor. Sets the bool to check if the timer has started to 0.
 * Also sets beginTime and duration to the same time.
 * @post Sets up the timer
 */
Timer::Timer(){
	timerWasStarted = 0;
	gettimeofday(&beginTime, NULL);
	gettimeofday(&duration, NULL);
}

/**
 * Checks if the timer was even started. Then calls the gettimeofday funciton
 * and sets beginTime to it.
 * @throw runtime_error If timer has been started, throw error.
 * @post Sets beginTime to gettimeofday
 */
void Timer::start() throw (runtime_error) {
	if(!timerWasStarted){
		gettimeofday(&beginTime, NULL);
	}else{
		throw runtime_error("Error start()");
	}
	timerWasStarted = 1;
}

/**
 * Checks that the timer is running, then calls gettimeofday function
 * to set duration to that time. If the timer hasn't started, it will throw 
 * a an error. After stop is called, sets the timer to not running(false).
 * @throw logic_error If timer hasn't been started, throw error.
 * @post Sets duration to gettimeofday
 */
void Timer::stop() throw (logic_error){
	if(timerWasStarted){
		gettimeofday(&duration, NULL);
	}else{
		throw logic_error("Error stop()");
	}
	timerWasStarted = 0;
}

/**
 * Finds the difference between beginTime and duration. It first does this by
 * adding up the seconds and microseconds together then subtracting
 * duration from beginTime. Then returns a double subtracting the two.
 * @post Finds difference between start and stop.
 * @throw logic_error If the timer hasn't run or if the duration is less then
 * beginTime, throw error.
 * @return ((t1 - t2) / (double)1000000) Returns the difference.
 */
double Timer::getElapsedTime() const throw (logic_error){
    long long t1, t2;
    t1 = duration.tv_sec * 1000000 + duration.tv_usec;
    t2 = beginTime.tv_sec * 1000000 + beginTime.tv_usec;
    if(t1 == t2){
    	throw logic_error("Error getElapsedTime()-");
    }else if(t1 - t2 < 0){
    	throw logic_error("Error getElapsedTime()!");
    }else{
    	return ((t1 - t2) / (double)1000000);
    }
}

#endif	//#ifndef TIMER_CPP
