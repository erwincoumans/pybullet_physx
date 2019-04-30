

project ("App_RobotSimulator")

		language "C++"
		kind "ConsoleApp"

		includedirs {"../../src", "../../examples",
		"../../examples/ThirdPartyLibs"}
		defines {"B3_USE_ROBOTSIM_GUI", "PHYSICS_IN_PROCESS_EXAMPLE_BROWSER"}

		if _OPTIONS["enable_grpc"] then
			initGRPC()
			files {
			"../../examples/SharedMemory/PhysicsClientGRPC.cpp",
                        "../../examples/SharedMemory/PhysicsClientGRPC.h",
                        "../../examples/SharedMemory/PhysicsClientGRPC_C_API.cpp",
                        "../../examples/SharedMemory/PhysicsClientGRPC_C_API.h",
			}
		end


	links{"BulletRobotics", "BulletExampleBrowserLib", "gwen", "OpenGL_Window","BulletFileLoader","BulletWorldImporter","BulletSoftBody", "BulletInverseDynamicsUtils", "BulletInverseDynamics", "BulletDynamics","BulletCollision","LinearMath","Bullet3Common"}
	initOpenGL()
	initGlew()

  	includedirs {
                ".",
                "../../src",
		"../../examples/SharedMemory",
                "../ThirdPartyLibs",
                }


	if os.is("MacOSX") then
		links{"Cocoa.framework"}
	end



		if _OPTIONS["audio"] then
			files {
				"../TinyAudio/b3ADSR.cpp",
				"../TinyAudio/b3AudioListener.cpp",
				"../TinyAudio/b3ReadWavFile.cpp",
				"../TinyAudio/b3SoundEngine.cpp",
				"../TinyAudio/b3SoundSource.cpp",
				"../TinyAudio/b3WriteWavFile.cpp",
				"../TinyAudio/RtAudio.cpp",
			}
			defines {"B3_ENABLE_TINY_AUDIO"}

			if _OPTIONS["serial"] then
				defines{"B3_ENABLE_SERIAL"}
				includedirs {"../../examples/ThirdPartyLibs/serial/include"}
				links {"serial"}
			end
			
			if os.is("Windows") then
				links {"winmm","Wsock32","dsound"}
				defines {"WIN32","__WINDOWS_MM__","__WINDOWS_DS__"}
			end
			
			if os.is("Linux") then initX11() 
			                defines  {"__OS_LINUX__","__LINUX_ALSA__"}
				links {"asound","pthread"}
			end


			if os.is("MacOSX") then
				links{"Cocoa.framework"}
				links{"CoreAudio.framework", "coreMIDI.framework", "Cocoa.framework"}
				defines {"__OS_MACOSX__","__MACOSX_CORE__"}
			end
		end
		files {
			"RobotSimulatorMain.cpp",
			"b3RobotSimulatorClientAPI.cpp",
			"b3RobotSimulatorClientAPI.h",
			"MinitaurSetup.cpp",
			"MinitaurSetup.h",
			"../../examples/ExampleBrowser/InProcessExampleBrowser.cpp",
			"../../examples/SharedMemory/PhysicsServerExample.cpp",
			"../../examples/SharedMemory/PhysicsServerExampleBullet2.cpp",
			"../../examples/SharedMemory/SharedMemoryInProcessPhysicsC_API.cpp",
		}

if (_OPTIONS["enable_static_vr_plugin"]) then
	files {"../../examples/SharedMemory/plugins/vrSyncPlugin/vrSyncPlugin.cpp"}
end

	if os.is("Linux") then
       		initX11()
	end


if _OPTIONS["serial"] then

