extern void initPhysics(bool interactive);
extern void stepPhysics(bool interactive);
extern void cleanupPhysics(bool interactive);
#include "PxPhysicsAPI.h"
using namespace physx;
#include "Bullet3Common/b3AlignedObjectArray.h"
#include "OpenGLWindow/ShapeData.h"
#include "LinearMath/btVector3.h"
extern physx::PxU32 getNbBounds();
extern const physx::PxBounds3* getBounds();

extern physx::PxU32 getNbContacts();
extern const physx::Gu::ContactPoint* getContacts();
extern PxU32 getNbGeoms();
extern const PxGeometryHolder* getGeoms();
extern const PxTransform* getGeomPoses();



//#define USE_OPENGL2
#ifdef USE_OPENGL2
#include "OpenGLWindow/SimpleOpenGL2App.h"
typedef SimpleOpenGL2App SimpleOpenGLApp;

#else
#include "OpenGLWindow/SimpleOpenGL3App.h"
typedef SimpleOpenGL3App SimpleOpenGLApp;

#endif  //USE_OPENGL2

#include "Bullet3Common/b3Quaternion.h"
#include "Bullet3Common/b3CommandLineArgs.h"
#include "assert.h"
#include <stdio.h>

static char* gVideoFileName = 0;
static char* gPngFileName = 0;

static b3WheelCallback sOldWheelCB = 0;
static b3ResizeCallback sOldResizeCB = 0;
static b3MouseMoveCallback sOldMouseMoveCB = 0;
static b3MouseButtonCallback sOldMouseButtonCB = 0;
static b3KeyboardCallback sOldKeyboardCB = 0;
//static b3RenderCallback sOldRenderCB = 0;

static float gWidth = 1024;
static float gHeight = 768;

void MyWheelCallback2(float deltax, float deltay)
{
	if (sOldWheelCB)
		sOldWheelCB(deltax, deltay);
}
void MyResizeCallback2(float width, float height)
{
	gWidth = width;
	gHeight = height;

	if (sOldResizeCB)
		sOldResizeCB(width, height);
}
void MyMouseMoveCallback2(float x, float y)
{
	printf("Mouse Move: %f, %f\n", x, y);

	if (sOldMouseMoveCB)
		sOldMouseMoveCB(x, y);
}
void MyMouseButtonCallback2(int button, int state, float x, float y)
{
	if (sOldMouseButtonCB)
		sOldMouseButtonCB(button, state, x, y);
}

extern void keyPress(unsigned char key, const PxTransform& /*camera*/);
#include "../../Utils/ChromeTraceUtil.h"

static void MyKeyboardCallback2(int keycode, int state)
{
	//keycodes are in examples/CommonInterfaces/CommonWindowInterface.h
	//for example B3G_ESCAPE for escape key
	//state == 1 for pressed, state == 0 for released.
	// use app->m_window->isModifiedPressed(...) to check for shift, escape and alt keys
	
	if (state)
	{
		PxTransform tr;
		
		keyPress((char)keycode,tr);
		if (keycode == 'p')
		{
			b3ChromeUtilsStartTimings();
		}
	}
	else
	{
		if (keycode == 'p')
		{
			b3ChromeUtilsStopTimingsAndWriteJsonFile("physx_lowlevel.json");
		}
	}


	printf("MyKeyboardCallback received key:%c in state %d\n", keycode, state);
	if (sOldKeyboardCB)
		sOldKeyboardCB(keycode, state);
}

SimpleOpenGLApp* app = 0;

static void DrawLine(const physx::PxVec3& p0, const physx::PxVec3& p1, const physx::PxVec3& color)
{
	double from[4] = { p0.x, p0.y, p0.z, 1 };
	double to[4] = { p1.x, p1.y, p1.z, 1 };
	double color2[4] = { color.x,color.y,color.z, 1 };
	app->m_renderer->drawLine(from, to, color2, 1);
}


