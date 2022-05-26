#pragma once
#include "utils.h"
#include "Text.h"
#include "Date.h"

class ToggleDate {
public:
	int mX, mY, mW, mH;

	ToggleDate(Date* date, Text* text, SDL_Renderer* renderer) {
		mX = 13, mY = 36;
		mDate = date;
		mRenderer = renderer;
		mTextDate = text;
	};
	~ToggleDate();
	void runSetup() {
		time(&mDate->rawtime);
		localtime_s(&mDate->timeinfo, &mDate->rawtime);

		updateDate();
	}
	void updateDate() {
		DayMonthYear dmy = isHijri ? getHDate() : getGDate();
		sprintf_s(
			mDate->buffer,
			"%s, %d %s %d",
			toDay(mDate->timeinfo.tm_wday - 1),
			dmy.day,
			isHijri ? toHMonth(dmy.month) : toGMonth(dmy.month),
			dmy.year
		);

		mTextDate->setText(mDate->buffer);
		int w, h;
		mTextDate->getSize(w, h);

		mW = w;
		mH = h;
	}
	DayMonthYear getGDate() {
		DayMonthYear dmy = { mDate->timeinfo.tm_year + 1900, mDate->timeinfo.tm_mon, mDate->timeinfo.tm_mday };
		return dmy;
	}
	DayMonthYear getHDate() {
		DayMonthYear dmy = toHijri(mDate->timeinfo.tm_year + 1900, mDate->timeinfo.tm_mon, mDate->timeinfo.tm_mday - 1);
		return dmy;
	}
	void handleEvents(SDL_Event* e) {
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
			else if (x > mX + mW)
			{
				inside = false;
			}
			// Mouse above the button
			else if (y < mY)
			{
				inside = false;
			}
			// Mouse below the button
			else if (y > mY + mH)
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
					break;

				case SDL_MOUSEBUTTONUP:
					mouseDown = false;
					toggleMode();
					updateDate();
					break;
				}
			}
		}
	}
	void toggleMode() {
		isHijri = !isHijri;
	};
	void draw() {
		if (mouseInside) {
			mTextDate->draw(mX, mY, 145, 145, 145, 255);
		}
		else {
			mTextDate->draw(mX, mY, 103, 103, 103, 255);
		}
	};
private:
	bool mouseInside, mouseDown;
	bool isHijri = true;
	Date* mDate;
	Text* mTextDate;
	SDL_Renderer* mRenderer;
	DayMonthYear mGeogrianDate;
	DayMonthYear mHijriDate;
};

ToggleDate::~ToggleDate() {}