#include "PhysicsDirect.h"

#include "SharedMemory/physx/PhysXC_API.h"
#include "SharedMemory/PhysicsDirect.h"
#include "SharedMemory/physx/PhysXServerCommandProcessor.h"
#include "SharedMemory/b3RobotSimulatorClientAPI_NoGUI.h"
#include "SharedMemory/b3RobotSimulatorClientAPI_InternalData.h"
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
	sim->setAdditionalSearchPath("/Users/erwincoumans/develop/pybullet_physx/examples/pybullet/gym/pybullet_physx_data");
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
//			args.m_useMultiBody = false;
			int boxIdx = sim->loadURDF("capsule.urdf",args);//sphere_small.urdf",args);//cube.urdf", args);
			if (i == 7)
				mass = 1000;
			dynamicsArgs.m_mass = mass;
			sim->changeDynamics(boxIdx, -1, dynamicsArgs);
			
		}
	}

	int count = 5;
	while (sim->isConnected())
	{
		b3RobotSimulatorGetCameraImageArgs args(320,200);
		b3CameraImageData imageData;
		float viewMat[16] = {
		0.34202006459236145, 0.16317586600780487, -0.9254165291786194, 0.0, -0.9396925568580627, 0.05939115956425667, -0.3368240296840668, 0.0, 7.450580596923828e-09, 0.9848076105117798, 0.1736481487751007, 0.0, -1.2438441387985222e-07, -0.0, -3.999999761581421, 1.0
		};
		args.m_viewMatrix = viewMat;
		float projMat[16] =
		{
		1.0825318098068237, 0.0, 0.0, 0.0, 0.0, 1.732050895690918, 0.0, 0.0, 0.0, 0.0, -1.0002000331878662, -1.0, 0.0, 0.0, -0.020002000033855438, 0.0
		};
		args.m_projectionMatrix = projMat;
		static int few=10;
		few--;
		if (few>0)
			sim->getCameraImage(320,200,args, imageData);
		static int skip=-1;
		skip--;
		if (skip<0)
		{
			skip=100;
			b3MouseEventsData me;
			sim->getMouseEvents(&me);
			if (me.m_numMouseEvents)
			{
			
				printf("numMouseEvents=%d\n",me.m_numMouseEvents);
				for (int i=0;i<me.m_numMouseEvents;i++)
				{
					b3MouseEvent* e = &me.m_mouseEvents[i];
					printf("event[%d].type=%d, x=%f, y=%f\n", i,e->m_eventType,e->m_mousePosX,e->m_mousePosY);
					if (e->m_eventType==MOUSE_BUTTON_EVENT)
					{
						printf("buttonIndex=%d, buttonState:%d\n", e->m_buttonIndex,e->m_buttonState);
					}
				}
				
			}
			else
			{
				printf("none\n");
			}
		}
		
		sim->stepSimulation();
		count--;
		if (count == 0)
		{
			sim->stopStateLogging(logId);
		}
	}
	delete sim;
}
