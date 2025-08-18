#pragma once
#include <C78E/Core/Log/SmartLog.h>
#include <C78E/Math/Core/Core.h>

namespace C78E::Math {

	/**
	 * @brief HomogeneousCoordinate, a Homogeneous Coordinate is a point in a dim dimensional space, represented by dim + 1 coordinates
	 * with the last coordinate being the scale factor, which is used to distinguish between a point(1) and a direction(0) (or a plane(distance from the origin))
	 * @tparam dim 2D or 3D
	 */
	template<Dimension dim>
	requires (dim >= 2)
	struct HomogeneousCoordinate {
	public:
		using vecd = vec<dim>;
		using CoordVec = vec<dim + 1>;
	protected:
		struct Separate {
			vecd coordinate = vecd(0.f);
			scalar scale;
		};
	public:
		HomogeneousCoordinate() : m_C{ CoordVec(0.f) } { }
		HomogeneousCoordinate(std::initializer_list<scalar> list, scalar scale = 0.f) { // scale can be inside of initializer list, but does not have to
			C78E_CORE_ASSERT(list.size() == dim + 1 || list.size() == dim, "HomogeneousCoordinate: initializer list does not fit the Dimension");
			auto it = list.begin();
			if(list.size() == dim + 1) {
				for(Dimension i = 0; i < dim + 1; i++) {
					m_C.coordinate[i] = *(it++);
				}
			} else { // scale was not provided in the initializer list
				for(Dimension i = 0; i < dim; i++) {
					if(it) {
						m_C.separate.coordinate[i] = *(it++);
					} else {
						m_C.separate.coordinate[i] = 0.f;
					}
				}
				m_C.separate.scale = scale;
			}
		}
		HomogeneousCoordinate(CoordVec homogeneousCoordinate) : m_C{ homogeneousCoordinate } { }
		HomogeneousCoordinate(vecd coordinate, scalar scale) : m_C{ coordinate, scale } { }
		HomogeneousCoordinate(HomogeneousCoordinate&) = default;
		HomogeneousCoordinate(const HomogeneousCoordinate&) = default;
		~HomogeneousCoordinate() = default;

		inline void setHomogeneousCoordinate(const CoordVec& coord) {
			m_C.coordinate = coord;
		}
		inline const CoordVec& getHomogeneousCoordinate() const {
			return m_C.coordinate;
		}

		inline void setCoordinate(const vecd& coord) {
			m_C.separate.coordinate = coord;
		}
		inline vecd& getCoordinate() {
			return m_C.separate.coordinate;
		}
		inline const vecd& getCoordinate() const {
			return m_C.separate.coordinate;
		}

	protected:
		inline scalar getScale() const {
			return m_C.separate.scale;
		}
		// use carefully!, normalize!
		inline void setScale(scalar scale) { 
			m_C.separate.scale = scale;
		}
	protected:
		union CoordinateStorage {
		public:
			Separate separate;
			CoordVec coordinate;

			CoordinateStorage() : coordinate(0.f) { }
			CoordinateStorage(const CoordinateStorage& other) : coordinate(other.coordinate) { }
			CoordinateStorage(CoordVec homogeneousCoordinate) : coordinate(homogeneousCoordinate) { }
			CoordinateStorage(vecd coordinate, scalar scale) : separate{ coordinate, scale } { }
			~CoordinateStorage() { }
		} m_C;
	};

	/*
	* Point and Vector
	*/
	template<Dimension dim>
		requires (dim >= 2)
	struct Point;

	template<Dimension dim>
	requires (dim >= 2)
	struct Vector;

	/**
	 * @brief Point, a Point in dim dimensional space, represented by a dim + 1 homogeneous coordinate
	 * @tparam dim the dimensionality of the Point
	 */
	template<Dimension dim>
	requires (dim >= 2)
	struct Point : protected HomogeneousCoordinate<dim> {
	public:
		using vecd = vec<dim>;
		using CoordVec = vec<dim + 1>;
		using HomogeneousCoordinate = HomogeneousCoordinate<dim>;
	public:
		Point() : HomogeneousCoordinate(vecd(0.f), 1.f) { }
		Point(std::initializer_list<scalar> list) : HomogeneousCoordinate(list, 1.f) { normalize(); }
		template <typename ...Args>
		Point(Args... args) : HomogeneousCoordinate(vecd{ static_cast<scalar>(args)... }, 1.f) {
			C78E_CORE_STATIC_ASSERT(sizeof...(args) == dim, "Point: Args do not match Dimension!");
			normalize();
		}
		Point(scalar value) : HomogeneousCoordinate(vecd(value), 1.f) { normalize(); }
		Point(vecd position) : HomogeneousCoordinate(position, 1.f) { normalize(); }
		Point(CoordVec point) : HomogeneousCoordinate(point) { normalize(); }
		Point(Point&) = default;
		Point(const Point&) = default;
		~Point() = default;

