/*
 * PURPOSE:
 *   Class representing an articulated rigid body. Stores the body's
 *   current state, allows forces and torques to be set, handles
 *   timestepping and implements Featherstone's algorithm.
 *   
 * COPYRIGHT:
 *   Copyright (C) Stephen Thompson, <stephen@solarflare.org.uk>, 2011-2013
 *   Portions written By Erwin Coumans: connection to LCP solver, various multibody constraints, replacing Eigen math library by Bullet LinearMath and a dedicated 6x6 matrix inverse (solveImatrix)
 *   Portions written By Jakub Stepien: support for multi-DOF constraints, introduction of spatial algebra and several other improvements

 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the use of this software.
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely,
 subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 
 */

#ifndef BT_MULTIBODY_H
#define BT_MULTIBODY_H

#include "LinearMath/btScalar.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btQuaternion.h"
#include "LinearMath/btMatrix3x3.h"
#include "LinearMath/btAlignedObjectArray.h"

///serialization data, don't change them if you are not familiar with the details of the serialization mechanisms
#ifdef BT_USE_DOUBLE_PRECISION
#define btMultiBodyData btMultiBodyDoubleData
#define btMultiBodyDataName "btMultiBodyDoubleData"
#define btMultiBodyLinkData btMultiBodyLinkDoubleData
#define btMultiBodyLinkDataName "btMultiBodyLinkDoubleData"
#else
#define btMultiBodyData btMultiBodyFloatData
#define btMultiBodyDataName "btMultiBodyFloatData"
#define btMultiBodyLinkData btMultiBodyLinkFloatData
#define btMultiBodyLinkDataName "btMultiBodyLinkFloatData"
#endif  //BT_USE_DOUBLE_PRECISION


#include "LinearMath/btTransform.h"

struct btBroadphaseProxy;
class btCollisionShape;
struct btCollisionShapeData;

typedef btAlignedObjectArray<class btCollisionObject*> btCollisionObjectArray;

#ifdef BT_USE_DOUBLE_PRECISION
#define btCollisionObjectData btCollisionObjectDoubleData
#define btCollisionObjectDataName "btCollisionObjectDoubleData"
#else
#define btCollisionObjectData btCollisionObjectFloatData
#define btCollisionObjectDataName "btCollisionObjectFloatData"
#endif

// clang-format off

///do not change those serialization structures, it requires an updated sBulletDNAstr/sBulletDNAstr64
struct	btCollisionObjectDoubleData
{
	void					*m_broadphaseHandle;
	void					*m_collisionShape;
	btCollisionShapeData	*m_rootCollisionShape;
	char					*m_name;

	btTransformDoubleData	m_worldTransform;
	btTransformDoubleData	m_interpolationWorldTransform;
	btVector3DoubleData		m_interpolationLinearVelocity;
	btVector3DoubleData		m_interpolationAngularVelocity;
	btVector3DoubleData		m_anisotropicFriction;
	double					m_contactProcessingThreshold;	
	double					m_deactivationTime;
	double					m_friction;
	double					m_rollingFriction;
	double                  m_contactDamping;
	double                  m_contactStiffness;
	double					m_restitution;
	double					m_hitFraction; 
	double					m_ccdSweptSphereRadius;
	double					m_ccdMotionThreshold;
	int						m_hasAnisotropicFriction;
	int						m_collisionFlags;
	int						m_islandTag1;
	int						m_companionId;
	int						m_activationState1;
	int						m_internalType;
	int						m_checkCollideWith;
	int						m_collisionFilterGroup;
	int						m_collisionFilterMask;
	int						m_uniqueId;//m_uniqueId is introduced for paircache. could get rid of this, by calculating the address offset etc.
};

///do not change those serialization structures, it requires an updated sBulletDNAstr/sBulletDNAstr64
struct	btCollisionObjectFloatData
{
	void					*m_broadphaseHandle;
	void					*m_collisionShape;
	btCollisionShapeData	*m_rootCollisionShape;
	char					*m_name;

