#pragma once
#include <C78E/Math/Core/Core.h>
#include <C78E/Math/Geometry/Geometry.h>

namespace C78E::Math {

	class Camera {
	public:
		using vec3 = vec<3>;
		using mat4 = mat<3 + 1>;
		using Frustum3 = Frustum<3>;
	protected:
	public:
		static inline  mat4 perspectiveMatrix(CoordinateSystemOrientation orientation, scalar fov, scalar aspectRatio, scalar nearClip, scalar farClip) {
			return (orientation == CoordinateSystemOrientation::LeftHanded) ?
				glm::perspectiveLH(fov, aspectRatio, nearClip, farClip) :
				glm::perspectiveRH(fov, aspectRatio, nearClip, farClip);
		}
		static mat4 orthogonalMatrix(CoordinateSystemOrientation orientation, scalar left, scalar right, scalar top, scalar bottom, scalar nearClip, scalar farClip) {
			return (orientation == CoordinateSystemOrientation::LeftHanded) ?
				glm::orthoLH(left, right, top, bottom, nearClip, farClip) :
				glm::orthoRH(left, right, top, bottom, nearClip, farClip);
		}
		static Camera perspective(CoordinateSystemOrientation orientation, scalar fov, scalar aspectRatio, scalar nearClip, scalar farClip) {
			return Camera(perspectiveMatrix(orientation, fov, aspectRatio, nearClip, farClip));
		}
		static Camera orthogonal(CoordinateSystemOrientation orientation, scalar left, scalar right, scalar top, scalar bottom, scalar nearClip, scalar farClip) {
			return Camera(orthogonalMatrix(orientation, left, right, top, bottom, nearClip, farClip));
		}
	public:
		Camera() = default;
		Camera(const mat4& projection) : m_ProjectionMatrix(projection) { }
		Camera(Camera&) = default;
		Camera(const Camera&) = default;
		virtual ~Camera() = default;

		mat4 getProjection() const {
			return m_ProjectionMatrix;
		}

		Frustum3 getViewFrustumWith(const mat4& viewMatrix) const {
			return Frustum3(getProjection(), viewMatrix);
		}
	protected:
		mat4 m_ProjectionMatrix = mat4(1.0f);
	};
	
	//TODO: needed?
	template<Dimension dim>
	requires (dim >= 4)
	class CameraND : protected Camera {
	public:
		using vecd = vec<dim>;
		using matd = mat<dim + 1>;
		using OrthographicAxisBounds = std::array<std::pair<scalar, scalar>, dim + 1>;
	protected:
		static CameraND orthogonalLH(const OrthographicAxisBounds& bounds, const matd& dimensionReduction) {
			matd ortho(1.0f);
			for(Dimension i = 0; i < dim; ++i) {
				const scalar negBound = bounds[i].first;
				const scalar posBound = bounds[i].second;
				ortho[i][i] = 2.0f / (posBound - negBound);
				ortho[i][dim] = -(posBound + negBound) / (posBound - negBound);
			}

			return CameraND(ortho * dimensionReduction, dimensionReduction);
		}
		static CameraND orthogonalRH(const OrthographicAxisBounds& bounds, const matd& dimensionReduction) {
			matd ortho(1.0f);
			for(Dimension i = 0; i < dim; ++i) {
				const scalar negBound = bounds[i].first;
				const scalar posBound = bounds[i].second;
				ortho[i][i] = 2.0f / (posBound - negBound);
				ortho[i][dim] = -(posBound + negBound) / (posBound - negBound);
			}

			return CameraND(ortho * dimensionReduction, dimensionReduction);
		}
	public:
		static CameraND orthogonal(CoordinateSystemOrientation orientation, const OrthographicAxisBounds& bounds, const matd& dimensionReduction) {
			return (orientation == CoordinateSystemOrientation::LeftHanded) ?
				glm::orthoLH(bounds, dimensionReduction)
				: orthogonalRH(bounds, dimensionReduction);
		}
	public:
		CameraND() = default;
		CameraND(const mat4& projection, const matd& dimensionReduction) : Camera(projection), m_DimensionReduction(projection) { }
		CameraND(CameraND&) = default;
		CameraND(const CameraND&) = default;
		virtual ~CameraND() = default;

		matd getProjection() const override {
			return refitMatrix<dim + 1>(m_ProjectionMatrix) * m_DimensionReduction;
		}

		template<Dimension dim>
		Frustum<dim> getViewFrustumWith(const mat<dim + 1>& viewMatrix) const {
			return Frustum<dim>(getProjection(), viewMatrix);
		}
	protected:
		matd m_DimensionReduction = matd(1.f);
	};

}