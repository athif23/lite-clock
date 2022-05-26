// testSkia.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define SK_GL	

#include <SDL.h>
#include <include/gpu/GrDirectContext.h>
#include <include/core/SkCanvas.h>
#include <include/utils/SkRandom.h>
#include <include/core/SkSurface.h>
#include <include/gpu/gl/GrGLInterface.h>
#include <include/core/SkFont.h>
#include <include/core/SkColor.h>
#if defined(SK_BUILD_FOR_ANDROID)
#include <GLES/gl.h>
#elif defined(SK_BUILD_FOR_UNIX)
#include <GL/gl.h>
#elif defined(SK_BUILD_FOR_MAC)
#include <gl.h>
#endif
#include <GL/glew.h>
#include <iostream>

SkRect rectBound = { SkFloatToScalar(0.0f), SkFloatToScalar(0.0f), SkFloatToScalar(70.0f), SkFloatToScalar(70.0f) };

struct ApplicationState {
    ApplicationState() : fQuit(false) {}
    bool fQuit;
};

static void handle_error() {
    const char* error = SDL_GetError();
    SkDebugf("SDL Error: %s\n", error);
    SDL_ClearError();
}

static void handle_events(ApplicationState* state, SkCanvas* canvas) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_MOUSEMOTION:
        {
            //if (event.motion.state == SDL_PRESSED) {
               // SkRect& rect = state->fRects.back();
            std::cout << " X " << event.motion.x << "\n";
            std::cout << " Y " << event.motion.y << "\n";

            int x = SkIntToScalar(event.motion.x);
            int y = SkIntToScalar(event.motion.y);
            if (x < rectBound.fRight && x > rectBound.fLeft && y < rectBound.fBottom && y > rectBound.fTop) {
                std::cout << "Move over\n";
            }
        }
        break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.state == SDL_PRESSED) {

            }
            break;
        case SDL_KEYDOWN: {
            SDL_Keycode key = event.key.keysym.sym;
            if (key == SDLK_ESCAPE) {
                state->fQuit = true;
            }
            break;
        }
        case SDL_QUIT:
            state->fQuit = true;
            break;
        default:
            break;
        }
    }
}

// Creates a star type shape using a SkPath
static void create_star(SkCanvas* canvas) {
    static const int kNumPoints = 5;
    SkPath concavePath;
    SkPaint paint(SkColors::kGreen);
    // paint.setStrokeWidth(SkIntToScalar(200));
    paint.setStyle(SkPaint::Style::kFill_Style);

    SkRRect rect{};
    rect.setRect(rectBound);
    canvas->drawRRect(rect, paint);

    canvas->save();
    return;
}

#if defined(SK_BUILD_FOR_ANDROID)
int SDL_main(int argc, char** argv) {
#else
int main(int argc, char** argv) {
#endif
    uint32_t windowFlags = 0;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GLContext glContext = nullptr;
#if defined(SK_BUILD_FOR_ANDROID) || defined(SK_BUILD_FOR_IOS)
    // For Android/iOS we need to set up for OpenGL ES and we make the window hi res & full screen
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN_DESKTOP |
        SDL_WINDOW_ALLOW_HIGHDPI;
#else
    // For all other clients we use the core profile and operate in a window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
#endif
    static const int kStencilBits = 8;  // Skia needs 8 stencil bits
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, kStencilBits);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // If you want multisampling, uncomment the below lines and set a sample count
    static const int kMsaaSampleCount = 0; //4;
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, kMsaaSampleCount);

    /*
     * In a real application you might want to initialize more subsystems
     */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        handle_error();
        return 1;
    }
    glewInit();
    // Setup window
    // This code will create a window with the same resolution as the user's desktop.
    SDL_DisplayMode dm;

    if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
        handle_error();
        return 1;
    }
    dm.h = 600;
    dm.w = 500;
    SDL_Window* window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, dm.w, dm.h, windowFlags);

    if (!window) {
        handle_error();
        return 1;
    }

    // To go fullscreen
    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    // try and setup a GL context
    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        handle_error();
        return 1;
    }

    int success = SDL_GL_MakeCurrent(window, glContext);
    if (success != 0) {
        handle_error();
        return success;
    }

    uint32_t windowFormat = SDL_GetWindowPixelFormat(window);
    int contextType;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &contextType);


    int dw, dh;
    SDL_GL_GetDrawableSize(window, &dw, &dh);

    glViewport(0, 0, dw, dh);
    glClearColor(1, 1, 1, 1);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // setup GrContext
    auto interface = GrGLMakeNativeInterface();

    // setup contexts
    sk_sp<GrDirectContext> grContext(GrDirectContext::MakeGL(interface));
    SkASSERT(grContext);

    // Wrap the frame buffer object attached to the screen in a Skia render target so Skia can
    // render to it
    GrGLint buffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);
    GrGLFramebufferInfo info;
    info.fFBOID = (GrGLuint)buffer;

#if defined(SK_BUILD_FOR_ANDROID)
    info.fFormat = GL_RGB8_OES;
#else
    info.fFormat = GL_RGB8;
#endif

    GrBackendRenderTarget target(dw, dh, kMsaaSampleCount, kStencilBits, info);

    // setup SkSurface
    // To use distance field text, use commented out SkSurfaceProps instead
    // SkSurfaceProps props(SkSurfaceProps::kUseDeviceIndependentFonts_Flag,
    //                      SkSurfaceProps::kLegacyFontHost_InitType);
    SkSurfaceProps props(SkSurfaceProps::kLegacyFontHost_InitType);

    sk_sp<SkSurface> surface(SkSurface::MakeFromBackendRenderTarget(grContext.get(), target,
        kBottomLeft_GrSurfaceOrigin,
        kRGB_888x_SkColorType, SkColorSpace::MakeSRGB(), &props));

    SkCanvas* canvas = surface->getCanvas();
    canvas->scale((float)dw / dm.w, (float)dh / dm.h);

    ApplicationState state;

    const char* helpMessage = "Click and drag to create rects.  Press esc to quit.";

    // SkPaint paint;

     // create a surface for CPU rasterization
    sk_sp<SkSurface> cpuSurface = canvas->makeSurface(canvas->imageInfo()); //(SkSurface::MakeRaster(canvas->imageInfo()));


    SkCanvas* offscreen = cpuSurface->getCanvas();
    offscreen->save();

    //offscreen->translate(50.0f, 50.0f);
   // offscreen->drawPath(create_star(), paint);
    create_star(offscreen);
    offscreen->restore();

    sk_sp<SkImage> image = cpuSurface->makeImageSnapshot();

    int rotation = 0;
    SkFont font;
    while (!state.fQuit) { // Our application loop

        handle_events(&state, canvas);
        // canvas->save();

        canvas->drawImage(image, 0.0f, 0.0f);
        canvas->restore();

        canvas->flush();
        SDL_GL_SwapWindow(window);
    }

    if (glContext) {
        SDL_GL_DeleteContext(glContext);
    }

    //Destroy window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();
    return 0;
}