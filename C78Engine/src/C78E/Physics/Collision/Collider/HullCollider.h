#pragma once
#include <C78E/Math/Math.h>
#include "AABBCollider.h"

namespace C78E::Physics {

	/**
	 * @brief HullCollider, for convex Meshes
	 * @tparam dim 
	 */
	template<Dimension dim>
	struct HullCollider : public AABBCollider<dim> {
		using vecd = vec<dim>;
		using matd = mat<dim + 1>;
		using PointD = Point<dim>;
		using VectorD = Vector<dim>;
		template<PointsCount count>
		using PointsD = Points<dim, count>;
		template<VectorsCount count>
		using VectorsD = Vectors<dim, count>;
		using DynamicPointsD = DynamicPoints<dim>;
		using DynamicVectorsD = DynamicVectors<dim>;
		using QuadD = Quad<dim>;
		using AABBD = AABB<dim>;
		using ColliderD = Collider<dim>;
		using AABBColliderD = AABBCollider<dim>;
		using TransformD = Transform<dim>;
	public:
		template<PointsCount count>
		static HullCollider<dim> createStatic(const PointsD<count>& points) {
			HullCollider<dim> collider;
			collider.m_HullPoints.reserve(count);
			for(size_t i = 0; i < count; i++) {
				collider.addPoint(points[i]);
			}
			//TODO: check convexity!
			return collider;
		}
		static HullCollider<dim> createTriangle() {
			return createStatic(Triangle<dim>::getUnitTriangle());
		}
		static HullCollider<dim> createSquare() {
			return createStatic(QuadD::getUnitQuadPoints());
		}
		static HullCollider<dim> createTetrahedon() {
			return createStatic(Tetrahedon<dim>::getUnitTetrahedon());
		}
		static HullCollider<dim> createCube() {
			return createStatic(Cube<dim>::getUnitCube());
		}

	
	public:
		HullCollider() = default;
		HullCollider(const DynamicPointsD& points) : AABBColliderD(), m_HullPoints(points) {
			calcBounds();
		}
		HullCollider(HullCollider&) = default;
		HullCollider(const HullCollider&) = default;
		~HullCollider() = default;

		virtual void addPoint(const PointD& p) {
			m_HullPoints.push_back(p);
			AABBD::growToInclude(p);
		}
		virtual void removePoint(const PointD& p) {
			m_HullPoints.erase(std::find(m_HullPoints.begin(), m_HullPoints.end(), p));
			calcBounds();
		}

		virtual const DynamicPointsD& getPoints() const {
			return m_HullPoints;
		}

		AABBD& calcBounds() {
			AABBD::reset();
			for(const PointD& point : m_HullPoints) {
				AABBD::growToInclude(point);
			}
			return *this;
		}

		/**
		 * @brief calculates the nearest surface point on the Mesh to the given point
		 * @param transformToPointSpace the Meshes Transform in the points space, probably world space -> the Meshes World Transform
		 * @param point the point to refer to
		 * @return the nearest surface point
		 */
		virtual PointD nearSurfacePoint(TransformD& transformToPointSpace, const PointD& point) const override {
			PointsD<dim> closestPoints; // closest Surface to point
			std::array<scalar, dim> closestDistances;
			closestDistances.fill(+scalar_limit::infinity());
			
			for(const PointD& hullPointLocal : m_HullPoints) {
				const PointD hullPoint = Math::transform(hullPointLocal, transformToPointSpace.toMat());
				const scalar pointDistance = (hullPoint - point).length();
				for(size_t i = 0; i < dim; i++) {
					if(pointDistance < closestDistances[i]) {
						for(size_t j = dim - 1; j > i; j--) {
							closestDistances[j] = closestDistances[j - 1];
							closestPoints[j] = closestPoints[j - 1];
						}
						closestDistances[i] = pointDistance;
						closestPoints[i] = hullPoint;
						break;
					}
				}
			}

			const VectorD pointSurfaceLocal = point - closestPoints[0];
			VectorsD<dim - 1> surfaceVectors;
			for(VectorsCount i = 0; i < dim - 1; i++) {
				surfaceVectors[i] = closestPoints[i + 1] - closestPoints[0];
			}
			const VectorD surfaceNormal = Math::cross<dim>(surfaceVectors);
			const VectorD pointSurfaceNormalComponentSurfaceLocal = pointSurfaceLocal.projectOn(surfaceNormal);

			VectorD nearSurfacePointSurfaceLocal = pointSurfaceLocal - pointSurfaceNormalComponentSurfaceLocal;

			for(VectorsCount i = 0; i < dim - 1; i++) {
				const scalar projection = nearSurfacePointSurfaceLocal.dot(surfaceVectors[i]) / surfaceVectors[i].lengthSquared();
				if(projection < 0.f) {
					nearSurfacePointSurfaceLocal += surfaceVectors[i] * -projection;
				} else if(projection > 1.f) {
					nearSurfacePointSurfaceLocal += surfaceVectors[i] * (1.f - projection);
				}
			}
			const PointD nearSurfacePoint = closestPoints[0] + nearSurfacePointSurfaceLocal;
			return nearSurfacePoint;
		}

		/**
		 * @brief 
		 * @param transformToVectorSpace 
		 * @param direction 
		 * @return 
		 */
		virtual PointD nearSurfacePoint(TransformD& transformToVectorSpace, const VectorD& direction) const override {
			const VectorD directionLocal = Math::transform(direction, transformToVectorSpace.toInvMat());

			PointD maxPoint = PointD(0.f);
			scalar maxDistance = -scalar_limit::infinity();
			for(const PointD& point : m_HullPoints) {
				const scalar distance = directionLocal.dot(point.getOriginVector());
				if(distance > maxDistance) {
					maxDistance = distance;
					maxPoint = point;
				}
			}
			return Math::transform(maxPoint, transformToVectorSpace.toMat());
		}

		virtual AABBD getBounds() const override {
			return AABBColliderD::getBounds();
		}

		virtual ColliderD::Type getType() const { return ColliderD::Type::Hull; }
	public:
		DynamicPointsD m_HullPoints;
	};

	template<Dimension dim>
	HullCollider<dim> transform(const HullCollider<dim>& collider, const Transform<dim>& transform) {
		return HullCollider<dim>(Math::transform(collider.getPoints(), transform.toMat()));
	}

	/**
	 * @brief DynamicHullColliders, 
	 * @tparam dim
	 */
	template<Dimension dim>
	struct DynamicHullColliders : public std::vector<HullCollider<dim>> {
	public:
		DynamicHullColliders() = default;
		DynamicHullColliders(std::initializer_list<HullCollider<dim>> hullColliders) : std::vector<HullCollider<dim>>() {
			this->reserve(hullColliders.size());
			for(const HullCollider<dim>& hullCollider : hullColliders) {
				this->emplace_back(hullCollider);
			}
		}
		DynamicHullColliders(DynamicHullColliders&) = default;
		DynamicHullColliders(const DynamicHullColliders&) = default;
		~DynamicHullColliders() = default;

		void addCollider(const HullCollider<dim>& collider) {
			this->emplace_back(collider);
		}
	};

	template<Dimension dim>
	DynamicHullColliders<dim> transform(const DynamicHullColliders<dim>& hullColliders, const Transform<dim>& transform) {
		DynamicHullColliders<dim> transformedHullColliders;
		transformedHullColliders.reserve(hullColliders.size());
		for(const HullCollider<dim>& hullCollider : hullColliders) {
			transformedHullColliders.emplace_back(Math::transform(hullCollider, transform));
		}
		return transformedHullColliders;
	}

}