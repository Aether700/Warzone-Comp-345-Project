/**
 * Observer Design Pattern
 *
 * Intent: Lets you define a subscription mechanism to notify multiple objects
 * about any events that happen to the object they're observing.
 *
 */

#ifndef OBSERVER_PATTERN_OBSERVER_HPP
#define OBSERVER_PATTERN_OBSERVER_HPP


namespace WZ{

/**
 * Interface for the Observer
 */
class Observer {

public:

    //Update the state of this observer
     
    virtual void update() = 0;

};


#include "Observer.h"

/**
 * Interface for the Subject
 * override this methods in classes that need observers
 */
class Subject {

public:

    /**
     * Register an observer
     * observer the observer object to be registered
     * observer pointer as an orgument
     */
    virtual void registerObserver();

    /**
     * Unregister an observer
     * observer the observer object to be unregistered
     */
    virtual void removeObserver();

    /**
     * Notify all the registered observers when a change happens
     */
    virtual void notifyObservers();

private:
    //List of Observer Object pointers
    std::vector<Observer*> m_observers;
};




}

#endif //OBSERVER_PATTERN_OBSERVER_HPP