		inline const vecd& getPosition() const {
			return HomogeneousCoordinate::getCoordinate();
		}
		inline void setPosition(vecd position) {
			HomogeneousCoordinate::setCoordinate(position);
		}
		
		inline Point& operator+=(const Vector<dim>& other) {
			HomogeneousCoordinate::setCoordinate(HomogeneousCoordinate::getCoordinate() + other.getDirection());
			return *this;
		}
		inline Point& operator-=(const Vector<dim>& other) {
			HomogeneousCoordinate::setCoordinate(HomogeneousCoordinate::getCoordinate() - other.getDirection());
			return *this;
		}

		inline const CoordVec& getHomogeneousCoordinate() const {
			return HomogeneousCoordinate::getHomogeneousCoordinate();
		}
		inline void setHomogeneousCoordinate(CoordVec point) {
			HomogeneousCoordinate::setHomogeneousCoordinate(point);
		}

		inline operator vecd&() {
			return HomogeneousCoordinate::getCoordinate();
		}
		inline operator const vecd& () const {
			return HomogeneousCoordinate::getCoordinate();
		}
		
		inline Point& operator=(const Point& other) {
			HomogeneousCoordinate::setHomogeneousCoordinate(other.getHomogeneousCoordinate());
			return *this;
		}

		inline Vector<dim> getOriginVector() const {
			return Vector<dim>(getPosition());
		}
		inline Vector<dim> getDirection() const {
			return Vector<dim>(getPosition()).normalize();
		}

		inline Point operator+() const {
			return *this;
		}
		inline Point operator-() const {
			return Point(-getPosition());
		}
		template<Dimension otherDim>
			requires (otherDim >= 2)
		inline Point operator+(const Vector<otherDim>& other) const {
			return Point(getPosition() + refitVector<otherDim, dim>(other.getDirection()));
		}
		template<Dimension otherDim>
			requires (otherDim >= 2)
		inline Point operator-(const Vector<otherDim>& other) const {
			return Point(getPosition() - refitVector<otherDim, dim>(other.getDirection()));
		}
		template<Dimension otherDim>
			requires (otherDim >= 2)
		inline Vector<otherDim> operator-(const Point<otherDim>& other) const {
			return Vector<otherDim>(refitVector<dim, otherDim>(getPosition()) - other.getPosition());
		}
		inline scalar& operator[](Dimension i) {
			return HomogeneousCoordinate::getCoordinate()[i];
		}
		inline const scalar& operator[](Dimension i) const {
			return HomogeneousCoordinate::getCoordinate()[i];
		}
		inline bool operator==(const Point& other) const {
			return HomogeneousCoordinate::getHomogeneousCoordinate() == other.getHomogeneousCoordinate();
		}
		inline bool operator!=(const Point& other) const {
			return !(*this == other);
		}

		template<Dimension target>
			requires (target >= dim)
		inline Point<target> pad(scalar paddingValue = scalar()) const {
			return Point<target>(padVector<dim, target>(getPosition(), paddingValue));
		}
		template<Dimension target>
			requires (target <= dim)
		inline Point<target> collapse() const {
			return Point<target>(collapseVector<dim, target>(getPosition()));
		}
		template<Dimension target>
			requires (target >= 2)
		inline Point<target> refit() const {
			return Point<target>(refitVector<dim, target>(getPosition()));
		}
	protected:
		virtual void normalize() {
			if(HomogeneousCoordinate::getScale() != 0.f) { // normalize the homogeneous coordinate, skip if degenerate
				HomogeneousCoordinate::setHomogeneousCoordinate(HomogeneousCoordinate::getHomogeneousCoordinate() / HomogeneousCoordinate::getScale());
			}
		}
	};

