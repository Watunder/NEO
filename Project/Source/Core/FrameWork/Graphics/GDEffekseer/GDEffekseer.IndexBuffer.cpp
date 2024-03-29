﻿
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#include "GDEffekseer.IndexBuffer.h"

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace GDEffekseer
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBuffer::IndexBuffer(RendererImplemented* renderer, int maxCount, bool isDynamic)
	: IndexBufferBase(maxCount, isDynamic)
	, m_buffer(sizeof(uint16_t) * maxCount)
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Effekseer::RefPtr<IndexBuffer> IndexBuffer::Create(RendererImplemented* renderer, int maxCount, bool isDynamic)
{
	return IndexBufferRef(new IndexBuffer(renderer, maxCount, isDynamic));
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void IndexBuffer::Lock()
{
	assert(!m_isLock);

	m_isLock = true;
	m_resource = m_buffer.data();
	m_indexCount = 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void IndexBuffer::Unlock()
{
	assert(m_isLock);

	m_resource = NULL;
	m_isLock = false;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace GDEffekseer
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
