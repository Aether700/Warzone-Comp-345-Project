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
#include <iostream>
#include "Player.h"

namespace WZ {
    
    enum class GamePhase {
        Reinforcement, IssuingOrders, OrderExecution
    };

    class Observer {

    public:
        //default virtual observer destructor to avoid memory leaks
        virtual ~Observer() { }

        //Update the state of this observer
        virtual void update() = 0;
    };

    /**
     * Interface for the Subject
     * override this methods in classes that need observers
     */


    template <typename T>
    class Subject {

    public:
        /*  Temporary for MainGameLoopDriver
        */
        size_t getCount() const {
            return m_observers.size();
        }

        /* default constructor
        */
        Subject() { }

        Subject(const Subject<T>& other)
        {
            m_observers.reserve(other.m_observers.size());
            for (T* t : other.m_observers)
            {
                m_observers.push_back(new T(t));
            }
        }

        //virtual destructor of the Subject class
        virtual ~Subject()
        {
            for (T* obs : m_observers)
            {
                delete obs;
            }
        }

        /**
         * Unregister an observer
         * observer the observer object to be unregistered
         */
        void  removeObserver(T* o) {
		    for (int i = 0; i < m_observers.size(); i++)
		    { 
			    if (o == m_observers[i])
			    {
				    m_observers.erase(m_observers.begin() + i);
				    break;
			    }
		}
	}

        /**
         * Notify all the registered observers when a change happens
         */
        void notifyObservers() const {
		    for (int i = 0; i < m_observers.size(); ++i) {
			    m_observers[i]->update();
		    }
	    }

        //Adding Observer Object to the list
        void AddObserver(T* o) { m_observers.push_back(o); }

        //assignment operator
        Subject<T>& operator=(const Subject<T>& other)
        {
            for (T* t : m_observers)
            {
                delete t;
            }

            for (T* t : other.m_observers)
            {
                m_observers.push_back(new T(t));
            }

            return *this;
        }

    private:
        //List of Observer Object pointers
        std::vector<T*> m_observers;
    };

    //stream insertion operator for the Subject class
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, const Subject<T>& obs)
    {
        stream << "Subject";
        return stream;
    }


    class PhaseObserver :public Observer {
    public:
        /* default constructor
        */
        PhaseObserver();

        /* copy constructor
        */
        PhaseObserver(const PhaseObserver& other);

        //update function of the PhaseObserver type. 
        //Prints information relating to the phases as required by the assignment guidelines
        void update();

        //assignment operator
        PhaseObserver& operator=(const PhaseObserver& other);

    };

    //stream insertion operator for the PhaseObserver class
    std::ostream& operator<<(std::ostream& stream, const PhaseObserver& obs);

    class StatisticsObserver :public Observer {
    public:

        //default constructor
        StatisticsObserver();

        //copy constructor
        StatisticsObserver(const StatisticsObserver& other);

        //update function of the StatisticsObserver type. 
        //Prints information relating to the territories owned as required by the assignment guidelines
        void update();

        //assignement operator
        StatisticsObserver& operator=(const StatisticsObserver& other);
    };

    //stream insertion operator for the StatisticsObserver class
    std::ostream& operator<<(std::ostream& stream, const StatisticsObserver& obs);

}

#endif //OBSERVER_PATTERN_OBSERVER_HPP