#include "GameObservers.h"
#include "GameEngine.h"
#include <iostream>

namespace WZ
{
	void PhaseObserver::update() {
		currentphase = GameManager::getCurrentPhase();
		p = GameManager::getCurrentPlayer();
		PrintPhaseAndPlayer();
	}

	void PhaseObserver::PrintPhaseAndPlayer() {
		std::cout << "Current Phase: " << currentphase << "\n";
		std::cout << "Current Player: " << *p << "\n";

	}


	void StatisticsObserver::update() {
		//gathering info from other classes to build a table
		//first build the string table
		//call the function DrawTable, assign the received string to the one that is constructor (private), print it, stdcout = String

	}
}