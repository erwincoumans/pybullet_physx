		

project ("pybullet_physx")
		language "C++"
		kind "SharedLib"
		
		includedirs {"../../src", "../../examples",
		"../../examples/ThirdPartyLibs"}
		defines {
      "BT_USE_DOUBLE_PRECISION",
		  "B3_DUMP_PYTHON_VERSION",
		  "EGL_ADD_PYTHON_INIT",
		  "B3_ENABLE_FILEIO_PLUGIN",
		  "BT_THREADSAFE=1",
		 }

    defines 
    {
      "BT_ENABLE_PHYSX",
      "PX_PHYSX_STATIC_LIB", 
      "PX_FOUNDATION_DLL=0", 
      "PX_COOKING", 
      "DISABLE_CUDA_PHYSX",
      "PX_PROFILE",
    }
		
		
	links{ }
	initOpenGL()
	initGlew()

  	includedirs {
                ".",
                "../../src",
                "../ThirdPartyLibs",
                }

	if os.is("MacOSX") then
--		targetextension {"so"}
		links{"Cocoa.framework","Python"}
	end

  if os.is("Windows") then
  defines {"WIN32"}
   files 
   {
      "../../examples/OpenGLWindow/Win32Window.cpp",
      "../../examples/OpenGLWindow/Win32OpenGLWindow.cpp",
      "../../examples/ThirdPartyLibs/glad/gl.c",
      "../../src/PhysX/physx/source/physx/src/windows/NpWindowsDelayLoadHook.cpp",
      "../../src/PhysXFoundationWindows.cpp",
      "../../src/PhysX/physx/source/physx/src/device/windows/PhysXIndicatorWindows.cpp",
  }
