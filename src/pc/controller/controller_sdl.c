#if !defined(_WIN32) && !defined(_WIN64)

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>

#include <ultra64.h>

#include "controller_api.h"
#include "pc/configfile.h"
#ifdef TOUCH_CONTROLS
#include "controller_touchscreen.h"
#endif

#define DEADZONE 4960

// One N64 button per configurable gamepad binding; resolved from the config
// strings on first use so the config file has been read by then.
struct GamepadBinding {
    const char *name;
    uint16_t mask;
    SDL_GameControllerButton button;
};

static struct GamepadBinding bindings[] = {
    { configGamepadA,      A_BUTTON,      SDL_CONTROLLER_BUTTON_INVALID },
    { configGamepadB,      B_BUTTON,      SDL_CONTROLLER_BUTTON_INVALID },
    { configGamepadZ,      Z_TRIG,        SDL_CONTROLLER_BUTTON_INVALID },
    { configGamepadR,      R_TRIG,        SDL_CONTROLLER_BUTTON_INVALID },
    { configGamepadL,      L_TRIG,        SDL_CONTROLLER_BUTTON_INVALID },
    { configGamepadStart,  START_BUTTON,  SDL_CONTROLLER_BUTTON_INVALID },
    { configGamepadCUp,    U_CBUTTONS,    SDL_CONTROLLER_BUTTON_INVALID },
    { configGamepadCDown,  D_CBUTTONS,    SDL_CONTROLLER_BUTTON_INVALID },
    { configGamepadCLeft,  L_CBUTTONS,    SDL_CONTROLLER_BUTTON_INVALID },
    { configGamepadCRight, R_CBUTTONS,    SDL_CONTROLLER_BUTTON_INVALID },
    { configGamepadDUp,    U_JPAD,        SDL_CONTROLLER_BUTTON_INVALID },
    { configGamepadDDown,  D_JPAD,        SDL_CONTROLLER_BUTTON_INVALID },
    { configGamepadDLeft,  L_JPAD,        SDL_CONTROLLER_BUTTON_INVALID },
    { configGamepadDRight, R_JPAD,        SDL_CONTROLLER_BUTTON_INVALID },
};
static SDL_GameControllerButton touch_toggle_button = SDL_CONTROLLER_BUTTON_INVALID;
static bool bindings_resolved;

static void resolve_bindings(void) {
    for (size_t i = 0; i < sizeof(bindings) / sizeof(bindings[0]); i++) {
        // SDL returns INVALID for "none" and for anything it does not know.
        bindings[i].button = SDL_GameControllerGetButtonFromString(bindings[i].name);
    }
    touch_toggle_button = SDL_GameControllerGetButtonFromString(configGamepadTouchToggle);
    bindings_resolved = true;
}

static bool init_ok;
static SDL_GameController *sdl_cntrl;

static void controller_sdl_init(void) {
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0) {
        fprintf(stderr, "SDL init error: %s\n", SDL_GetError());
        return;
    }

    init_ok = true;
}

static void controller_sdl_read(OSContPad *pad) {
    if (!init_ok) {
        return;
    }

    SDL_GameControllerUpdate();

    if (sdl_cntrl != NULL && !SDL_GameControllerGetAttached(sdl_cntrl)) {
        SDL_GameControllerClose(sdl_cntrl);
        sdl_cntrl = NULL;
    }
    if (sdl_cntrl == NULL) {
        for (int i = 0; i < SDL_NumJoysticks(); i++) {
            if (SDL_IsGameController(i)) {
                sdl_cntrl = SDL_GameControllerOpen(i);
                if (sdl_cntrl != NULL) {
                    break;
                }
            }
        }
    }

    if (!bindings_resolved) {
        resolve_bindings();
    }

#ifdef TOUCH_CONTROLS
    static bool was_attached;
    static bool toggle_was_down;
    bool attached = sdl_cntrl != NULL;
    if (attached != was_attached) {
        was_attached = attached;
        if (configTouchControls && configTouchAutoHide) {
            gTouchControlsVisible = !attached;
        }
    }
#endif

    if (sdl_cntrl == NULL) {
        return;
    }

#ifdef TOUCH_CONTROLS
    if (touch_toggle_button != SDL_CONTROLLER_BUTTON_INVALID) {
        bool down = SDL_GameControllerGetButton(sdl_cntrl, touch_toggle_button);
        if (down && !toggle_was_down) {
            gTouchControlsVisible = !gTouchControlsVisible;
        }
        toggle_was_down = down;
    }
#endif

    for (size_t i = 0; i < sizeof(bindings) / sizeof(bindings[0]); i++) {
        if (bindings[i].button != SDL_CONTROLLER_BUTTON_INVALID &&
            SDL_GameControllerGetButton(sdl_cntrl, bindings[i].button)) {
            pad->button |= bindings[i].mask;
        }
    }

    int16_t leftx = SDL_GameControllerGetAxis(sdl_cntrl, SDL_CONTROLLER_AXIS_LEFTX);
    int16_t lefty = SDL_GameControllerGetAxis(sdl_cntrl, SDL_CONTROLLER_AXIS_LEFTY);
    int16_t rightx = SDL_GameControllerGetAxis(sdl_cntrl, SDL_CONTROLLER_AXIS_RIGHTX);
    int16_t righty = SDL_GameControllerGetAxis(sdl_cntrl, SDL_CONTROLLER_AXIS_RIGHTY);

    int16_t ltrig = SDL_GameControllerGetAxis(sdl_cntrl, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
    int16_t rtrig = SDL_GameControllerGetAxis(sdl_cntrl, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

#ifdef TARGET_WEB
    // Firefox has a bug: https://bugzilla.mozilla.org/show_bug.cgi?id=1606562
    // It sets down y to 32768.0f / 32767.0f, which is greater than the allowed 1.0f,
    // which SDL then converts to a int16_t by multiplying by 32767.0f, which overflows into -32768.
    // Maximum up will hence never become -32768 with the current version of SDL2,
    // so this workaround should be safe in compliant browsers.
    if (lefty == -32768) {
        lefty = 32767;
    }
    if (righty == -32768) {
        righty = 32767;
    }
#endif

    if (rightx < -0x4000) pad->button |= L_CBUTTONS;
    if (rightx > 0x4000) pad->button |= R_CBUTTONS;
    if (righty < -0x4000) pad->button |= U_CBUTTONS;
    if (righty > 0x4000) pad->button |= D_CBUTTONS;

    if (ltrig > 30 * 256) pad->button |= Z_TRIG;
    if (rtrig > 30 * 256) pad->button |= R_TRIG;

    uint32_t magnitude_sq = (uint32_t)(leftx * leftx) + (uint32_t)(lefty * lefty);
    if (magnitude_sq > (uint32_t)(DEADZONE * DEADZONE)) {
        // Game expects stick coordinates within -80..80
        // 32768 / 409 = ~80
        pad->stick_x = leftx / 409;
        pad->stick_y = -lefty / 409;
    }
}

struct ControllerAPI controller_sdl = {
    controller_sdl_init,
    controller_sdl_read
};

#endif
