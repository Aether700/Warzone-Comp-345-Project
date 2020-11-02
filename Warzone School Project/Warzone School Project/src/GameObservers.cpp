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
		//switch tring based on phase 
		string phase_status;
		switch(currentphase){
			case GamePhase::Reinforcement:
			phase_status = "Reinforcement"; 
			break;
			case GamePhase::IssuingOrders:
			phase_status = "IssuingOrders"; 
			break;
			case GamePhase::OrderExecution:
			phase_status = "OrderExecution"; 
			break;

		}

		std::cout << "Current Phase: " << phase_status << "\n";
		std::cout << "Current Player: " << *p << "\n";
		

	}


	void StatisticsObserver::update() {
		//gathering info from other classes to build a table
		//first build the string table
		//call the function DrawTable, assign the received string to the one that is constructor (private), print it, stdcout = String
		DrawTable(std::string* table, size_t width, size_t height)

	}
}