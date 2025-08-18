#pragma once
#include "Dimension.h"
#include "Scalar.h"

namespace C78E::Math {
	
	/**
	 * @brief Matrix with dim columns and rows, provides a default Matrix type to use, using scalar type and formatting glm::packed_highp
	 * "most commonly used matrix"
	 * @tparam dim the Matrix size
	 */
	template<Dimension dim>
	using mat = glm::mat<dim, dim, scalar, glm::packed_highp>;
	template<Dimension dim1, Dimension dim2>
	using matNxM = glm::mat<dim1, dim2, scalar, glm::packed_highp>;
	using mat2 = mat<Dimensions::D2>;
	using mat3 = mat<Dimensions::D3>;
	using mat4 = mat<Dimensions::D4>;

	/**
	 * @brief Pad a Matrix from native Dimension to target Dimension,
	 * assumes Identity for the new Dimension
	 * @tparam native Dimension of the given Matrix
	 * @tparam target Dimension of the padded Matrix
	 * @param nativeMatrix the Matrix to provide
	 * @param assumeIdentity whether to create a Identity Matrix in the padded Dimensions, requires paddingValue 0
	 * @param paddingValue the value which to pad the Matrix with, default 0
	 * @return the padded Matrix
	 */
	template<Dimension native, Dimension target>
	requires (native <= target && native > 0)
	mat<target> padMatrix(const mat<native>& nativeMatrix, bool assumeIdentity = true, scalar paddingValue = 0.f) {
		mat<target> targetMatrix = mat<target>(0.f);
		for(Dimension i = 0; i < target; i++) {
			for(Dimension j = 0; j < target; j++) {
				if(i < native && j < native) {
					targetMatrix[i][j] = nativeMatrix[i][j];
				} else if(i == j && assumeIdentity) {
					targetMatrix[i][j] = 1.f;
				} else {
					targetMatrix[i][j] = paddingValue;
				}
			}
		}
		return targetMatrix;
	}

	/**
	 * @brief Collapse a Matrix from native Dimension to target Dimension,
	 * cuts lost Dimensions
	 * @tparam native Dimension of the given Matrix
	 * @tparam target Dimension of the collapsed Matrix
	 * @param nativeMatrix the Matrix to provide
	 * @return the collapsed Matrix
	 */
	template<Dimension native, Dimension target>
	requires (native >= target && target > 0)
	mat<target> collapseMatrix(const mat<native>& nativeMatrix) {
		mat<target> targetMatrix = mat<target>(0.f);
		for(Dimension i = 0; i < target; i++) {
			for(Dimension j = 0; j < target; j++) {
				targetMatrix[i][j] = nativeMatrix[i][j];
			}
		}
		return targetMatrix;
	}

	/**
	 * @brief Refit a Matrix from native Dimension to target Dimension,
	 * (native < target) assumes Identity transform for new Dimensions, (native > target) cuts lost Dimensions
	 * @tparam native Dimension of the given Matrix
	 * @tparam target Dimension of the refit Matrix
	 * @param nativeMatrix the Matrix to provide
	 * @param assumeIdentity whether to create a Identity Matrix in the padded Dimensions, requires paddingValue 0
	 * @param paddingValue the value which to pad the Matrix with, default 0
	 * @return the refit Matrix
	 */
	template<Dimension native, Dimension target>
	requires (native >= target && target > 0)
	mat<target> refitMatrix(const mat<native>& nativeMatrix, bool assumeIdentity = true, scalar paddingValue = 0.f) {
		if constexpr(native < target) {
			return padMatrix(nativeMatrix, paddingValue);
		} else {
			return collapseMatrix(nativeMatrix);
		}
	}

	/**
	 * @brief Pad a Matrix from native Dimension to native + 1 Dimension,
	 * assumes Identity for the new Dimension
	 * @tparam native Dimension of the given Matrix
	 * @param nativeMatrix the Matrix to provide
	 * @return the padded Matrix
	 */
	template<Dimension native>
		requires (native > 0)
	mat<native + 1> padMatrixOnce(const mat<native>& nativeMatrix, bool assumeIdentity = true, scalar paddingValue = 0.f) {
		return padMatrix<native, native + 1>(nativeMatrix, assumeIdentity, paddingValue);
	}

	/**
	 * @brief Collapse a Matrix from native Dimension to native - 1 Dimension,
	 * cuts the lost Dimension
	 * @tparam native Dimension of the given Matrix
	 * @param nativeMatrix the Matrix to provide
	 * @return the collapsed Matrix
	 */
	template<Dimension native>
		requires (native > 1)
	mat<native - 1> collapseMatrixOnce(const mat<native>& nativeMatrix) {
		return collapseMatrix<native, native - 1>(nativeMatrix);
	}

	/**
	 * @brief Computes the determinant of a Matrix
	 * @tparam dim the Dimension of the Matrix
	 * @param matrix the Matrix to provide
	 * @return the determinant of the Matrix
	 */
	template <Dimension dim>
	scalar determinant(const mat<dim>& matrix) {
		if constexpr(dim == 1) {
			return matrix[0][0];
		} else if constexpr(dim == 2) {
			return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
		} else {
			scalar det = 0.0;
			for(Dimension col = 0; col < dim; ++col) {
				mat<dim - 1> submat{};
				for(Dimension i = 1; i < dim; ++i) {
					Dimension subcol = 0;
					for(Dimension j = 0; j < dim; ++j) {
						if(j == col) continue;
						submat[i - 1][subcol++] = matrix[i][j];
					}
				}
				scalar cofactor = ((col % 2 == 0) ? 1 : -1) * matrix[0][col] * determinant<dim - 1>(submat);
				det += cofactor;
			}
			return det;
		}
	}

