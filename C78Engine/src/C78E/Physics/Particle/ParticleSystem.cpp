#include "C78EPCH.h"
#include "ParticleSystem.h"

namespace C78E {
	/*
	ParticleSystem::ParticleIterator::ParticleIterator(ParticleIteratorBase it, ParticleSystem& particleSystem)
		: ParticleIteratorBase((!particleSystem.empty()) ? it : particleSystem.m_Particles.end()), m_ParticleSystem(particleSystem) {
		// Skip dead particles at the beginning
		while (*this != m_ParticleSystem.m_Particles.end() && !operator*().isAlive()) {
			ParticleIteratorBase::operator++();
		}
	}

	Particle& ParticleSystem::ParticleIterator::operator*() {
		return ParticleIteratorBase::operator*();
	}

	Particle* ParticleSystem::ParticleIterator::operator->() {
		return ParticleIteratorBase::operator->();
	}

	ParticleSystem::ParticleIterator& ParticleSystem::ParticleIterator::operator++() {
		if (*this == m_ParticleSystem.m_Particles.end())
			return *this;
		do {
			ParticleIteratorBase::operator++();
		} while (*this != m_ParticleSystem.m_Particles.end() && !this->operator*().isAlive());
		return *this;
	}

	ParticleSystem::ParticleIterator ParticleSystem::ParticleIterator::operator++(int) {
		ParticleIterator it = *this;
		ParticleIterator::operator++();
		return it;
	}


	DeltaTransform ParticleSystem::interpolate(const Transform& start, const Transform& end, float lifeTime) {
		return (end - start) / lifeTime;
	}

	DeltaTransform ParticleSystem::expandUniform(float scale, float lifeTime) {
		return DeltaTransform{ glm::vec3{ scale * C78E::Random::getFloat(-.5f, .5f), scale * C78E::Random::getFloat(-.5f, .5f), scale * C78E::Random::getFloat(-.5f, .5f) } / lifeTime, glm::vec3{ 0.f, 0.f, 0.f } };
	}

	DeltaTransform ParticleSystem::expandUniform2D(float scale, float lifeTime) {
		return DeltaTransform{ glm::vec3{ scale * C78E::Random::getFloat(-.5f, .5f), scale * C78E::Random::getFloat(-.5f, .5f), 0.f } / lifeTime, glm::vec3{ 0.f, 0.f, 0.f } };
	}

	DeltaTransform ParticleSystem::rise(float height, float lifeTime) {
		return DeltaTransform{ glm::vec3{ 0.f, height * C78E::Random::getFloat(), 0.f } / lifeTime, glm::vec3{ 0.f, 0.f, 0.f } };
	}

	ParticleSystem::ParticleSystem(size_t size) : m_Particles(size) {
		for (size_t i = 0; i < size; i++) {
			m_FreeList.push(i);
		}
	}

	void ParticleSystem::emit(const Particle& p) {
		if (!m_FreeList.empty()) {
			size_t index = m_FreeList.top();
			m_FreeList.pop();
			m_Particles[index] = p;
		} else { // No free slots, add to end, vector resize is not that bad/often, since +size is 1.5/2.0 * current size (compiler dependend)
			m_Particles.emplace_back(p);
		}
	}

	void ParticleSystem::onUpdate(C78E::Timestep delta) {
		if (empty()) return;
		for (ParticleIterator it = this->begin(); it != end(); it++) {
			it->onUpdate(delta);
			if (!it->isAlive()) { // Particle is dead, add to free list
				m_FreeList.push(it - m_Particles.begin());
			}
		}
	}

	bool ParticleSystem::empty() const {
		return m_FreeList.size() >= m_Particles.size();
	}

	size_t ParticleSystem::size() const {
		return m_Particles.size() - m_FreeList.size();
	}

	ParticleSystem::ParticleIterator ParticleSystem::begin() {
		return ParticleIterator(m_Particles.begin(), *this);
	}

	ParticleSystem::ParticleIterator ParticleSystem::end() {
		return ParticleIterator(m_Particles.end(), *this);
	}
	*/

} // namespace C78E