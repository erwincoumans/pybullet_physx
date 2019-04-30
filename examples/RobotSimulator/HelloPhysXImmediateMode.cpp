#if 0
#include "b3RobotSimulatorClientAPI_NoGUI.h"
#include "PhysicsServerCommandProcessor.h"
#include "b3RobotSimulatorClientAPI_InternalData.h"
#include "PhysicsDirect.h"

#include "SharedMemory/physx/PhysXC_API.h"
#include "SharedMemory/PhysicsDirect.h"
#include "SharedMemory/physx/PhysXServerCommandProcessor.h"

#include "SharedMemory/plugins/eglPlugin/eglRendererPlugin.h"
#include "SharedMemory/b3PluginManager.h"



int main(int argc, char* argv[])
{
	
	b3RobotSimulatorClientAPI_NoGUI* sim = new b3RobotSimulatorClientAPI_NoGUI();

#define USE_PHYSX_EXAMPLE
#ifdef USE_PHYSX_EXAMPLE
	
	PhysXServerCommandProcessor* sdk = new PhysXServerCommandProcessor(argc, argv);
	PhysicsDirect* direct = new PhysicsDirect(sdk, true);
	bool connected;
	connected = direct->connect();
	b3RobotSimulatorClientAPI_InternalData data;
	data.m_guiHelper = 0;
	data.m_physicsClientHandle = (b3PhysicsClientHandle)direct;

	//we bypass 'sim->connect' since we have a custom connection to the PhysX sdk
	sim->setInternalData(&data);
#else //USE_PHYSX_EXAMPLE
	
	PhysicsServerCommandProcessor* sdk = new PhysicsServerCommandProcessor;
	

	PhysicsDirect* direct = new PhysicsDirect(sdk, true);
	bool connected;
	connected = direct->connect();
	b3RobotSimulatorClientAPI_InternalData data;
	data.m_guiHelper = 0;
	data.m_physicsClientHandle = (b3PhysicsClientHandle)direct;

	//we bypass 'sim->connect' since we have a custom connection to the PhysX sdk
	sim->setInternalData(&data);
	//sim->connect(eCONNECT_DIRECT);

	b3PluginManager* pluginManager = sdk->getPluginManager();
	{
		bool initPlugin = false;
		b3PluginFunctions funcs(initPlugin_eglRendererPlugin, exitPlugin_eglRendererPlugin, executePluginCommand_eglRendererPlugin);
		funcs.m_getRendererFunc = getRenderInterface_eglRendererPlugin;
		int renderPluginId = pluginManager->registerStaticLinkedPlugin("eglRendererPlugin", funcs, initPlugin);
		pluginManager->selectPluginRenderer(renderPluginId);
	}

#endif //USE_PHYSX_EXAMPLE
	if (!sim->isConnected())
	{
		printf("Error connecting to PhysX\n");
		exit(0);
	}

	int pluginUid = sim->loadPlugin("eglRendererPlugin");

	int logId = sim->startStateLogging(STATE_LOGGING_PROFILE_TIMINGS, "physx.json");
	//remove all existing objects (if any)
	//sim->resetSimulation();
	sim->setGravity(btVector3(0, 0, -9.8));
	sim->setNumSolverIterations(1);
	b3RobotSimulatorSetPhysicsEngineParameters args1;
	sim->getPhysicsEngineParameters(args1);
	btScalar dt = 1. / 60.;// 1. / 240.;
	//dt /= 25;// 25;// 128.;
	sim->setTimeStep(dt);

	b3RobotSimulatorLoadUrdfFileArgs args2;
	args2.m_useMultiBody = false;
	
	int planeUid = sim->loadURDF("plane.urdf", args2);
	printf("planeUid = %d\n", planeUid);

	
	{
		b3RobotSimulatorLoadUrdfFileArgs args;
		b3RobotSimulatorChangeDynamicsArgs dynamicsArgs;
		int massRatio = 2;
		int mass = 1;
		btScalar boxSize = 1;
		for (int i = 0; i < 10; i++)
		{
			args.m_startPosition.setValue(0, 0, boxSize*0.5+i * boxSize);
			args.m_useMultiBody = false;
			int boxIdx = sim->loadURDF("cube.urdf", args);
			if (i == 7)
				mass = 1000;
			dynamicsArgs.m_mass = mass;
			sim->changeDynamics(boxIdx, -1, dynamicsArgs);
			
		}
	}

	int count = 5;
	while (sim->isConnected())
	{
		sim->stepSimulation();
		count--;
		if (count == 0)
		{
			sim->stopStateLogging(logId);
		}
	}
	delete sim;
}
#endif