#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include <curses.h>

/**
 * @typedef RGBA color type
 */
typedef uint32_t color_t;
typedef int16_t coord_t;
typedef uint8_t mono_symbol_t;

/**
 * @brief Describes monochrome fb symbol id.
 *
 * Numbers in item name describe will be
 * angle at X position (LT, RT, LB, RB) filled or not.
 */
enum mono_doubled_fb_item_id {
    /// [ ] symbol (space)
    MONO_SYMBOL_0000,
    
    /// [▗] symbol
    MONO_SYMBOL_0001,
    
    /// [▖] symbol
    MONO_SYMBOL_0010,
    
    /// [▄] symbol
    MONO_SYMBOL_0011,
    
    /// [▝] symbol
    MONO_SYMBOL_0100,
    
    /// [▐] symbol
    MONO_SYMBOL_0101,
    
    /// [▞] symbol
    MONO_SYMBOL_0110,
    
    /// [▟] symbol
    MONO_SYMBOL_0111,
    
    /// [▘] symbol
    MONO_SYMBOL_1000,
    
    /// [▚] symbol
    MONO_SYMBOL_1001,
    
    /// [▌] symbol
    MONO_SYMBOL_1010,
    
    /// [▙] symbol
    MONO_SYMBOL_1011,
    
    /// [▀] symbol
    MONO_SYMBOL_1100,
    
    /// [▜] symbol
    MONO_SYMBOL_1101,
    
    /// [▛] symbol
    MONO_SYMBOL_1110,
    
    /// [█] symbol
    MONO_SYMBOL_1111,
    
    /// Max symbols count
    MONO_SYMBOL_MAX,
};

/**
 * @brief Framebuffer types
 */
typedef enum __fb_type {
    /// Symbol framebuffer
    SYMBOL_FRAMEBUFFER,

    /// Monochrome framebuffer with doubled resolution
    MONO_DOUBLED_FRAMEBUFFER,
} fb_type_t;

/* Symbols described in mono_doubled_fb_item_id */
static cchar_t mono_doubled_fb_item_symbol[MONO_SYMBOL_MAX] = {
    [MONO_SYMBOL_0000] = {
        .chars = U' ', // Space symbol
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_0001] = {
        .chars = U'▗',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_0010] = {
        .chars = U'▖',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_0011] = {
        .chars = U'▄',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_0100] = {
        .chars = U'▝',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_0101] = {
        .chars = U'▐',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_0110] = {
        .chars = U'▞',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_0111] = {
        .chars = U'▟',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_1000] = {
        .chars = U'▘',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_1001] = {
        .chars = U'▚',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_1010] = {
        .chars = U'▌',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_1011] = {
        .chars = U'▙',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_1100] = {
        .chars = U'▀',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_1101] = {
        .chars = U'▜',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_1110] = {
        .chars = U'▛',
        .attr = A_NORMAL,
    },
    [MONO_SYMBOL_1111] = {
        .chars = U'█',
        .attr = A_NORMAL,
    },
};

/**
 * @brief Symbol-framebuffer's item (symbol) definition
 */
typedef struct __symbol_fb_item {
    /// ASCII symbol of item
    char symbol;

    /// Symbol color
    color_t fg;

    /// Background color
    color_t bg;
} symbol_fb_item_t;

/**
 * @brief Framebuffer size
 */
typedef struct __fb_size {
    /// Size by X
    coord_t x;

    /// Size by Y
    coord_t y;
} fb_size_t;

typedef fb_size_t fb_pos_t;

/**
 * @brief Framebuffer made of symbols with different fg and bg colors
 */
struct cli_symbol_fb {
    /** Should be equal to #SYMBOL_FRAMEBUFFER */
    fb_type_t type;
    
    /** Size of framebuffer, doesn't change dynamically */
    fb_size_t size;

    /** Framebuffer items (symbols) */
    symbol_fb_item_t *items;
};

/**
 * @brief Monochrome framebuffer with doubled resolution
 *        because of cheaty UTF-8 symbols.
 */
struct cli_mono_doubled_fb {
    /** Should be equal to #MONO_DOUBLED_FRAMEBUFFER */
    fb_type_t type;
    
    /** Size of framebuffer, doesn't change dynamically */
    fb_size_t size;

    /** Framebuffer symbols (0 = nothing, 1 = filled) */
    bool *items;

    /** ncurses window */
    WINDOW *window;
};

struct __cli_common_fb_data {
    fb_type_t type;
    
    /** Size of framebuffer, doesn't change dynamically */
    fb_size_t size;
};

/*
 * Mono-doubled framebuffer functions
 */

/**
 * @brief Init cli_mono_doubled_fb struct
 * @return NULL on failure, otherwise a pointer to cli_mono_doubled_fb
 */
struct cli_mono_doubled_fb *init_mono_doubled_fb();

/**
 * @brief Destroy cli_mono_doubled_fb struct
 * 
 * @param fb Pointer to cli_mono_doubled_fb
 */
void destroy_mono_doubled_fb(struct cli_mono_doubled_fb *fb);

/**
 * @brief Refresh cli_mono_doubled_fb struct contents on screen
 * 
 * @param fb Pointer to cli_mono_doubled_fb
 */
void refresh_mono_doubled_fb(struct cli_mono_doubled_fb *fb);

void nc_mono_doubled_fb_refresh(void);
void nc_mono_doubled_fb_mvadd_wch(int y, int x, cchar_t *c);
void nc_mono_doubled_fb_mvaddstr(int y, int x, char *str);

/*
 * Common functions
 */

/**
 * @brief Init framebuffer object,
 *        also inits ncurses screen.
 * 
 * @param[in] type 
 * Type of framebuffer
 * 
 * @param[out] fb 
 * Here will be set pointer to the framebuffer
 * 
 * @return 0 on success
 */
int init_fb(fb_type_t type, void **fb);

/**
 * @brief Destroy framebuffer object,
 *        also destroys ncurses screen.
 * 
 * @param[in] fb 
 * Framebuffer object to destroy
 */
void destroy_fb(void *fb);

/**
 * @brief Refresh screen with framebuffer object contents
 * 
 * @param[in, out] fb 
 * Framebuffer object to refresh
 */
void refresh_fb(void *fb);

#define GET_MONO_FB_PIXEL(fb, uy, ux) \
    (*((fb)->items + ((uy) * (fb)->size.x) + (ux)))

#define SET_MONO_FB_PIXEL(fb, uy, ux, value) \
    { \
        *((fb)->items + ((uy) * (fb)->size.x) + (ux)) = (value); \
    }