static void DrawBounds(const physx::PxBounds3& box)
{
	DrawLine(PxVec3(box.minimum.x, box.minimum.y, box.minimum.z), PxVec3(box.maximum.x, box.minimum.y, box.minimum.z), PxVec3(1.0f, 1.0f, 0.0f));
	DrawLine(PxVec3(box.maximum.x, box.minimum.y, box.minimum.z), PxVec3(box.maximum.x, box.maximum.y, box.minimum.z), PxVec3(1.0f, 1.0f, 0.0f));
	DrawLine(PxVec3(box.maximum.x, box.maximum.y, box.minimum.z), PxVec3(box.minimum.x, box.maximum.y, box.minimum.z), PxVec3(1.0f, 1.0f, 0.0f));
	DrawLine(PxVec3(box.minimum.x, box.maximum.y, box.minimum.z), PxVec3(box.minimum.x, box.minimum.y, box.minimum.z), PxVec3(1.0f, 1.0f, 0.0f));
	DrawLine(PxVec3(box.minimum.x, box.minimum.y, box.minimum.z), PxVec3(box.minimum.x, box.minimum.y, box.maximum.z), PxVec3(1.0f, 1.0f, 0.0f));
	DrawLine(PxVec3(box.minimum.x, box.minimum.y, box.maximum.z), PxVec3(box.maximum.x, box.minimum.y, box.maximum.z), PxVec3(1.0f, 1.0f, 0.0f));
	DrawLine(PxVec3(box.maximum.x, box.minimum.y, box.maximum.z), PxVec3(box.maximum.x, box.maximum.y, box.maximum.z), PxVec3(1.0f, 1.0f, 0.0f));
	DrawLine(PxVec3(box.maximum.x, box.maximum.y, box.maximum.z), PxVec3(box.minimum.x, box.maximum.y, box.maximum.z), PxVec3(1.0f, 1.0f, 0.0f));
	DrawLine(PxVec3(box.minimum.x, box.maximum.y, box.maximum.z), PxVec3(box.minimum.x, box.minimum.y, box.maximum.z), PxVec3(1.0f, 1.0f, 0.0f));
	DrawLine(PxVec3(box.minimum.x, box.minimum.y, box.maximum.z), PxVec3(box.minimum.x, box.minimum.y, box.minimum.z), PxVec3(1.0f, 1.0f, 0.0f));
	DrawLine(PxVec3(box.maximum.x, box.minimum.y, box.minimum.z), PxVec3(box.maximum.x, box.minimum.y, box.maximum.z), PxVec3(1.0f, 1.0f, 0.0f));
	DrawLine(PxVec3(box.maximum.x, box.maximum.y, box.minimum.z), PxVec3(box.maximum.x, box.maximum.y, box.maximum.z), PxVec3(1.0f, 1.0f, 0.0f));
	DrawLine(PxVec3(box.minimum.x, box.maximum.y, box.minimum.z), PxVec3(box.minimum.x, box.maximum.y, box.maximum.z), PxVec3(1.0f, 1.0f, 0.0f));
}

static void DrawFrame(const PxVec3& pt, float scale = 1.0f)
{
	DrawLine(pt, pt + PxVec3(scale, 0.0f, 0.0f), PxVec3(1.0f, 0.0f, 0.0f));
	DrawLine(pt, pt + PxVec3(0.0f, scale, 0.0f), PxVec3(0.0f, 1.0f, 0.0f));
	DrawLine(pt, pt + PxVec3(0.0f, 0.0f, scale), PxVec3(0.0f, 0.0f, 1.0f));
}

