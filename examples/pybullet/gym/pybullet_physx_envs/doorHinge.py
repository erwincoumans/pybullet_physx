import matplotlib.pyplot as plt
import pybullet_physx as p
import pybullet_physx_data as pd
import time
import math
import numpy as np  #to reshape for matplotlib

plt.ion()

img = [[1, 2, 3] * 50] * 100  #np.random.rand(200, 320)
#img = [tandard_normal((50,100))
image = plt.imshow(img, interpolation='none', animated=True, label="blah")
ax = plt.gca()

camTargetPos = [0, 0, 0]
cameraUp = [0, 0, 1]
cameraPos = [1, 1, 1]

pitch = -10.0

roll = 0
upAxisIndex = 2
camDistance = 14
pixelWidth = 320
pixelHeight = 200
nearPlane = 0.01
farPlane = 100

fov = 60



usePhysX = True
useMaximalCoordinates = True
if usePhysX:
  p.connect(p.PhysX,options="--numCores=8 --solver=pgs")
  p.loadPlugin("eglRendererPlugin")
else:
  p.connect(p.GUI)

p.setPhysicsEngineParameter(fixedTimeStep=1./240.,numSolverIterations=4, minimumSolverIslandSize=1024)
p.setPhysicsEngineParameter(contactBreakingThreshold=0.01)

p.setAdditionalSearchPath(pd.getDataPath())
#Always make ground plane maximal coordinates, to avoid performance drop in PhysX
#See https://github.com/NVIDIAGameWorks/PhysX/issues/71

p.loadURDF("plane.urdf", useMaximalCoordinates=True)#useMaximalCoordinates)
p.configureDebugVisualizer(p.COV_ENABLE_TINY_RENDERER,0)
p.configureDebugVisualizer(p.COV_ENABLE_RENDERING,0)
logId = p.startStateLogging(p.STATE_LOGGING_PROFILE_TIMINGS,"physx_door_hinge.json")

#p.changeDynamics(sphere ,-1, mass=1000)

door = p.loadURDF("door.urdf",[0,0,0.05])
p.changeDynamics(door ,1, linearDamping=0, angularDamping=0, jointDamping=0, mass=1)
print("numJoints = ", p.getNumJoints(door))


p.setGravity(0,0,-10)
position_control = True

angle = math.pi*0.25
p.resetJointState(door,1,angle)  
angleread = p.getJointState(door,1)
print("angleread = ",angleread)
prevTime = time.time()

angle = math.pi*0.5

count=0

for k in range(3):
  for yaw in range(0, 360, 50):
    p.stepSimulation()
    start = time.time()
    viewMatrix = p.computeViewMatrixFromYawPitchRoll(camTargetPos, camDistance, yaw, pitch,
                                                            roll, upAxisIndex)
    aspect = pixelWidth / pixelHeight
    projectionMatrix = p.computeProjectionMatrixFOV(fov, aspect, nearPlane, farPlane)
    #print("viewMatrix=",viewMatrix)
    #print("projMatrix=",projectionMatrix)
    img_arr = p.getCameraImage(pixelWidth,
                                      pixelHeight,
                                      viewMatrix,
                                      projectionMatrix,
                                      shadow=1,
                                      lightDirection=[1, 1, 1],
                                      renderer=p.ER_BULLET_HARDWARE_OPENGL)
    stop = time.time()
    print("renderImage %f" % (stop - start))

    w = img_arr[0]  #width of the image, in pixels
    h = img_arr[1]  #height of the image, in pixels
    rgb = img_arr[2]  #color data RGB
    dep = img_arr[3]  #depth data
    #print(rgb)
    print('width = %d height = %d' % (w, h))

    #note that sending the data using imshow to matplotlib is really slow, so we use set_data

    #plt.imshow(rgb,interpolation='none')

    #reshape is needed
    np_img_arr = np.reshape(rgb, (h, w, 4))
    np_img_arr = np_img_arr * (1. / 255.)

    image.set_data(np_img_arr)
    ax.plot([0])
    #plt.draw()
    #plt.show()
    plt.pause(0.01)


while (1):
  count+=1
  if (count==12):
      p.stopStateLogging(logId)
      p.configureDebugVisualizer(p.COV_ENABLE_RENDERING,1)

  curTime = time.time()
  
  diff = curTime - prevTime
  #every second, swap target angle
  if (diff>1):
    angle = - angle
    prevTime = curTime
  
  if position_control:
    p.setJointMotorControl2(door,1,p.POSITION_CONTROL, targetPosition = angle, positionGain=10.1, velocityGain=1, force=11.001)
  else:  
    p.setJointMotorControl2(door,1,p.VELOCITY_CONTROL, targetVelocity=1, force=1011)
  contacts = p.getContactPoints()
  print("contacts=",contacts)
  p.stepSimulation()
  time.sleep(1./240.)
