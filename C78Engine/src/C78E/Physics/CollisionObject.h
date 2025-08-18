#pragma once
#include "Collision/Collider/Collider.h"
#include "PhysicsMaterial.h"

namespace C78E::Physics {

	template<Dimension dim>
	struct Collision;

	template<Dimension dim>
	using CollisionCallbackFunc = std::function<void(Collision<dim>&, Timestep)>;
	
	template<Dimension dim>
	class RigidBody;

	template<Dimension dim>
	class CollisionObject {
	public:
		using vecd = vec<dim>;
		using Point = Point<dim>;
		using Vector = Vector<dim>;
		using Transform = Transform<dim>;
	public:
		CollisionObject(Collider<dim>* collider, CollisionCallbackFunc<dim> onCollisionCallback = nullptr)
		: m_Collider(collider), m_OnCollision(onCollisionCallback) {
			C78E_CORE_ASSERT(m_Collider, "CollisionObject::CollisionObject: collider provided was null!");
		}
		CollisionObject(Collider<dim>* collider, bool isTrigger, CollisionCallbackFunc<dim> onCollisionCallback)
			: m_Collider(collider), m_IsTrigger(isTrigger), m_OnCollision(onCollisionCallback) {
			C78E_CORE_ASSERT(m_Collider, "CollisionObject::CollisionObject: collider provided was null!");
		}
		CollisionObject(CollisionObject& other)
			: m_Transform(nullptr), m_Collider(other.m_Collider), m_IsTrigger(other.m_IsTrigger), m_OnCollision(other.m_OnCollision) { }
		CollisionObject(const CollisionObject& other)
			: m_Transform(nullptr), m_Collider(other.m_Collider), m_IsTrigger(other.m_IsTrigger), m_OnCollision(other.m_OnCollision) {
		}
		virtual ~CollisionObject() = default;
		
		virtual Transform& getTransform() final {
			return *m_Transform;
		}
		virtual const Transform& getTransform() const final {
			return *m_Transform;
		}
		virtual Point getTranslation() const final {
			if constexpr(dim != Math::Dimensions::D2) {
				return m_Transform->getTranslation();
			} else {
				return m_Transform->getTranslation().refit<Math::Dimensions::D2>();
			}
		}
		virtual const Vector& getScale() const final {
			return m_Transform->getScale();
		}

		virtual Collider<dim>* getCollider() final {
			return m_Collider;
		}
		virtual const Collider<dim>* getCollider() const final {
			return m_Collider;
		}
		virtual bool isTrigger() const final {
			return m_IsTrigger;
		}

		virtual bool isStatic() const final {
			const RigidBody<dim>* rb = dynamic_cast<const RigidBody<dim>*>(this);
			return rb == nullptr;
		}

		virtual void setCollisionCallback(CollisionCallbackFunc<dim> onCollisionCallback) final {
			m_OnCollision = onCollisionCallback;
		}
		virtual void callCollisionCallback(Collision<dim>& collision, Timestep deltaTime) final {
			if(m_OnCollision)
				m_OnCollision(collision, deltaTime);
		}

		virtual void useTransform(Transform* transform) final {
			m_Transform = transform;
		}
		virtual void useCollider(Collider<dim>* collider) final {
			m_Collider = collider;
		}

		CollisionObject<dim>& operator=(const CollisionObject<dim>& other) {
			m_Transform = other.m_Transform;
			m_Collider = other.m_Collider;
			m_IsTrigger = other.m_IsTrigger;
			m_OnCollision = other.m_OnCollision;
			return *this;
		}
	protected:
		Transform* m_Transform = nullptr;
		Collider<dim>* m_Collider;
		bool m_IsTrigger = false;
		CollisionCallbackFunc<dim> m_OnCollision = nullptr;
	};

	template<Dimension dim>
	class RigidBody : public CollisionObject<dim> {
	public:
		using vecd = vec<dim>;
		using matd = mat<dim + 1>;
		using Transform = Transform<dim>;
		using Rotation = Rotation<dim>;
	public:
		RigidBody(Collider<dim>* collider,
			bool isKinematic, float mass, bool useWorldGravity, vecd gravity,
			PhysicsMaterial material, CollisionCallbackFunc<dim> onCollisionCallback = nullptr)
			: CollisionObject<dim>(collider, onCollisionCallback),
			m_IsSimulated(isKinematic),
			m_Mass(mass),
			m_UseWorldGravity(useWorldGravity),
			m_Gravity(gravity),
			m_Material(material) {
		}
		RigidBody(RigidBody&) = default;
		RigidBody(const RigidBody&) = default;
		virtual ~RigidBody() = default;