	/**
	 * @brief Computes the (componentwise) minimum of 2 Points
	 * @tparam dim the Dimension of the Points
	 * @param a one of the Points to provide
	 * @param b the other Point to provide
	 * @return the (componentwise) minimum Point
	 */
	template<Dimension dim>
		requires (dim >= 2)
	inline Point<dim> min(const Point<dim>& a, const Point<dim>& b) {
		return Point<dim>(Math::elementWiseMin<dim>(a.getPosition(), b.getPosition()));
	}

	/**
	 * @brief Computes the (componentwise) maximum of 2 Points
	 * @tparam dim the Dimension of the Points
	 * @param a one of the Points to provide
	 * @param b the other Point to provide
	 * @return the (componentwise) maximum Point
	 */
	template<Dimension dim>
		requires (dim >= 2)
	inline Point<dim> max(const Point<dim>& a, const Point<dim>& b) {
		return Point<dim>(Math::elementWiseMax<dim>(a.getPosition(), b.getPosition()));
	}

	//TODO
	template<Dimension dim>
		requires (dim >= 2)
	inline Point<dim> clamp(const Point<dim>& p, const Point<dim>& b1, const Point<dim>& b2) {
		return Point<dim>(Math::elementWiseClamp<dim>(p.getPosition(), b1.getPosition(), b2.getPosition()));
	}

	template<Dimension dim>
		requires (dim >= 2)
	inline Point<dim> snapToBounds(const Point<dim>& p, const Point<dim>& b1, const Point<dim>& b2) {
		return Point<dim>(Math::snapToBounds<dim>(p.getPosition(), b1.getPosition(), b2.getPosition()));
	}

	/**
	 * @brief Transforms a Point with a TransformMatrix
	 * @tparam native the Dimension of the Point
	 * @tparam transforming the Dimension of the TransformMatrix
	 * @param point the Point to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Point
	 */
	template<Dimension native, Dimension transforming>
	Point<native> transform(const Point<native>& point, const mat<transforming>& t) {
		return Point<native>(transformPosition<native, transforming>(point.getPosition(), t));
	}

	/**
	 * @brief Vector, a Vector in dim dimensional space, represented by a dim + 1 homogeneous coordinate
	 * @tparam dim 
	 */
	template<Dimension dim>
	requires (dim >= 2)
	struct Vector : protected HomogeneousCoordinate<dim> {
	public:
		using vecd = vec<dim>;
		using CoordVec = vec<dim + 1>;
		using HomogeneousCoordinate = HomogeneousCoordinate<dim>;
	public:
		Vector() : HomogeneousCoordinate(vecd(0.f), 0.f) { }
		Vector(std::initializer_list<scalar> list) : HomogeneousCoordinate (list, 0.f) {
			C78E_CORE_ASSERT(list.size() != dim + 1, "Vector: initializer list does not fit the Dimension!(setting the homogeneous w coordinate, is not supported for Vector! w=0!)");
			C78E_CORE_ASSERT(list.size() == dim, "Vector: initializer list does not fit the Dimension!(setting the homogeneous w coordinate, is not supported for Vector! w=0!)");
		}
		template <typename ...Args>
		Vector(Args... args) : HomogeneousCoordinate(vecd{ static_cast<scalar>(args)... }, 0.f) {
			C78E_CORE_STATIC_ASSERT(sizeof...(args) == dim, "Point: Args do not match Dimension!");
			normalize();
		}
		Vector(scalar value) : HomogeneousCoordinate(vecd(value), 0.f) { }
		Vector(vecd direction) : HomogeneousCoordinate(direction, 0.f) { }
		Vector(CoordVec direction) : HomogeneousCoordinate(direction) { }
		Vector(Vector&) = default;
		Vector(const Vector&) = default;
		~Vector() = default;

		inline const vecd& getDirection() const {
			return HomogeneousCoordinate::getCoordinate();
		}
		inline void setDirection(vecd position) {
			HomogeneousCoordinate::setCoordinate(position);
		}

		inline const CoordVec& getHomogeneousDirection() const {
			return HomogeneousCoordinate::getHomogeneousCoordinate();
		}

		inline operator vecd& () {
			return HomogeneousCoordinate::getCoordinate();
		}
		inline operator const vecd&() const {
			return HomogeneousCoordinate::getCoordinate();
		}

