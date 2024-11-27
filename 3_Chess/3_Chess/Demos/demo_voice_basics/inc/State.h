#pragma once

class State
{
public:
	enum States
	{
		INITIALIZED = 0,
		CONNECTING,
		CONNECTED,
		JOINING,
		JOINED,
		LEAVING,
		LEFT,
		DISCONNECTING,
		DISCONNECTED
	};
};