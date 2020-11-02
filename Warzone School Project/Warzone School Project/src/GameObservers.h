/**
 * Observer Design Pattern
 *
 * Intent: Lets you define a subscription mechanism to notify multiple objects
 * about any events that happen to the object they're observing.
 *
 */

#ifndef OBSERVER_PATTERN_OBSERVER_HPP
#define OBSERVER_PATTERN_OBSERVER_HPP
#include <vector>
#include "Player.h"


namespace WZ {
    
    enum class GamePhase {
        Reinforcement, IssuingOrders, OrderExecution
    };

    class Observer {

    public:

        //Update the state of this observer
        virtual void update() = 0;
    };

    /**
     * Interface for the Subject
     * override this methods in classes that need observers
     */
    class Subject {

    public:

        /**
         * Register an observer
         * observer the observer object to be registered
         *

        /**
         * Unregister an observer
         * observer the observer object to be unregistered
         */
        void removeObserver(Observer* o);

        /**
         * Notify all the registered observers when a change happens
         */
        void notifyObservers();

        //Adding Observer Object to the list
        void AddObserver(Observer* o);

    private:
        //List of Observer Object pointers
        std::vector<Observer*> m_observers;
    };


    class PhaseObserver :public Observer {
    public:
        void update();
    private:
        GamePhase currentphase;
        const Player* p;
        //Private function that is being called by update  Current Phase: Player who is playing
        void PrintPhaseAndPlayer();
    };


    class StatisticsObserver :public Observer {
    public:
        void update();
    private:

    };



}

#endif //OBSERVER_PATTERN_OBSERVER_HPP