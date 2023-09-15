#include "gui_i.h"

#include <pthread.h>
#include <ncurses.h>
#include <time.h>

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
        if(c == KEY_UP) event->key = InputKeyUp;
        else if(c == KEY_DOWN) event->key = InputKeyDown;
        else if(c == KEY_LEFT) event->key = InputKeyLeft;
        else if(c == KEY_RIGHT) event->key = InputKeyRight;
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
        clock_t delay_start = clock();
        if(view_port->draw_callback != NULL)
            view_port->draw_callback(view_port->gui->canvas, view_port->draw_callback_context);
        clear();
        
        for(uint8_t x = 0; x < view_port->gui->canvas->width; x++) {
            for(uint8_t y = 0; y < view_port->gui->canvas->height; y++)
                printw("%s", view_port->gui->canvas->fb[x][y] ? "⣿" : "⠀");
            printw("%s", "\r\n");
        }
        refresh();
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