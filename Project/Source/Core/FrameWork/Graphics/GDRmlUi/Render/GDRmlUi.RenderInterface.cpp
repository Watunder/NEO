#include <RmlUi/Core.h>

#include "GDRmlUi.FileInterface.h"
#include "GDRmlUi.RenderInterface.h"

namespace GDRmlUi
{
	struct Geometry
	{
		godot::PoolIntArray indices;
		godot::PoolVector2Array position;
		godot::PoolColorArray color;
		godot::PoolVector2Array uv;

		Geometry(int num_vertices)
		{
			indices.resize(num_vertices * 6);
			position.resize(num_vertices);
			color.resize(num_vertices);
			uv.resize(num_vertices);
		}
	};

	RenderInterface::RenderInterface()
	{
		m_render = new GDBinder::RendererImplemented();
	}

	RenderInterface::~RenderInterface()
	{

	}

	void RenderInterface::RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation)
	{
		auto vs = godot::VisualServer::get_singleton();


	}

	Rml::CompiledGeometryHandle RenderInterface::CompileGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture)
	{


		return (Rml::CompiledGeometryHandle) nullptr;
	}

	void RenderInterface::EnableScissorRegion(bool enable)
	{
		// the scissor is not contrlled here
	}

	void RenderInterface::SetScissorRegion(int x, int y, int width, int height)
	{
		// the scissor is not contrlled here
	}

	bool RenderInterface::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source)
	{


		return true;
	}

	bool RenderInterface::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions)
	{


		return true;
	}

	void RenderInterface::ReleaseTexture(Rml::TextureHandle texture_handle)
	{

	}
}
