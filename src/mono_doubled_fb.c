#include <clifb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

/**
 * @brief Init cli_mono_doubled_fb struct
 * @return NULL on failure, otherwise a pointer to cli_mono_doubled_fb
 */
struct cli_mono_doubled_fb *init_mono_doubled_fb() {
    struct cli_mono_doubled_fb *fb = 
        malloc(sizeof(struct cli_mono_doubled_fb));
    
    if (fb == NULL) {
        fprintf(
            stderr, 
            "%s: failed to allocate memory for cli_mono_doubled_fb\n",
            __func__
        );
        return NULL;
    }

    memset(fb, 0, sizeof(struct cli_mono_doubled_fb));

    setlocale(LC_ALL, "");
    initscr();
    curs_set(0);

    fb->type = MONO_DOUBLED_FRAMEBUFFER;
    fb->size.x = getmaxx(stdscr) * 2;
    fb->size.y = getmaxy(stdscr) * 2;

    fb->items = calloc(fb->size.y * fb->size.x, sizeof(bool));
    if (fb->items == NULL) {
        fprintf(
            stderr, 
            "%s: failed to allocate memory for items (first layer)\n",
            __func__
        );
        return NULL;
    }

    return fb;
}

/**
 * @brief Destroy cli_mono_doubled_fb struct
 * 
 * @param fb Pointer to cli_mono_doubled_fb
 */
void destroy_mono_doubled_fb(struct cli_mono_doubled_fb *fb) {
    free(fb->items);
    free(fb);
    endwin();
}

/**
 * @brief Refresh cli_mono_doubled_fb struct contents on screen
 * 
 * @param fb Pointer to cli_mono_doubled_fb
 */
void refresh_mono_doubled_fb(struct cli_mono_doubled_fb *fb) {
    for (int y = 0; y < fb->size.y; y += 2) {
        for (int x = 0; x < fb->size.x; x += 2) {
            uint8_t symbol_id = 0;

            // Optimization to increase render performance,
            // construct binary number in enum by shifting bits
            symbol_id |= (GET_MONO_FB_PIXEL(fb, y, x) & 1) << 3;
            symbol_id |= (GET_MONO_FB_PIXEL(fb, y, x + 1) & 1) << 2;
            symbol_id |= (GET_MONO_FB_PIXEL(fb, y + 1, x) & 1) << 1;
            symbol_id |= (GET_MONO_FB_PIXEL(fb, y + 1, x + 1) & 1) << 0;

            nc_mono_doubled_fb_mvadd_wch(y / 2, x / 2, 
                &mono_doubled_fb_item_symbol[symbol_id]);
        }
    }

    nc_mono_doubled_fb_refresh();

    memset(fb->items, 0, fb->size.y * fb->size.x);
}

/*
 * ncurses functions
 *
 * Added to avoid SEGFAULTs when user tries to
 * access ncurses without clifb
 */

void nc_mono_doubled_fb_refresh(void) {
    wrefresh(stdscr);
}

void nc_mono_doubled_fb_mvadd_wch(int y, int x, cchar_t *c) {
    mvadd_wch(y, x, c);
}

void nc_mono_doubled_fb_mvaddstr(int y, int x, char *str) {
    mvwaddstr(stdscr, y, x, str);
}
