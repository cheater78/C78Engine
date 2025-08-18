#pragma once
#include <C78E/Math/Core/Core.h>
#include <C78E/Math/Geometry/Geometry.h>

namespace C78E::Math {

    /**
     * @brief generic solution for Rotations in dim Dimensions
     * @tparam dim 
     */
    template <Dimension dim>
    struct Rotation {
    public:
        using vecd = vec<dim>;
        using matd = mat<dim + 1>;
    public:
        static const Dimension eulerAngleCount = (dim * (dim - 1)) / 2;
        static matd eulerAnglesToTransformMatrix(const vec<eulerAngleCount>& eulerAngles) {
            matd result = matd(1.f);
            size_t index = 0;
            for(size_t i = 0; i < dim; ++i) {
                for(size_t j = i + 1; j < dim; ++j) {
                    const scalar theta = eulerAngles[index++];
                    mat<dim> rot = glm::identity<mat<dim>>();
                    rot[i][i] = std::cos(theta);
                    rot[j][j] = std::cos(theta);
                    rot[i][j] = -std::sin(theta);
                    rot[j][i] = std::sin(theta);
                    result *= rot;
                }
            }
            return result;
        }
        static vec<eulerAngleCount> eulerAnglesFromTransformMatrix(const matd& matrix) {
            vec<eulerAngleCount> angles;
            size_t index = 0;
            for(size_t i = 0; i < dim; ++i) {
                for(size_t j = i + 1; j < dim; ++j) {
                    angles[index++] = std::atan2(matrix[j][i], matrix[i][i]);
                }
            }
            return angles;
        }
        static Rotation getTorque(const vecd& force, const vecd& position) {
            matd torqueMatrix(0.f);
            for(Dimension i = 0; i < dim; ++i) {
                for(Dimension j = 0; j < dim; ++j) {
                    torqueMatrix[i][j] = position[i] * force[j] - position[j] * force[i];
                }
            }
            return torqueMatrix; // This is antisymmetric: torqueMatrix[i][j] = -torqueMatrix[j][i]
        }
    public:
        Rotation() = default;
        Rotation(const vec<eulerAngleCount>& eulerAngles) : m_Rotation(eulerAnglesToTransformMatrix(eulerAngles)) { }
        Rotation(const matd& rotation) : m_Rotation(rotation) { /*TODO: validate rotation matrix*/ }
        ~Rotation() = default;

        matd toMat() const {
            return m_Rotation;
        }
        matd toInvMat() const {
            return glm::inverse(m_Rotation);
        }

        vec<eulerAngleCount> toEulerAngles() const {
            return eulerAnglesFromTransformMatrix(m_Rotation);
        }

		bool operator==(const Rotation& other) const {
			return m_Rotation == other.m_Rotation;
		}
		bool operator!=(const Rotation& other) const {
			return !(*this == other);
		}

		operator bool() const {
			return m_Rotation != matd(1.f);
		}

		Rotation& operator=(const Rotation& other) {
			if(this != &other) {
				m_Rotation = other.m_Rotation;
			}
			return *this;
		}
		Rotation& operator+=(const Rotation& other) {
            matd logA = matrixLog(m_Rotation);
            matd logB = matrixLog(other.m_Rotation);

            matd logAvg = (logA + logB) * static_cast<scalar>(0.5f);

            m_Rotation = matrixExp(logAvg);
            return *this;
		}

    private:
        // Approximate matrix log via series (valid for small angles)
        matd matrixLog(const matd& R) {
            // Skew-symmetric approximation: log(R) ≈ (R - R^T)/2
            return (R - glm::transpose(R)) * static_cast<scalar>(0.5f);
        }
        // Approximate matrix exp via Taylor series (only valid for skew-symmetric matrices)
        matd matrixExp(const matd& A) {
            matd result = matd(1.0f);
            matd term = matd(1.0f);
            scalar factorial = 1.0f;

            const int kMaxTerms = 10;
            for(int i = 1; i < kMaxTerms; i++) {
                term = term * A;
                factorial *= static_cast<scalar>(i);
                result += term / factorial;
            }

            return result;
        }

