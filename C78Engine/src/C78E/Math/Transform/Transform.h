#pragma once
#include <C78E/Math/Core/Core.h>
#include <C78E/Math/Geometry/Geometry.h>
#include "Rotation.h"

namespace C78E::Math {

	template<Dimension dim>
	requires (dim >= 2)
	struct Transform {
	public:
		using matd = mat<dim + 1>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		using Rotation = Rotation<dim>;
	public:
		Transform(const Point& translation = Point(0.f), const Rotation& rotation = Rotation(), const Vector& scale = Vector(1.f))
			: m_Translation(translation), m_Rotation(rotation), m_Scale(scale), m_MatrixCacheValid(false) { }
		Transform(const matd& transform) {
			m_Translation = Point(transform[dim]);
			
			for(Dimension i = 0; i < dim; ++i) {
				m_Scale[i] = glm::length(collapseVectorOnce(transform[i]));
			}

			matd rotationMatrix = transform;
			for(Dimension i = 0; i < dim; ++i) {
				rotationMatrix[i] /= m_Scale[i];
			}
			m_Rotation = Rotation(rotationMatrix);
			//skew and projection are left out
		}
		Transform(Transform&) = default;
		Transform(const Transform&) = default;

		template<Dimension otherDim>
		requires (otherDim != dim && otherDim >= 2)
		Transform(const Transform<otherDim>& other) {
			m_Translation = Point(refitVector<otherDim, dim>(other.getTranslation().getPosition()));
			m_Rotation = Rotation(other.getRotation().toMat()); // allows for consistent conversion across Dimensions
			m_Scale = Vector(refitVector<otherDim, dim>(other.getScale().getDirection()));
			m_MatrixCacheValid = false;
		}
		~Transform() = default;

		const Point& getTranslation() const {
			return m_Translation;
		}
		const Rotation& getRotation() const {
			return m_Rotation;
		}
		const Vector& getScale() const {
			return m_Scale;
		}

		void setTranslation(const Point& translation) {
			if(m_Translation != translation) {
				m_MatrixCacheValid = false;
				m_Translation = translation;
			}
		}
		void setRotation(const Rotation& rotation) {
			if(m_Rotation != rotation) {
				m_MatrixCacheValid = false;
				m_Rotation = rotation;
			}
		}
		void setScale(const Point& scale) {
			if(m_Scale != scale) {
				m_MatrixCacheValid = false;
				m_Scale = scale;
			}
		}

		const matd& toMat() {
			if(!m_MatrixCacheValid) {
				calculateMatricies();
			}
			return m_TransformMatrix;
		}
		const matd& toInvMat() {
			if(!m_MatrixCacheValid) {
				calculateMatricies();
			}
			return m_TransformMatrixInv;
		}
		const matd& toInvTransposeMat() {
			if(!m_MatrixCacheValid) {
				calculateMatricies();
			}
			return m_TransformMatrixInvT;
		}

		bool isIdentity() const {
			return m_Translation == Point(0.f) && m_Rotation == Rotation(0.f) && m_Scale == Vector(1.f);
		}

		bool operator==(const Transform& other) const {
			return m_Translation == other.m_Translation && m_Rotation == other.m_Rotation && m_Scale == other.m_Scale;
		}
	protected:
		matd scaleToMat() const {
			matd scaleMatrix = matd(1.f);
			for(Dimension i = 0; i < dim; ++i) {
				scaleMatrix[i][i] = m_Scale[i]; // Set diagonal elements
			}
			return scaleMatrix;
		}
		void calculateMatricies() {
			m_TransformMatrix = matd(1.f);
			m_TransformMatrix[dim] = m_Translation.getHomogeneousCoordinate();
			m_TransformMatrix = m_TransformMatrix * m_Rotation.toMat();
			m_TransformMatrix = m_TransformMatrix * scaleToMat();
			m_TransformMatrixInv = glm::inverse(m_TransformMatrix);
			m_TransformMatrixInvT = glm::transpose(m_TransformMatrixInv);
			m_MatrixCacheValid = true;
		}
	protected:
		Point m_Translation = Point(0.f);
		Rotation m_Rotation = Rotation(0.f);
		Vector m_Scale = Vector(1.f);

		bool m_MatrixCacheValid = false;
		matd m_TransformMatrix = matd(1.f);
		matd m_TransformMatrixInv = matd(1.f);
		matd m_TransformMatrixInvT = matd(1.f);
	};