	btTransformFloatData	m_worldTransform;
	btTransformFloatData	m_interpolationWorldTransform;
	btVector3FloatData		m_interpolationLinearVelocity;
	btVector3FloatData		m_interpolationAngularVelocity;
	btVector3FloatData		m_anisotropicFriction;
	float					m_contactProcessingThreshold;	
	float					m_deactivationTime;
	float					m_friction;
	float					m_rollingFriction;
	float                   m_contactDamping;
    float                   m_contactStiffness;
	float					m_restitution;
	float					m_hitFraction; 
	float					m_ccdSweptSphereRadius;
	float					m_ccdMotionThreshold;
	int						m_hasAnisotropicFriction;
	int						m_collisionFlags;
	int						m_islandTag1;
	int						m_companionId;
	int						m_activationState1;
	int						m_internalType;
	int						m_checkCollideWith;
	int						m_collisionFilterGroup;
	int						m_collisionFilterMask;
	int						m_uniqueId;
};
// clang-format on


struct btMultiBodyLinkDoubleData
{
	btQuaternionDoubleData m_zeroRotParentToThis;
	btVector3DoubleData m_parentComToThisPivotOffset;
	btVector3DoubleData m_thisPivotToThisComOffset;
	btVector3DoubleData m_jointAxisTop[6];
	btVector3DoubleData m_jointAxisBottom[6];

	btVector3DoubleData m_linkInertia;  // inertia of the base (in local frame; diagonal)
	btVector3DoubleData m_absFrameTotVelocityTop;
	btVector3DoubleData m_absFrameTotVelocityBottom;
	btVector3DoubleData m_absFrameLocVelocityTop;
	btVector3DoubleData m_absFrameLocVelocityBottom;

	double m_linkMass;
	int m_parentIndex;
	int m_jointType;

	int m_dofCount;
	int m_posVarCount;
	double m_jointPos[7];
	double m_jointVel[6];
	double m_jointTorque[6];

	double m_jointDamping;
	double m_jointFriction;
	double m_jointLowerLimit;
	double m_jointUpperLimit;
	double m_jointMaxForce;
	double m_jointMaxVelocity;

	char *m_linkName;
	char *m_jointName;
	btCollisionObjectDoubleData *m_linkCollider;
	char *m_paddingPtr;
};

struct btMultiBodyLinkFloatData
{
	btQuaternionFloatData m_zeroRotParentToThis;
	btVector3FloatData m_parentComToThisPivotOffset;
	btVector3FloatData m_thisPivotToThisComOffset;
	btVector3FloatData m_jointAxisTop[6];
	btVector3FloatData m_jointAxisBottom[6];
	btVector3FloatData m_linkInertia;  // inertia of the base (in local frame; diagonal)
	btVector3FloatData m_absFrameTotVelocityTop;
	btVector3FloatData m_absFrameTotVelocityBottom;
	btVector3FloatData m_absFrameLocVelocityTop;
	btVector3FloatData m_absFrameLocVelocityBottom;

	int m_dofCount;
	float m_linkMass;
	int m_parentIndex;
	int m_jointType;

	float m_jointPos[7];
	float m_jointVel[6];
	float m_jointTorque[6];
	int m_posVarCount;
	float m_jointDamping;
	float m_jointFriction;
	float m_jointLowerLimit;
	float m_jointUpperLimit;
	float m_jointMaxForce;
	float m_jointMaxVelocity;

	char *m_linkName;
	char *m_jointName;
	btCollisionObjectFloatData *m_linkCollider;
	char *m_paddingPtr;
};

///do not change those serialization structures, it requires an updated sBulletDNAstr/sBulletDNAstr64
struct btMultiBodyDoubleData
{
	btVector3DoubleData m_baseWorldPosition;
	btQuaternionDoubleData m_baseWorldOrientation;
	btVector3DoubleData m_baseLinearVelocity;
	btVector3DoubleData m_baseAngularVelocity;
	btVector3DoubleData m_baseInertia;  // inertia of the base (in local frame; diagonal)
	double m_baseMass;
	int m_numLinks;
	char m_padding[4];

	char *m_baseName;
	btMultiBodyLinkDoubleData *m_links;
	btCollisionObjectDoubleData *m_baseCollider;
};

///do not change those serialization structures, it requires an updated sBulletDNAstr/sBulletDNAstr64
struct btMultiBodyFloatData
{
	btVector3FloatData m_baseWorldPosition;
	btQuaternionFloatData m_baseWorldOrientation;
	btVector3FloatData m_baseLinearVelocity;
	btVector3FloatData m_baseAngularVelocity;

	btVector3FloatData m_baseInertia;  // inertia of the base (in local frame; diagonal)
	float m_baseMass;
	int m_numLinks;

	char *m_baseName;
	btMultiBodyLinkFloatData *m_links;
	btCollisionObjectFloatData *m_baseCollider;
};

#endif
