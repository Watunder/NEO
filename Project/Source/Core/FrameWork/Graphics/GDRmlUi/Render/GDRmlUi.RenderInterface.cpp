#include <RmlUi/Core.h>

#include <ArrayMesh.hpp>
#include <Transform2D.hpp>
#include <Image.hpp>
#include <Texture.hpp>
#include <ImageTexture.hpp>
#include <VisualServer.hpp>
#include <ResourceLoader.hpp>

#include "GDRmlUi.RenderInterface.h"

namespace GDRmlUi
{
	struct Triangle2D
	{
		godot::PoolIntArray indices;
		godot::PoolVector2Array position;
		godot::PoolColorArray color;
		godot::PoolVector2Array uv;

		Triangle2D(int num_vertices, int num_indices)
		{
			indices.resize(num_indices);
			position.resize(num_vertices);
			color.resize(num_vertices);
			uv.resize(num_vertices);
		}
	};

	RenderInterface::RenderInterface()
	{
		auto vs = godot::VisualServer::get_singleton();

		m_canvas_item = vs->canvas_item_create();
		m_mesh = vs->mesh_create();
		m_material = vs->material_create();
	}

	RenderInterface::~RenderInterface()
	{
		auto vs = godot::VisualServer::get_singleton();

		vs->free_rid(m_canvas_item);
		vs->free_rid(m_mesh);
		vs->free_rid(m_material);
	}

	void RenderInterface::RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation)
	{
		auto vs = godot::VisualServer::get_singleton();

		auto triangle = new Triangle2D(num_vertices, num_indices);

		int* _indices = triangle->indices.write().ptr();
		memcpy(_indices, indices, num_indices);

		godot::Vector2* _position = triangle->position.write().ptr();
		godot::Color* _color = triangle->color.write().ptr();
		godot::Vector2* _uv = triangle->uv.write().ptr();
		for (int v = 0; v < num_vertices; v++)
		{
			_position[v].x = vertices[v].position.x;
			_position[v].y = vertices[v].position.y;

			_color[v].r = vertices[v].colour.red;
			_color[v].g = vertices[v].colour.green;
			_color[v].b = vertices[v].colour.blue;
			_color[v].a = vertices[v].colour.alpha;

			_uv[v].x = vertices[v].tex_coord.x;
			_uv[v].y = vertices[v].tex_coord.y;
		}

		godot::Array mesh_array;
		mesh_array.resize(godot::ArrayMesh::ARRAY_MAX);
		mesh_array[godot::ArrayMesh::ARRAY_INDEX] = triangle->indices;
		mesh_array[godot::ArrayMesh::ARRAY_VERTEX] = triangle->position;
		mesh_array[godot::ArrayMesh::ARRAY_COLOR] = triangle->color;
		mesh_array[godot::ArrayMesh::ARRAY_TEX_UV] = triangle->uv;

		auto transform = godot::Transform2D(0, godot::Vector2(translation.x, translation.y));

		vs->canvas_item_clear(m_canvas_item);
		vs->canvas_item_set_parent(m_canvas_item, godot::RID());

		vs->mesh_add_surface_from_arrays(m_mesh, godot::Mesh::PRIMITIVE_TRIANGLES, mesh_array, godot::Array(), godot::Mesh::ARRAY_FLAG_USE_2D_VERTICES);
		vs->canvas_item_add_mesh(m_canvas_item, m_mesh, transform, godot::Color(1, 1, 1, 1), (godot::RID&)texture);

		vs->canvas_item_set_parent(m_canvas_item, godot::RID());
		vs->canvas_item_set_material(m_canvas_item, m_material);
	}

	void RenderInterface::EnableScissorRegion(bool enable)
	{
		auto vs = godot::VisualServer::get_singleton();

		vs->canvas_item_set_clip(m_canvas_item, enable);
	}

	void RenderInterface::SetScissorRegion(int x, int y, int width, int height)
	{
		auto vs = godot::VisualServer::get_singleton();

		vs->canvas_item_set_custom_rect(m_canvas_item, true, godot::Rect2(x, y, width, height));
	}

	bool RenderInterface::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source)
	{
		auto vs = godot::VisualServer::get_singleton();

		auto loader = godot::ResourceLoader::get_singleton();
		auto resource = loader->load(source.c_str());
		if (!resource.is_valid())
		{
			return false;
		}
		auto _image = (godot::Image*)resource.ptr();
		
		godot::RID m_texture = vs->texture_create_from_image(_image);

		texture_handle = (Rml::TextureHandle&)m_texture;

		return true;
	}

	bool RenderInterface::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions)
	{
		auto vs = godot::VisualServer::get_singleton();

		godot::Ref<godot::Image> _image;
		_image.instance();
		godot::PoolByteArray _image_data;
		_image_data.resize(source_dimensions.x * source_dimensions.y * 4);

		uint8_t* dptr = _image_data.write().ptr();
		uint8_t* sptr = (uint8_t*)source;
		std::memcpy(dptr, sptr, (size_t)source_dimensions.x * source_dimensions.y * 4);

		_image->create_from_data(source_dimensions.x, source_dimensions.y, false, godot::Image::FORMAT_RGBA8, _image_data);

		godot::RID m_texture = vs->texture_create_from_image(_image);

		texture_handle = (Rml::TextureHandle&)m_texture;

		return true;
	}

	void RenderInterface::ReleaseTexture(Rml::TextureHandle texture_handle)
	{
		auto vs = godot::VisualServer::get_singleton();

		vs->free_rid((godot::RID&)texture_handle);
	}
}
