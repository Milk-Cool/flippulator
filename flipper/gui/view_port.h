#pragma once

#include "canvas.h"
#include <input/input.h>

typedef struct ViewPort ViewPort;

typedef enum {
    ViewPortOrientationHorizontal,
    ViewPortOrientationHorizontalFlip,
    ViewPortOrientationVertical,
    ViewPortOrientationVerticalFlip,
    ViewPortOrientationMAX, /**< Special value, don't use it */
} ViewPortOrientation;

/** ViewPort Draw callback
 * @warning    called from GUI thread
 */
typedef void (*ViewPortDrawCallback)(Canvas* canvas, void* context);

/** ViewPort Input callback
 * @warning    called from GUI thread
 */
typedef void (*ViewPortInputCallback)(InputEvent* event, void* context);

/** ViewPort allocator
 *
 * always returns view_port or stops system if not enough memory.
 *
 * @return     ViewPort instance
 */
ViewPort* view_port_alloc();

/** ViewPort deallocator
 *
 * Ensure that view_port was unregistered in GUI system before use.
 *
 * @param      view_port  ViewPort instance
 */
void view_port_free(ViewPort* view_port);

/** Set view_port width.
 *
 * Will be used to limit canvas drawing area and autolayout feature.
 *
 * @param      view_port  ViewPort instance
 * @param      width      wanted width, 0 - auto.
 */
void view_port_set_width(ViewPort* view_port, uint8_t width);
uint8_t view_port_get_width(const ViewPort* view_port);

/** Set view_port height.
 *
 * Will be used to limit canvas drawing area and autolayout feature.
 *
 * @param      view_port  ViewPort instance
 * @param      height     wanted height, 0 - auto.
 */
void view_port_set_height(ViewPort* view_port, uint8_t height);
uint8_t view_port_get_height(const ViewPort* view_port);

/** Enable or disable view_port rendering.
 *
 * @param      view_port  ViewPort instance
 * @param      enabled    Indicates if enabled
 * @warning    automatically dispatches update event
 */
void view_port_enabled_set(ViewPort* view_port, bool enabled);
bool view_port_is_enabled(const ViewPort* view_port);

/** ViewPort event callbacks
 *
 * @param      view_port  ViewPort instance
 * @param      callback   appropriate callback function
 * @param      context    context to pass to callback
 */
void view_port_draw_callback_set(ViewPort* view_port, ViewPortDrawCallback callback, void* context);
void view_port_input_callback_set(
    ViewPort* view_port,
    ViewPortInputCallback callback,
    void* context);

void view_port_update(ViewPort* view_port);