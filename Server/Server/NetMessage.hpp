#pragma once

struct NetMessage {
public:
	enum Type {
		SET_CLIENT_ID,
		INITIAL_DATA,
		DATA,
		CAST_SPELL
	};
	struct InitialData {
		int m_clientID;
		char m_username[16];
		float x;
		float y;
	};

	struct Data {
		int m_clientID;
		float x;
		float y;
	};
	struct ServerData {
		int m_clientID;
		float m_serverTimestep;
	};

	struct SpellData {
		int m_clientID;
		int m_spellID;
		float m_duration;
		int m_endTick;
	};

public:
	Type m_type;
	int m_tick;
	union {
		ServerData m_serverData;
		InitialData m_initialData;
		Data m_data;
		SpellData m_spellData;
	};
};