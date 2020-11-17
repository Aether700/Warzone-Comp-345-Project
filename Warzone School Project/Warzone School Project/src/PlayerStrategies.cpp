#include "PlayerStrategies.h"
#include "Player.h"
#include <assert.h>

namespace WZ
{
	// PlayerStrategy /////////////////////////////////////////////////////

	PlayerStrategy::PlayerStrategy(Player* player) : m_player(player), m_reinforcements(0)
	{
		assert(player != nullptr);
	}

	PlayerStrategy::~PlayerStrategy() { }

	std::vector<Territory*>& PlayerStrategy::toDefend() { return m_toDef; }
	
	std::vector<Territory*>& PlayerStrategy::toAttack() { return m_toAtk; }

	void PlayerStrategy::SetPlayer(Player* p) 
	{
		assert(p != nullptr);
		m_player = p; 
	}

}