end
		

		files 
		{
			"pybullet_physx.c",
      "../../examples/SharedMemory/physx/PhysXC_API.cpp",
      "../../examples/SharedMemory/physx/PhysXServerCommandProcessor.cpp",
      "../../examples/SharedMemory/physx/PhysXUrdfImporter.cpp",
      "../../examples/SharedMemory/physx/URDF2PhysX.cpp",
      "../../examples/SharedMemory/plugins/eglPlugin/eglRendererPlugin.cpp",
      "../../examples/SharedMemory/plugins/eglPlugin/eglRendererVisualShapeConverter.cpp",
      "../../examples/TinyRenderer/geometry.cpp",
      "../../examples/TinyRenderer/model.cpp",
      "../../examples/TinyRenderer/tgaimage.cpp",
      "../../examples/TinyRenderer/our_gl.cpp",
      "../../examples/TinyRenderer/TinyRenderer.cpp",
      "../../examples/SharedMemory/PhysicsClient.cpp",
      "../../examples/SharedMemory/PhysicsDirect.cpp",
      "../../examples/SharedMemory/PhysicsClientC_API.cpp",
      "../../examples/SharedMemory/b3PluginManager.cpp",
      "../../examples/Utils/b3ResourcePath.cpp",
      "../../examples/Utils/RobotLoggingUtil.cpp",
      "../../examples/Utils/ChromeTraceUtil.cpp",
      "../../examples/Utils/b3Clock.cpp",
      "../../examples/Utils/b3Quickprof.cpp",
      "../../examples/ThirdPartyLibs/tinyxml2/tinyxml2.cpp",
      "../../examples/ThirdPartyLibs/Wavefront/tiny_obj_loader.cpp",
      "../../examples/ThirdPartyLibs/stb_image/stb_image.cpp",
      "../../examples/ThirdPartyLibs/stb_image/stb_image_write.cpp",
      "../../examples/ThirdPartyLibs/minizip/ioapi.c",
      "../../examples/ThirdPartyLibs/minizip/unzip.c",
      "../../examples/ThirdPartyLibs/minizip/zip.c",
      "../../examples/ThirdPartyLibs/zlib/adler32.c",
      "../../examples/ThirdPartyLibs/zlib/compress.c",
      "../../examples/ThirdPartyLibs/zlib/crc32.c",
      "../../examples/ThirdPartyLibs/zlib/deflate.c",
      "../../examples/ThirdPartyLibs/zlib/gzclose.c",
      "../../examples/ThirdPartyLibs/zlib/gzlib.c",
      "../../examples/ThirdPartyLibs/zlib/gzread.c",
      "../../examples/ThirdPartyLibs/zlib/gzwrite.c",
      "../../examples/ThirdPartyLibs/zlib/infback.c",
      "../../examples/ThirdPartyLibs/zlib/inffast.c",
      "../../examples/ThirdPartyLibs/zlib/inflate.c",
      "../../examples/ThirdPartyLibs/zlib/inftrees.c",
      "../../examples/ThirdPartyLibs/zlib/trees.c",
      "../../examples/ThirdPartyLibs/zlib/uncompr.c",
      "../../examples/ThirdPartyLibs/zlib/zutil.c",
      "../../examples/Importers/ImportColladaDemo/LoadMeshFromCollada.cpp",
      "../../examples/Importers/ImportObjDemo/LoadMeshFromObj.cpp",
      "../../examples/Importers/ImportObjDemo/Wavefront2GLInstanceGraphicsShape.cpp",
      "../../examples/Importers/ImportURDFDemo/UrdfParser.cpp",
      "../../examples/Importers/ImportURDFDemo/urdfStringSplit.cpp",
      "../../examples/Importers/ImportMeshUtility/b3ImportMeshUtility.cpp",
      "../../examples/MultiThreading/b3PosixThreadSupport.cpp",
      "../../examples/MultiThreading/b3Win32ThreadSupport.cpp",
      "../../examples/MultiThreading/b3ThreadSupportInterface.cpp",
		}

  	
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
      "../../examples",
      "../../examples/SharedMemory",
      "../../src/PhysX/physx/source/common/include",
      "../../src/PhysX/physx/source/common/src",
      "../../src/PhysX/physx/source/fastxml/include",
      "../../src/PhysX/physx/source/filebuf/include",
      "../../src/PhysX/physx/source/foundation/include",
      "../../src/PhysX/physx/source/geomutils/include",
      "../../src/PhysX/physx/source/geomutils/src",
      "../../src/PhysX/physx/source/geomutils/src/ccd",
      "../../src/PhysX/physx/source/geomutils/src/common",
      "../../src/PhysX/physx/source/geomutils/src/contact",
      "../../src/PhysX/physx/source/geomutils/src/convex",
      "../../src/PhysX/physx/source/geomutils/src/distance",
      "../../src/PhysX/physx/source/geomutils/src/gjk",
      "../../src/PhysX/physx/source/geomutils/src/hf",
      "../../src/PhysX/physx/source/geomutils/src/intersection",
      "../../src/PhysX/physx/source/geomutils/src/mesh",
      "../../src/PhysX/physx/source/geomutils/src/pcm",
      "../../src/PhysX/physx/source/geomutils/src/sweep",
      "../../src/PhysX/physx/source/lowlevel/api/include",
      "../../src/PhysX/physx/source/lowlevel/common/include",
      "../../src/PhysX/physx/source/lowlevel/common/include/collision",
      "../../src/PhysX/physx/source/lowlevel/common/include/pipeline",
      "../../src/PhysX/physx/source/lowlevel/common/include/utils",
      "../../src/PhysX/physx/source/lowlevel/software/include",
      "../../src/PhysX/physx/source/lowlevelaabb/include",
      "../../src/PhysX/physx/source/lowleveldynamics/include",
      "../../src/PhysX/physx/source/physx/src",
      "../../src/PhysX/physx/source/physx/src/buffering",
      "../../src/PhysX/physx/source/physx/src/device",
      "../../src/PhysX/physx/source/physxcooking/src",
      "../../src/PhysX/physx/source/physxcooking/src/convex",
      "../../src/PhysX/physx/source/physxcooking/src/mesh",
      "../../src/PhysX/physx/source/physxextensions/src",
      "../../src/PhysX/physx/source/physxextensions/src/serialization/Binary",
      "../../src/PhysX/physx/source/physxextensions/src/serialization/File",
      "../../src/PhysX/physx/source/physxextensions/src/serialization/Xml",
      "../../src/PhysX/physx/source/physxmetadata/core/include",
      "../../src/PhysX/physx/source/physxmetadata/extensions/include",
      "../../src/PhysX/physx/source/physxvehicle/src",
      "../../src/PhysX/physx/source/physxvehicle/src/physxmetadata/include",
      "../../src/PhysX/physx/source/pvd/include",
      "../../src/PhysX/physx/source/scenequery/include",
      "../../src/PhysX/physx/source/simulationcontroller/include",
      "../../src/PhysX/physx/source/simulationcontroller/src",
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
				"PhysX","OpenGL_Window", "BulletFileLoader", "Bullet3Common","LinearMath",
			}
		

	
	includedirs {
		_OPTIONS["python_include_dir"],
	}
	libdirs {
		_OPTIONS["python_lib_dir"]
	}
	
	if os.is("Linux") then
       		initX11()
	end

	
