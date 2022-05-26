#pragma once
#include "utils.h"
#include "Text.h"

class Button
{
public:
	Button(Text* text, SDL_Renderer* renderer, int width, int height);
	~Button();
	void setPosition(int x, int y);
	bool handleEvents(SDL_Event* e);
	void draw();
	int mX;
	int mY;
	int mWidth;
	int mHeight;

private:
	Text* mText;
	int mTextW;
	int mTextH;
	sdl_stb_font_cache* mFC;
	bool mouseInside;
	bool mouseDown;
	sdl_stb_prerendered_text mPrt;
	SDL_Renderer* mRenderer;
};
