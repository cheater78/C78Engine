#pragma once

namespace C78E {

	template<typename T>
	concept HashablePrimitive = Primitive<T> || ShallowType<T>;

	class Hash {
	public:
		Hash() = delete;
	private:
		Hash(size_t raw) : m_Hash(raw) { }
	public:
		template<HashablePrimitive T>
		Hash(T value)
			: m_Hash(hashBytes(&value, sizeof(T))) {
		}
		Hash(const void* data, size_t size)
			: m_Hash(hashBytes(data, size)) {
		}
		Hash(const Hash& other) = default;
		Hash(Hash&& other) = default;
		~Hash() = default;

		const size_t& raw() const;

		bool operator==(const Hash& other) const {
			return m_Hash == other.m_Hash;
		}
		bool operator!=(const Hash& other) const {
			return !(*this == other);
		}
		Hash operator+(const Hash& other) const {
			return Hash(combine(raw(), other.raw()));
		}
		Hash& operator+=(const Hash& other) {
			m_Hash = combine(m_Hash, other.raw());
			return *this;
		}

	protected:
		static size_t combine(size_t hash0, size_t hash1);
		static size_t hashByte(const uint8_t& byte);
		static size_t hashBytes(const void* data, size_t size);
	private:
		size_t m_Hash;

	};

	class Hashable {
	public:
		Hashable() = default;
		virtual ~Hashable() = default;

		virtual Hash hash() const = 0;
	};

	template<typename T>
	concept HashableType = std::is_base_of_v<Hashable, T>;
	

}