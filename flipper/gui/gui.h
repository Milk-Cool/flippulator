#pragma once

#include "view_port.h"
#include "canvas.h"

typedef enum {
    GuiLayerDesktop, /**< Desktop layer for internal use. Like fullscreen but with status bar */

    GuiLayerWindow, /**< Window layer, status bar is shown */

    GuiLayerStatusBarLeft, /**< Status bar left-side layer, auto-layout */
    GuiLayerStatusBarRight, /**< Status bar right-side layer, auto-layout */

    GuiLayerFullscreen, /**< Fullscreen layer, no status bar */

    GuiLayerMAX /**< Don't use or move, special value */
} GuiLayer;

typedef void (*GuiCanvasCommitCallback)(
    uint8_t* data,
    size_t size,
    CanvasOrientation orientation,
    void* context);

#define RECORD_GUI "gui"

typedef struct Gui Gui;

void exit_sdl(uint8_t code);

void gui_add_view_port(Gui* gui, ViewPort* view_port, GuiLayer layer);
void gui_remove_view_port(Gui* gui, ViewPort* view_port);
Gui* gui_alloc();
int32_t gui_srv(void* p);