	/**
	 * @brief Computes the cross product of dim - vectorCount vectors in dim space
	 * @tparam dim the Dimension of the vectors
	 * @tparam vectorCount the number of vectors to provide
	 * @param vectors the dim - vectorCount vectors to provide
	 * @return the cross product of the vectors
	 */
	template <Dimension dim, size_t vectorCount>
		requires (vectorCount < dim)
	std::array<vec<dim>, dim - vectorCount> crossN(const std::array<vec<dim>, vectorCount>& vectors) { //TODO: fix
		mat<dim> basis(1.f); // N-Dimensional Identity Matrix, as basis

		// Remove components in span of input vectors (project out)
		std::array<vec<dim>, dim - vectorCount> orthogonal{};
		size_t count = 0;

		for(Dimension i = 0; i < dim && count < dim - vectorCount; i++) {
			vec<dim> candidate = basis[i];

			// Remove each input vector's component from the candidate
			for(const vec<dim>& v : vectors) {
				candidate -= glm::dot(candidate, v) / glm::dot(v, v) * v;
			}

			// Check if the result is non-zero
			scalar norm_sq = glm::dot(candidate, candidate);
			if(!isNearZero(norm_sq)) {
				orthogonal[count++] = glm::normalize(candidate);
			}
		}
		C78E_CORE_ASSERT(count == dim - vectorCount, "Cross product failed to find orthogonal vectors");
		return orthogonal;
	}

	/**
	 * @brief Computes the cross product of dim - 1 vectors in dim space
	 * @tparam dim the Dimension of the vectors
	 * @param vectors the dim - 1 vectors to provide
	 * @return the cross product of the vectors
	 */
	template<Dimension dim>
		requires (dim >= 2)
	vec<dim> cross(const std::array<vec<dim>, dim - 1>& vectors) {
		return crossN<dim, dim - 1>(vectors)[0];
	}



}

namespace std {

	template<::C78E::Math::Dimension dim>
	requires (dim > 0)
	_EXPORT_STD _NODISCARD inline string to_string(const ::C78E::Math::mat<dim>& matrix, size_t& decimals = -1, bool asColVecs = true) {
		string out;
		out.reserve(3 + log10(dim) + 2 + (dim * (2 + (dim * (1 + 1 + decimals)) + 2)) + 2);
		
		out += "mat" + to_string(dim) + "( ";
		for(::C78E::Math::Dimension i = 0; i < dim; i++) {
			if(i) {
				out += ", ";
			}
			// Column Vector(if asColVecs)
			out += "( " + to_string(matrix[(asColVecs) ? i : 0][(asColVecs) ? 0 : i], decimals);
			for(::C78E::Math::Dimension j = 1; j < dim; j++) {
				out += ", " + to_string(matrix[(asColVecs) ? i : j][(asColVecs) ? j : i], decimals);
			}
			out += " )";
			// ~Column Vector
		}
		return out + " )";
	}


	template<::C78E::Math::Dimension dim>
	requires (dim > 0)
	struct hash<::C78E::Math::mat<dim>> {
		size_t operator()(const ::C78E::Math::mat<dim>& matrix) const {
			size_t seed = 0;
			for(::C78E::Math::Dimension i = 0; i < dim; i++) {
				for(::C78E::Math::Dimension j = 0; j < dim; j++) {
					hashCombine(seed, hash()(matrix[i][j]));
				}
			}
			return seed;
		}
	};

}

#include <C78E/Utils/YamlUtils.h>
namespace YAML {

	// GLM::mat
	template<glm::length_t L, typename T, glm::qualifier Q>
	inline Emitter& operator<<(Emitter& out, const glm::mat<L, L, T, Q>& m) {
		out << YAML::Flow;
		out << YAML::BeginSeq;
		for(C78E::Math::Dimension i = 0; i < L; i++) {
			for(C78E::Math::Dimension j = 0; j < L; j++) {
				out << m[i][j];
			}
		}
		out << YAML::EndSeq;
		return out;
	}
	template<glm::length_t L, typename T, glm::qualifier Q>
	struct convert<glm::mat<L, L, T, Q>> {
		static Node encode(const glm::mat<L, L, T, Q>& m) {
			Node node;
			for(C78E::Math::Dimension i = 0; i < L; i++) {
				for(C78E::Math::Dimension j = 0; j < L; j++) {
					node.push_back(m[i][j]);
				}
			}
			return node;
		}
		static bool decode(const Node& node, glm::mat<L, L, T, Q>& m) {
			if(!node.IsSequence() || node.size() != L)
				return false;
			for(C78E::Math::Dimension i = 0; i < L; i++) {
				for(C78E::Math::Dimension j = 0; j < L; j++) {
					m[i][j] = node[i * L + j].as<T>();
				}
			}
			return true;
		}

	};

}