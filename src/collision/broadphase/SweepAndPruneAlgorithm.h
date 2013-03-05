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

#ifndef SWEEP_AND_PRUNE_ALGORITHM_H
#define SWEEP_AND_PRUNE_ALGORITHM_H

// Libraries
#include "BroadPhaseAlgorithm.h"
#include "../../collision/shapes/AABB.h"
#include <map>
#include <vector>


/// Namespace ReactPhysics3D
namespace reactphysics3d {
   
// Structure EndPoint
/**
 * EndPoint structure that represent an end-point of an AABB
 * on one of the three x,y or z axis.
 */
struct EndPoint {

    public:
        
        /// ID of the AABB box corresponding to this end-point
        bodyindex boxID;

        /// True if the end-point is a minimum end-point of a box
        bool isMin;

        /// Value (one dimension coordinate) of the end-point
        uint value;
        
        /// Set the values of the endpoint
        void setValues(bodyindex boxID, bool isMin, uint value) {
            this->boxID = boxID;
            this->isMin = isMin;
            this->value = value;
        }
};            

// Structure BoxAABB
/**
 * This structure represents an AABB in the Sweep-And-Prune algorithm
 */
struct BoxAABB {

    public:

        /// Index of the 3 minimum end-points of the AABB over the x,y,z axis
        bodyindex min[3];

        /// Index of the 3 maximum end-points of the AABB over the x,y,z axis
        bodyindex max[3];

        /// Body that corresponds to the owner of the AABB
        CollisionBody* body;
};

// Structure AABBInt
/**
 * Axis-Aligned Bounding box with integer coordinates.
 */
struct AABBInt {

    public:

        /// Minimum values on the three axis
        uint min[3];

        /// Maximum values on the three axis
        uint max[3];
        
        /// Constructor
        AABBInt(const AABB& aabb);
};

// Class SweepAndPruneAlgorithm
/**
 * This class implements the Sweep-And-Prune (SAP) broad-phase
 * collision detection algorithm. This class implements an
 * array-based implementation of the algorithm from Pierre Terdiman
 * that is described here : www.codercorner.com/SAP.pdf.
 */
class SweepAndPruneAlgorithm : public BroadPhaseAlgorithm {
    
    protected :

        // -------------------- Attributes -------------------- //

        /// Invalid array index
        static bodyindex INVALID_INDEX;

        /// Array that contains all the AABB boxes of the broad-phase
        BoxAABB* mBoxes;

        /// Array of end-points on the three axis
        EndPoint* mEndPoints[3];

        /// Number of AABB boxes in the broad-phase
        bodyindex mNbBoxes;

        /// Max number of boxes in the boxes array
        bodyindex mNbMaxBoxes;

        /// Indices that are not used by any boxes
        std::vector<bodyindex> mFreeBoxIndices;

        /// Map a body pointer to a box index
        std::map<CollisionBody*,bodyindex> mMapBodyToBoxIndex;
        
        // -------------------- Methods -------------------- //

        /// Private copy-constructor
        SweepAndPruneAlgorithm(const SweepAndPruneAlgorithm& algorithm);

        /// Private assignment operator
        SweepAndPruneAlgorithm& operator=(const SweepAndPruneAlgorithm& algorithm);

        /// Resize the boxes and end-points arrays when it's full
        void resizeArrays();

        /// Add an overlapping pair of AABBS
        void addPair(CollisionBody* body1, CollisionBody* body2);

        /// Check for 1D box intersection between two boxes that are sorted on the given axis.
        bool testIntersect1DSortedAABBs(const BoxAABB& box1, const AABBInt& box2,
                                        const EndPoint* const baseEndPoint, uint axis) const;

        /// Check for 2D box intersection.
        bool testIntersect2D(const BoxAABB& box1, const BoxAABB& box2,
                             luint axis1, uint axis2) const;
        
    public :

        // -------------------- Methods -------------------- //

        /// Constructor
        SweepAndPruneAlgorithm(CollisionDetection& mCollisionDetection);

        /// Destructor
        virtual ~SweepAndPruneAlgorithm();

        /// Notify the broad-phase about a new object in the world.
        virtual void addObject(CollisionBody* body, const AABB& aabb);

        /// Notify the broad-phase about a object that has been removed from the world
        virtual void removeObject(CollisionBody* body);

        /// Notify the broad-phase that the AABB of an object has changed
        virtual void updateObject(CollisionBody* body, const AABB& aabb);
};

/// Encode a floating value into a integer value in order to
/// work with integer comparison in the Sweep-And-Prune algorithm
/// because it is faster. The main issue when encoding floating
/// number into integer is to keep to sorting order. This is a
/// problem for negative float number. This article describes
/// how to solve this issue : http://www.stereopsis.com/radix.html
inline uint encodeFloatIntoInteger(float number) {
    uint intNumber = (uint&) number;
    
    // If it's a negative number
    if(intNumber & 0x80000000)
        intNumber = ~intNumber;
    else {     // If it is a positive number
        intNumber |= 0x80000000;
    }
     
    return intNumber;
}                

        
// Check for 1D box intersection between two boxes that are sorted on the given axis.
/// Therefore, only one test is necessary here. We know that the
/// minimum of box1 cannot be larger that the maximum of box2 on the axis.
inline bool SweepAndPruneAlgorithm::testIntersect1DSortedAABBs(const BoxAABB& box1,
                                                               const AABBInt& box2,
                                                               const EndPoint* const endPointsArray,
                                                               uint axis) const {
    return !(endPointsArray[box1.max[axis]].value < box2.min[axis]);
}    

// Check for 2D box intersection. This method is used when we know
/// that two boxes already overlap on one axis and when want to test
/// if they also overlap on the two others axis.
inline bool SweepAndPruneAlgorithm::testIntersect2D(const BoxAABB& box1, const BoxAABB& box2,
                                                    luint axis1, uint axis2) const {
    return !(box2.max[axis1] < box1.min[axis1] || box1.max[axis1] < box2.min[axis1] ||
             box2.max[axis2] < box1.min[axis2] || box1.max[axis2] < box2.min[axis2]);
}          

}

#endif


