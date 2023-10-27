#include "view_port_i.h"

#include "gui.h"
#include "gui_i.h"

#include <furi.h>
#include <flippulator_defines.h>

// static const InputKey view_port_input_mapping[ViewPortOrientationMAX][InputKeyMAX] = {
//     {InputKeyUp,
//      InputKeyDown,
//      InputKeyRight,
//      InputKeyLeft,
//      InputKeyOk,
//      InputKeyBack}, //ViewPortOrientationHorizontal
//     {InputKeyDown,
//      InputKeyUp,
//      InputKeyLeft,
//      InputKeyRight,
//      InputKeyOk,
//      InputKeyBack}, //ViewPortOrientationHorizontalFlip
//     {InputKeyRight,
//      InputKeyLeft,
//      InputKeyDown,
//      InputKeyUp,
//      InputKeyOk,
//      InputKeyBack}, //ViewPortOrientationVertical
//     {InputKeyLeft,
//      InputKeyRight,
//      InputKeyUp,
//      InputKeyDown,
//      InputKeyOk,
//      InputKeyBack}, //ViewPortOrientationVerticalFlip
// };

// static const InputKey view_port_left_hand_input_mapping[InputKeyMAX] =
//     {InputKeyDown, InputKeyUp, InputKeyLeft, InputKeyRight, InputKeyOk, InputKeyBack};

// static const CanvasOrientation view_port_orientation_mapping[ViewPortOrientationMAX] = {
//     [ViewPortOrientationHorizontal] = CanvasOrientationHorizontal,
//     [ViewPortOrientationHorizontalFlip] = CanvasOrientationHorizontalFlip,
//     [ViewPortOrientationVertical] = CanvasOrientationVertical,
//     [ViewPortOrientationVerticalFlip] = CanvasOrientationVerticalFlip,
// };

ViewPort* view_port_alloc() {
    ViewPort* view_port = malloc(sizeof(ViewPort));
    view_port->orientation = ViewPortOrientationHorizontal;
    view_port->is_enabled = true;
    view_port->width = 128;
    view_port->height = 64;
    // view_port->mutex = furi_mutex_alloc(FuriMutexTypeRecursive);
    return view_port;
}

void view_port_free(ViewPort* view_port) {
    furi_assert(view_port);
    /*
    furi_check(furi_mutex_acquire(view_port->mutex, FuriWaitForever) == FuriStatusOk);
    furi_check(view_port->gui == NULL);
    furi_check(furi_mutex_release(view_port->mutex) == FuriStatusOk);
    furi_mutex_free(view_port->mutex);
    */
    free(view_port);
}

void view_port_set_width(ViewPort* view_port, uint8_t width) {
    view_port->width = width;
}
uint8_t view_port_get_width(const ViewPort* view_port) {
    return view_port->width;
}
void view_port_set_height(ViewPort* view_port, uint8_t height) {
    view_port->height = height;
}
uint8_t view_port_get_height(const ViewPort* view_port) {
    return view_port->height;
}

void view_port_enabled_set(ViewPort* view_port, bool enabled) {
    view_port->is_enabled = enabled;
}
bool view_port_is_enabled(const ViewPort* view_port) {
    return view_port->is_enabled;
}

void view_port_draw_callback_set(ViewPort* view_port, ViewPortDrawCallback callback, void* context) {
    view_port->draw_callback = callback;
    view_port->draw_callback_context = context;
}

void view_port_input_callback_set(ViewPort* view_port, ViewPortInputCallback callback, void* context) {
    view_port->input_callback = callback;
    view_port->input_callback_context = context;
}

void view_port_update(ViewPort* view_port) {
    #ifdef FLIPPULATOR_USES_WEBASSEMBLY
    furi_delay_us(1);
    #endif
}

// TODO: other methods