#include "gui_i.h"

#include <pthread.h>
#include <ncurses.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include <stdio.h>

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
        c = getch();
        InputEvent* event = malloc(sizeof(InputEvent));
        event->type = InputTypePress;
        if(c == 3) event->key = InputKeyUp;
        else if(c == 2) event->key = InputKeyDown;
        else if(c == 4) event->key = InputKeyLeft;
        else if(c == 5) event->key = InputKeyRight;
        else if(c == 122) event->key = InputKeyOk;
        else if(c == 120) event->key = InputKeyBack;
        else if(c == 113) {
            endwin();
            exit(0);
        }
        view_port->input_callback(event, view_port->input_callback_context);
    }
    return NULL;
}
void* handle_gui(void* _view_port) {
    ViewPort* view_port = _view_port;
    while(true) {
        if(view_port->draw_callback != NULL)
            view_port->draw_callback(view_port->gui->canvas, view_port->draw_callback_context);
        clear();
        
        for(uint8_t x = 0; x < view_port->width; x++)
            for(uint8_t y = 0; y < view_port->height; y++) {
                move(y, x);
                printw("%c", view_port->gui->canvas->fb[view_port->gui->canvas->offset_x + x][view_port->gui->canvas->offset_y + y] == 1 ? 'X' : ' ');
            }
        refresh();
        // ~60 FPS
        #ifdef _WIN32
            Sleep(16); // 16ms
        #else
            usleep(16666); // 16.666 ms
        #endif
    }
    return NULL;   
}
void gui_add_view_port(Gui* gui, ViewPort* view_port, GuiLayer layer) {
    UNUSED(layer);
    furi_assert(gui); // <--
    furi_assert(view_port);
    gui->view_port = view_port;
    view_port->gui = gui;

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    
    pthread_t draw_thread_id;
    pthread_create(&draw_thread_id, NULL, handle_gui, view_port);
    pthread_t input_thread_id;
    pthread_create(&input_thread_id, NULL, handle_input, view_port);
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