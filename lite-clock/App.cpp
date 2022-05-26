#ifdef _WIN32
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define SDL_STB_FONT_IMPL
#include "utils.h"
#include "hijriConverter.h"

#include <iostream>

#include "Text.h"
#include "Button.h"
#include "ToggleDate.h"

#include "prayertimes.h"

Clock gClock;
Date gDate;

Uint32 callback_run_clock(Uint32 interval, void* param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	time(&gClock.rawtime);
	localtime_s(&gClock.timeinfo, &gClock.rawtime);
	strftime(gClock.buffer, 80, "%T", &gClock.timeinfo);

	/* In this example, our callback pushes a function
	into the queue, and causes our callback to be called again at the
	same interval: */

	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return (interval);
}

Uint32 callback_run_date(Uint32 interval, void* param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	time(&gDate.rawtime);
	localtime_s(&gDate.timeinfo, &gDate.rawtime);

	/* In this example, our callback pushes a function
	into the queue, and causes our callback to be called again at the
	same interval: */

	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = (char*)"Date";
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return (interval);
}

struct HitTestInfo {
	Button button;
	ToggleDate toggleDate;
};

SDL_HitTestResult hit_test_callback(SDL_Window* win, const SDL_Point* area, void* data) {
	HitTestInfo* htData = (HitTestInfo*)data;
	int bx = htData->button.mX;
	int by = htData->button.mY;
	int bw = htData->button.mWidth;
	int bh = htData->button.mHeight;
	int tx = htData->toggleDate.mX;
	int ty = htData->toggleDate.mY;
	int tw = htData->toggleDate.mW;
	int th = htData->toggleDate.mH;
	if ((area->x < bx || area->x > bx + bw || area->y < by || area->y > by + bh) && (area->x < tx || area->x > tx + tw || area->y < ty || area->y > ty + th)) {
		return SDL_HITTEST_DRAGGABLE;
	}
	return SDL_HITTEST_NORMAL;
}

int main(int argc, char** argv)
{
	bool quit = false;

	sdl_stb_font_cache fcButton;
	fcButton.faceSize = 15;
	fcButton.tabWidthInSpaces = 12;

	Text tClock{ "00:00:00", 27 };
	Text tReminder{ "1 hour 5 minutes before Dzuhur 12:08", 14 };
	Text tCloseButton{ "close", 15 };
	Text tDate{ "Sunday, 23 September 2001", 15 };

	//HijriDate hijriDate = toHijri(2022, 5, 25);
	//std::cout << hijriDate.year << " " << hijriDate.month << " " << hijriDate.day << std::endl;

	// Setup the SDL window & renderer
	int windowWidth = 395;
	int windowHeight = 60;

	SDL_Init(0);
	SDL_Window* mWindow = SDL_CreateWindow(
		"Example Test",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR);

	SDL_Renderer* mSdlRenderer = SDL_CreateRenderer(mWindow, SDL_RENDERER_SOFTWARE, 0);
	// Must bind a renderer before generating any glyphs
	fcButton.bindRenderer(mSdlRenderer); 
	tClock.bindRenderer(mSdlRenderer);
	tReminder.bindRenderer(mSdlRenderer);
	tCloseButton.bindRenderer(mSdlRenderer);
	tDate.bindRenderer(mSdlRenderer);

	ToggleDate toggleDate{ &gDate, &tDate, mSdlRenderer };

	// Prerendered texts
	//sdl_stb_prerendered_text* prtDate = tDate.prerenderText();
	sdl_stb_prerendered_text* prtReminder = tReminder.prerenderText();
	sdl_stb_prerendered_text* prtClose = tCloseButton.prerenderText();
	sdl_stb_prerendered_text* prtDate = tDate.prerenderText();

	Uint32 delaySecond = 1000; /* Every 1 second */
	SDL_TimerID clock_timer_id = SDL_AddTimer(delaySecond, callback_run_clock, NULL);

	Uint32 delayDate = 60000; /* Every 1 minutes */
	SDL_TimerID date_timer_id = SDL_AddTimer(delayDate, callback_run_date, NULL);

	// Run to init date
	toggleDate.runSetup();

	// Close button
	Button btnClose{ &tCloseButton, mSdlRenderer, 40, 20 };
	btnClose.setPosition(windowWidth - 50, (windowHeight - 26));
	HitTestInfo htData = { btnClose, toggleDate };
	SDL_SetWindowHitTest(mWindow, hit_test_callback, &htData);

	sttfont_formatted_text ttAdzan;
	ttAdzan << sttfont_format::white << "1 hour 5 minutes " 
			<< sttfont_format::color(123, 172, 84) 
			<< "before Dzuhur 12:08";
	tReminder.setText(ttAdzan);
	int wReminder, hReminder;
	tReminder.getSize(wReminder, hReminder);

	// Set cursor
	SDL_Cursor* handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	SDL_SetCursor(handCursor);

	while (!quit)
	{
		SDL_Event ev;
		if (SDL_WaitEvent(&ev) != 0)
		{
			switch (ev.type)
			{
				case SDL_USEREVENT:
				{
					if (ev.user.data1 == "Date") {
						toggleDate.updateDate();
					}
					break;
				}
				case SDL_QUIT:
				{
					quit = true;
					break;
				}
			}
			toggleDate.handleEvents(&ev);
			quit = btnClose.handleEvents(&ev);
		}

		SDL_SetRenderDrawColor(mSdlRenderer, 18, 20, 23, 255);
		SDL_RenderClear(mSdlRenderer);

		// Render Clock
		tClock.draw(13, 9, (gClock.buffer));

		// Render Date
		toggleDate.draw();

		// Render Adzan Reminder
		//tReminder.draw(windowWidth - wReminder - 13, 10);

		// Render button
		btnClose.draw();

		SDL_RenderPresent(mSdlRenderer);
	}

	prtReminder->freeTexture();

	// Cleanup - just let fc fall out of scope
	return 1;
}
