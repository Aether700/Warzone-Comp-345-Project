#include "GameObservers.h"
#include "GameEngine.h"
#include "Orders.h"
#include <iostream>

#define NUM_COL 2

namespace WZ
{

	static float CalculatePercentage(const Player* p, const Map* a){
		float total_number_of_territories = (float) a->getTerritoryCount();
		float number_of_territories_owned_by_player = (float) p->getNumOfTerritories();
		return (number_of_territories_owned_by_player*100.0f)/total_number_of_territories;
	}

	PhaseObserver::PhaseObserver() { }

	
	//no field to copy
	PhaseObserver::PhaseObserver(const PhaseObserver& other) { }

	void PhaseObserver::update() {
		const Player* p = GameManager::getCurrentPlayer();
		
		//by default assume Reinforcement Phase
		const char* currentphase = "Reinforcement phase";
		switch(GameManager::getCurrentPhase())
		{
		case GamePhase::IssuingOrders:
			currentphase = "Issuing Orders phase";
			break;

		case GamePhase::OrderExecution:
			currentphase = "Order Execution phase";
			break;
		}
		std::cout << *p << ": " << currentphase << "\n";
		const Order* lastOrder = GameManager::getLastOrder();

		if (lastOrder != nullptr)
		{
			std::cout << *lastOrder << "\n";
		}
	}

	PhaseObserver& PhaseObserver::operator=(const PhaseObserver& other)
	{
		//nothing to copy
		return *this;
	}

	std::ostream& operator<<(std::ostream& stream, const PhaseObserver& obs)
	{
		stream << "Phase Observer";
		return stream;
	}

	StatisticsObserver::StatisticsObserver() { }

	//copy constructor
	StatisticsObserver::StatisticsObserver(const StatisticsObserver& other) 
	{ 
		//nothing to copy 
	}

	void StatisticsObserver::update() {
		//gathering info from other classes to build a table
		const Map* map = GameManager::getMap();
		const std::vector<Player*>& ActivePlayers = GameManager::getActivePlayers();

		size_t number_of_players_check = ActivePlayers.size();
		if (number_of_players_check == 1){
			const Player* current = ActivePlayers[0];
			std::cout<< "Congratulations to the winner! "<< current->getPlayerName()<<std::endl;
		}
		else
		{
			size_t height = ActivePlayers.size() + 1;
			//first build the string table
			std::string* DataTable = new std::string[NUM_COL * height];
			DataTable[0] = "Player";
			DataTable[1] = "Amount conquered (%)";

			for (size_t i = 1; i <= ActivePlayers.size(); ++i) {
				const Player* current = ActivePlayers[i - 1];
				DataTable[0 + NUM_COL * i] = current->getPlayerName();
				DataTable[1 + NUM_COL * i] = std::to_string(CalculatePercentage(current, map));
			}

			std::string tableStr = DrawTable(DataTable, NUM_COL, height);
			std::cout << tableStr << std::endl;
			delete[] DataTable;
		}
	}

	StatisticsObserver& StatisticsObserver::operator=(const StatisticsObserver& other)
	{
		//nothing to copy
		return *this;
	}

	std::ostream& operator<<(std::ostream& stream, const StatisticsObserver& obs)
	{
		stream << "Statistics Observer";
		return stream;
	}

}