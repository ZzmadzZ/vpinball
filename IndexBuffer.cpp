#include "stdafx.h"
#include "IndexBuffer.h"
#include "RenderDevice.h"

vector<IndexBuffer*> IndexBuffer::pendingSharedBuffers;

IndexBuffer::IndexBuffer(RenderDevice* rd, const unsigned int numIndices, const bool isDynamic, const IndexBuffer::Format format)
   : m_rd(rd)
   , m_indexCount(numIndices)
   , m_indexFormat(format)
   , m_sizePerIndex(format == FMT_INDEX16 ? 2 : 4)
   , m_isStatic(!isDynamic)
   , m_size(numIndices * (format == FMT_INDEX16 ? 2 : 4))
{
   if (m_isStatic)
   {
      if (pendingSharedBuffers.size() > 0 && pendingSharedBuffers[0]->m_indexFormat != m_indexFormat)
         CreatePendingSharedBuffer();
      pendingSharedBuffers.push_back(this);
   }
   else
   {
      #if defined(ENABLE_SDL) // OpenGL
      #ifndef __OPENGLES__
      if (GLAD_GL_VERSION_4_5)
      {
         glCreateBuffers(1, &m_ib);
         glNamedBufferStorage(m_ib, m_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
      }
      else if (GLAD_GL_VERSION_4_4)
      {
         glGenBuffers(1, &m_ib);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
         glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, m_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
      }
      else
      #endif
      {
         glGenBuffers(1, &m_ib);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, nullptr, GL_DYNAMIC_DRAW);
      }

      #else // DirectX 9
      // NB: We always specify WRITEONLY since MSDN states,
      // "Buffers created with D3DPOOL_DEFAULT that do not specify D3DUSAGE_WRITEONLY may suffer a severe performance penalty."
      // This means we cannot read from index buffers, but I don't think we need to.
      CHECKD3D(rd->GetCoreDevice()->CreateIndexBuffer(m_size, D3DUSAGE_WRITEONLY | (m_isStatic ? 0 : D3DUSAGE_DYNAMIC),
         m_indexFormat == FMT_INDEX16 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_ib, nullptr));

      #endif
   }
}

IndexBuffer::IndexBuffer(RenderDevice* rd, const unsigned int numIndices, const unsigned int* indices)
   : IndexBuffer(rd, numIndices, false, IndexBuffer::FMT_INDEX32)
{
   void* buf;
   lock(0, 0, &buf, WRITEONLY);
   memcpy(buf, indices, numIndices * sizeof(indices[0]));
   unlock();
}

IndexBuffer::IndexBuffer(RenderDevice* rd, const unsigned int numIndices, const WORD* indices)
   : IndexBuffer(rd, numIndices, false, IndexBuffer::FMT_INDEX16)
{
   void* buf;
   lock(0, 0, &buf, WRITEONLY);
   memcpy(buf, indices, numIndices * sizeof(indices[0]));
   unlock();
}

IndexBuffer::IndexBuffer(RenderDevice* rd, const vector<WORD>& indices)
   : IndexBuffer(rd, (unsigned int)indices.size(), indices.data())
{
}

IndexBuffer::IndexBuffer(RenderDevice* rd, const vector<unsigned int>& indices)
   : IndexBuffer(rd, (unsigned int)indices.size(), indices.data())
{
}

IndexBuffer::~IndexBuffer()
{
   if (!IsCreated())
      RemoveFromVectorSingle(pendingSharedBuffers, this);
   for (PendingUpload upload : m_pendingUploads)
      delete[] upload.data;

#if defined(ENABLE_SDL) // OpenGL
   if (m_sharedBufferRefCount != nullptr)
   {
      (*m_sharedBufferRefCount)--;
      if ((*m_sharedBufferRefCount) == 0)
      {
         delete m_sharedBufferRefCount;
         if (m_ib != 0)
            glDeleteBuffers(1, &m_ib);
      }
   }
   else if (m_ib != 0)
      glDeleteBuffers(1, &m_ib);
#else // DirectX 9
   SAFE_RELEASE(m_ib);
#endif
}

void IndexBuffer::lock(const unsigned int offsetToLock, const unsigned int sizeToLock, void **dataBuffer, const DWORD flags)
{
   assert(m_lock.data == nullptr); // Lock is not reentrant
   m_rd->m_curLockCalls++;
   m_lock.offset = offsetToLock;
   m_lock.size = sizeToLock == 0 ? m_size : sizeToLock;
   m_lock.data = new BYTE[m_lock.size];
   *dataBuffer = m_lock.data;
}

