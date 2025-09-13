#pragma once
#include <C78E/Core/Core.h>
#include <C78E/Core/Log/SmartLog.h>

namespace C78E {

	template<typename T, typename S>
	constexpr Ref<T> castRef(Ref<S> srcRef) {
		C78E_CORE_ASSERT(srcRef, "castRef: srcRef given was nullptr!");
		if constexpr (std::is_base_of<T, S>()) { // S specializes T -> just static cast the to the Base
			return std::static_pointer_cast<T>(srcRef);
		}
		C78E_CORE_ASSERT(C78E_EXPANDALL_MACRO((std::is_base_of<S, T>())), "castRef: S type and T type are not related!");
		Ref<T> target = std::dynamic_pointer_cast<T>(srcRef);
		C78E_CORE_ASSERT(target, "castRef: couldn't cast to targeted type!");
		return target;
	}

	template<typename T, typename S>
	constexpr Ref<T> castCreateRef(Ref<S> srcRef) {
		if (!srcRef) {
			return createRef<T>();
		} else {
			return castRef<T>(srcRef);
		}
	}

}