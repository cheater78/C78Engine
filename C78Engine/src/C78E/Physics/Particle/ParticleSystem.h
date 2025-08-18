#pragma once
#include "Particle.h"

namespace C78E {

    /*
	class ParticleSystem {
	private:
		using ParticleIteratorBase = std::vector<Particle>::iterator;
    public:
        class ParticleIterator : public ParticleIteratorBase {
        public:
            ParticleIterator(ParticleIteratorBase it, ParticleSystem& particleSystem);
            ~ParticleIterator() = default;

            Particle& operator*();
            Particle* operator->();
            ParticleIterator& operator++();
            ParticleIterator operator++(int);

        private:
            ParticleSystem& m_ParticleSystem;
        };

    public:
		static DeltaTransform interpolate(const Transform& start, const Transform& end, float lifeTime);
		static DeltaTransform expandUniform(float scale, float lifeTime);
        static DeltaTransform expandUniform2D(float scale, float lifeTime);
        static DeltaTransform rise(float height, float lifeTime);

    public:
		ParticleSystem(size_t size = 256);
        ~ParticleSystem() = default;

        void emit(const Particle& p);
        void onUpdate(C78E::Timestep delta);

		bool empty() const;
		size_t size() const;
		ParticleIterator begin();
		ParticleIterator end();

    public:
        std::vector<Particle> m_Particles;
        std::stack<size_t> m_FreeList; // Stores available particle slots
	};
    */
}