		//virtual void applyVelocity(const vecd& velocity) {
		//	(*(vecd*)&CollisionObject<dim>::getTransform().translation) += velocity;
		//	m_Velocity = velocity;
		//}
		virtual vecd& getVelocity() {
			return m_Velocity;
		}
		virtual vecd getVelocity() const {
			return m_Velocity;
		}
		
		virtual void applyForce(const vecd& force, const vecd& position = vecd()) {
			m_Force += force;
			applyTorque(Rotation::getTorque(force, position));
		}
		virtual vecd& getForce() {
			return m_Force;
		}
		virtual vecd getForce() const {
			return m_Force;
		}

		//virtual void applyAngularVelocity(const vecd& angularVelocity) {
		//	CollisionObject<dim>::getTransform().rotation) += <quat to vecd op> angularVelocity;
		//	m_AngularVelocity = angularVelocity;
		//}
		virtual bool hasAngularVelocity() const {
			return (bool)getAngularVelocity();
		}
		virtual Rotation& getAngularVelocity() {
			return m_AngularVelocity;
		}
		virtual Rotation getAngularVelocity() const {
			return m_AngularVelocity;
		}

		virtual void applyTorque(const Rotation& torque) {
			m_Torque += torque;
		}
		virtual Rotation& getTorque() {
			return m_Torque;
		}
		virtual Rotation getTorque() const {
			return m_Torque;
		}

		virtual matd& getInvInertia() {
			return m_InvInteria;
		}
		virtual matd getInvInertia() const {
			return m_InvInteria;
		}

		virtual bool usesWorldGravity() const {
			return m_UseWorldGravity;
		}
		virtual bool hasOwnGravity() const {
			return !usesWorldGravity();
		}
		virtual vecd getOwnGravity() const {
			return m_Gravity;
		}
		virtual vecd setOwnGravity(const vecd& gravity) {
			return m_Gravity = gravity;
		}
		virtual void applyOwnGravity() {
			applyForce(getOwnGravity() * getMass());
		}
		virtual void applyGravity(const vecd& worldGravity) {
			applyForce(
				((usesWorldGravity()) ? worldGravity : getOwnGravity())
				* getMass()
			);
		}

		virtual bool isSimulated() const {
			return m_IsSimulated;
		}

		virtual scalar getMass() const {
			return m_Mass;
		}
		virtual scalar setMass(scalar mass) {
			return m_Mass = mass;
		}

		virtual PhysicsMaterial& getMaterial() {
			return m_Material;
		}
		virtual PhysicsMaterial getMaterial() const {
			return m_Material;
		}

		RigidBody<dim>& operator=(const RigidBody<dim>& other) {
			m_Force = other.m_Force;
			m_Velocity = other.m_Velocity;
			m_Torque = other.m_Torque;
			m_AngularVelocity = other.m_AngularVelocity;
			m_InvInteria = other.m_InvInteria;
			m_Mass = other.m_Mass;
			m_UseWorldGravity = other.m_UseWorldGravity;
			m_Gravity = other.m_Gravity;
			m_IsSimulated = other.m_IsSimulated;
			m_ExudeGravity = other.m_ExudeGravity;
			m_Material = other.m_Material;
			return *this;
		}
	protected:
		vecd m_Force = vecd(0.f);
		vecd m_Velocity = vecd(0.f);

		Rotation m_Torque = Rotation();
		Rotation m_AngularVelocity = Rotation();
		matd m_InvInteria = glm::inverse(matd(1.0f));

		scalar m_Mass = 0.f; //TODO: invMass? why?

		bool m_UseWorldGravity = true;
		vecd m_Gravity = vecd(0.f);

		bool m_IsSimulated = true;
		bool m_ExudeGravity = false; //TODO: impl

		PhysicsMaterial m_Material = PhysicsMaterial();
	};

	template<Dimension dim>
	struct Collision {
		CollisionObject<dim>* objA;
		CollisionObject<dim>* objB;
		CollisionPoints<dim> points;
	};

}