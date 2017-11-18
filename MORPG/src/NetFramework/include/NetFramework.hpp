#pragma once

// Net Framework 
//
// by Pablo Tomico Perez

#include "UDPSocket.hpp"
#include "TCPSocket.hpp"

namespace net {

	class NetFramework {
	public:

	private:
		fd_set m_readSet;
		fd_set m_writeSet;

	};

}