project ("App_VRGloveHandSimulator")

		language "C++"
		kind "ConsoleApp"

		includedirs {"../../src", "../../examples",
		"../../examples/ThirdPartyLibs"}
		defines {"PHYSICS_IN_PROCESS_EXAMPLE_BROWSER"}

	hasCL = findOpenCL("clew")

	links{"BulletRobotics", "BulletExampleBrowserLib","gwen", "OpenGL_Window","BulletFileLoader","BulletWorldImporter","BulletSoftBody", "BulletInverseDynamicsUtils", "BulletInverseDynamics", "BulletDynamics","BulletCollision","LinearMath","BussIK","Bullet3Common"}
	initOpenGL()
	initGlew()

  	includedirs {
                ".",
                "../../src",
                "../ThirdPartyLibs",
                }


	if os.is("MacOSX") then
		links{"Cocoa.framework"}
	end

		if (hasCL) then
			links {
				"Bullet3OpenCL_clew",
				"Bullet3Dynamics",
				"Bullet3Collision",
				"Bullet3Geometry",
				"Bullet3Common",
			}
		end

		if _OPTIONS["audio"] then
			files {
				"../TinyAudio/b3ADSR.cpp",
				"../TinyAudio/b3AudioListener.cpp",
				"../TinyAudio/b3ReadWavFile.cpp",
				"../TinyAudio/b3SoundEngine.cpp",
				"../TinyAudio/b3SoundSource.cpp",
				"../TinyAudio/b3WriteWavFile.cpp",
				"../TinyAudio/RtAudio.cpp",
			}
			defines {"B3_ENABLE_TINY_AUDIO"}

			
			defines{"B3_ENABLE_SERIAL"}
			includedirs {"../../examples/ThirdPartyLibs/serial/include"}
			links {"serial"}
		
			if os.is("Windows") then
				links {"winmm","Wsock32","dsound"}
				defines {"WIN32","__WINDOWS_MM__","__WINDOWS_DS__"}
			end
			
			if os.is("Linux") then initX11() 
			                defines  {"__OS_LINUX__","__LINUX_ALSA__"}
				links {"asound","pthread"}
			end


			if os.is("MacOSX") then
				links{"Cocoa.framework"}
				links{"CoreAudio.framework", "coreMIDI.framework", "Cocoa.framework"}
				defines {"__OS_MACOSX__","__MACOSX_CORE__"}
			end
		end
		files {
			"VRGloveSimulatorMain.cpp",
			"b3RobotSimulatorClientAPI.cpp",
			"b3RobotSimulatorClientAPI.h",
			
		}

if (_OPTIONS["enable_static_vr_plugin"]) then
	files {"../../examples/SharedMemory/plugins/vrSyncPlugin/vrSyncPlugin.cpp"}
end

	if os.is("Linux") then
       		initX11()
	end
end


project ("App_HelloBulletRobotics")

	language "C++"
	kind "ConsoleApp"

	links{"BulletRobotics","BulletFileLoader","BulletWorldImporter","BulletSoftBody", "BulletInverseDynamicsUtils", "BulletInverseDynamics", "BulletDynamics","BulletCollision","LinearMath","Bullet3Common"}
	
  includedirs {
                ".",
                "../../src",
                "../../examples/SharedMemory",
                "../ThirdPartyLibs",
                }

    if os.is("Linux") then
    	defines {"_LINUX"}
    end
		if os.is("MacOSX") then
    	defines {"_DARWIN"}
		end


	if os.is("MacOSX") then
		links{"Cocoa.framework"}
	end

	
	if os.is("Linux") then initX11()
                     links {"pthread"}
        end

	
		files {
			 "HelloBulletRobotics.cpp"
		}



project ("App_HelloPhysXRobotics")

	language "C++"
	kind "ConsoleApp"

	links{"OpenGL_Window","BulletFileLoader", "BulletRobotics","LinearMath","Bullet3Common"}
	
  includedirs {
                ".",
                "../../src",
								"../../examples",
                "../../examples/SharedMemory",
                "../ThirdPartyLibs",
                }

	initOpenGL()
	initGlew()

if os.is("MacOSX") then
	files {"../../src/PhysXFoundationUnix.cpp"}                
end
if os.is("Linux") then
	files {"../../src/PhysXFoundationUnix.cpp"}
