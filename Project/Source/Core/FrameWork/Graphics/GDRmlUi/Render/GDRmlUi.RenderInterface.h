#pragma once

#include <RmlUi/Core/RenderInterface.h>

#include <Godot.hpp>

namespace GDRmlUi
{
	class RenderInterface : public Rml::RenderInterface
	{
	public:
		RenderInterface();
		~RenderInterface();

		/// Called by RmlUi when it wants to render geometry that it does not wish to optimise.
		virtual void RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation);

		// Called by RmlUi when it wants to compile geometry it believes will be static for the forseeable future.
		//Rml::CompiledGeometryHandle CompileGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture) override;

		/// Called by RmlUi when it wants to render application-compiled geometry.
		//void RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry, const Rml::Vector2f& translation) override;
		/// Called by RmlUi when it wants to release application-compiled geometry.
		//void ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry) override;

		/// Called by RmlUi when it wants to enable or disable scissoring to clip content.
		virtual void EnableScissorRegion(bool enable);
		/// Called by RmlUi when it wants to change the scissor region.
		virtual void SetScissorRegion(int x, int y, int width, int height);

		/// Called by RmlUi when a texture is required by the library.
		virtual bool LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source);
		/// Called by RmlUi when a texture is required to be built from an internally-generated sequence of pixels.
		virtual bool GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions);
		/// Called by RmlUi when a loaded texture is no longer required.
		virtual void ReleaseTexture(Rml::TextureHandle texture_handle);

		/// Called by RmlUi when it wants to set the current transform matrix to a new matrix.
		//virtual void SetTransform(const Rml::Matrix4f* transform);
	private:
		godot::RID m_canvas_item;
		godot::RID m_mesh;
		godot::RID m_material;
	};
}
