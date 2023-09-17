#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <u8g2.h>

/** Color enumeration */
typedef enum {
    ColorWhite = 0x00,
    ColorBlack = 0x01,
    ColorXOR = 0x02,
} Color;

/** Fonts enumeration */
typedef enum {
    FontPrimary,
    FontSecondary,
    FontKeyboard,
    FontBigNumbers,

    // Keep last for fonts number calculation
    FontTotalNumber,
} Font;

/** Alignment enumeration */
typedef enum {
    AlignLeft,
    AlignRight,
    AlignTop,
    AlignBottom,
    AlignCenter,
} Align;

/** Canvas Orientation */
typedef enum {
    CanvasOrientationHorizontal,
    CanvasOrientationHorizontalFlip,
    CanvasOrientationVertical,
    CanvasOrientationVerticalFlip,
} CanvasOrientation;

/** Font Direction */
typedef enum {
    CanvasDirectionLeftToRight,
    CanvasDirectionTopToBottom,
    CanvasDirectionRightToLeft,
    CanvasDirectionBottomToTop,
} CanvasDirection;

/** Font parameters */
typedef struct {
    uint8_t leading_default;
    uint8_t leading_min;
    uint8_t height;
    uint8_t descender;
} CanvasFontParameters;

/** Icon flip */
typedef enum {
    IconFlipNone,
    IconFlipHorizontal,
    IconFlipVertical,
    IconFlipBoth,
} IconFlip;

/** Icon rotation */
typedef enum {
    IconRotation0,
    IconRotation90,
    IconRotation180,
    IconRotation270,
} IconRotation;

/** Canvas anonymous structure */
typedef struct Canvas Canvas;

Canvas* canvas_init();
void canvas_free(Canvas* canvas);
void canvas_commit(Canvas* canvas);
uint8_t* canvas_get_buffer(Canvas* canvas);
size_t canvas_get_buffer_size(const Canvas* canvas);
void canvas_frame_set(
    Canvas* canvas,
    uint8_t offset_x,
    uint8_t offset_y,
    uint8_t width,
    uint8_t height);
uint8_t canvas_width(const Canvas* canvas);
uint8_t canvas_height(const Canvas* canvas);
uint8_t canvas_current_font_height(const Canvas* canvas);
const CanvasFontParameters* canvas_get_font_params(const Canvas* canvas, Font font);
void canvas_set_color(Canvas* canvas, Color color);
void canvas_set_font_direction(Canvas* canvas, CanvasDirection dir);
void canvas_invert_color(Canvas* canvas);
void canvas_set_font(Canvas* canvas, Font font);
void canvas_set_custom_u8g2_font(Canvas* canvas, const uint8_t* font);
void canvas_draw_str(Canvas* canvas, uint8_t x, uint8_t y, const char* str);
void canvas_draw_str_aligned(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    Align horizontal,
    Align vertical,
    const char* str);
uint16_t canvas_string_width(Canvas* canvas, const char* str);
uint8_t canvas_glyph_width(Canvas* canvas, char symbol);
void canvas_draw_bitmap(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height,
    const uint8_t* compressed_bitmap_data);
void canvas_draw_u8g2_bitmap(
    u8g2_t* u8g2,
    u8g2_uint_t x,
    u8g2_uint_t y,
    u8g2_uint_t w,
    u8g2_uint_t h,
    const uint8_t* bitmap,
    IconRotation rotation);

void canvas_reset(Canvas* canvas);
void canvas_clear(Canvas* canvas);
void canvas_draw_dot(Canvas* canvas, uint8_t x, uint8_t y);