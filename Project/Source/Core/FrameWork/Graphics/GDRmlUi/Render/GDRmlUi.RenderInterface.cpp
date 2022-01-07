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

		auto m_canvasItem = vs->canvas_create();
		auto m_transform = godot::Transform2D(0, godot::Vector2(translation.x, translation.y));
		auto m_material = vs->material_create();

		godot::PoolIntArray indexArray;
		godot::PoolVector2Array pointArray;
		godot::PoolColorArray colorArray;
		godot::PoolVector2Array uvArray;

		indexArray.resize(num_indices);
		pointArray.resize(num_vertices);
		colorArray.resize(num_vertices);
		uvArray.resize(num_vertices);

		int* _indices = indexArray.write().ptr();
		_indices = indices;
		godot::Vector2* _point = pointArray.write().ptr();
		godot::Color* _color = colorArray.write().ptr();
		godot::Vector2* _uv = uvArray.write().ptr();
		for (int v = 0; v < num_vertices; v++)
		{
			_point[v].x = vertices[v].position.x;
			_point[v].y = vertices[v].position.y;

			_color[v].r = vertices[v].colour.red;
			_color[v].g = vertices[v].colour.green;
			_color[v].b = vertices[v].colour.blue;
			_color[v].a = vertices[v].colour.alpha;

			_uv[v].x = vertices[v].tex_coord.x;
			_uv[v].y = vertices[v].tex_coord.y;
		}

		vs->canvas_item_add_triangle_array(m_canvasItem, indexArray, pointArray, colorArray, uvArray);
		vs->canvas_item_set_parent(m_canvasItem, NULL);
		vs->canvas_item_set_transform(m_canvasItem, m_transform);
		vs->canvas_item_set_material(m_canvasItem, m_material);
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

	void RenderInterface::SetTransform(const Rml::Matrix4f* transform)
	{

	}
}
