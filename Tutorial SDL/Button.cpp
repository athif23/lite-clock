#include "Button.h"

Button::Button(Text* text, SDL_Renderer* renderer, int width, int height)
{
	mText = text;
	mWidth = width;
	mHeight = height;
	mRenderer = renderer;

	int w, h;
	mText->getSize(w, h);

	mTextW = w;
	mTextH = h;
}

Button::~Button()
{
}

void Button::setPosition(int x, int y)
{
	mX = x;
	mY = y;
}

bool Button::handleEvents(SDL_Event* e)
{
	// If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		// Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		// Check if mouse is in button
		bool inside = true;

		// Mouse is left of the button
		if (x < mX)
		{
			inside = false;
		}
		// Mouse is right of the button
		else if (x > mX + mWidth)
		{
			inside = false;
		}
		// Mouse above the button
		else if (y < mY)
		{
			inside = false;
		}
		// Mouse below the button
		else if (y > mY + mHeight)
		{
			inside = false;
		}
		// Mouse is outside button
		if (!inside)
		{
			mouseInside = false;
			// mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		// Mouse is inside button
		else
		{
			// Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				mouseInside = true;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mouseDown = true;
				return true;
				break;

			case SDL_MOUSEBUTTONUP:
				mouseDown = false;
				break;
			}
		}
	}
	return false;
}
void Button::draw()
{
	SDL_Rect rect = { mX, mY, mWidth, mHeight };
	if (!mouseInside && !mouseDown)
	{
		SDL_SetRenderDrawColor(mRenderer, 14, 15, 17, 255);
		SDL_RenderFillRect(mRenderer, &rect);
	}
	else if (mouseInside && mouseDown)
	{
		rect.x += 1;
		rect.y += 1;
		rect.w -= 1;
		rect.h -= 1;
		SDL_SetRenderDrawColor(mRenderer, 7, 8, 9, 150);
		SDL_RenderFillRect(mRenderer, &rect);
		rect.x -= 1;
		rect.y -= 1;
		rect.w += 1;
		rect.h += 1;
		SDL_SetRenderDrawColor(mRenderer, 95, 94, 99, 255);
		SDL_RenderDrawRect(mRenderer, &rect);
	}
	else
	{
		rect.x += 1;
		rect.y += 1;
		rect.w -= 1;
		rect.h -= 1;
		SDL_SetRenderDrawColor(mRenderer, 14, 15, 17, 255);
		SDL_RenderFillRect(mRenderer, &rect);
		rect.x -= 1;
		rect.y -= 1;
		rect.w += 1;
		rect.h += 1;
		SDL_SetRenderDrawColor(mRenderer, 95, 94, 99, 255);
		SDL_RenderDrawRect(mRenderer, &rect);
	}
	mText->draw(mX + (mWidth - mTextW)/2, mY + (mHeight - mTextH) / 2, 236, 77, 77, 255);
}