		inline Vector operator+() const {
			return *this;
		}
		inline Vector operator-() const {
			return Vector(-getDirection());
		}
		inline Vector operator+(const Vector& other) const {
			return Vector(getDirection() + other.getDirection());
		}
		inline Vector operator-(const Vector& other) const {
			return Vector(getDirection() - other.getDirection());
		}
		inline Point<dim> operator+(const Point<dim>& other) const {
			return Point<dim>(getDirection() + other.getPosition());
		}
		inline Point<dim> operator-(const Point<dim>& other) const {
			return Point<dim>(getDirection() - other.getPosition());
		}
		inline Vector& operator=(const Vector& other) {
			HomogeneousCoordinate::setHomogeneousCoordinate(other.getHomogeneousDirection());
			return *this;
		}
		inline Vector& operator+=(const Vector& other) {
			*this = *this + other;
			return *this;
		}
		inline Vector& operator-=(const Vector& other) {
			*this = *this - other;
			return *this;
		}
		inline scalar& operator[](Dimension i) {
			return HomogeneousCoordinate::getCoordinate()[i];
		}
		inline const scalar& operator[](Dimension i) const {
			return HomogeneousCoordinate::getCoordinate()[i];
		}
		inline bool operator==(const Vector& other) const {
			return HomogeneousCoordinate::getHomogeneousCoordinate() == other.getHomogeneousCoordinate();
		}
		inline bool operator!=(const Vector& other) const {
			return !(*this == other);
		}

		inline Vector operator*(scalar s) const {
			return Vector(getDirection() * s);
		}
		inline Vector operator/(scalar s) const {
			C78E_CORE_ASSERT(s != 0.f, "Vector::operator/: Division by zero!");
			return Vector(getDirection() / s);
		}

		inline scalar dot(const Vector& other) const {
			return Math::dot<dim>(getDirection(), other.getDirection());
		}
		inline Vector projectOn(const Vector& other) const {
			return Vector(Math::projectOn<dim>(getDirection(), other.getDirection()));
		}
		inline scalar projectionScaleOn(const Vector& other) const {
			return Math::projectionScaleOn(getDirection(), other.getDirection());
		}
		inline scalar projectionLengthOn(const Vector& other) const {
			return Math::projectionLengthOn(getDirection(), other.getDirection());
		}
		inline scalar length() const {
			return glm::length(getDirection());
		}
		inline scalar lengthSquared() const {
			return glm::length2(getDirection());
		}
		inline Vector normalize() const {
			if(isNullVector()) return *this;
			return Vector(glm::normalize(getDirection()));
		}
		inline Vector abs() const {
			return Vector(Math::elementWiseAbs(getDirection()));
		}
		inline Vector signs() const {
			const Vector abs = this->abs();
			Vector signs = *this / abs;
			for(Dimension i = 0; i < dim; i++) {
				if(abs[i] == 0.f) {
					signs[i] = 0.f;
				}
			}
			return signs;
		}
		inline bool alignsWith(const Vector& other) const {
			return Math::doAlign(getDirection(), other.getDirection());
		}
		inline bool isParallel(const Vector& other) const {
			return Math::inParallel(getDirection(), other.getDirection());
		}

		inline Vector operator*(const Vector& other) const {
			return Math::elementWiseMul(getDirection(), other.getDirection());
		}
		inline Vector operator/(const Vector& other) const {
			return Math::elementWiseDiv(getDirection(), other.getDirection());
		}

		inline bool isNullVector() const {
			return isNearZero(length());
		}

		Vector constructNormal() {
			if constexpr(dim == 2) {
				// 2D normal: rotate 90 degrees (counter-clockwise)
				vec<2> d = getDirection();
				return Vector<dim>({ -d.y, d.x });
			} else { //TODO: use Math gram-schmidt 
				// For dim > 2, we can't return a unique normal without more context.
				// Fallback: Find *any* vector orthogonal to v (e.g. using Gram-Schmidt on basis vector)
				vec<dim> d = glm::normalize(getDirection());

				// Pick a non-parallel basis vector
				vec<dim> basis(0.0f);
				basis[0] = 1.0f;

				if(isNearZero(1.f - glm::abs(glm::dot(basis, d)))) {
					basis[0] = 0.0f;
					basis[1] = 1.0f;
				}

				// Gram-Schmidt orthogonalization
				vec<dim> ortho = basis - glm::dot(basis, d) * d;
				ortho = glm::normalize(ortho);

				return Vector(ortho);
			}
		}

