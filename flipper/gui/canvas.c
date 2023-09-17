// https://github.com/flipperdevices/flipperzero-firmware/blob/dev/applications/services/gui/canvas.c

#include "canvas_i.h"

#include <furi.h>
#include <u8g2_glue.h>

const CanvasFontParameters canvas_font_params[FontTotalNumber] = {
    [FontPrimary] = {.leading_default = 12, .leading_min = 11, .height = 8, .descender = 2},
    [FontSecondary] = {.leading_default = 11, .leading_min = 9, .height = 7, .descender = 2},
    [FontKeyboard] = {.leading_default = 11, .leading_min = 9, .height = 7, .descender = 2},
    [FontBigNumbers] = {.leading_default = 18, .leading_min = 16, .height = 15, .descender = 0},
};

Canvas* canvas_init() {
    Canvas* canvas = malloc(sizeof(Canvas));
    u8g2_Setup_st756x_flipper(&canvas->fb, U8G2_R0, u8x8_hw_spi_stm32, u8g2_gpio_and_delay_stm32);
    canvas->orientation = CanvasOrientationHorizontal;
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
    return u8g2_GetBufferPtr(&canvas->fb);
}

size_t canvas_get_buffer_size(const Canvas* canvas) {
    furi_assert(canvas);
    return u8g2_GetBufferTileWidth(&canvas->fb) * u8g2_GetBufferTileHeight(&canvas->fb) * 8;
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

uint8_t canvas_current_font_height(const Canvas* canvas) {
    furi_assert(canvas);
    uint8_t font_height = u8g2_GetMaxCharHeight(&canvas->fb);

    if(canvas->fb.font == u8g2_font_haxrcorp4089_tr) {
        font_height += 1;
    }

    return font_height;
}

const CanvasFontParameters* canvas_get_font_params(const Canvas* canvas, Font font) {
    furi_assert(canvas);
    furi_assert(font < FontTotalNumber);
    return &canvas_font_params[font];
}

void canvas_reset(Canvas* canvas) {
    canvas_clear(canvas);
}

void canvas_set_color(Canvas* canvas, Color color) {
    furi_assert(canvas);
    u8g2_SetDrawColor(&canvas->fb, color);
}

void canvas_set_font_direction(Canvas* canvas, CanvasDirection dir) {
    furi_assert(canvas);
    u8g2_SetFontDirection(&canvas->fb, dir);
}

void canvas_invert_color(Canvas* canvas) {
    canvas->fb.draw_color = !canvas->fb.draw_color;
}

void canvas_set_font(Canvas* canvas, Font font) {
    furi_assert(canvas);
    u8g2_SetFontMode(&canvas->fb, 1);
    if(font == FontPrimary) {
        u8g2_SetFont(&canvas->fb, u8g2_font_helvB08_tr);
    } else if(font == FontSecondary) {
        u8g2_SetFont(&canvas->fb, u8g2_font_haxrcorp4089_tr);
    } else if(font == FontKeyboard) {
        u8g2_SetFont(&canvas->fb, u8g2_font_profont11_mr);
    } else if(font == FontBigNumbers) {
        u8g2_SetFont(&canvas->fb, u8g2_font_profont22_tn);
    } else {
        furi_crash(NULL);
    }
}

void canvas_set_custom_u8g2_font(Canvas* canvas, const uint8_t* font) {
    furi_assert(canvas);
    u8g2_SetFontMode(&canvas->fb, 1);
    u8g2_SetFont(&canvas->fb, font);
}

void canvas_draw_str(Canvas* canvas, uint8_t x, uint8_t y, const char* str) {
    furi_assert(canvas);
    if(!str) return;
    x += canvas->offset_x;
    y += canvas->offset_y;
    u8g2_DrawStr(&canvas->fb, x, y, str);
}

void canvas_draw_str_aligned(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    Align horizontal,
    Align vertical,
    const char* str) {
    furi_assert(canvas);
    if(!str) return;
    x += canvas->offset_x;
    y += canvas->offset_y;

    switch(horizontal) {
    case AlignLeft:
        break;
    case AlignRight:
        x -= u8g2_GetStrWidth(&canvas->fb, str);
        break;
    case AlignCenter:
        x -= (u8g2_GetStrWidth(&canvas->fb, str) / 2);
        break;
    default:
        furi_crash(NULL);
        break;
    }

    switch(vertical) {
    case AlignTop:
        y += u8g2_GetAscent(&canvas->fb);
        break;
    case AlignBottom:
        break;
    case AlignCenter:
        y += (u8g2_GetAscent(&canvas->fb) / 2);
        break;
    default:
        furi_crash(NULL);
        break;
    }

    u8g2_DrawStr(&canvas->fb, x, y, str);
}

uint16_t canvas_string_width(Canvas* canvas, const char* str) {
    furi_assert(canvas);
    if(!str) return 0;
    return u8g2_GetStrWidth(&canvas->fb, str);
}

uint8_t canvas_glyph_width(Canvas* canvas, char symbol) {
    furi_assert(canvas);
    return u8g2_GetGlyphWidth(&canvas->fb, symbol);
}

void canvas_draw_bitmap(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height,
    const uint8_t* compressed_bitmap_data) {
    furi_assert(canvas);

    x += canvas->offset_x;
    y += canvas->offset_y;
    uint8_t* bitmap_data = NULL;
    compress_icon_decode(canvas->compress_icon, compressed_bitmap_data, &bitmap_data);
    canvas_draw_u8g2_bitmap(&canvas->fb, x, y, width, height, bitmap_data, IconRotation0);
}

// TODO: animation

static void canvas_draw_u8g2_bitmap_int(
    u8g2_t* u8g2,
    u8g2_uint_t x,
    u8g2_uint_t y,
    u8g2_uint_t w,
    u8g2_uint_t h,
    bool mirror,
    bool rotation,
    const uint8_t* bitmap) {
    u8g2_uint_t blen;
    blen = w;
    blen += 7;
    blen >>= 3;

    if(rotation && !mirror) {
        x += w + 1;
    } else if(mirror && !rotation) {
        y += h - 1;
    }

    while(h > 0) {
        const uint8_t* b = bitmap;
        uint16_t len = w;
        uint16_t x0 = x;
        uint16_t y0 = y;
        uint8_t mask;
        uint8_t color = u8g2->draw_color;
        uint8_t ncolor = (color == 0 ? 1 : 0);
        mask = 1;

        while(len > 0) {
            if(u8x8_pgm_read(b) & mask) {
                u8g2->draw_color = color;
                u8g2_DrawHVLine(u8g2, x0, y0, 1, 0);
            } else if(u8g2->bitmap_transparency == 0) {
                u8g2->draw_color = ncolor;
                u8g2_DrawHVLine(u8g2, x0, y0, 1, 0);
            }

            if(rotation) {
                y0++;
            } else {
                x0++;
            }

            mask <<= 1;
            if(mask == 0) {
                mask = 1;
                b++;
            }
            len--;
        }

        u8g2->draw_color = color;
        bitmap += blen;

        if(mirror) {
            if(rotation) {
                x++;
            } else {
                y--;
            }
        } else {
            if(rotation) {
                x--;
            } else {
                y++;
            }
        }
        h--;
    }
}

void canvas_draw_u8g2_bitmap(
    u8g2_t* u8g2,
    u8g2_uint_t x,
    u8g2_uint_t y,
    u8g2_uint_t w,
    u8g2_uint_t h,
    const uint8_t* bitmap,
    IconRotation rotation) {
    u8g2_uint_t blen;
    blen = w;
    blen += 7;
    blen >>= 3;
#ifdef U8G2_WITH_INTERSECTION
    if(u8g2_IsIntersection(u8g2, x, y, x + w, y + h) == 0) return;
#endif /* U8G2_WITH_INTERSECTION */

    switch(rotation) {
    case IconRotation0:
        canvas_draw_u8g2_bitmap_int(u8g2, x, y, w, h, 0, 0, bitmap);
        break;
    case IconRotation90:
        canvas_draw_u8g2_bitmap_int(u8g2, x, y, w, h, 0, 1, bitmap);
        break;
    case IconRotation180:
        canvas_draw_u8g2_bitmap_int(u8g2, x, y, w, h, 1, 0, bitmap);
        break;
    case IconRotation270:
        canvas_draw_u8g2_bitmap_int(u8g2, x, y, w, h, 1, 1, bitmap);
        break;
    default:
        break;
    }
}

void canvas_clear(Canvas* canvas) {
    furi_assert(canvas);
    u8g2_ClearBuffer(&canvas->fb);
}

void canvas_draw_dot(Canvas* canvas, uint8_t x, uint8_t y) {
    furi_assert(canvas);
    x += canvas->offset_x;
    y += canvas->offset_y;
    u8g2_DrawPixel(&canvas->fb, x, y);
}

// TODO: other drawing functions