#pragma once
#include <C78E/Math/Math.h>
#include <C78E/Physics/Collision/CollisionPoints.h>

namespace C78E::Physics {

	/**
	 * @brief Splitting Axes Theorem
	 * @tparam dim 2D or 3D
	 */

	template<Dimension dim>
		requires (dim >= 2)
	struct SAT {
	public:
		using vecd = vec<dim>;
		using matd = mat<dim + 1>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		template<PointsCount size>
		using Points = Points<dim, size>;
		template<VectorsCount size>
		using Vectors = Vectors<dim, size>;
	public:
		template<PointsCount vertexCountA, VectorsCount axisCountA, PointsCount vertexCountB, VectorsCount axisCountB>
		static CollisionPoints<dim> calculate(
			Points<vertexCountA>& verticesA, Vectors<axisCountA>& axesA, const Point& localCenterA, const matd& transformAToWorld,
			Points<vertexCountB>& verticesB, Vectors<axisCountB>& axesB, const Point& localCenterB, const matd& transformBToWorld
		) {
			const Point obbACenterWorld = Math::transform(localCenterA, transformAToWorld);
			const Point obbBCenterWorld = Math::transform(localCenterB, transformBToWorld);
			const Vector distanceAtoBWorld = obbBCenterWorld - obbACenterWorld;

			constexpr VectorsCount baseAxisCountA = axisCountA;
			constexpr VectorsCount baseAxisCountB = axisCountB;
			constexpr VectorsCount crossDimensions = dim - 2;
			constexpr VectorsCount crossAxisCount = baseAxisCountA * baseAxisCountB * crossDimensions;
			constexpr VectorsCount allAxisCount = baseAxisCountA + baseAxisCountB + crossAxisCount;

			Vectors<crossAxisCount> crossAxes;
			if constexpr(dim != 2) {
				for(VectorsCount iA = 0; iA < baseAxisCountA; iA++) {
					for(VectorsCount iB = 0; iB < baseAxisCountB; iB++) {
						Vectors<dim - 2> crossAxesAB = Math::crossN<dim, 2>({ axesA[iA], axesB[iB] });
						for(VectorsCount iC = 0; iC < crossDimensions; iC++) {
							const VectorsCount i = iA * baseAxisCountB * crossDimensions + iB * crossDimensions + iC;
							crossAxes[i] = crossAxesAB[iC];
						}
					}
				}
			}
			
			CollisionPoints<dim> points;

			scalar minOverlap = scalar_limit::max();
			points.hasCollision = true;

			Point onAxisCollisionPointsWorld;
			for(VectorsCount i = 0; i < allAxisCount; i++) {
				Vector axis;
				if(i < baseAxisCountA) {
					axis = axesA[i];
				} else if( i - baseAxisCountA < baseAxisCountB) {
					axis = axesB[i - baseAxisCountA];
				} else {
					axis = crossAxes[i - baseAxisCountA - baseAxisCountB];
				}

				const scalar axisLen2 = axis.lengthSquared();
				if(axisLen2 < 1e-6) continue; // degenerate axis
				const scalar signedWorldAlignment = distanceAtoBWorld.dot(axis);
				if(isNearZero(signedWorldAlignment)) continue; // skip Axes that are almost perpendicular to the Object Object World Normal
				const scalar invAxisLen2 = 1.f / axisLen2;

				const auto& [minA, maxA] = projectRangeOn<vertexCountA>(verticesA, axis, invAxisLen2);
				const auto& [minB, maxB] = projectRangeOn<vertexCountB>(verticesB, axis, invAxisLen2);

				if(maxA < minB || maxB < minA) { // seperating Axis found
					points.hasCollision = false;
					return points;
				} else { // Axis has overlap
					const scalar overlapMin = std::max(minA, minB);
					const scalar overlapMax = std::min(maxA, maxB);
					onAxisCollisionPointsWorld += axis * ((overlapMin + overlapMax) / 2.f);

					const scalar overlap = overlapMax - overlapMin;
					if(overlap < minOverlap) {
						minOverlap = overlap;
						points.normal = (signedWorldAlignment > 0.f) ? axis : -axis; // align Normal with the Object Object World Normal
					}
				}
			}
			const Vector collisionPointWorld = onAxisCollisionPointsWorld.getOriginVector() * (1.f / allAxisCount);

			if(points.hasCollision) {
				points.normal = points.normal.normalize();
				points.depth = minOverlap;

				// Calculate collision points
				points.a = Point(collisionPointWorld.getDirection()) + 0.5f * minOverlap * points.normal;
				points.b = Point(collisionPointWorld.getDirection()) - 0.5f * minOverlap * points.normal;
			}

			return points;
		}

	private:
		template<PointsCount vertexCount>
		static std::pair<scalar, scalar> projectRangeOn(const Points<vertexCount>& vertices, const Vector& axis, scalar invAxisLen2 = 0.f) {
			std::pair<scalar, scalar> range = { +scalar_limit::max(), -scalar_limit::max() };
			if(invAxisLen2 == 0.f) invAxisLen2 = 1.f / axis.lengthSquared();
			for(const Point& vertex : vertices) {
				const scalar projection = axis.dot(vertex.getOriginVector()) * invAxisLen2;
				range.first = std::min(range.first, projection);
				range.second = std::max(range.second, projection);
			}
			return range;
		}
	};

}
