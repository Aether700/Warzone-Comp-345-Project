#include "GameObservers.h"
#include "GameEngine.h"
#include <iostream>

namespace WZ
{

	void Subject::AddObserver(Observer* o) { m_observers.push_back(o); }

	void Subject::removeObserver(Observer* o) {
		for (int i = 0; i < m_observers.size(); i++)
		{
			if (o == m_observers[i])
			{
				m_observers.erase(m_observers.begin() + i);
				break;
			}
		}
	}

	void Subject::notifyObservers() {
		for (int i = 0; i < m_observers.size(); ++i) {
			m_observers[i]->update();
		}
	}


	void PhaseObserver::update() {
		currentphase = GameManager::getCurrentPhase();
		p = GameManager::getCurrentPlayer();
	}

	void PhaseObserver::PrintPhaseAndPlayer() {
		std::cout << "Current Phase: " << currentphase << "\n";
		std::cout << "Current Player: " << *p << "\n";

	}


	void StatisticsObserver::update() {

	}
}