static void renderGeometry(const PxMat44& pose, const PxGeometry& geom, int textureHandle)
{
	switch (geom.getType())
	{
	case PxGeometryType::eBOX:
	{
		const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(geom);
		//glScalef(boxGeom.halfExtents.x, boxGeom.halfExtents.y, boxGeom.halfExtents.z);
		//glutSolidCube(2);
		int strideInBytes = 9 * sizeof(float);
		int numVertices = sizeof(cube_vertices_textured) / strideInBytes;
		int numIndices = sizeof(cube_indices) / sizeof(int);
		b3AlignedObjectArray<unsigned int> indices;
		indices.resize(numIndices);
		for (int i = 0; i < numIndices; i++)
		{
			indices[i] = cube_indices[i];
		}

		b3AlignedObjectArray<GfxVertexFormat0> verts;
		verts.resize(numVertices);
		float textureScaling = 1;
		btVector3 halfExtents(boxGeom.halfExtents.x, boxGeom.halfExtents.y, boxGeom.halfExtents.z);
		for (int i = 0; i < numVertices; i++)
		{
			verts[i].x = halfExtents.x()*cube_vertices_textured[i * 9];
			verts[i].y = halfExtents.y()*cube_vertices_textured[i * 9 + 1];
			verts[i].z = halfExtents.z()*cube_vertices_textured[i * 9 + 2];
			verts[i].w = cube_vertices_textured[i * 9 + 3];
			verts[i].u = cube_vertices_textured[i * 9 + 7] * textureScaling;
			verts[i].v = cube_vertices_textured[i * 9 + 8] * textureScaling;
		}
		float color[4] = { 1, 1, 1, 1 };
		
		float pos[3] = { pose.getPosition().x, pose.getPosition().y, pose.getPosition().z };
		PxTransform tr(pose);
		float orn[4] = { tr.q.x, tr.q.y, tr.q.z, tr.q.w };
		app->m_renderer->drawTexturedTriangleMesh(pos, orn, &verts[0].x, verts.size(), &indices[0], numIndices, color, textureHandle);
	}
	break;

	case PxGeometryType::eSPHERE:
	{
		const PxSphereGeometry& sphereGeom = static_cast<const PxSphereGeometry&>(geom);
		
		int strideInBytes = 9 * sizeof(float);
		int numVertices = sizeof(textured_detailed_sphere_vertices) / strideInBytes;
		int numIndices = sizeof(textured_detailed_sphere_indices) / sizeof(int);
		b3AlignedObjectArray<unsigned int> indices;
		indices.resize(numIndices);
		for (int i = 0; i < numIndices; i++)
		{
			indices[i] = textured_detailed_sphere_indices[i];
		}

		b3AlignedObjectArray<GfxVertexFormat0> verts;
		verts.resize(numVertices);
		float textureScaling = 1;
		for (int i = 0; i < numVertices; i++)
		{
			verts[i].x = 2.*sphereGeom.radius * textured_detailed_sphere_vertices[i * 9];
			verts[i].y = 2.*sphereGeom.radius * textured_detailed_sphere_vertices[i * 9 + 1];
			verts[i].z = 2.*sphereGeom.radius * textured_detailed_sphere_vertices[i * 9 + 2];
			verts[i].w = textured_detailed_sphere_vertices[i * 9 + 3];
			verts[i].u = textured_detailed_sphere_vertices[i * 9 + 7] * textureScaling;
			verts[i].v = textured_detailed_sphere_vertices[i * 9 + 8] * textureScaling;
		}
		float color[4] = { 1, 1, 1, 1 };
		
		float pos[3] = { pose.getPosition().x, pose.getPosition().y, pose.getPosition().z };
		PxTransform tr(pose);
		float orn[4] = { tr.q.x, tr.q.y, tr.q.z, tr.q.w };
		app->m_renderer->drawTexturedTriangleMesh(pos, orn, &verts[0].x, verts.size(), &indices[0], numIndices, color, textureHandle);
		//glutSolidSphere(GLdouble(sphereGeom.radius), 10, 10);
	}
	break;

	case PxGeometryType::eCAPSULE:
	{
#if 0
		const PxCapsuleGeometry& capsuleGeom = static_cast<const PxCapsuleGeometry&>(geom);
		const PxF32 radius = capsuleGeom.radius;
		const PxF32 halfHeight = capsuleGeom.halfHeight;

		//Sphere
		glPushMatrix();
		glTranslatef(halfHeight, 0.0f, 0.0f);
		glScalef(radius, radius, radius);
		glutSolidSphere(1, 10, 10);
		glPopMatrix();

		//Sphere
		glPushMatrix();
		glTranslatef(-halfHeight, 0.0f, 0.0f);
		glScalef(radius, radius, radius);
		glutSolidSphere(1, 10, 10);
		glPopMatrix();

		//Cylinder
		glPushMatrix();
		glTranslatef(-halfHeight, 0.0f, 0.0f);
		glScalef(2.0f*halfHeight, radius, radius);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(3, GL_FLOAT, 2 * 3 * sizeof(float), gCylinderData);
		glNormalPointer(GL_FLOAT, 2 * 3 * sizeof(float), gCylinderData + 3);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 13 * 2);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glPopMatrix();
#endif
	}

	break;

	case PxGeometryType::eCONVEXMESH:
	{
#if 0
		const PxConvexMeshGeometry& convexGeom = static_cast<const PxConvexMeshGeometry&>(geom);

		//Compute triangles for each polygon.
		const PxVec3& scale = convexGeom.scale.scale;
		PxConvexMesh* mesh = convexGeom.convexMesh;
		const PxU32 nbPolys = mesh->getNbPolygons();
		const PxU8* polygons = mesh->getIndexBuffer();
		const PxVec3* verts = mesh->getVertices();
		PxU32 nbVerts = mesh->getNbVertices();
		PX_UNUSED(nbVerts);

		PxU32 numTotalTriangles = 0;
		for (PxU32 i = 0; i < nbPolys; i++)
		{
			PxHullPolygon data;
			mesh->getPolygonData(i, data);

			const PxU32 nbTris = PxU32(data.mNbVerts - 2);
			const PxU8 vref0 = polygons[data.mIndexBase + 0];
			PX_ASSERT(vref0 < nbVerts);
			for (PxU32 j = 0; j<nbTris; j++)
			{
				const PxU32 vref1 = polygons[data.mIndexBase + 0 + j + 1];
				const PxU32 vref2 = polygons[data.mIndexBase + 0 + j + 2];

				//generate face normal:
				PxVec3 e0 = verts[vref1] - verts[vref0];
				PxVec3 e1 = verts[vref2] - verts[vref0];

				PX_ASSERT(vref1 < nbVerts);
				PX_ASSERT(vref2 < nbVerts);

				PxVec3 fnormal = e0.cross(e1);
				fnormal.normalize();

				if (numTotalTriangles * 6 < MAX_NUM_MESH_VEC3S)
				{
					gVertexBuffer[numTotalTriangles * 6 + 0] = fnormal;
					gVertexBuffer[numTotalTriangles * 6 + 1] = verts[vref0];
					gVertexBuffer[numTotalTriangles * 6 + 2] = fnormal;
					gVertexBuffer[numTotalTriangles * 6 + 3] = verts[vref1];
					gVertexBuffer[numTotalTriangles * 6 + 4] = fnormal;
					gVertexBuffer[numTotalTriangles * 6 + 5] = verts[vref2];
					numTotalTriangles++;
				}
			}
		}
		glPushMatrix();
		glScalef(scale.x, scale.y, scale.z);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glNormalPointer(GL_FLOAT, 2 * 3 * sizeof(float), gVertexBuffer);
		glVertexPointer(3, GL_FLOAT, 2 * 3 * sizeof(float), gVertexBuffer + 1);
		glDrawArrays(GL_TRIANGLES, 0, int(numTotalTriangles * 3));
		glPopMatrix();
#endif
	}
	break;

	case PxGeometryType::eTRIANGLEMESH:
	{
#if 0
		const PxTriangleMeshGeometry& triGeom = static_cast<const PxTriangleMeshGeometry&>(geom);

		const PxTriangleMesh& mesh = *triGeom.triangleMesh;
		const PxVec3 scale = triGeom.scale.scale;

		const PxU32 triangleCount = mesh.getNbTriangles();
		const PxU32 has16BitIndices = mesh.getTriangleMeshFlags() & PxTriangleMeshFlag::e16_BIT_INDICES;
		const void* indexBuffer = mesh.getTriangles();

		const PxVec3* vertexBuffer = mesh.getVertices();

		const PxU32* intIndices = reinterpret_cast<const PxU32*>(indexBuffer);
		const PxU16* shortIndices = reinterpret_cast<const PxU16*>(indexBuffer);
		PxU32 numTotalTriangles = 0;
		for (PxU32 i = 0; i < triangleCount; ++i)
		{
			PxVec3 triVert[3];

			if (has16BitIndices)
			{
				triVert[0] = vertexBuffer[*shortIndices++];
				triVert[1] = vertexBuffer[*shortIndices++];
				triVert[2] = vertexBuffer[*shortIndices++];
			}
			else
			{
				triVert[0] = vertexBuffer[*intIndices++];
				triVert[1] = vertexBuffer[*intIndices++];
				triVert[2] = vertexBuffer[*intIndices++];
			}

			PxVec3 fnormal = (triVert[1] - triVert[0]).cross(triVert[2] - triVert[0]);
			fnormal.normalize();

			if (numTotalTriangles * 6 < MAX_NUM_MESH_VEC3S)
			{
				gVertexBuffer[numTotalTriangles * 6 + 0] = fnormal;
				gVertexBuffer[numTotalTriangles * 6 + 1] = triVert[0];
				gVertexBuffer[numTotalTriangles * 6 + 2] = fnormal;
				gVertexBuffer[numTotalTriangles * 6 + 3] = triVert[1];
				gVertexBuffer[numTotalTriangles * 6 + 4] = fnormal;
				gVertexBuffer[numTotalTriangles * 6 + 5] = triVert[2];
				numTotalTriangles++;
			}
		}
		glPushMatrix();
		glScalef(scale.x, scale.y, scale.z);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glNormalPointer(GL_FLOAT, 2 * 3 * sizeof(float), gVertexBuffer);
		glVertexPointer(3, GL_FLOAT, 2 * 3 * sizeof(float), gVertexBuffer + 1);
		glDrawArrays(GL_TRIANGLES, 0, int(numTotalTriangles * 3));
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glPopMatrix();
#endif
	}
	break;

	case PxGeometryType::eINVALID:
	case PxGeometryType::eHEIGHTFIELD:
	case PxGeometryType::eGEOMETRY_COUNT:
	case PxGeometryType::ePLANE:
		break;
	}
}