    protected:
        matd m_Rotation = matd(1.f);
    };

    /**
     * @brief Rotation optimization for 2D -> scalar angle
     */
    template <>
    struct Rotation<Dimensions::D2> {
    public:
        using vecd = vec<2>;
        using matd = mat<2 + 1>;
    public:
        static scalar matToAngle(const matd& rotationMatrix) {
            return std::atan2(rotationMatrix[1][0], rotationMatrix[0][0]);
        }
        static Rotation getTorque(const vecd& force, const vecd& position) {
            return Rotation(glm::dot(force, position));
        }
    public:
        Rotation() = default;
        Rotation(scalar angle) : m_Rotation(angle) { }
        Rotation(const matd& rotation) : m_Rotation(matToAngle(rotation)) { /*TODO: validate rotation matrix*/ }
        ~Rotation() = default;

        matd toMat() const {
            matd mat = matd(1.f);
            mat[0][0] = std::cos(m_Rotation);
            mat[0][1] = -std::sin(m_Rotation);
            mat[1][0] = std::sin(m_Rotation);
            mat[1][1] = std::cos(m_Rotation);
            return mat;
        }
        matd toInvMat() const {
            return glm::inverse(toMat());
        }

        scalar toEulerAngles() const {
            return m_Rotation;
        }

		bool operator==(const Rotation& other) const {
			return m_Rotation == other.m_Rotation;
		}
        bool operator!=(const Rotation& other) const {
            return !(*this == other);
        }

        operator bool() const {
            return m_Rotation != 0.f;
        }

		Rotation& operator=(const Rotation& other) {
			if(this != &other) {
				m_Rotation = other.m_Rotation;
			}
			return *this;
		}
		Rotation& operator+=(const Rotation& other) {
			m_Rotation = glm::mod(m_Rotation + other.m_Rotation, 360.f);
			return *this;
		}

    private:
        scalar m_Rotation = 0.f;
    };

    /**
     * @brief Rotation optimization for 3D -> quaternion
     */
    template <>
    struct Rotation<Dimensions::D3> {
    public:
        using quat = glm::quat;
        using vecd = vec<3>;
        using matd = mat<3 + 1>;
    public:
        static Rotation getTorque(const vecd& force, const vecd& position) {
            return Rotation(glm::quat(0.f, glm::cross(force, position)));
        }
    public:
        Rotation() = default;
        Rotation(quat rotation) : m_Rotation(rotation) { }
        Rotation(const vecd& eulerAngles) : m_Rotation(eulerAngles) { }
        Rotation(const matd& rotation) : m_Rotation(glm::quat_cast(mat<Dimensions::D3>(rotation))) { /*TODO: validate rotation matrix*/ }
        ~Rotation() = default;

        matd toMat() const {
            return glm::toMat4(m_Rotation);
        }
        matd toInvMat() const {
            return glm::inverse(toMat());
        }

        vecd toEulerAngles() const {
            return glm::eulerAngles(m_Rotation);
        }

        quat getQuat() const {
            return m_Rotation;
        }

		bool operator==(const Rotation& other) const {
			return m_Rotation == other.m_Rotation;
		}
		bool operator!=(const Rotation& other) const {
			return !(*this == other);
		}

        operator bool() const {
            return m_Rotation != quat(1.f, 0.f, 0.f, 0.f);
        }

		Rotation& operator=(const Rotation& other) {
			if(this != &other) {
				m_Rotation = other.m_Rotation;
			}
			return *this;
		}
		Rotation& operator+=(const Rotation& other) {
			m_Rotation = glm::normalize(m_Rotation * other.m_Rotation);
			return *this;
		}

    protected:
        quat m_Rotation = quat(1.f, 0.f, 0.f, 0.f);
    };

