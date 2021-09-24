/*
 * signal_generator.c
 *
 *  Created on: Sep 24, 2021
 *      Author: Brian
 */


#include "signal_generator.h"

void start_signgen(){
	sys_status_refresh();
	lv_obj_align(sys_status, LV_ALIGN_TOP_LEFT, 0, 0);

    btn = lv_btn_create(app_scr);
    lv_obj_set_size(btn, 480, 40);
    lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_event_cb(btn, reset_app, LV_EVENT_CLICKED, NULL);
    label = lv_label_create(btn);
    lv_label_set_text(label, "Exit");
    lv_obj_center(label);

    cont = lv_obj_create(app_scr);
    lv_obj_set_size(cont, 240, 280);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    char opts1[200];
    sprintf(opts1, "");
    for (uint8_t i = 0; i < sizeof(waveform_list) / sizeof(waveform_list[0]); i++)
    {
        sprintf(opts1 + strlen(opts1), "%s", waveform_list[i]);
        if (i + 1 < sizeof(waveform_list) / sizeof(waveform_list[0])) sprintf(opts1 + strlen(opts1), "\n");
    }
    roller_wave1 = lv_roller_create(cont);
    lv_roller_set_options(roller_wave1, opts1, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller_wave1, 2);
    lv_obj_set_width(roller_wave1, 150);
    lv_obj_set_style_text_align(roller_wave1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(roller_wave1, LV_ALIGN_TOP_MID, 0, 5);
    lv_roller_set_selected(roller_wave1, waveform[0], LV_ANIM_OFF);
    lv_obj_add_event_cb(roller_wave1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    slider_frq1 = lv_slider_create(cont);
    lv_obj_set_width(slider_frq1, 180);
    lv_obj_align(slider_frq1, LV_ALIGN_TOP_MID, 0, 100);
    lv_slider_set_range(slider_frq1, 0, 20000);
    lv_slider_set_value(slider_frq1, frequency[0], LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_frq1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    label_frq1 = lv_label_create(cont);
    lv_label_set_text_fmt(label_frq1, "%dHz", frequency[0]);
    lv_obj_align_to(label_frq1, slider_frq1, LV_ALIGN_OUT_TOP_MID, 0, 0);

    slider_phs1 = lv_slider_create(cont);
    lv_obj_set_width(slider_phs1, 180);
    lv_obj_align(slider_phs1, LV_ALIGN_TOP_MID, 0, 150);
    lv_slider_set_range(slider_phs1, 0, 360);
    lv_slider_set_value(slider_phs1, phase[0], LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_phs1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    label_phs1 = lv_label_create(cont);
    lv_label_set_text_fmt(label_phs1, "%d°", phase[0]);
    lv_obj_align_to(label_phs1, slider_phs1, LV_ALIGN_OUT_TOP_MID, 0, 0);

    slider_amp1 = lv_slider_create(cont);
    lv_obj_set_width(slider_amp1, 180);
    lv_obj_align(slider_amp1, LV_ALIGN_TOP_MID, 0, 200);
    lv_slider_set_range(slider_amp1, 0, 1000);
    lv_slider_set_value(slider_amp1, amplitude[0], LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_amp1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    label_amp1 = lv_label_create(cont);
    lv_label_set_text_fmt(label_amp1, "%dmV", amplitude[0]);
    lv_obj_align_to(label_amp1, slider_amp1, LV_ALIGN_OUT_TOP_MID, 0, 0);

}

void event_cb(lv_event_t* e){
	if (lv_event_get_target(e) == slider_frq1){
		frequency[0] = lv_slider_get_value(lv_event_get_target(e));
		lv_label_set_text_fmt(label_frq1, "%dHz", frequency[0]);
	}
	if (lv_event_get_target(e) == slider_phs1){
		phase[0] = lv_slider_get_value(lv_event_get_target(e));
		lv_label_set_text_fmt(label_phs1, "%d°", phase[0]);
	}
	if (lv_event_get_target(e) == slider_amp1){
		amplitude[0] = lv_slider_get_value(lv_event_get_target(e));
		lv_label_set_text_fmt(label_amp1, "%dmV", amplitude[0]);
	}
}
