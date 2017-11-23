#pragma once

struct NetMessage {
public:
	enum Type {
		SET_CLIENT_ID,
		INITIAL_DATA,
		DATA
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

public:
	Type m_type;
	union {
		int m_clientID;
		InitialData m_initialData;
		Data m_data;
	};
};