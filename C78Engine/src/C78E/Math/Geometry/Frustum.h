#pragma once
#include "Plane.h"

namespace C78E::Math {

    /**
     * @brief Frustum, defined by 2*dim Planes, constraining a dim dimensional Space
     * @tparam dim Dimensioniality
     */
    template<Dimension dim> 
    struct Frustum {
    public:
		using vecd = vec<dim>; // a Vector in a dim dimensional space has dim rows and 1 column
        using matd = mat<dim + 1>; // a Transform Matrix in a dim dimensional space has dim rows and colums for scale, rotation,.. and 1 for translation
		using VectorD = Vector<dim>; // a Vector in a dim dimensional space has dim rows and 1 column
        using FrustumPlane = Plane<dim>; // Frustum planes are of the same Dimension -> normal vector of dim and a scalar distance
		using FrustumPlaneVector = vec<dim + 1>; // every FrustumPlane is defined by a normal vector of dim and a scalar distance
        using FrustumDimensionPlanes = std::pair<Plane<dim>, Plane<dim>>; // every Dimension is constrained by 2 Planes
        using FrustumPlanes = Planes<dim, 2 * dim>; // every Dimension is constrained by 2 Planes
	public:
        Frustum() = default;
        Frustum(const matd& projectionMatrix, const matd& viewMatrix) : m_ProjectionMatrix(projectionMatrix), m_ViewMatrix(viewMatrix), m_Planes() {
            m_Planes = buildPlanes(projectionMatrix * viewMatrix);
        }
        Frustum(Frustum&) = default;
        Frustum(const Frustum&) = default;
        ~Frustum() = default;

        /**
         * @brief returns a single FrustumPlane given its index, FrustumPlanes are sorted by Dimension, from negative to positive
         *  e.g. 0 = Left(-X); 1 = Right(+X); 2 = Bottom(-Y); 3 = Top(+Y); 4 = Near(-Z); 5 = Far(+Z); and thats were human imaginaton stops working
         * @param planeIndex the index of the FrustumPlane
         * @return the FrustumPlane
         */
        inline const FrustumPlane& getPlane(size_t planeIndex) const {
            return m_Planes[planeIndex];
        }
        inline const FrustumPlanes& getPlanes() const {
            return m_Planes;
        }

        inline matd getProjectionMatrix() const {
            return m_ProjectionMatrix;
        }
        inline matd getViewMatrix() const {
            return m_ViewMatrix;
        }

	private:
        static FrustumPlanes buildPlanes(const matd& viewProjectionMatrix) {
            FrustumPlanes planes;
            for(Dimension dimension = 0; dimension < dim; dimension++) {
                FrustumPlaneVector negPlaneVector = FrustumPlaneVector(0.f);
                FrustumPlaneVector posPlaneVector = FrustumPlaneVector(0.f);

                // Extraction yields the plane normal and distance v = (n, d)
                // with a Plane defined as nT * x + d = 0, with all Points x on the Plane
                // but since Plane uses nT * x = d -> nT * x - d = 0, d(de) = -de;
                // also Gribb & Hartmann provides outward facing normals -> flip them

                for(Dimension i = 0; i < dim + 1; i++) {
                    // n(ne) = -ne; TODO: check why, this works?
                    negPlaneVector[i] = viewProjectionMatrix[i][dim] + viewProjectionMatrix[i][dimension];
                    posPlaneVector[i] = viewProjectionMatrix[i][dim] - viewProjectionMatrix[i][dimension];
                }

				// d(de) = -de;
                const scalar negD = -negPlaneVector[dim];
                const scalar posD = -posPlaneVector[dim];

				planes[2 * dimension + 0] = FrustumPlane(vecd(negPlaneVector), negD);
                planes[2 * dimension + 1] = FrustumPlane(vecd(posPlaneVector), posD);
            }
            return planes;
		}

    private:
        matd m_ProjectionMatrix = matd(1.f);
        matd m_ViewMatrix = matd(1.f);
        FrustumPlanes m_Planes; // in world space, or whatever viewMatrix comes from
	};


	template<Dimension dim>
	using FrustumPointIntersection = Intersection<dim, Point<dim>>;

	template<Dimension dim>
	static FrustumPointIntersection<dim> isInside(const Frustum<dim>& frustum, const Point<dim>& position) {
		bool isIdentical = false;
		for(auto& plane : frustum.getPlanes()) {
			PlanePointIntersection<dim> planeIntersection = isInFrontOf(plane, position);
			if(planeIntersection.hasNone()) return FrustumPointIntersection<dim>::createNone(); // outside of any Plane means outside of the Frustum
			if(planeIntersection.has(PlanePointIntersection<dim>::Type::Identical)) isIdentical = true; // point on the plane, makes it only Identical if it is in front of all planes
			// needs to be in front of all planes to be inside the frustum
		}
		if(isIdentical) {
			return FrustumPointIntersection<dim>::createIdentical();
		} else {
			return FrustumPointIntersection<dim>::createIntersection(position);
		}
	}

