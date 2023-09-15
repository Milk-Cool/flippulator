#include "gui_i.h"

#include <pthread.h>
#include <stdio.h>
#include <time.h>

Gui* gui_alloc() {
    Gui* gui = malloc(sizeof(Gui));
    gui->canvas = canvas_init();
    return gui;
}

// TODO: long presses and stuff
void* handle_input(void* _view_port) {
    ViewPort* view_port = _view_port;
    while(true) {
        char c;
        scanf("%c", &c);
        InputEvent* event = malloc(sizeof(InputEvent));
        event->type = InputTypePress;
        if(c == 27) {
            scanf("%c", &c);
            if(c == 91) {
                scanf("%c", &c);
                switch(c) {
                    case 65: // Up
                        event->key = InputKeyUp;
                        break;
                    case 66: // Down
                        event->key = InputKeyDown;
                        break;
                    case 67: // Right
                        event->key = InputKeyLeft;
                        break;
                    case 68: // Left
                        event->key = InputKeyRight;
                        break;
                }
            }
        }
        else if(c == 122) event->key = InputKeyOk;
        else if(c == 120) event->key = InputKeyBack;
        view_port->input_callback(event, view_port->input_callback_context);
    }
    return NULL;
}
void* handle_gui(void* _view_port) {
    ViewPort* view_port = _view_port;
    while(true) {
        clock_t delay_start = clock();
        view_port->draw_callback(view_port->gui->canvas, view_port->draw_callback_context);
        printf("\e[1;1H\e[2J");
        for(uint8_t x = 0; x < view_port->gui->canvas->width; x++)
            for(uint8_t y = 0; y < view_port->gui->canvas->height; y++)
                printf("%s", view_port->gui->canvas->fb[x][y] ? "⣿" : "⠀");
        while(clock() < delay_start + 1);
    }
    return NULL;   
}
void gui_add_view_port(Gui* gui, ViewPort* view_port, GuiLayer layer) {
    UNUSED(layer);
    furi_assert(gui); // <--
    furi_assert(view_port);
    gui->view_port = view_port;
    view_port->gui = gui;

    pthread_t input_thread_id;
    pthread_create(&input_thread_id, NULL, handle_input, view_port);
    pthread_t draw_thread_id;
    pthread_create(&draw_thread_id, NULL, handle_gui, view_port);
}
void gui_remove_view_port(Gui* gui, ViewPort* view_port) {
    // TODO
}

int32_t gui_srv(void* p) {
    UNUSED(p);
    Gui* gui = gui_alloc();
    furi_record_create(RECORD_GUI, gui);
    return 0;
}