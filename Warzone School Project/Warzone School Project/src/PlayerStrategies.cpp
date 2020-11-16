#include "PlayerStrategies.h"

namespace WZ
{
	// PlayerStrategy /////////////////////////////////////////////////////

	PlayerStrategy::~PlayerStrategy() { }

	std::vector<Territory*>& PlayerStrategy::toDefend() { return m_toDef; }
	
	std::vector<Territory*>& PlayerStrategy::toAttack() { return m_toAtk; }


}