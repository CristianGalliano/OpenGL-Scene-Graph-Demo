#pragma once
#include "Texture.h"
namespace SOF
{
	class Font
	{
		// bitmap font - loads font description + texture atlas produced by AngelCode bitmap font tool
		struct Glyph
		{
			int x, y, w, h, xOffset, yOffset, xAdvance;
		};
		Glyph glyphs[256];  // all the glyphs, up to 256
		Texture *atlas;
		int texSize;
		int ParseLine(std::string &line, Glyph &glyph);
		int ParseFile(const std::string &fileName);
	public:
		Font(const std::string &name );
		int GetX(char c) { return glyphs[c].x; }
		int GetY(char c) { return glyphs[c].y; }
		int GetW(char c) { return glyphs[c].w; }
		int GetH(char c) { return glyphs[c].h; }
		int GetXOffs(char c) { return glyphs[c].xOffset; }
		int GetYOffs(char c) { return glyphs[c].yOffset; }
		int GetXAdvance(char c) { return glyphs[c].xAdvance; }
		int GetTexSize() { return texSize; }
		Texture *GetTexture() { return atlas; }
	};
}