/**
 * @file gui.h
 *
 */

#ifndef GUI_H
#define GUI_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void reset_scr();
void reset_app();

void start_main();
void start_dsp();
void start_oscilloscope();
void start_synthesizer();
void start_fft();

void start_app(lv_event_t* e);

void dsp_menu_cb(lv_event_t* e);
void dsp_new_project();
void dsp_open_project();
void dsp_add_fx(lv_event_t* e);
void dsp_change_fx(lv_event_t* e);
void dsp_refresh_fx_slider(lv_event_t* e);
void dsp_refresh_fx_list(lv_event_t* e);
void dsp_open_edit(lv_event_t* e);
void dsp_close_edit();
void dsp_filter_response_refresh(uint8_t type, uint8_t order, float frq1, float frq2);

void sys_status_refresh();



/**********************
 *      MACROS        *
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*GUI_H*/