	template<Dimension dim>
	using FrustumPlaneIntersection = Intersection<dim, Line<dim>>;

	template<Dimension dim>
	static FrustumPlaneIntersection<dim> intersection(const Frustum<dim>& frustum, const Plane<dim>& plane, std::array<vec<dim>, 1 << (dim - 1)>& intersectionVertecies, size_t& count) {
		count = 0;
		for(auto& fplane : frustum.getPlanes()) {
			if(fplane == plane) return FrustumPlaneIntersection<dim>::createIdentical();
		}
		for(size_t i = 0; i < 2 * dim; i++) {
			for(size_t j = i + 1; j < 2 * dim; j++) {
				// yup, just gonna borrow a dimension
				Plane<dim + 1> plane1 = Plane<dim + 1>(vec<dim + 1>(frustum.getPlane(i).getNormal(), 0.f), frustum.getPlane(i).getDistance());
				Plane<dim + 1> plane2 = Plane<dim + 1>(vec<dim + 1>(frustum.getPlane(j).getNormal(), 0.f), frustum.getPlane(j).getDistance());

				PlanePlaneIntersection<dim + 1> planePlaneIntersiontion = intersection(plane1, plane2);
				if(!planePlaneIntersiontion.hasNone()) { // can't be Identical -> just for Frustum(0.f) which shouldn't intersect with anything!
					Plane<dim + 1> plane3 = Plane<dim + 1>(vec<dim + 1>(plane.getNormal(), 0.f), plane.getDistance());

					PlaneLineIntersection<dim + 1> planeLineIntersection = intersection(planePlaneIntersiontion.getResult(), plane3);
					if(!planeLineIntersection.hasNone()) {
						if(planeLineIntersection.isIdentical()) {
							if(glm::abs(planePlaneIntersiontion.getResult().getDirection()[dim]) == 1.f) {
								intersectionVertecies[count] = planeLineIntersection.getResult().getPosition();
								count++;
							} else {
								//C78E_CORE_ASSERT(false, "unhandled!");
							}
						} else if(!planeLineIntersection.hasNone()) { // assure that point is on/in the frustum
							intersectionVertecies[count] = planeLineIntersection.getResult().getPosition();
							count++;
						}
					}
				}
			}
		}
		//was (Type)count;
		return FrustumPlaneIntersection<dim>::createNone(); //TODO
	}

	//TODO: see if theres anything useful in here
	//static Type intersect(const FrustumD& frustum, const PlaneD& plane, std::array<vecd, 1 << (dim - 1)>& intersectionVertices, size_t& count) {
	//	count = 0;
	//	for(auto& fplane : frustum.getPlanes()) {
	//		if(fplane == plane) return Type::Identical;
	//	}
	//
	//	constexpr size_t numPlanes = 2 * dim; // Number of frustum planes
	//	std::array<Plane<dim + 1>, numPlanes + 1> planes;
	//
	//	// Convert frustum planes to (dim+1)D representation
	//	for(size_t i = 0; i < numPlanes; i++) {
	//		planes[i] = Plane<dim + 1>(vec<dim + 1>(frustum.getPlane(i).getNormal(), 0.f), frustum.getPlane(i).getDistance());
	//	}
	//
	//	// Include the given plane
	//	planes[numPlanes] = Plane<dim + 1>(vec<dim + 1>(plane.getNormal(), 0.f), plane.getDistance());
	//
	//	// Find intersections of dim planes
	//	size_t intersectionCount = 0;
	//	std::array<size_t, dim> indices;
	//	for(size_t i = 0; i < dim; i++) {
	//		indices[i] = i;
	//	}
	//
	//	do {
	//		vec<dim + 1> intersection;
	//		if(Intersection<dim + 1>::intersect(planes[indices[0]], planes[indices[1]], intersection)) {
	//			for(size_t d = 2; d < dim; d++) {
	//				if(!Intersection<dim + 1>::intersect(planes[indices[d]], intersection)) {
	//					break;
	//				}
	//			}
	//			vecd nativeIntersection = vecd(intersection);
	//			if(Intersection<dim>::intersect(frustum, nativeIntersection)) {
	//				intersectionVertices[intersectionCount++] = nativeIntersection;
	//			}
	//		}
	//	} while(std::next_permutation(indices.begin(), indices.end()) && intersectionCount < (1 << (dim - 1)));
	//
	//	return static_cast<Type>(count);
	//}

}