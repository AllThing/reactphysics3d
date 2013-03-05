/********************************************************************************
* ReactPhysics3D physics library, http://code.google.com/p/reactphysics3d/      *
* Copyright (c) 2010-2013 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

#ifndef CONTACT_POINT_H
#define CONTACT_POINT_H

// Libraries
#include "Constraint.h"
#include "../collision/ContactInfo.h"
#include "../body/RigidBody.h"
#include "../configuration.h"
#include "../mathematics/mathematics.h"
#include "../memory/MemoryPool.h"
#include "../configuration.h"

#if defined(VISUAL_DEBUG)
	#if defined(APPLE_OS)
		#include <GLUT/glut.h>
		#include <OpenGL/gl.h>
	#elif defined(WINDOWS_OS)
		#include <GL/glut.h>
		#include <GL/gl.h>
	#elif defined(LINUX_OS)
        #include <GL/freeglut.h>
        #include <GL/gl.h>
    #endif
#endif

/// ReactPhysics3D namespace
namespace reactphysics3d {

// Class ContactPoint
/**
 * This class represents a collision contact point between two
 * bodies in the physics engine. The ContactPoint class inherits from
 * the Constraint class.
 */
class ContactPoint : public Constraint {

    protected :

        // -------------------- Attributes -------------------- //

        /// Normal vector of the contact (From body1 toward body2) in world space
        const Vector3 mNormal;

        /// Penetration depth
        decimal mPenetrationDepth;

        /// Contact point on body 1 in local space of body 1
        const Vector3 mLocalPointOnBody1;

        /// Contact point on body 2 in local space of body 2
        const Vector3 mLocalPointOnBody2;

        /// Contact point on body 1 in world space
        Vector3 mWorldPointOnBody1;

        /// Contact point on body 2 in world space
        Vector3 mWorldPointOnBody2;

        /// True if the contact is a resting contact (exists for more than one time step)
        bool mIsRestingContact;

        /// Two orthogonal vectors that span the tangential friction plane
        std::vector<Vector3> mFrictionVectors;
        
        // -------------------- Methods -------------------- //

        /// Private copy-constructor
        ContactPoint(const ContactPoint& contact);

        /// Private assignment operator
        ContactPoint& operator=(const ContactPoint& contact);

    public :

        // -------------------- Methods -------------------- //

        /// Constructor
        ContactPoint(RigidBody* const body1, RigidBody* const body2, const ContactInfo* contactInfo);

        /// Destructor
        virtual ~ContactPoint();

        /// Return the normal vector of the contact
        Vector3 getNormal() const;

        /// Set the penetration depth of the contact
        void setPenetrationDepth(decimal penetrationDepth);

        /// Return the contact local point on body 1
        Vector3 getLocalPointOnBody1() const;

        /// Return the contact local point on body 2
        Vector3 getLocalPointOnBody2() const;

        /// Return the contact world point on body 1
        Vector3 getWorldPointOnBody1() const;

        /// Return the contact world point on body 2
        Vector3 getWorldPointOnBody2() const;

        /// Set the contact world point on body 1
        void setWorldPointOnBody1(const Vector3& worldPoint);

        /// Set the contact world point on body 2
        void setWorldPointOnBody2(const Vector3& worldPoint);

        /// Return true if the contact is a resting contact
        bool getIsRestingContact() const;

        /// Set the mIsRestingContact variable
        void setIsRestingContact(bool isRestingContact);

        /// Get the first friction vector
        Vector3 getFrictionVector1() const;

        /// Set the first friction vector
        void setFrictionVector1(const Vector3& frictionVector1);

        /// Get the second friction vector
        Vector3 getFrictionVector2() const;

        /// Set the second friction vector
        void setFrictionVector2(const Vector3& frictionVector2);

        /// Return the penetration depth
        decimal getPenetrationDepth() const;

        #ifdef VISUAL_DEBUG
            /// Draw the contact (for debugging)
           void draw() const;
        #endif
};

// Return the normal vector of the contact
inline Vector3 ContactPoint::getNormal() const {
    return mNormal;
}

// Set the penetration depth of the contact
inline void ContactPoint::setPenetrationDepth(decimal penetrationDepth) {
    this->mPenetrationDepth = penetrationDepth;
}

// Return the contact point on body 1
inline Vector3 ContactPoint::getLocalPointOnBody1() const {
    return mLocalPointOnBody1;
}

// Return the contact point on body 2
inline Vector3 ContactPoint::getLocalPointOnBody2() const {
    return mLocalPointOnBody2;
}

// Return the contact world point on body 1
inline Vector3 ContactPoint::getWorldPointOnBody1() const {
    return mWorldPointOnBody1;
}

// Return the contact world point on body 2
inline Vector3 ContactPoint::getWorldPointOnBody2() const {
    return mWorldPointOnBody2;
}

// Set the contact world point on body 1
inline void ContactPoint::setWorldPointOnBody1(const Vector3& worldPoint) {
    mWorldPointOnBody1 = worldPoint;
}

// Set the contact world point on body 2
inline void ContactPoint::setWorldPointOnBody2(const Vector3& worldPoint) {
    mWorldPointOnBody2 = worldPoint;
}

// Return true if the contact is a resting contact
inline bool ContactPoint::getIsRestingContact() const {
    return mIsRestingContact;
}

// Set the mIsRestingContact variable
inline void ContactPoint::setIsRestingContact(bool isRestingContact) {
    mIsRestingContact = isRestingContact;
}

// Get the first friction vector
inline Vector3 ContactPoint::getFrictionVector1() const {
    return mFrictionVectors[0];
}

// Set the first friction vector
inline void ContactPoint::setFrictionVector1(const Vector3& frictionVector1) {
    mFrictionVectors[0] = frictionVector1;
}

// Get the second friction vector
inline Vector3 ContactPoint::getFrictionVector2() const {
    return mFrictionVectors[1];
}

// Set the second friction vector
inline void ContactPoint::setFrictionVector2(const Vector3& frictionVector2) {
    mFrictionVectors[1] = frictionVector2;
}

// Return the penetration depth of the contact
inline decimal ContactPoint::getPenetrationDepth() const {
    return mPenetrationDepth;
}


#ifdef VISUAL_DEBUG
inline void ContactPoint::draw() const {
    glColor3f(1.0, 0.0, 0.0);
    glutSolidSphere(0.3, 20, 20);
}
#endif 

}

#endif