		template<Dimension target>
			requires (target >= dim)
		inline Vector<target> pad() const {
			return Vector<target>(pad<dim, target>(getDirection()));
		}
		template<Dimension target>
			requires (target <= dim)
		inline Vector<target> collapse() const {
			return Vector<target>(collapse<dim, target>(getDirection()));
		}
		template<Dimension target>
			requires (target >= 2)
		inline Vector<target> refit() const {
			return Vector<target>(Math::refitVector<dim, target>(getDirection()));
		}
	};

	/**
	 * @brief commutative operator for Vector::operator*(scalar)
	 * @tparam dim the Dimension of the Vector
	 * @param scalar the scalar to scale by
	 * @param vec the Vector to scale
	 * @return the scaled Vector
	 */
	template<Dimension dim>
	Vector<dim> operator*(scalar scalar, const Vector<dim>& vec) {
		return vec * scalar;
	}

	/**
	 * @brief Computes the (componentwise) minimum of 2 Vectors
	 * @tparam dim the Dimension of the Vectors
	 * @param a one of the Vectors to provide
	 * @param b the other Vector to provide
	 * @return the (componentwise) minimum Vector
	 */
	template<Dimension dim>
		requires (dim >= 2)
	Vector<dim> min(const Vector<dim>& a, const Vector<dim>& b) {
		return Vector<dim>(glm::min(a.getDirection(), b.getDirection()));
	}

	/**
	 * @brief Computes the (componentwise) maximum of 2 Vectors
	 * @tparam dim the Dimension of the Vectors
	 * @param a one of the Vectors to provide
	 * @param b the other Vector to provide
	 * @return the (componentwise) maximum Vector
	 */
	template<Dimension dim>
		requires (dim >= 2)
	Vector<dim> max(const Vector<dim>& a, const Vector<dim>& b) {
		return Vector<dim>(glm::max(a.getDirection(), b.getDirection()));
	}

	//TODO
	template<Dimension dim>
		requires (dim >= 2)
	inline Vector<dim> clamp(const Vector<dim>& v, const Vector<dim>& b1, const Vector<dim>& b2) {
		return Vector<dim>(Math::elementWiseClamp<dim>(v.getDirection(), b1.getDirection(), b2.getDirection()));
	}

	template<Dimension dim>
		requires (dim >= 2)
	inline Vector<dim> snapToBounds(const Vector<dim>& p, const Vector<dim>& b1, const Vector<dim>& b2) {
		return Vector<dim>(Math::snapToBounds<dim>(p.getDirection(), b1.getDirection(), b2.getDirection()));
	}

	/**
	 * @brief Transforms a Vector with a TransformMatrix
	 * @tparam native the Dimension of the Vector
	 * @tparam transforming the Dimension of the TransformMatrix
	 * @param vector the Vector to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Vector
	 */
	template<Dimension native, Dimension transforming>
	Vector<native> transform(const Vector<native>& vector, const mat<transforming>& t) {
		return Vector<native>(transformVector<native, transforming>(vector.getDirection(), t));
	}


	/*
	* Point and Vector arrays
	*/
	using PointsCount = size_t;
	template<Dimension dim, PointsCount size>
	struct Points;
	
	using VectorsCount = size_t;
	template<Dimension dim, VectorsCount size>
	struct Vectors;

	/**
	 * @brief Points, array of Points of the same Dimension
	 * @tparam dim the Dimension of the Points
	 * @tparam size the number of Points in the array
	 */
	template<Dimension dim, PointsCount size>
	struct Points : public std::array<Point<dim>, size> {
	public:
		using Count = PointsCount;
	public:
		Points() = default;
		Points(std::initializer_list<Point<dim>> init) {
			C78E_CORE_ASSERT(init.size() == size, "Points::Points: Initializer list size did not match Points size!");
			std::copy(init.begin(), init.end(), this->begin());
		}
		~Points() = default;

