#include "canvas_i.h"

#include <furi.h>

Canvas* canvas_init() {
    Canvas* canvas = malloc(sizeof(Canvas));
    canvas->offset_x = 0;
    canvas->offset_y = 0;
    canvas->width = 128;
    canvas->height = 64;
    return canvas;
}

void canvas_free(Canvas* canvas) {
    furi_assert(canvas);
    free(canvas);
}

void canvas_commit(Canvas* canvas) {
    // TODO: two framebuffers, one is final, the other one is temp
    return;
}

uint8_t* canvas_get_buffer(Canvas* canvas) {
    furi_assert(canvas);
    return (uint8_t*)canvas->fb;
}

size_t canvas_get_buffer_size(const Canvas* canvas) {
    furi_assert(canvas);
    return 256 * 256;
}

void canvas_frame_set(
    Canvas* canvas,
    uint8_t offset_x,
    uint8_t offset_y,
    uint8_t width,
    uint8_t height) {
    furi_assert(canvas);
    canvas->offset_x = offset_x;
    canvas->offset_y = offset_y;
    canvas->width = width;
    canvas->height = height;
}

uint8_t canvas_width(const Canvas* canvas) {
    furi_assert(canvas);
    return canvas->width;
}

uint8_t canvas_height(const Canvas* canvas) {
    furi_assert(canvas);
    return canvas->height;
}

void canvas_reset(Canvas* canvas) {
    canvas_clear(canvas);
}

void canvas_clear(Canvas* canvas) {
    furi_assert(canvas);
    memset(canvas->fb, 0, sizeof(canvas->fb[0][0]) * 256 * 256);
}

void canvas_draw_dot(Canvas* canvas, uint8_t x, uint8_t y) {
    furi_assert(canvas);
    x += canvas->offset_x;
    y += canvas->offset_y;
    canvas->fb[x][y] = 1;
}

// TODO: other drawing functions