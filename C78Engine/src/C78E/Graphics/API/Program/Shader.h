#pragma once
#include <C78E/Asset/Asset.h>
#include <C78E/Graphics/API/Program/ShaderStage.h>

namespace C78E {

	class GraphicsContext;

	class Shader : public Asset {
	public:
		static Ref<Shader> create(GraphicsContext& ctx, ShaderStage stage, Ref<ScopedBuffer> binCode);
	public:
		Shader(GraphicsContext& ctx, ShaderStage stage);
		virtual ~Shader() = default;

		virtual ShaderStage getStage() const { return m_Stage; }

		virtual Asset::Type getType() const override { return Asset::Type::Shader; }
		static Type getClassType() { return Type::Shader; };
	protected:
		ShaderStage m_Stage = ShaderStage::None;
		GraphicsContext& m_GraphicsContext;
	};

}
