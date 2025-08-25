#pragma once
#include <C78E/Core/Core.h>
#include <C78E/Core/Log/SmartLog.h>

namespace C78E {

	template<typename T, typename S>
	constexpr Ref<T> castRef(Ref<S> srcRef) {
		if (!srcRef) {
			return nullptr;
		}
		if (std::is_base_of<T, S>()) { // S specializes T -> just static cast the to the Base
			return std::static_pointer_cast<T>(srcRef);
		}
		C78E_CORE_STATIC_ASSERT(C78E_EXPANDALL_MACRO((std::is_base_of<S, T>())), "castRef: S type and T type are not related!");
		return std::dynamic_pointer_cast<T>(srcRef);
	}

	template<typename T, typename S>
	constexpr Ref<T> castCreateRef(Ref<S> srcRef) {
		if (!srcRef) {
			return createRef<T>();
		} else {
			Ref<T> target = castRef<T>(srcRef);
			if (!target) { // srcRef wasn't alr of type T
				target = createRef<T>(); // create New T -> requires public default constructor
				*std::static_pointer_cast<S>(target) = *srcRef; // write Base value
			}
			return target;
		}
	}

}
