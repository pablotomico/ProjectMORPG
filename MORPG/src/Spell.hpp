#pragma once
#include <utility>


using SpellID = int;

// Pair of ending tick and Spell 
using SpellCast = std::pair<int, SpellID>;

class Spell {

public:
	Spell(SpellID l_id, float l_duration) : m_id(l_id), m_duration(l_duration) {}
	SpellID GetSpellID() {
		return m_id;
	}
	float GetDuration() {
		return m_duration;
	}

private:
	SpellID m_id;
	float m_duration;
};