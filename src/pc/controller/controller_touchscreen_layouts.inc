//Not gonna name it .inc.c because of makefile.
//This file will have more layouts for dynamic controls later.

//DEFAULT
// Left thumb: stick only. Right thumb: A big at the bottom right with B
// diagonally up-left of it, Z under A, R as a shoulder in the top-right
// corner, and the C-button diamond below R. Start stays bottom centre.
struct Position GetDefaultJoystickPos() {
    struct Position ret = { .x = GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(56) << 2, .y = 700 };
    return ret;
}

struct Position GetDefaultStartPos() {
    struct Position ret = { .x = SCREEN_WIDTH_API / 2, .y = 900 };
    return ret;
}

struct Position GetDefaultAPos() {
    struct Position ret = { .x = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(24) << 2, .y = 640 };
    return ret;
}

struct Position GetDefaultBPos() {
    struct Position ret = { .x = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(64) << 2, .y = 560 };
    return ret;
}

struct Position GetDefaultZPos() {
    struct Position ret = { .x = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(24) << 2, .y = 800 };
    return ret;
}

struct Position GetDefaultRPos() {
    struct Position ret = { .x = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(20) << 2, .y = 80 };
    return ret;
}

struct Position GetDefaultUpPos() {
    struct Position ret = { .x = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(50) << 2, .y = 200 };
    return ret;
}

struct Position GetDefaultDownPos() {
    struct Position ret = { .x = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(50) << 2, .y = 400 };
    return ret;
}

struct Position GetDefaultLeftPos() {
    struct Position ret = { .x = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(80) << 2, .y = 300 };
    return ret;
}

struct Position GetDefaultRightPos() {
    struct Position ret = { .x = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(20) << 2, .y = 300 };
    return ret;
}

static struct ControlElement ControlElementsDefault[] = {
{.type = Joystick, .GetPos = GetDefaultJoystickPos},
{.type = Button, .GetPos = GetDefaultStartPos, .character = 's', .buttonID = START_BUTTON},
{.type = Button, .GetPos = GetDefaultAPos, .character = 'a', .buttonID = A_BUTTON, .hitSize = 170, .drawHalf = 84},
{.type = Button, .GetPos = GetDefaultBPos, .character = 'b', .buttonID = B_BUTTON, .hitSize = 150, .drawHalf = 72},
{.type = Button, .GetPos = GetDefaultZPos, .character = 'z', .buttonID = Z_TRIG}, // HUD font has Z at index 35 ('z' - 87)
{.type = Button, .GetPos = GetDefaultRPos, .character = 'r', .buttonID = R_TRIG},
{.type = Button, .GetPos = GetDefaultUpPos, .character = 'u', .buttonID = U_CBUTTONS},
{.type = Button, .GetPos = GetDefaultDownPos, .character = 'd', .buttonID = D_CBUTTONS},
{.type = Button, .GetPos = GetDefaultLeftPos, .character = 'l', .buttonID = L_CBUTTONS},
{.type = Button, .GetPos = GetDefaultRightPos, .character = 'r', .buttonID = R_CBUTTONS}
};