		Vectors<dim, size> getOriginVectors() const {
			Vectors<dim, size> vectors;
			for(PointsCount i = 0; i < size; i++) {
				vectors[i] = this->operator[](i).getOriginVector();
			}
			return vectors;
		}
	};

	/**
	 * @brief Computes the (componentwise) minimum of multiple Points
	 * @tparam dim the Dimension of the Points
	 * @param points the Points to provide
	 * @return the (componentwise) minimum Point of all Points
	 */
	template<Dimension dim, PointsCount size>
		requires (dim >= 2)
	Point<dim> min(const Points<dim, size>& points) {
		Point<dim> minPoint = points[0];
		for(PointsCount i = 1; i < size; i++) {
			minPoint = Math::min<dim>(minPoint, points[i]);
		}
		return minPoint;
	}

	/**
	 * @brief Computes the (componentwise) maximum of multiple Points
	 * @tparam dim the Dimension of the Points
	 * @param points the Points to provide
	 * @return the (componentwise) maximum Point of all Points
	 */
	template<Dimension dim, PointsCount size>
		requires (dim >= 2)
	Point<dim> max(const Points<dim, size>& points) {
		Point<dim> maxPoint = points[0];
		for(PointsCount i = 1; i < size; i++) {
			maxPoint = Math::max<dim>(maxPoint, points[i]);
		}
		return maxPoint;
	}

	/**
	 * @brief Transforms a Point with a TransformMatrix
	 * @tparam native the Dimension of the Points
	 * @tparam transforming the Dimension of the TransformMatrix
	 * @param points the Points to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Points
	 */
	template<Dimension native, PointsCount size, Dimension transforming>
	Points<native, size> transform(const Points<native, size>& points, const mat<transforming>& t) {
		Points<native, size> transformedPoints;
		for(PointsCount i = 0; i < size; i++) {
			transformedPoints[i] = transform(points[i], t);
		}
		return transformedPoints;
	}


	/**
	 * @brief Vectors, array of Vectors of the same Dimension
	 * @tparam dim the Dimension of the Vectors
	 * @tparam size the number of Vectors in the array
	 */
	template<Dimension dim, VectorsCount size>
	struct Vectors : public std::array<Vector<dim>, size> {
	public:
		using Count = VectorsCount;
	public:
		Vectors() = default;
		Vectors(std::initializer_list<Vector<dim>> init) {
			C78E_CORE_ASSERT(init.size() == size, "Vectors::Vectors: Initializer list size did not match Vector size!");
			std::copy(init.begin(), init.end(), this->begin());
		}
		~Vectors() = default;

	};

	/**
	 * @brief Computes the (componentwise) minimum of multiple Vectors
	 * @tparam dim the Dimension of the Vectors
	 * @param vectors the Vectors to provide
	 * @return the (componentwise) minimum Vector of all Vectors
	 */
	template<Dimension dim, VectorsCount size>
		requires (dim >= 2)
	Vector<dim> min(const Vectors<dim, size>& vectors) {
		Vector<dim> minVector = vectors[0];
		for(VectorsCount i = 1; i < size; i++) {
			minVector = Math::min<dim>(minVector, vectors[i]);
		}
		return minVector;
	}

	/**
	 * @brief Computes the (componentwise) maximum of multiple Vectors
	 * @tparam dim the Dimension of the Vectors
	 * @param vectors the Vectors to provide
	 * @return the (componentwise) maximum Vector of all Vectors
	 */
	template<Dimension dim, VectorsCount size>
		requires (dim >= 2)
	Vector<dim> max(const Vectors<dim, size>& vectors) {
		Vector<dim> maxVector = vectors[0];
		for(VectorsCount i = 1; i < size; i++) {
			maxVector = Math::max<dim>(maxVector, vectors[i]);
		}
		return maxVector;
	}

	/**
	 * @brief Computes the cross product of dim - 1 vectors in dim space
	 * @tparam dim the Dimension of the vectors
	 * @param vectors the dim - 1 vectors to provide
	 * @return the cross product of the vectors
	 */
	template<Dimension dim>
		requires (dim >= 2)
	Vector<dim> cross(const Vectors<dim, dim - 1>& vectors) {
		std::array<vec<dim>, dim - 1> vecs;
		for(VectorsCount i = 0; i < dim - 1; i++) {
			vecs[i] = vectors[i].getDirection();
		}
		const vec<dim> result = Math::cross<dim>(vecs);
		return Vector<dim>(result);
	}