void IndexBuffer::unlock()
{
   PendingUpload upload = m_lock;
   m_pendingUploads.push_back(upload);
   m_lock.data = nullptr;
}

void IndexBuffer::ApplyOffset(VertexBuffer* vb)
{
   if (vb->m_offset == 0)
      return;
   for (PendingUpload upload : m_pendingUploads)
   {
      const unsigned int offset = vb->GetVertexOffset();
      const unsigned int count = upload.size / m_sizePerIndex;
      if (m_indexFormat == FMT_INDEX16)
      {
         U16* const __restrict indices = (U16*) upload.data;
         for (unsigned int i = 0; i < count; i++)
            indices[i] += offset;
      }
      else // FMT_INDEX32
      {
         assert(m_indexFormat == FMT_INDEX32);
         U32* const __restrict indices = (U32*) upload.data;
         for (unsigned int i = 0; i < count; i++)
            indices[i] += offset;
      }
   }
}

void IndexBuffer::CreatePendingSharedBuffer()
{
   UINT size = 0;
   for (IndexBuffer* buffer : pendingSharedBuffers)
      size += buffer->m_size;

   #if defined(ENABLE_SDL) // OpenGL
   UINT8* data = (UINT8*)malloc(size);

   #else // DirectX 9
   IDirect3DIndexBuffer9* ib = nullptr;
   CHECKD3D(pendingSharedBuffers[0]->m_rd->GetCoreDevice()->CreateIndexBuffer(
      size, D3DUSAGE_WRITEONLY, pendingSharedBuffers[0]->m_indexFormat == FMT_INDEX16 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_DEFAULT, &ib, nullptr));
   UINT8* data;
   CHECKD3D(ib->Lock(0, size, (void**)&data, 0));
   #endif

   UINT offset = 0;
   for (IndexBuffer* buffer : pendingSharedBuffers)
   {
      assert(buffer->m_indexFormat == pendingSharedBuffers[0]->m_indexFormat);
      buffer->m_offset = offset;
      for (PendingUpload upload : buffer->m_pendingUploads)
      {
         memcpy(data + offset + upload.offset, upload.data, upload.size);
         delete[] upload.data;
      }
      buffer->m_pendingUploads.clear();
      offset += buffer->m_size;
   }

   #if defined(ENABLE_SDL) // OpenGL
   GLuint ib = 0;
   #ifndef __OPENGLES__
   if (GLAD_GL_VERSION_4_5)
   {
      glCreateBuffers(1, &ib);
      glNamedBufferStorage(ib, size, data, 0);
   }
   else if (GLAD_GL_VERSION_4_4)
   {
      glGenBuffers(1, &ib);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
      glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, size, data, 0);
   }
   else
   #endif
   {
      glGenBuffers(1, &ib);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
   }
   free(data);
   int* refCount = new int();
   (*refCount) = (int)pendingSharedBuffers.size();
   for (IndexBuffer* buffer : pendingSharedBuffers)
   {
      buffer->m_ib = ib;
      buffer->m_sharedBufferRefCount = refCount;
   }

   #else // DirectX 9
   CHECKD3D(ib->Unlock());
   for (IndexBuffer* buffer : pendingSharedBuffers)
   {
      buffer->m_ib = ib;
      ib->AddRef();
   }
   ib->Release();

   #endif
   pendingSharedBuffers.clear();
}

void IndexBuffer::Upload()
{
   if (!IsCreated())
      CreatePendingSharedBuffer();

   if (m_pendingUploads.size() > 0)
   {
      for (PendingUpload upload : m_pendingUploads)
      {
         #if defined(ENABLE_SDL) // OpenGL
         #ifndef __OPENGLES__
         if (GLAD_GL_VERSION_4_5)
            glNamedBufferSubData(m_ib, m_offset + upload.offset, upload.size, upload.data);
         else
         #endif
         {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_offset + upload.offset, upload.size, upload.data);
         }

         #else // DirectX 9
         // It would be better to perform a single lock but in fact, I don't think there are situations where more than one update is pending
         UINT8* data;
         CHECKD3D(m_ib->Lock(m_offset + upload.offset, upload.size, (void**)&data, 0));
         memcpy(data, upload.data, upload.size);
         CHECKD3D(m_ib->Unlock());

         #endif
         delete[] upload.data;
      }
      m_pendingUploads.clear();
   }
}