end

	
    if os.is("Linux") then
    	defines {"_LINUX"}
    end
		if os.is("MacOSX") then
    	defines {"_DARWIN"}
		end


	if os.is("MacOSX") then
		links{"Cocoa.framework"}
	end

	
	if os.is("Linux") then initX11()
                     links {"pthread"}
        end

	
		files {
			 "HelloPhysXRobotics.cpp"
		}

	if _OPTIONS["enable_physx"] then
  	defines {"BT_ENABLE_PHYSX","PX_PHYSX_STATIC_LIB", "PX_FOUNDATION_DLL=0", "PX_PROFILE"}
		
		configuration {"x64", "debug"}			
				defines {"_DEBUG"}
		configuration {"x86", "debug"}
				defines {"_DEBUG"}
		configuration {"x64", "release"}
				defines {"NDEBUG"}
		configuration {"x86", "release"}
				defines {"NDEBUG"}
		configuration{}

		includedirs {
				".",
				"../../src/PhysX/physx/source/foundation/include",
				"../../src/PhysX/physx/source/common/src",
				"../../src/PhysX/physx/source/physxextensions/src",
				"../../src/PhysX/physx/include",
				"../../src/PhysX/physx/include/characterkinematic",
				"../../src/PhysX/physx/include/common",
				"../../src/PhysX/physx/include/cooking",
				"../../src/PhysX/physx/include/extensions",
				"../../src/PhysX/physx/include/geometry",
				"../../src/PhysX/physx/include/geomutils",
				"../../src/PhysX/physx/include/vehicle",
				"../../src/PhysX/pxshared/include",
                }
		links {
				"PhysX",
			}
			
			files {
				"../../examples/SharedMemory/plugins/eglPlugin/eglRendererPlugin.cpp",
				"../../examples/SharedMemory/plugins/eglPlugin/eglRendererPlugin.h",
				"../../examples/SharedMemory/plugins/eglPlugin/eglRendererVisualShapeConverter.cpp",
				"../../examples/SharedMemory/plugins/eglPlugin/eglRendererVisualShapeConverter.h",
				"../../examples/SharedMemory/physx/PhysXC_API.cpp",
				"../../examples/SharedMemory/physx/PhysXServerCommandProcessor.cpp",
				"../../examples/SharedMemory/physx/PhysXUrdfImporter.cpp",
				"../../examples/SharedMemory/physx/URDF2PhysX.cpp",
				"../../examples/SharedMemory/physx/PhysXC_API.h",
				"../../examples/SharedMemory/physx/PhysXServerCommandProcessor.h",
				"../../examples/SharedMemory/physx/PhysXUrdfImporter.h",
				"../../examples/SharedMemory/physx/URDF2PhysX.h",
				"../../examples/SharedMemory/physx/PhysXUserData.h",
				}
  end


project ("App_HelloPhysXImmediateMode")

	language "C++"
	kind "ConsoleApp"

	links{"OpenGL_Window", "LinearMath","Bullet3Common"}
	
  includedirs {
                ".",
                "../../src",
								"../../examples",
                "../../examples/SharedMemory",
                "../ThirdPartyLibs",
                }

	initOpenGL()
	initGlew()

if os.is("MacOSX") then
	files {"../../src/PhysXFoundationUnix.cpp"}                
end
if os.is("Linux") then
	files {"../../src/PhysXFoundationUnix.cpp"}
end

	
    if os.is("Linux") then
    	defines {"_LINUX"}
    end
		if os.is("MacOSX") then
    	defines {"_DARWIN"}
		end

		links{"Cocoa.framework"}

	
	if os.is("Linux") then initX11()
                     links {"pthread"}
        end

	
		files {
			 "HelloPhysXImmediateMode.cpp",
			 "../Utils/ChromeTraceUtil.cpp",
			 "../Utils/ChromeTraceUtil.h",
			 "../Snippet/SnippetImmediateArticulation.cpp",
			 "../Snippet/SnippetImmediateArticulationRender.cpp",
			 "../Snippet/snippetutils/SnippetUtils.cpp",
			 "../Snippet/snippetutils/SnippetUtils.h",
		}

	if _OPTIONS["enable_physx"] then
  	defines {"BT_ENABLE_PHYSX","PX_PHYSX_STATIC_LIB", "PX_FOUNDATION_DLL=0", "PX_PROFILE"}
		
		configuration {"x64", "debug"}			
				defines {"_DEBUG"}
		configuration {"x86", "debug"}
				defines {"_DEBUG"}
		configuration {"x64", "release"}
				defines {"NDEBUG"}
		configuration {"x86", "release"}
				defines {"NDEBUG"}
		configuration{}

		includedirs {
				".",
				"../../src/PhysX/physx/source/foundation/include",
				"../../src/PhysX/physx/source/common/src",
				"../../src/PhysX/physx/source/physxextensions/src",
				"../../src/PhysX/physx/include",
				"../../src/PhysX/physx/include/characterkinematic",
				"../../src/PhysX/physx/include/common",
				"../../src/PhysX/physx/include/cooking",
				"../../src/PhysX/physx/include/extensions",
				"../../src/PhysX/physx/include/geometry",
				"../../src/PhysX/physx/include/geomutils",
				"../../src/PhysX/physx/include/vehicle",
				"../../src/PhysX/pxshared/include",
                }
		links {
				"PhysX",
			}
			
			
  end
