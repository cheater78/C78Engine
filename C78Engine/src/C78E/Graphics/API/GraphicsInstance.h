#pragma once

namespace C78E {

	enum class API {
		None = 0,
		OpenGL, // WIP
		Vulkan, // WIP
		DirectX, // Not yet
		Metal, // Not yet
	};

	class GraphicsInstance {
	public:
		static void create(API api = API::OpenGL);

		template<typename T>
		requires std::derived_from<T, GraphicsInstance>
		static Ref<T> getAs() {
			Ref<T> instance = castRef<T>(s_GraphicsInstance);
			C78E_CORE_ASSERT(instance, "GraphicsInstance::getAs: Failed to cast GraphicsInstance to the requested type!");
			return instance;
		}

		static API api();

	protected:
		GraphicsInstance() = default;
	public:
		virtual ~GraphicsInstance() = default;

		virtual API getAPIType() const = 0;

	private:
		static Ref<GraphicsInstance> s_GraphicsInstance;
	};

}