	/**
	 * @brief Transforms a Vector with a TransformMatrix
	 * @tparam native the Dimension of the Vectors
	 * @tparam transforming the Dimension of the TransformMatrix
	 * @param vectors the Vectors to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Vectors
	 */
	template<Dimension native, VectorsCount size, Dimension transforming>
	Vectors<native, size> transform(const Vectors<native, size>& vectors, const mat<transforming>& t) {
		Vectors<native, size> transformedVectors;
		for(VectorsCount i = 0; i < size; i++) {
			transformedVectors[i] = transform(vectors[i], t);
		}
		return transformedVectors;
	}

	/*
	* Point and Vector dynamic vectors
	*/
	template<Dimension dim>
	struct DynamicPoints;

	template<Dimension dim>
	struct DynamicVectors;

	/**
	 * @brief DynamicPoints, dynamic array of Points of the same Dimension
	 * @tparam dim the Dimension of the Points
	 */
	template<Dimension dim>
	struct DynamicPoints : public std::vector<Point<dim>> {
	public:
		using Count = size_t;
	public:
		DynamicPoints() = default;
		DynamicPoints(std::initializer_list<Point<dim>> init) {
			std::copy(init.begin(), init.end(), this->begin());
		}
		~DynamicPoints() = default;
	};

	/**
	 * @brief Computes the (componentwise) minimum of multiple Points
	 * @tparam dim the Dimension of the Points
	 * @param points the Points to provide
	 * @return the (componentwise) minimum Point of all Points
	 */
	template<Dimension dim>
		requires (dim >= 2)
	inline Point<dim> min(const DynamicPoints<dim>& points) {
		Point<dim> minPoint = points[0];
		for(PointsCount i = 1; i < points.size(); i++) {
			minPoint = Math::min<dim>(minPoint, points[i]);
		}
		return minPoint;
	}

	/**
	 * @brief Computes the (componentwise) maximum of multiple Points
	 * @tparam dim the Dimension of the Points
	 * @param points the Points to provide
	 * @return the (componentwise) maximum Point of all Points
	 */
	template<Dimension dim>
		requires (dim >= 2)
	inline Point<dim> max(const DynamicPoints<dim>& points) {
		Point<dim> maxPoint = points[0];
		for(PointsCount i = 1; i < points.size(); i++) {
			maxPoint = Math::max<dim>(maxPoint, points[i]);
		}
		return maxPoint;
	}

	/**
	 * @brief Transforms a Point with a TransformMatrix
	 * @tparam native the Dimension of the Points
	 * @tparam transforming the Dimension of the TransformMatrix
	 * @param points the Points to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Points
	 */
	template<Dimension dim, Dimension transforming>
	DynamicPoints<dim> transform(const DynamicPoints<dim>& points, const mat<transforming>& t) {
		DynamicPoints<dim> transformedPoints;
		transformedPoints.reserve(points.size());
		for(PointsCount i = 0; i < points.size(); i++) {
			transformedPoints.emplace_back(transform(points[i], t));
		}
		return transformedPoints;
	}

	/**
	 * @brief DynamicVectors, dynamic array of Vectors of the same Dimension
	 * @tparam dim the Dimension of the Vectors
	 */
	template<Dimension dim>
	struct DynamicVectors : public std::vector<Vector<dim>> {
	public:
		using Count = size_t;
	public:
		DynamicVectors() = default;
		DynamicVectors(std::initializer_list<Vector<dim>> init) {
			std::copy(init.begin(), init.end(), this->begin());
		}
		~DynamicVectors() = default;
	};

	/**
	 * @brief Computes the (componentwise) minimum of multiple Vectors
	 * @tparam dim the Dimension of the Vectors
	 * @param vectors the Vectors to provide
	 * @return the (componentwise) minimum Vector of all Vectors
	 */
	template<Dimension dim>
		requires (dim >= 2)
	inline Vector<dim> min(const DynamicVectors<dim>& vectors) {
		Vector<dim> minVector = vectors[0];
		for(VectorsCount i = 1; i < vectors.size(); i++) {
			minVector = Math::min<dim>(minVector, vectors[i]);
		}
		return minVector;
	}

