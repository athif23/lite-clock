#include "Text.h"

Text::Text(const char* text, const int size) {
	mFC.faceSize = size;
	mFC.tabWidthInSpaces = 12;

	mText = text;

	mLoadFont();
}

Text::~Text() {

}

sdl_stb_prerendered_text* Text::prerenderText() {
	mPrt.mRenderer = mRenderer;
	mFC.renderTextToObject(&mPrt, mText);
	isPrerender = true;

	return &mPrt;
}

void Text::setText(sttfont_formatted_text const& str) 
{
	mFormattedText = str;
	isFormattedText = true;
	mFC.renderTextToObject(&mPrt, str);
}

void Text::setText(const char* newText) {
	mText = newText;
	isFormattedText = false;
	mFC.renderTextToObject(&mPrt, mText);
}

void Text::bindRenderer(SDL_Renderer* renderer) {
	mFC.bindRenderer(renderer);
	mRenderer = renderer;
}

void Text::mLoadFont() {
	sttfont_memory inter;
	sttfont_memory interBold;
	readFileRaw_toMemory("fonts/Inter-Regular.ttf", inter);
	readFileRaw_toMemory("fonts/Inter-Bold.ttf", interBold);

	mFC.loadFontManaged(inter);
	mFC.addFormatFontManaged(sttfont_format::FORMAT_BOLD, interBold);
}

void Text::draw(int const x, int const y, const char* text) {
	mText = text;
	mFC.drawText(x, y, text);
}

void Text::draw(int const x, int const y) {
	if (isPrerender) {
		mPrt.draw(x, y);
	}
	else {
		mFC.drawText(x, y, mText);
	}
}

void Text::draw(int const x, int const y, uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a) {
	mPrt.drawWithColorMod(x, y, r, g, b, a);
}

void Text::getSize(int& w, int& h) {
	if (isPrerender) {
		if (isFormattedText) {
			mFC.getTextSize(w, h, mFormattedText);
		}
		else {
			mFC.getTextSize(w, h, mText);
		}
	}
	else {
		mFC.getTextSize(w, h, mText);
	}
}