	template<>
	struct Transform<Dimensions::D2> {
	public:
		using matd = mat<3 + 1>;
		using Point2 = Point<2>;
		using Point3 = Point<3>;
		using Point = Point3;
		using Vector = Vector<2>;
		using Rotation = Rotation<2>;
	public:
		Transform(const Point2& translation = Point2(0.f), const Rotation& rotation = Rotation(), const Vector& scale = Vector(1.f))
			: m_Translation(translation.pad<3>(0.f)), m_Rotation(rotation), m_Scale(scale), m_MatrixCacheValid(false) {
		}
		Transform(const Point3& translation, const Rotation& rotation = Rotation(), const Vector& scale = Vector(1.f))
			: m_Translation(translation), m_Rotation(rotation), m_Scale(scale), m_MatrixCacheValid(false) {
		}
		Transform(const matd& transform) {
			m_Translation = Point3(transform[3]);

			for(Dimension i = 0; i < 2; i++) {
				m_Scale[i] = glm::length(collapseVectorOnce(transform[i]));
			}

			matd rotationMatrix = transform;
			for(Dimension i = 0; i < 2; i++) {
				rotationMatrix[i] /= m_Scale[i];
			}
			m_Rotation = Rotation(rotationMatrix);
			//skew and projection are left out
		}
		Transform(Transform&) = default;
		Transform(const Transform&) = default;

		template<Dimension otherDim>
		requires (otherDim > 2)
		Transform(const Transform<otherDim>& other) {
			m_Translation = Point3(refitVector<otherDim, 3>(other.getTranslation().getPosition()));
			m_Rotation = Rotation(other.getRotation().toMat()); // allows for consistent conversion across Dimensions
			m_Scale = Vector(refitVector<otherDim, 2>(other.getScale().getDirection()));
			m_MatrixCacheValid = false;
		}
		~Transform() = default;

		const Point3& getTranslation3() const {
			return m_Translation;
		}
		Point2 getTranslation() const {
			return m_Translation.collapse<2>();
		}
		const Rotation& getRotation() const {
			return m_Rotation;
		}
		const Vector& getScale() const {
			return m_Scale;
		}

		void setTranslation(const Point3& translation) {
			if(m_Translation != translation) {
				m_MatrixCacheValid = false;
				m_Translation = translation;
			}
		}
		inline void setTranslation(const Point2& translation) {
			setTranslation(translation.pad<3>(m_Translation[2]));
		}
		void setRotation(const Rotation& rotation) {
			if(m_Rotation != rotation) {
				m_MatrixCacheValid = false;
				m_Rotation = rotation;
			}
		}
		void setScale(const Vector& scale) {
			if(m_Scale != scale) {
				m_MatrixCacheValid = false;
				m_Scale = scale;
			}
		}

		const scalar& getLayer() const {
			return m_Translation[2];
		}
		void setLayer(scalar layer) {
			if(m_Translation[2] != layer) {
				m_MatrixCacheValid = false;
				m_Translation[2] = layer;
			}
		}

		const matd& toMat() {
			if(!m_MatrixCacheValid) {
				calculateMatricies();
			}
			return m_TransformMatrix;
		}
		const matd& toInvMat() {
			if(!m_MatrixCacheValid) {
				calculateMatricies();
			}
			return m_TransformMatrixInv;
		}
		const matd& toInvTransposeMat() {
			if(!m_MatrixCacheValid) {
				calculateMatricies();
			}
			return m_TransformMatrixInvT;
		}

		bool isIdentity() const {
			return m_Translation == Point3(0.f) && m_Rotation == Rotation(0.f) && m_Scale == Vector(1.f);
		}

		bool operator==(const Transform& other) const {
			return m_Translation == other.m_Translation && m_Rotation == other.m_Rotation && m_Scale == other.m_Scale;
		}
	protected:
		matd scaleToMat() const {
			matd scaleMatrix = matd(1.f);
			for(Dimension i = 0; i < 2; ++i) {
				scaleMatrix[i][i] = m_Scale[i]; // Set diagonal elements
			}
			return scaleMatrix;
		}
		void calculateMatricies() {
			m_TransformMatrix = matd(1.f);
			m_TransformMatrix[3] = m_Translation.getHomogeneousCoordinate();
			m_TransformMatrix = m_TransformMatrix * padMatrixOnce(m_Rotation.toMat());
			m_TransformMatrix = m_TransformMatrix * scaleToMat();
			m_TransformMatrixInv = glm::inverse(m_TransformMatrix);
			m_TransformMatrixInvT = glm::transpose(m_TransformMatrixInv);
			m_MatrixCacheValid = true;
		}
	protected:
		Point3 m_Translation = Point3(0.f);
		Rotation m_Rotation = Rotation(0.f);
		Vector m_Scale = Vector(1.f);

