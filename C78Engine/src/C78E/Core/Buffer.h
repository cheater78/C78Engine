#pragma once
#include <C78E/Utils/StdUtils.h>
#include <C78E/Core/Log/SmartLog.h>

namespace C78E {

	/**
	 * @brief Non owning Buffer, used for temporary data storage
	 * don't forget to release the buffer after usage
	 */
	struct Buffer {
	public:
		/**
		 * @brief Copies the data of the given Buffer to a new Buffer,
		 * both Buffers need to be released separately after usage
		 * @param other 
		 * @return 
		 */
		static bool copy(Buffer& from, Buffer& to);
	public:
		/**
		 * @brief Creates an empty Buffer, fields and allocations need to be handled manually
		 */
		Buffer() = default;
		/**
		 * @brief Creates a Buffer with the given size, 
		 * @param size 
		 */
		Buffer(size_t _size);
		Buffer(size_t _size, const void* _data);
		Buffer(const Buffer&) = delete;
		~Buffer();

		void allocate(size_t _size);
		void release();

		/**
		 * @brief 
		 * 
		 * [Important Note] Template functions must be defined in the header file -> Instatiation must be included (cant be linked bc non existing)
		 * @tparam T 
		 * @return 
		 */
		template<typename T>
		T* as() const {
			return (T*)data;
		}
		
		/**
		 * @brief 
		 * 
		 * [Important Note] Template functions must be defined in the header file -> Instatiation must be included (cant be linked bc non existing)
		 * @tparam T 
		 * @param value 
		 */
		template<typename T>
		void clear(const T& value) {
			C78E_CORE_ASSERT(size % sizeof(T) == 0, "Buffer::clear: Buffers size({}bytes) is not a multiple of the given Types size({}bytes)!", size, sizeof(T));
			for (T* elem = (T*)data; (size_t)elem < (size_t)data + size; elem++)
				*elem = value;
		}

		/**
		 * @brief 
		 * 
		 * [Important Note] Template functions must be defined in the header file -> Instatiation must be included (cant be linked bc non existing)
		 * @tparam T 
		 * @param index 
		 * @return 
		 */
		template<typename T>
		T& at(size_t index) {
			C78E_CORE_ASSERT(size % sizeof(T) == 0, "Buffer::at: Buffers size({}bytes) is not a multiple of the given Types size({}bytes)!", size, sizeof(T));
			C78E_CORE_ASSERT(index * sizeof(T) < size, "Buffer::at: Index({}) is out of bounds! Buffer has {} elements.", index, size / sizeof(T));
			return *((T*)data + index);
		}

		operator bool() const {
			return (bool)data;
		}

	public:
		uint8_t* data = nullptr;
		size_t size = 0;
	};

	struct ScopedBuffer {
	public:
		ScopedBuffer(size_t size);

		ScopedBuffer(size_t size, const void* data);

		ScopedBuffer(ScopedBuffer& other);

		~ScopedBuffer();

		uint8_t* data();
		size_t size() const;

		/**
		 * @brief 
		 * 
		 * [Important Note] Template functions must be defined in the header file -> Instatiation must be included (cant be linked bc non existing)
		 * @tparam T 
		 * @return 
		 */
		template<typename T>
		T* as() const {
			return m_Buffer.as<T>();
		}
		
		/**
		 * @brief 
		 * 
		 * [Important Note] Template functions must be defined in the header file -> Instatiation must be included (cant be linked bc non existing)
		 * @tparam T 
		 * @param value 
		 */
		template<typename T>
		void clear(const T& value) {
			m_Buffer.clear<T>(value);
		}

		/**
		 * @brief 
		 * 
		 * [Important Note] Template functions must be defined in the header file -> Instatiation must be included (cant be linked bc non existing)
		 * @tparam T 
		 * @param index 
		 * @return 
		 */
		template<typename T>
		T& at(size_t index) {
			return m_Buffer.at<T>(index);
		}

		operator bool() const { return m_Buffer; }
	private:
		Buffer m_Buffer;
	};


}
