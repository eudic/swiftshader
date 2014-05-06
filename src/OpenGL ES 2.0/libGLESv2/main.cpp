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

// main.cpp: DLL entry point and management of thread-local data.

#include "main.h"

#include "Framebuffer.h"
#include "libEGL/Surface.h"
#include "common/debug.h"

static DWORD currentTLS = TLS_OUT_OF_INDEXES;

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
      case DLL_PROCESS_ATTACH:
        {
            currentTLS = TlsAlloc();

            if(currentTLS == TLS_OUT_OF_INDEXES)
            {
                return FALSE;
            }
        }
        // Fall throught to initialize index
      case DLL_THREAD_ATTACH:
        {
            gl::Current *current = (gl::Current*)LocalAlloc(LPTR, sizeof(gl::Current));

            if(current)
            {
                TlsSetValue(currentTLS, current);

                current->context = NULL;
                current->display = NULL;
            }
        }
        break;
      case DLL_THREAD_DETACH:
        {
            void *current = TlsGetValue(currentTLS);

            if(current)
            {
                LocalFree((HLOCAL)current);
            }
        }
        break;
      case DLL_PROCESS_DETACH:
        {
            void *current = TlsGetValue(currentTLS);

            if(current)
            {
                LocalFree((HLOCAL)current);
            }

            TlsFree(currentTLS);
        }
        break;
      default:
        break;
    }

    return TRUE;
}

namespace gl
{
void makeCurrent(Context *context, egl::Display *display, egl::Surface *surface)
{
    Current *current = (Current*)TlsGetValue(currentTLS);

    current->context = context;
    current->display = display;

    if(context && display && surface)
    {
        context->makeCurrent(display, surface);
    }
}

Context *getContext()
{
    Current *current = (Current*)TlsGetValue(currentTLS);

    return current->context;
}

egl::Display *getDisplay()
{
    Current *current = (Current*)TlsGetValue(currentTLS);

    return current->display;
}

Device *getDevice()
{
    egl::Display *display = getDisplay();

    return display->getDevice();
}
}

// Records an error code
void error(GLenum errorCode)
{
    gl::Context *context = glGetCurrentContext();

    if(context)
    {
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            context->recordInvalidEnum();
            TRACE("\t! Error generated: invalid enum\n");
            break;
        case GL_INVALID_VALUE:
            context->recordInvalidValue();
            TRACE("\t! Error generated: invalid value\n");
            break;
        case GL_INVALID_OPERATION:
            context->recordInvalidOperation();
            TRACE("\t! Error generated: invalid operation\n");
            break;
        case GL_OUT_OF_MEMORY:
            context->recordOutOfMemory();
            TRACE("\t! Error generated: out of memory\n");
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            context->recordInvalidFramebufferOperation();
            TRACE("\t! Error generated: invalid framebuffer operation\n");
            break;
        default: UNREACHABLE();
        }
    }
}
