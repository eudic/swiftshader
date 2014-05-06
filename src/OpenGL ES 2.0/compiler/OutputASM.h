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

#ifndef COMPILER_OUTPUTASM_H_
#define COMPILER_OUTPUTASM_H_

#include "intermediate.h"
#include "ParseHelper.h"
#include "Shader/PixelShader.hpp"
#include "Shader/VertexShader.hpp"

#define GL_APICALL
#include <GLES2/gl2.h>

#include <list>
#include <set>
#include <map>

namespace gl
{
	class Shader;
}

namespace sh
{
	struct Uniform
	{
		Uniform(GLenum type, const std::string &name, int arraySize, int registerIndex);

		GLenum type;
		std::string name;
		int arraySize;
	
		int registerIndex;
	};

	typedef std::vector<Uniform> ActiveUniforms;

	struct Attribute
	{
		Attribute();
		Attribute(GLenum type, const std::string &name, int arraySize, int registerIndex);

		GLenum type;
		std::string name;
		int arraySize;
	
		int registerIndex;
	};

	struct Function
	{
		Function(int label, const char *name, TIntermSequence *arg, TIntermTyped *ret) : label(label), name(name), arg(arg), ret(ret)
		{
		}

		Function(int label, const TString &name, TIntermSequence *arg, TIntermTyped *ret) : label(label), name(name), arg(arg), ret(ret)
		{
		}

		const int label;
		const TString name;
		TIntermSequence *const arg;
		TIntermTyped *const ret;
	};
	
	typedef sw::Shader::Instruction Instruction;
	typedef std::vector<Attribute> ActiveAttributes;

	class Temporary;

	class OutputASM : public TIntermTraverser
	{
	public:
		explicit OutputASM(TParseContext &context, gl::Shader *shaderObject);
		~OutputASM();

		void output();

		void freeTemporary(Temporary *temporary);

	protected:
		enum Scope
		{
			GLOBAL,
			FUNCTION
		};

		void emitShader(Scope scope);

		// Visit AST nodes and output their code to the body stream
		virtual bool visitBinary(Visit visit, TIntermBinary*);
		virtual bool visitUnary(Visit visit, TIntermUnary*);
		virtual bool visitSelection(Visit visit, TIntermSelection*);
		virtual bool visitAggregate(Visit visit, TIntermAggregate*);
		virtual bool visitLoop(Visit visit, TIntermLoop*);
		virtual bool visitBranch(Visit visit, TIntermBranch*);

		Instruction *emit(sw::Shader::Opcode op, TIntermTyped *dst = 0, TIntermNode *src0 = 0, TIntermNode *src1 = 0, TIntermNode *src2 = 0);
		void emitAssign(sw::Shader::Opcode op, TIntermTyped *result, TIntermTyped *lhs, TIntermTyped *src0, TIntermTyped *src1 = 0);
		void emitCmp(sw::Shader::Control cmpOp, TIntermTyped *dst, TIntermNode *left, TIntermNode *right, int index = 0);
		void argument(sw::Shader::SourceParameter &parameter, TIntermNode *argument, int index = 0);
		void copy(TIntermTyped *dst, TIntermNode *src, int offset = 0);
		void assignLvalue(TIntermTyped *dst, TIntermNode *src);
		int lvalue(sw::Shader::DestinationParameter &dst, Temporary &address, TIntermTyped *node);
		sw::Shader::ParameterType registerType(TIntermTyped *operand);
		int registerIndex(TIntermTyped *operand);
		int writeMask(TIntermTyped *destination, int index = 0);
		bool trivial(TIntermTyped *expression, int budget);   // Fast to compute and no side effects
		int cost(TIntermNode *expression, int budget);
		const Function &findFunction(const TString &name);

		int temporaryRegister(TIntermTyped *temporary);
		int varyingRegister(TIntermTyped *varying);
		int uniformRegister(TIntermTyped *uniform);
		int attributeRegister(TIntermTyped *attribute);
		int samplerRegister(TIntermTyped *sampler);

		typedef std::vector<TIntermTyped*> VariableArray;

		int lookup(VariableArray &list, TIntermTyped *variable);
		int allocate(VariableArray &list, TIntermTyped *variable);
		void free(VariableArray &list, TIntermTyped *variable);

		void declareUniform(const TType &type, const TString &name, int index);
		GLenum glVariableType(const TType &type);

		static int dim(TIntermNode *v);
		static int dim2(TIntermNode *m);

		gl::Shader *const shaderObject;
		sw::Shader *shader;
		sw::PixelShader *pixelShader;
		sw::VertexShader *vertexShader;

		VariableArray temporaries;
		VariableArray uniforms;
		VariableArray varyings;
		VariableArray attributes;
		VariableArray samplers;

		Scope emitScope;
		Scope currentScope;

		int currentFunction;
		std::vector<Function> functionArray;

		TParseContext &mContext;
	};
}

#endif   // COMPILER_OUTPUTASM_H_