	/**
	 * @brief Computes the (componentwise) maximum of multiple Vectors
	 * @tparam dim the Dimension of the Vectors
	 * @param vectors the Vectors to provide
	 * @return the (componentwise) maximum Vector of all Vectors
	 */
	template<Dimension dim>
		requires (dim >= 2)
	inline Vector<dim> max(const DynamicVectors<dim>& vectors) {
		Vector<dim> maxVector = vectors[0];
		for(VectorsCount i = 1; i < vectors.size(); i++) {
			maxVector = Math::max<dim>(maxVector, vectors[i]);
		}
		return maxVector;
	}

	/**
	 * @brief Transforms a Vector with a TransformMatrix
	 * @tparam native the Dimension of the Vectors
	 * @tparam transforming the Dimension of the TransformMatrix
	 * @param vectors the Vectors to transform
	 * @param t the TransformMatrix to transform with
	 * @return the transformed Vectors
	 */
	template<Dimension dim, Dimension transforming>
	DynamicVectors<dim> transform(const DynamicVectors<dim>& vectors, const mat<transforming>& t) {
		DynamicVectors<dim> transformedVectors;
		transformedVectors.reserve(vectors.size());
		for(VectorsCount i = 0; i < vectors.size(); i++) {
			transformedVectors.emplace_back(transform(vectors[i], t));
		}
		return transformedVectors;
	}

}

#include <C78E/Utils/StdUtils.h>
namespace std {

	//Point
	template<C78E::Math::Dimension dim>
	_EXPORT_STD _NODISCARD inline string to_string(const C78E::Math::Point<dim>& point, const size_t& decimals = -1) {
		return to_string(point.getHomogeneousCoordinate(), decimals);
	}

	template<C78E::Math::Dimension dim>
	struct hash<C78E::Math::Point<dim>> {
		size_t operator()(const C78E::Math::Point<dim>& point) const {
			return hash<C78E::Math::Point<dim>::CoordVec>()(point.getHomogeneousCoordinate());
		}
	};

	//Vector
	template<C78E::Math::Dimension dim>
	_EXPORT_STD _NODISCARD inline string to_string(const C78E::Math::Vector<dim>& vector, const size_t& decimals = -1) {
		return to_string(vector.getHomogeneousCoordinate(), decimals);
	}

	template<C78E::Math::Dimension dim>
	struct hash<C78E::Math::Vector<dim>> {
		size_t operator()(const C78E::Math::Vector<dim>& vector) const {
			return hash<C78E::Math::Vector<dim>::CoordVec>()(vector.getHomogeneousCoordinate());
		}
	};

}

#include <C78E/Utils/YamlUtils.h>
namespace YAML {

	//Point
	template<C78E::Math::Dimension dim>
	inline Emitter& operator<<(Emitter& out, const C78E::Math::Point<dim>& point) {
		out << point.getHomogeneousCoordinate();
		return out;
	}
	template<C78E::Math::Dimension dim>
	struct convert<C78E::Math::Point<dim>> {
		static Node encode(const C78E::Math::Point<dim>& point) {
			return convert<C78E::Math::vec<dim + 1>>::encode(point.getHomogeneousCoordinate());
		}
		static bool decode(const Node& node, C78E::Math::Point<dim>& point) {
			C78E::Math::vec<dim + 1> coordinate;
			const bool result = convert<C78E::Math::vec<dim + 1>>::decode(node, coordinate);
			point.setHomogeneousCoordinate(coordinate);
			return result;
		}

	};

	//Vector
	template<C78E::Math::Dimension dim>
	inline Emitter& operator<<(Emitter& out, const C78E::Math::Vector<dim>& vector) {
		out << vector.getHomogeneousCoordinate();
		return out;
	}
	template<C78E::Math::Dimension dim>
	struct convert<C78E::Math::Vector<dim>> {
		static Node encode(const C78E::Math::Vector<dim>& vector) {
			return convert<C78E::Math::vec<dim + 1>>::encode(vector.getHomogeneousCoordinate());
		}
		static bool decode(const Node& node, C78E::Math::Vector<dim>& vector) {
			C78E::Math::vec<dim + 1> coordinate;
			const bool result = convert<C78E::Math::vec<dim + 1>>::decode(node, coordinate);
			vector = C78E::Math::Vector<dim>(coordinate);
			return result;
		}

	};
}