		bool m_MatrixCacheValid = false;
		matd m_TransformMatrix = matd(1.f);
		matd m_TransformMatrixInv = matd(1.f);
		matd m_TransformMatrixInvT = matd(1.f);
	};

	template<Dimension native, Dimension transforming>
	Point<native> transform(const Point<native>& point, Transform<transforming>& t) {
		return transform<native, transforming>(point, t.toMat());
	}

	template<Dimension native, Dimension transforming>
	Vector<native> transform(const Vector<native>& vector, Transform<transforming>& t) {
		return transform<native, transforming>(vector, t.toMat());
	}


	template<Dimension native, Dimension transforming>
	Point<native> scale(const Point<native>& point, Transform<transforming>& t) {
		return point * refitVector<transforming, native>(t.getScale());
	}

	template<Dimension native, Dimension transforming>
	Vector<native> scale(const Vector<native>& vector, Transform<transforming>& t) {
		return vector * refitVector<transforming, native>(t.getScale());
	}

	template<Dimension native, Dimension transforming>
	Point<native> scaleAndTranslate(const Point<native>& point, Transform<transforming>& t) {
		return scale(point, t) + refitVector<transforming, native>(t.getTranslation());
	}


	// most used Transforms
	using Transform2D = Transform<Dimensions::D2>;
	using Transform3D = Transform<Dimensions::D3>;
}

#include <C78E/Utils/StdUtils.h>
namespace std {

	//Transform 
	template<C78E::Math::Dimension dim>
	_EXPORT_STD _NODISCARD inline string to_string(const C78E::Math::Transform<dim>& transform, const size_t& decimals = -1) {
		return "Transform(Translation: " + to_string(transform.getTranslation(), decimals) +
			"Rotation: " + to_string(transform.getRotation(), decimals) +
			"Scale: " + to_string(transform.getScale(), decimals) + ")";
	}

	template<C78E::Math::Dimension dim>
	struct hash<C78E::Math::Transform<dim>> {
		size_t operator()(const C78E::Math::Transform<dim>& transform) const {
			size_t seed = 0;
			hashCombine(seed, transform.getTranslation(), transform.getRotation(), transform.getScale());
			return seed;
		}
	};

}

#include <C78E/Utils/YamlUtils.h>
namespace YAML {

	//Transform
	template<C78E::Math::Dimension dim>
	inline Emitter& operator<<(Emitter& out, const C78E::Math::Transform<dim>& transform) {
		out << YAML::Key << "Translation" << YAML::Value << transform.getTranslation();
		out << YAML::Key << "Rotation" << YAML::Value << transform.getRotation();
		out << YAML::Key << "Scale" << YAML::Value << transform.getScale();
		return out;
	}
	template<C78E::Math::Dimension dim>
	struct convert<C78E::Math::Transform<dim>> {
		static Node encode(const C78E::Math::Transform<dim>& transform) {
			Node node(YAML::NodeType::Map);
			node["Translation"] = transform.getTranslation();
			node["Rotation"] = transform.getRotation();
			node["Scale"] = transform.getScale();
			return node;
		}

		static bool decode(const Node& node, C78E::Math::Transform<dim>& transform) {
			C78E_CORE_VALIDATE(node, return false, "Transform::decode: Corrupted Transform!");
			YAML::Node nodeTransformTranslation = node["Translation"];
			YAML::Node nodeTransformRotation = node["Rotation"];
			YAML::Node nodeTransformScale = node["Scale"];
			C78E_CORE_VALIDATE(
				nodeTransformTranslation && nodeTransformRotation && nodeTransformScale,
				return false,
				"Transform::decode: Corrupted Transform!\n    Translation:{}\n    Rotation:{}\n    Scale:{}",
				(nodeTransformTranslation) ? nodeTransformTranslation.as<std::string>() : "<corrupted>",
				(nodeTransformRotation) ? nodeTransformRotation.as<std::string>() : "<corrupted>",
				(nodeTransformScale) ?nodeTransformScale.as<std::string>() : "<corrupted>"
			);
			transform.setTranslation(nodeTransformTranslation.as<C78E::Math::Point<dim>>());
			transform.setRotation(nodeTransformRotation.as<C78E::Math::Rotation<dim>>());
			transform.setScale(nodeTransformScale.as<C78E::Math::Vector<dim>>());
			return true;
		}

	};

}