void renderGeoms(const PxU32 nbGeoms, const PxGeometryHolder* geoms, const PxTransform* poses, bool shadows, const PxVec3& color, int textureHandle)
{
	
	//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	const PxVec3 shadowDir(0.0f, -0.7071067f, -0.7071067f);
	const PxReal shadowMat[] = { 1, 0, 0, 0, -shadowDir.x / shadowDir.y, 0, -shadowDir.z / shadowDir.y, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

	for (PxU32 j = 0; j<nbGeoms; j++)
	{
		const PxMat44 shapePose(poses[j]);

		const PxGeometry& geom = geoms[j].any();

		// render object
		renderGeometry(shapePose, geom, textureHandle);

	}
}


int main(int argc,  char* argv[])
{
	initPhysics(true);
	{
		b3CommandLineArgs myArgs(argc, argv);

		app = new SimpleOpenGLApp("SimpleOpenGL3App", 1024, 768);

		app->m_renderer->getActiveCamera()->setCameraDistance(13);
		app->m_renderer->getActiveCamera()->setCameraPitch(0);
		app->m_renderer->getActiveCamera()->setCameraTargetPosition(0, 0, 0);
		sOldKeyboardCB = app->m_window->getKeyboardCallback();
		app->m_window->setKeyboardCallback(MyKeyboardCallback2);
		sOldMouseMoveCB = app->m_window->getMouseMoveCallback();
		app->m_window->setMouseMoveCallback(MyMouseMoveCallback2);
		sOldMouseButtonCB = app->m_window->getMouseButtonCallback();
		app->m_window->setMouseButtonCallback(MyMouseButtonCallback2);
		sOldWheelCB = app->m_window->getWheelCallback();
		app->m_window->setWheelCallback(MyWheelCallback2);
		sOldResizeCB = app->m_window->getResizeCallback();
		app->m_window->setResizeCallback(MyResizeCallback2);

		myArgs.GetCmdLineArgument("mp4_file", gVideoFileName);
		if (gVideoFileName)
			app->dumpFramesToVideo(gVideoFileName);

		myArgs.GetCmdLineArgument("png_file", gPngFileName);
		char fileName[1024];

		int textureWidth = 128;
		int textureHeight = 128;

		b3AlignedObjectArray<unsigned char> texels;
		texels.resize(textureWidth * textureHeight * 3);

		int reds[16] = { 0 };
		int greens[16] = { 0 };
		int blues[16] = { 0 };
		reds[0] = 64;
		reds[1] = 128;
		reds[2] = 255;
		reds[3] = 255;
		blues[4] = 64;
		blues[5] = 128;
		blues[6] = 255;
		greens[7] = 128;
		blues[8] = 255;
		reds[9] = 255;
		blues[10] = 128;
		blues[11] = 128;
		reds[11] = 128;
		blues[12] = 255;
		reds[12] = 255;
		greens[13] = 255;
		reds[14] = 255;
		greens[14] = 255;
		greens[15] = 255;
		blues[15] = 255;

		for (int i = 0; i < textureWidth; i ++)
		{
			for (int j = 0; j < textureHeight; j++)
			{
				int a = i / (textureWidth / 4);
				int b = j / (textureHeight / 4);
				int index = a + b * 4;
				int red = reds[index];
				int green = greens[index];
				int blue = blues[index];
				//mark a black border
				if ((i % 32)==0 || (j%32)==0 || (i % 32) == 31 || (j % 32) == 31)
				{
					red = 0;
					green = 0;
					blue = 0;
				}

				texels[j*3*textureWidth+i*3 + 0] = red;
				texels[j * 3 * textureWidth+ i * 3 + 1] = green;
				texels[j * 3 * textureWidth+ i * 3 + 2] = blue;
			}
		}
		
		int textureHandle = app->m_renderer->registerTexture(&texels[0], textureWidth, textureHeight);

		do
		{
			stepPhysics(true);
			static int frameCount = 0;
			frameCount++;
			if (gPngFileName)
			{
				printf("gPngFileName=%s\n", gPngFileName);

				sprintf(fileName, "%s%d.png", gPngFileName, frameCount++);
				app->dumpNextFrameToPng(fileName);
			}

			
			app->m_renderer->init();
			int upAxis = 1;
			app->m_renderer->updateCamera(upAxis);

			app->m_renderer->renderScene();

			app->drawGrid();
			
			
			const physx::PxBounds3* bounds = getBounds();
			const physx::PxU32 nbBounds = getNbBounds();
			for (physx::PxU32 i = 0; i<nbBounds; i++)
			{
				DrawBounds(bounds[i]);
			}

			if (1)
			{
				const PxU32 nbContacts = getNbContacts();
				const Gu::ContactPoint* contacts = getContacts();
				for (PxU32 j = 0; j<nbContacts; j++)
				{
					DrawFrame(contacts[j].point, 1.0f);
				}
			}

			PxVec3 color(1, 1, 1);
			renderGeoms(getNbGeoms(), getGeoms(), getGeomPoses(), true, color, textureHandle);

			app->swapBuffer();
		} while (!app->m_window->requestedExit());

		delete app;

	}
	return 0;
}
