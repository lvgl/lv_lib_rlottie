/**
 * @file lv_rlottie.c
 *
 */

 /*********************
  *      INCLUDES
  *********************/

#include "lv_rlottie.h"
#include <rlottie_capi.h>


 /*********************
  *      DEFINES
  *********************/
#define LV_OBJX_NAME "lv_rlottie"

   /**********************
    *      TYPEDEFS
    **********************/
typedef struct {
    lv_img_ext_t img_ext;
    Lottie_Animation* animation;
    lv_task_t* task;
    lv_img_dsc_t imgdsc;
    size_t total_frames;
    size_t current_frame;
    size_t framerate;
    uint32_t* allocated_buf;
    size_t allocated_buffer_size;
    size_t scanline_width;
}lv_rlottie_ext_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void next_frame_task_cb(lv_task_t* t);
static lv_res_t lv_rlottie_signal(lv_obj_t* btn, lv_signal_t sign, void* param);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_signal_cb_t ancestor_signal;

/**********************
 *      MACROS
 **********************/

 /**********************
  *   GLOBAL FUNCTIONS
  **********************/

static void common_rlottie_setup(lv_rlottie_ext_t* ext,lv_obj_t* image,lv_obj_t* parent, lv_coord_t width, lv_coord_t height)
{
    ext->total_frames = lottie_animation_get_totalframe(ext->animation);
    ext->framerate = lottie_animation_get_framerate(ext->animation);
    ext->current_frame = 0;

    lv_coord_t obj_width = LV_MATH_MIN(width, lv_obj_get_width(parent));
    lv_coord_t obj_height = LV_MATH_MIN(height, lv_obj_get_height(parent));

    ext->scanline_width = obj_width * LV_COLOR_DEPTH / 8;

    size_t allocaled_buf_size = (obj_width * obj_height * LV_COLOR_DEPTH / 8);
    ext->allocated_buf = lv_mem_alloc(allocaled_buf_size);
    if (ext->allocated_buf != NULL)
    {
        ext->allocated_buffer_size = allocaled_buf_size;
        memset(ext->allocated_buf, 0, allocaled_buf_size);
    }

    ext->imgdsc.header.always_zero = 0;
    ext->imgdsc.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
    ext->imgdsc.header.h = obj_height;
    ext->imgdsc.header.w = obj_width;
    ext->imgdsc.data = ext->allocated_buf;
    ext->imgdsc.data_size = allocaled_buf_size;

    lv_img_set_src(image, &ext->imgdsc);

    ext->task = lv_task_create(next_frame_task_cb, 10, LV_TASK_PRIO_HIGH, image);
}

lv_obj_t* lv_rlottie_create_from_file(lv_obj_t* parent, lv_coord_t width, lv_coord_t height, const char* path)
{

    lv_obj_t* img = lv_img_create(parent, NULL);
    lv_rlottie_ext_t* ext = lv_obj_allocate_ext_attr(img, sizeof(lv_rlottie_ext_t));
    LV_ASSERT_MEM(ext);

    if (ancestor_signal == NULL) ancestor_signal = lv_obj_get_signal_cb(img);
    lv_obj_set_signal_cb(img, lv_rlottie_signal);

    ext->animation = lottie_animation_from_file(path);
    if (ext->animation == NULL) return img;

    common_rlottie_setup(ext, img, parent, width, height);

    return img;
}

lv_obj_t* lv_rlottie_create_from_raw(lv_obj_t* parent, lv_coord_t width, lv_coord_t height, const char* rlottie_desc)
{
    lv_obj_t* img = lv_img_create(parent, NULL);
    lv_rlottie_ext_t* ext = lv_obj_allocate_ext_attr(img, sizeof(lv_rlottie_ext_t));
    LV_ASSERT_MEM(ext);

    if (ancestor_signal == NULL) ancestor_signal = lv_obj_get_signal_cb(img);
    lv_obj_set_signal_cb(img, lv_rlottie_signal);

    ext->animation = lottie_animation_from_data(rlottie_desc, rlottie_desc,"");
    if (ext->animation == NULL) return img;

    common_rlottie_setup(ext, img, parent, width, height);

    return img;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void next_frame_task_cb(lv_task_t* t)
{
    static uint32_t counter = 0;
    if (counter > 0) {
        counter--;
        return;
    }

    lv_obj_t* img = t->user_data;
    lv_rlottie_ext_t* ext = lv_obj_get_ext_attr(img);
    if (ext->current_frame == ext->total_frames)
        ext->current_frame = 0;
    else
        ++ext->current_frame;
    
    lottie_animation_render(
        ext->animation,
        ext->current_frame,
        ext->allocated_buf,
        ext->imgdsc.header.w,
        ext->imgdsc.header.h,
        ext->scanline_width
    );

    lv_event_send(img, LV_EVENT_LEAVE, NULL);
    lv_obj_invalidate(img);
}

/**
 * Signal function of the image
 * @param btn pointer to a button object
 * @param sign a signal type from lv_signal_t enum
 * @param param pointer to a signal specific variable
 * @return LV_RES_OK: the object is not deleted in the function; LV_RES_INV: the object is deleted
 */
static lv_res_t lv_rlottie_signal(lv_obj_t* img, lv_signal_t sign, void* param)
{
    lv_res_t res;

    /* Include the ancient signal function */
    res = ancestor_signal(img, sign, param);
    if (res != LV_RES_OK) return res;
    if (sign == LV_SIGNAL_GET_TYPE) return lv_obj_handle_get_type_signal(param, LV_OBJX_NAME);

    if (sign == LV_SIGNAL_CLEANUP) {
        lv_rlottie_ext_t* ext = lv_obj_get_ext_attr(img);
        if (ext->animation) {
            lottie_animation_destroy(ext->animation);
            ext->animation = 0;
            ext->current_frame = 0;
            ext->framerate = 0;
            ext->scanline_width = 0;
            ext->total_frames = 0;
        }

        if (ext->task){
            lv_task_del(ext->task);
            ext->task = NULL;
        }

        if (ext->allocated_buf) {
            lv_mem_free(ext->allocated_buf);
            ext->allocated_buf = NULL;
            ext->allocated_buffer_size = 0;
        }
            
    }

    return LV_RES_OK;
}
