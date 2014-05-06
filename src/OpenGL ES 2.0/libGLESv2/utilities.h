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

// utilities.h: Conversion functions and other utility routines.

#ifndef LIBGLESV2_UTILITIES_H
#define LIBGLESV2_UTILITIES_H

#include "Device.hpp"
#include "Image.hpp"
#include "Texture.h"

#define GL_APICALL
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <string>

namespace gl
{
	struct Color;

	enum CubeFace
	{
		CUBEMAP_FACE_POSITIVE_X = 0,
		CUBEMAP_FACE_NEGATIVE_X = 1,
		CUBEMAP_FACE_POSITIVE_Y = 2,
		CUBEMAP_FACE_NEGATIVE_Y = 3,
		CUBEMAP_FACE_POSITIVE_Z = 4,
		CUBEMAP_FACE_NEGATIVE_Z = 5
	};

	int UniformComponentCount(GLenum type);
	GLenum UniformComponentType(GLenum type);
	size_t UniformTypeSize(GLenum type);
	int VariableRowCount(GLenum type);
	int VariableColumnCount(GLenum type);

	int AllocateFirstFreeBits(unsigned int *bits, unsigned int allocationSize, unsigned int bitsSize);

	int ComputePixelSize(GLenum format, GLenum type);
	GLsizei ComputePitch(GLsizei width, GLenum format, GLenum type, GLint alignment);
	GLsizei ComputeCompressedPitch(GLsizei width, GLenum format);
	GLsizei ComputeCompressedSize(GLsizei width, GLsizei height, GLenum format);
	bool IsCompressed(GLenum format);
	bool IsCubemapTextureTarget(GLenum target);
	CubeFace ConvertCubeFace(GLenum cubeTarget);
	bool IsTextureTarget(GLenum target);
	bool CheckTextureFormatType(GLenum format, GLenum type);

	bool IsColorRenderable(GLenum internalformat);
	bool IsDepthRenderable(GLenum internalformat);
	bool IsStencilRenderable(GLenum internalformat);
}

namespace es2sw
{
	sw::Context::DepthCompareMode ConvertDepthComparison(GLenum comparison);
	sw::Context::StencilCompareMode ConvertStencilComparison(GLenum comparison);
	sw::Color<float> ConvertColor(gl::Color color);
	sw::Context::BlendFactor ConvertBlendFunc(GLenum blend);
	sw::Context::BlendOperation ConvertBlendOp(GLenum blendOp);
	sw::Context::StencilOperation ConvertStencilOp(GLenum stencilOp);
	sw::AddressingMode ConvertTextureWrap(GLenum wrap);
	sw::Context::CullMode ConvertCullMode(GLenum cullFace, GLenum frontFace);
	unsigned int ConvertColorMask(bool red, bool green, bool blue, bool alpha);
	sw::FilterType ConvertMagFilter(GLenum magFilter);
	void ConvertMinFilter(GLenum texFilter, sw::FilterType *minFilter, sw::MipmapType *mipFilter);
	bool ConvertPrimitiveType(GLenum primitiveType, GLsizei elementCount,  gl::PrimitiveType &swPrimitiveType, int &primitiveCount);
	sw::Format ConvertRenderbufferFormat(GLenum format);
}

namespace sw2es
{
	GLuint GetAlphaSize(sw::Format colorFormat);
	GLuint GetRedSize(sw::Format colorFormat);
	GLuint GetGreenSize(sw::Format colorFormat);
	GLuint GetBlueSize(sw::Format colorFormat);
	GLuint GetDepthSize(sw::Format depthFormat);
	GLuint GetStencilSize(sw::Format stencilFormat);

	GLenum ConvertBackBufferFormat(sw::Format format);
	GLenum ConvertDepthStencilFormat(sw::Format format);
}

#endif  // LIBGLESV2_UTILITIES_H
