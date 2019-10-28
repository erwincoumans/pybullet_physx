import pybullet_physx as p
import pybullet_physx_data as pd
import time
import math

usePhysX = True
useMaximalCoordinates = True
if usePhysX:
  p.connect(p.PhysX,options="--numCores=1 --solver=tgs")#can also be pgs
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
logId = p.startStateLogging(p.STATE_LOGGING_PROFILE_TIMINGS,"physx_create_dominoes.json")
jran = 50
iran = 100

orn=[0,0,0,1]
pos = [0,0,0.03]

spheres=[]
num_spheres=10
for i in range (num_spheres):
  pos = [0,0,0.03+i*0.06]#
  if i == num_spheres-1:
    sphere = p.loadURDF("sphere_small_heavy.urdf",pos, orn, useMaximalCoordinates=useMaximalCoordinates)
  else:
    sphere = p.loadURDF("sphere_small.urdf",pos, orn, useMaximalCoordinates=useMaximalCoordinates)
  spheres.append(sphere)  
print("loaded!")


#p.changeDynamics(sphere ,-1, mass=1000)


p.setGravity(0,0,-10)
position_control = True

angle = math.pi*0.25
prevTime = time.time()

angle = math.pi*0.5

count=0
while (1):
  #for s in spheres:
  #  print("sphere: ",s)
  #  #print(p.getBasePositionAndOrientation(s))

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
  
  #contacts = p.getContactPoints()
  #print("contacts=",contacts)
  p.stepSimulation()
  #time.sleep(1./240.)
