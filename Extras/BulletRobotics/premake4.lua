		

project ("BulletRobotics")
		language "C++"
		kind "StaticLib"
		
		includedirs {"../../src", "../../examples",
		"../../examples/ThirdPartyLibs"}
		defines {"PHYSICS_IN_PROCESS_EXAMPLE_BROWSER"}
	hasCL = findOpenCL("clew")

	links{ "BulletFileLoader","OpenGL_Window","LinearMath", "Bullet3Common"}
	initOpenGL()
	initGlew()

  	includedirs {
                "../../src",
                "../../examples",
                "../../examples/SharedMemory",
                "../ThirdPartyLibs",
                "../ThirdPartyLibs/enet/include",
                "../ThirdPartyLibs/clsocket/src",
                }

	if os.is("MacOSX") then
--		targetextension {"so"}
		links{"Cocoa.framework","Python"}
	end

	


		files {
		"../../examples/OpenGLWindow/SimpleCamera.cpp",
		"../../examples/OpenGLWindow/SimpleCamera.h",
		"../../examples/TinyRenderer/geometry.cpp",
		"../../examples/TinyRenderer/model.cpp",
		"../../examples/TinyRenderer/tgaimage.cpp",
		"../../examples/TinyRenderer/our_gl.cpp",
		"../../examples/TinyRenderer/TinyRenderer.cpp",
		"../../examples/SharedMemory/PhysicsDirect.cpp",
		"../../examples/SharedMemory/PhysicsDirect.h",
		"../../examples/SharedMemory/PhysicsClient.cpp",
		"../../examples/SharedMemory/b3PluginManager.cpp",
		"../../examples/SharedMemory/b3PluginManager.h",
		"../../examples/SharedMemory/b3RobotSimulatorClientAPI_NoGUI.cpp",			
		"../../examples/SharedMemory/b3RobotSimulatorClientAPI_NoDirect.cpp",	
		"../../examples/SharedMemory/PhysicsClientC_API.h",
		"../../examples/SharedMemory/SharedMemoryPublic.h",
		"../../examples/SharedMemory/PhysicsClientC_API.cpp",

		"../../examples/Utils/b3ResourcePath.cpp",
		"../../examples/Utils/b3ResourcePath.h",
		"../../examples/Utils/RobotLoggingUtil.cpp",
		"../../examples/Utils/RobotLoggingUtil.h",
		"../../examples/Utils/b3Clock.cpp",
		"../../examples/Utils/b3ResourcePath.cpp",
		"../../examples/Utils/b3ERPCFMHelper.hpp",
		"../../examples/Utils/b3ReferenceFrameHelper.hpp",
		"../../examples/Utils/ChromeTraceUtil.cpp",

		"../../examples/ThirdPartyLibs/tinyxml2/tinyxml2.cpp",

		"../../examples/ThirdPartyLibs/Wavefront/tiny_obj_loader.cpp",
		"../../examples/ThirdPartyLibs/Wavefront/tiny_obj_loader.h",

		"../../examples/ThirdPartyLibs/stb_image/stb_image.cpp",


		"../../examples/Importers/ImportColladaDemo/LoadMeshFromCollada.cpp",
		"../../examples/Importers/ImportObjDemo/LoadMeshFromObj.cpp",
		"../../examples/Importers/ImportObjDemo/Wavefront2GLInstanceGraphicsShape.cpp",
		"../../examples/Importers/ImportURDFDemo/UrdfParser.cpp",
		"../../examples/Importers/ImportURDFDemo/urdfStringSplit.cpp",
		"../../examples/Importers/ImportMeshUtility/b3ImportMeshUtility.cpp",

			}


	
