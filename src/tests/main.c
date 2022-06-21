#include <clifb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

struct clifb_test {
    void (*func)(struct cli_mono_doubled_fb *);
    const char *name;
    int delay;
};

void test_pure_white(struct cli_mono_doubled_fb *fb) {
    for (int y = 0; y < fb->size.y; y++)
        for (int x = 0; x < fb->size.x; x++)
            SET_MONO_FB_PIXEL(fb, y, x, true);

    refresh_fb((void *)fb);
}

void test_screen_borders(struct cli_mono_doubled_fb *fb) {
    for (int y = 0; y < fb->size.y; y++) {
        if (y == 0 || y == (fb->size.y - 1)) {
            for (int x = 0; x < fb->size.x; x++) {
                SET_MONO_FB_PIXEL(fb, y, x, true);
            }
        } else {
            SET_MONO_FB_PIXEL(fb, y, 0, true);
            SET_MONO_FB_PIXEL(fb, y, fb->size.x - 1, true);
        }
    }

    refresh_fb((void *)fb);
}

void test_fps(struct cli_mono_doubled_fb *fb) {
    bool filled = true;
    struct timeval sv, ev, diff;
    char *dbg = malloc(40);

    for (int i = 0; i < 200; i++) {
        gettimeofday(&sv, NULL);

        for (int y = 0; y < fb->size.y; y++) {
            for (int x = 0; x < fb->size.x; x++) {
                if (i % 2 == 0) {
                    SET_MONO_FB_PIXEL(fb, y, x, filled);
                } else {
                    SET_MONO_FB_PIXEL(fb, y, x, !filled);
                }

                filled = !filled;
            }

            filled = !filled;
        }

        refresh_fb((void *)fb);

        gettimeofday(&ev, NULL);

        timersub(&sv, &ev, &diff);

        sprintf(dbg, "%.1f FPS (%.2f ms, frame %d)", 
            1000.0 / ((float)diff.tv_usec / 1.0e6f), 
            (float)diff.tv_usec / 1.0e6f, i);
        nc_mono_doubled_fb_mvaddstr(0, 0, dbg);
        nc_mono_doubled_fb_refresh();
    }

    free(dbg);
}

void draw_rectangle(struct cli_mono_doubled_fb *fb, int ux, int uy, 
                    int w, int h) {
    for (int y = uy; y < uy + h; y++) {
        if (y >= fb->size.y || y < 0)
            continue;
        if (y == uy || y == (uy + h - 1)) {
            for (int x = ux; x < ux + w; x++) {
                if (x >= fb->size.x || x < 0)
                    continue;
                SET_MONO_FB_PIXEL(fb, y, x, true);
            }
        } else {
            if (ux < fb->size.x || ux >= 0)
                SET_MONO_FB_PIXEL(fb, y, ux, true);
            if ((ux + w - 1) < fb->size.x || (ux + w - 1) >= 0)
                SET_MONO_FB_PIXEL(fb, y, ux + w - 1, true);
        }
    }
}

void draw_filled_rectangle(struct cli_mono_doubled_fb *fb, int ux, int uy, 
                    int w, int h) {
    for (int y = uy; y < uy + h; y++) {
        if (y < 0 || y >= fb->size.y)
            continue;
        
        for (int x = ux; x < ux + w; x++) {
            if (x < 0 || x >= fb->size.x)
                continue;
            
            SET_MONO_FB_PIXEL(fb, y, x, true);
        }
    }
}

void test_rects(struct cli_mono_doubled_fb *fb) {
    int x = 10, y = 1;

    while (true) {
        if (x == fb->size.x && y == fb->size.y)
            break;

        if (x < fb->size.x)
            x++;

        if (y < fb->size.y)
            y++;

        draw_rectangle(fb, 0, 0, x, y);

        usleep(1000 * 5);

        refresh_fb((void *)fb);
    }
}

void test_frects(struct cli_mono_doubled_fb *fb) {
    int x = 10, y = 1;

    while (true) {
        if (x == fb->size.x && y == fb->size.y)
            break;

        if (x < fb->size.x)
            x++;

        if (y < fb->size.y)
            y++;

        draw_filled_rectangle(fb, 0, 0, x, y);

        usleep(1000 * 5);

        refresh_fb((void *)fb);
    }
}

