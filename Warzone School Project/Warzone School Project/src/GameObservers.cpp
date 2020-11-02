#include "GameObservers.h"
#include "GameEngine.h"
#include <iostream>

#define NUM_COL 2

namespace WZ
{

	static float CalculatePercentage(const Player* p, const Map* a){
		float total_number_of_territories = (float) a->getTerritoryCount();
		float number_of_territories_owned_by_player = (float) p->getNumOfTerritories();
		return (number_of_territories_owned_by_player*100.0f)/total_number_of_territories;
	}

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
		const Map* map = GameManager::getMap();
		const std::vector <Player*> ActivePlayers = GameManager::getActivePlayers();
		size_t height = ActivePlayers.size()+1;
		//first build the string table
		std::string* DataTable = new std::string[NUM_COL*height];
		DataTable[0]= "Player";
		DataTable[1]= "Amount conquered (%)";

		for (int i = 1; i<=ActivePlayers.size(); ++i){
			const Player* current = ActivePlayers[i-1];
			DataTable[0+ NUM_COL *i]= current->getPlayerName();
			DataTable[1+ NUM_COL *i]= std::to_string(CalculatePercentage(current, map));
		}

		TableStat=DrawTable(DataTable, NUM_COL, height);
		delete[] DataTable;
		std::cout<<TableStat<<std::endl;
	}

}