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

#ifndef sw_Blitter_hpp
#define sw_Blitter_hpp

#include "Surface.hpp"
#include "LRUCache.hpp"
#include "Reactor/Nucleus.hpp"

namespace sw
{
	class Routine;

	class Blitter
	{
	public:
		Blitter();

		virtual ~Blitter();

		void blit(Surface *source, const Rect &sRect, Surface *dest, const Rect &dRect, bool filter);

	private:
		bool read(Float4 &color, Pointer<Byte> element, Format format);
		bool blitReactor(Surface *source, const Rect &sRect, Surface *dest, const Rect &dRect, bool filter);

		struct BlitState
		{
			bool operator==(const BlitState &state) const
			{
				return memcmp(this, &state, sizeof(BlitState)) == 0;
			}

			Format sourceFormat;
			Format destFormat;
			bool filter;
		};

		struct BlitData
		{
			void *source;
			void *dest;
			int sPitchB;
			int dPitchB;

			float x0;
			float y0;
			float w;
			float h;

			int y0d;
			int y1d;
			int x0d;
			int x1d;

			int sWidth;
			int sHeight;
		};

		LRUCache<BlitState, Routine> *blitCache;
	};
}

#endif   // sw_Blitter_hpp
