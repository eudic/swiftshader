// SwiftShader Software Renderer
//
// Copyright(c) 2005-2012 TransGaming Inc.
//
// All rights reserved. No part of this software may be copied, distributed, transmitted,
// transcribed, stored in a retrieval system, translated into any human or computer
// language by any means, or disclosed to third parties without the explicit written
// agreement of TransGaming Inc. Without such an agreement, no rights or licenses, express
// or implied, including but not limited to any patent rights, are granted to you.
//

// ResourceManager.h : Defines the ResourceManager class, which tracks objects
// shared by multiple GL contexts.

#ifndef LIBGLESV2_RESOURCEMANAGER_H_
#define LIBGLESV2_RESOURCEMANAGER_H_

#include "HandleAllocator.h"
#include "common/angleutils.h"

#define GL_APICALL
#include <GLES2/gl2.h>

#include <hash_map>

namespace gl
{
class Buffer;
class Shader;
class Program;
class Texture;
class Renderbuffer;

enum TextureType
{
    TEXTURE_2D,
    TEXTURE_CUBE,

    TEXTURE_TYPE_COUNT,
    TEXTURE_UNKNOWN
};

class ResourceManager
{
  public:
    ResourceManager();
    ~ResourceManager();

    void addRef();
    void release();

    GLuint createBuffer();
    GLuint createShader(GLenum type);
    GLuint createProgram();
    GLuint createTexture();
    GLuint createRenderbuffer();

    void deleteBuffer(GLuint buffer);
    void deleteShader(GLuint shader);
    void deleteProgram(GLuint program);
    void deleteTexture(GLuint texture);
    void deleteRenderbuffer(GLuint renderbuffer);

    Buffer *getBuffer(GLuint handle);
    Shader *getShader(GLuint handle);
    Program *getProgram(GLuint handle);
    Texture *getTexture(GLuint handle);
    Renderbuffer *getRenderbuffer(GLuint handle);
    
    void setRenderbuffer(GLuint handle, Renderbuffer *renderbuffer);

    void checkBufferAllocation(unsigned int buffer);
    void checkTextureAllocation(GLuint texture, TextureType type);
    void checkRenderbufferAllocation(GLuint renderbuffer);

  private:
    DISALLOW_COPY_AND_ASSIGN(ResourceManager);

    std::size_t mRefCount;

    typedef stdext::hash_map<GLuint, Buffer*> BufferMap;
    BufferMap mBufferMap;
    HandleAllocator mBufferHandleAllocator;

    typedef stdext::hash_map<GLuint, Shader*> ShaderMap;
    ShaderMap mShaderMap;

    typedef stdext::hash_map<GLuint, Program*> ProgramMap;
    ProgramMap mProgramMap;
    HandleAllocator mProgramShaderHandleAllocator;

    typedef stdext::hash_map<GLuint, Texture*> TextureMap;
    TextureMap mTextureMap;
    HandleAllocator mTextureHandleAllocator;

    typedef stdext::hash_map<GLuint, Renderbuffer*> RenderbufferMap;
    RenderbufferMap mRenderbufferMap;
    HandleAllocator mRenderbufferHandleAllocator;
};

}

#endif // LIBGLESV2_RESOURCEMANAGER_H_