void test_crects(struct cli_mono_doubled_fb *fb) {
    int x = 2, y = 1;

    while (true) {
        if (x == fb->size.x && y == fb->size.y)
            break;

        if (x < fb->size.x)
            x += 2;

        if (y < fb->size.y)
            y++;

        int posx = (fb->size.x - x) / 2;
        int posy = (fb->size.y - y) / 2;

        draw_rectangle(fb, posx, posy, x, y);

        usleep(1000 * 5);

        refresh_fb((void *)fb);
    }
}

void test_cfrects(struct cli_mono_doubled_fb *fb) {
    int x = 2, y = 1;

    while (true) {
        if (x == fb->size.x && y == fb->size.y)
            break;

        if (x < fb->size.x)
            x += 2;

        if (y < fb->size.y)
            y++;

        int posx = (fb->size.x - x) / 2;
        int posy = (fb->size.y - y) / 2;

        draw_filled_rectangle(fb, posx, posy, x, y);

        usleep(1000 * 5);

        refresh_fb((void *)fb);
    }
}

static const struct clifb_test tests[] = {
    {
        .func = test_pure_white,
        .name = "Pure white",
        .delay = 1000,
    },
    {
        .func = test_screen_borders,
        .name = "Screen borders",
        .delay = 1000,
    },
    {
        .func = test_fps,
        .name = "FPS test\n(2-3ms after every frame\nfor fps counter update!)",
        .delay = 1000,
    },
    {
        .func = test_rects,
        .name = "Rectangles",
        .delay = 0,
    },
    {
        .func = test_frects,
        .name = "Filled rectangles",
        .delay = 0,
    },
    {
        .func = test_crects,
        .name = "Centered rectangles",
        .delay = 0,
    },
    {
        .func = test_cfrects,
        .name = "Centered filled rectangles",
        .delay = 0,
    },
};

void run_test(struct cli_mono_doubled_fb *fb, int i) {
    struct clifb_test item = tests[i];

    refresh_fb((void *)fb);

    char *test_name = malloc(strlen(item.name) + 30);
    sprintf(test_name, "Running test #%d: %s", i + 1, item.name);
    nc_mono_doubled_fb_mvaddstr(0, 0, test_name);
    nc_mono_doubled_fb_refresh();
    free(test_name);
    
    usleep(1000 * 1000); // 1s
    item.func(fb);
    usleep(1000 * item.delay);
}

int main_with_args(int argc, char **argv, struct cli_mono_doubled_fb *fb) {
    for (int i = 1; i < argc; i++) {
        if (atoi(argv[i]) >= 0) {
            int testnum = atoi(argv[i]);
            
            if (testnum > (sizeof(tests) / sizeof(tests[0])) || testnum == 0) {
                destroy_fb(fb);
                printf("Invalid test number: %d (of %lu available)\n", testnum, (sizeof(tests) / sizeof(tests[0])));
                return 0;
            }

            run_test(fb, testnum - 1);
        } else if (strstr(argv[i], "tests")) {
            destroy_fb(fb);

            printf("\nAll available tests:\n");
            for (int i = 0; i < sizeof(tests) / sizeof(tests[0]); i++)
                printf("%d) %s\n", i, tests[i].name);
            printf("\nTo run test, use %s 'test number'\n", argv[0]);

            return 0;
        }
    }

    destroy_fb(fb);

    return 0;
}

int main(int argc, char **argv) {
    struct cli_mono_doubled_fb *fb = NULL;

    printf("Initializing Monochrome Doubled Framebuffer...\n");
    init_fb(MONO_DOUBLED_FRAMEBUFFER, (void **)&fb);

    if (argc > 1)
        return main_with_args(argc, argv, fb);

    printw("WARNING: Running automated tests, it'll take long time (10s delay)...\n");
    printw("To run one test, use %s 'test number'\n", argv[0]);
    printw("To see all tests with their numbers, use %s --tests\n", argv[0]);
    nc_mono_doubled_fb_refresh();
    usleep(1000 * 10000);

    for (int i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
        struct clifb_test item = tests[i];

        refresh_fb((void *)fb);

        char *test_name = malloc(strlen(item.name) + 30);
        sprintf(test_name, "Running test #%d: %s", i + 1, item.name);
        nc_mono_doubled_fb_mvaddstr(0, 0, test_name);
        nc_mono_doubled_fb_refresh();
        free(test_name);
        
        usleep(1000 * 1000); // 1s
        item.func(fb);
        usleep(1000 * item.delay);
    }

    destroy_fb(fb);

    printf("All tests done!\n");

    return 0;
}