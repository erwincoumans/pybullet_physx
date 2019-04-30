#include "b3RobotSimulatorClientAPI_NoGUI.h"

#include "PhysicsClientC_API.h"
#include "b3RobotSimulatorClientAPI_InternalData.h"

#ifdef BT_ENABLE_ENET
#include "PhysicsClientUDP_C_API.h"
#endif  //PHYSICS_UDP

#ifdef BT_ENABLE_CLSOCKET
#include "PhysicsClientTCP_C_API.h"
#endif  //PHYSICS_TCP


#include "SharedMemoryPublic.h"
#include "Bullet3Common/b3Logging.h"

b3RobotSimulatorClientAPI_NoGUI::b3RobotSimulatorClientAPI_NoGUI()
{
}

b3RobotSimulatorClientAPI_NoGUI::~b3RobotSimulatorClientAPI_NoGUI()
{
}

bool b3RobotSimulatorClientAPI_NoGUI::connect(int mode, const std::string& hostName, int portOrKey)
{
	if (m_data->m_physicsClientHandle)
	{
		b3Warning("Already connected, disconnect first.");
		return false;
	}
	b3PhysicsClientHandle sm = 0;
	int udpPort = 1234;
	int tcpPort = 6667;
	int key = SHARED_MEMORY_KEY;

	switch (mode)
	{

		default:
		{
			b3Warning("connectPhysicsServer unexpected argument");
		}
	};

	if (sm)
	{
		m_data->m_physicsClientHandle = sm;
		if (!b3CanSubmitCommand(m_data->m_physicsClientHandle))
		{
			disconnect();
			return false;
		}
		return true;
	}
	return false;
}