    // most used Transforms
    using Rotation2 = Rotation<Dimensions::D2>;
    using Rotation3 = Rotation<Dimensions::D3>;
} // namespace C78E::Math

#include <C78E/Utils/StdUtils.h>
namespace std {

    //Rotation 
    template<::C78E::Math::Dimension dim>
    _EXPORT_STD _NODISCARD inline std::string to_string(const ::C78E::Math::Rotation<dim>& rot, size_t& decimals = -1) {
        return to_string(rot.toMat(), decimals);
    }

    template<>
    _EXPORT_STD _NODISCARD inline std::string to_string(const ::C78E::Math::Rotation2& rot, size_t& decimals) {
        return to_string(rot.toEulerAngles(), decimals);
    }
    template<>
    _EXPORT_STD _NODISCARD inline std::string to_string(const ::C78E::Math::Rotation3& rot, size_t& decimals) {
        return to_string(rot.getQuat(), decimals);
    }

    template<::C78E::Math::Dimension dim>
    struct hash<::C78E::Math::Rotation<dim>> {
        size_t operator()(const ::C78E::Math::Rotation<dim>& rot) const {
            return hash<::C78E::Math::Rotation<dim>::matd>()(rot.toMat());
        }
    };

    template<>
    struct hash<::C78E::Math::Rotation2> {
        size_t operator()(const ::C78E::Math::Rotation2& rot) const {
            return hash<::C78E::Math::scalar>()(rot.toEulerAngles());
        }
    };
    template<>
    struct hash<::C78E::Math::Rotation3> {
        size_t operator()(const ::C78E::Math::Rotation3& rot) const {
            return hash<glm::quat>()(rot.getQuat());
        }
    };

}

#include <C78E/Utils/YamlUtils.h>
namespace YAML {

    //Rotation
    template<::C78E::Math::Dimension dim>
    inline Emitter& operator<<(Emitter& out, const ::C78E::Math::Rotation<dim>& rot) {
        out << rot.toMat();
        return out;
    }

    template<>
    inline Emitter& operator<<(Emitter& out, const ::C78E::Math::Rotation2& rot) {
        out << rot.toEulerAngles();
        return out;
    }
    template<>
    inline Emitter& operator<<(Emitter& out, const ::C78E::Math::Rotation3& rot) {
        out << rot.getQuat();
        return out;
    }


    template<::C78E::Math::Dimension dim>
    struct convert<::C78E::Math::Rotation<dim>> {
        static Node encode(const ::C78E::Math::Rotation<dim>& rot) {
            return convert<::C78E::Math::mat<dim + 1>>::encode(rot.toMat());
        }
        static bool decode(const Node& node, ::C78E::Math::Rotation<dim>& rot) {
            ::C78E::Math::mat<dim + 1> mat;
            const bool result = convert<::C78E::Math::mat<dim + 1>>::decode(node, mat);
            rot = ::C78E::Math::Rotation<dim>(mat);
            return result;
        }

    };

    template<>
    struct convert<::C78E::Math::Rotation2> {
        static Node encode(const ::C78E::Math::Rotation2& rot) {
            return convert<::C78E::Math::scalar>::encode(rot.toEulerAngles());
        }
        static bool decode(const Node& node, ::C78E::Math::Rotation2& rot) {
            ::C78E::Math::scalar angle;
            const bool result = convert<::C78E::Math::scalar>::decode(node, angle);
            rot = ::C78E::Math::Rotation2(angle);
            return result;
        }

    };
    template<>
    struct convert<::C78E::Math::Rotation3> {
        static Node encode(const ::C78E::Math::Rotation3& rot) {
            return convert<glm::quat>::encode(rot.getQuat());
        }
        static bool decode(const Node& node, ::C78E::Math::Rotation3& rot) {
            glm::quat quat;
            const bool result = convert<glm::quat>::decode(node, quat);
            rot = ::C78E::Math::Rotation3(quat);
            return result;
        }

    };

}