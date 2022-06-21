#include <ncurses.h>
#include <clifb.h>
#include <stdio.h>

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
int init_fb(fb_type_t type, void **fb) {
    switch (type) {
        case MONO_DOUBLED_FRAMEBUFFER:
            *fb = (void *)init_mono_doubled_fb();
            return 0;
        default:
            *fb = NULL;
            fprintf(stderr, "%s: Invalid framebuffer type: %d\n", __func__, type);
            return -1;
    }
}

/**
 * @brief Destroy framebuffer object,
 *        also destroys ncurses screen.
 * 
 * @param[in] fb 
 * Framebuffer object to destroy
 */
void destroy_fb(void *fb) {
    struct __cli_common_fb_data *common = (struct __cli_common_fb_data *)fb;

    switch (common->type) {
        case MONO_DOUBLED_FRAMEBUFFER:
            destroy_mono_doubled_fb((struct cli_mono_doubled_fb *)fb);
            return;
        default:
            fprintf(stderr, "%s: Invalid framebuffer type: %d\n", __func__, common->type);
            return;
    }
}

/**
 * @brief Refresh screen with framebuffer object contents
 * 
 * @param[in, out] fb 
 * Framebuffer object to refresh
 */
void refresh_fb(void *fb) {
    struct __cli_common_fb_data *common = (struct __cli_common_fb_data *)fb;

    switch (common->type) {
        case MONO_DOUBLED_FRAMEBUFFER:
            refresh_mono_doubled_fb((struct cli_mono_doubled_fb *)fb);
            return;
        default:
            fprintf(stderr, "%s: Invalid framebuffer type: %d\n", __func__, common->type);
            return;
    }
}
