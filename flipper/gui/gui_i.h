#pragma once

#include "gui.h"
#include "canvas_i.h"
#include "view_port_i.h"

#define GUI_DISPLAY_WIDTH 128
#define GUI_DISPLAY_HEIGHT 64

struct Gui {
    Canvas* canvas;
    ViewPort* view_port;
};