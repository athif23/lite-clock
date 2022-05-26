#pragma once
#include "utils.h"

class Text {
public:
	Text(const char* text, const int size = 25);
	~Text();
	void setText(sttfont_formatted_text const& str);
	void setText(const char* newText);
	sdl_stb_prerendered_text* prerenderText();
	void draw(int const x, int const y, const char* text);
	void draw(int const x, int const y);
	void draw(int const x, int const y, uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a = 255);
	void bindRenderer(SDL_Renderer* renderer);
	void getSize(int& w, int& y);

private:
	const char* mText;
	sttfont_formatted_text mFormattedText;
	sdl_stb_font_cache mFC;
	sdl_stb_prerendered_text mPrt;
	SDL_Renderer* mRenderer;
	void mLoadFont();
	bool isPrerender = false;
	bool isFormattedText = false;
};