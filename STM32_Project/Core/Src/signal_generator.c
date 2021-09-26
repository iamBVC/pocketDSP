/*
 * signal_generator.c
 *
 *  Created on: Sep 24, 2021
 *      Author: Brian
 */


#include "signal_generator.h"

void start_signgen(){

    for (uint8_t i = 0; i < 2; i++)
        for (uint8_t j = 0; j < 4; j++)
        	sg_settings[i][j] = 0;

    char opts[200];
    sprintf(opts, "");
    for (uint8_t i = 0; i < sizeof(waveform_list) / sizeof(waveform_list[0]); i++)
    {
        sprintf(opts + strlen(opts), "%s", waveform_list[i]);
        if (i + 1 < sizeof(waveform_list) / sizeof(waveform_list[0])) sprintf(opts + strlen(opts), "\n");
    }

	sys_status_refresh();
	lv_obj_align(sys_status, LV_ALIGN_TOP_LEFT, 0, 0);

    btn = lv_btn_create(app_scr);
    lv_obj_set_size(btn, 200, 40);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_event_cb(btn, reset_app, LV_EVENT_CLICKED, NULL);
    label = lv_label_create(btn);
    lv_label_set_text(label, "Exit");
    lv_obj_center(label);


	//Left Channel
    cont = lv_obj_create(app_scr);
    lv_obj_set_size(cont, 240, 280);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    roller_wave1 = lv_roller_create(cont);
    lv_roller_set_options(roller_wave1, opts, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller_wave1, 2);
    lv_obj_set_width(roller_wave1, 150);
    lv_obj_set_style_text_align(roller_wave1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(roller_wave1, LV_ALIGN_TOP_MID, 0, 5);
    lv_roller_set_selected(roller_wave1, sg_settings[0][3], LV_ANIM_OFF);
    lv_obj_add_event_cb(roller_wave1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    slider_frq1 = lv_slider_create(cont);
    lv_obj_set_width(slider_frq1, 180);
    lv_obj_align(slider_frq1, LV_ALIGN_TOP_MID, 0, 100);
    lv_slider_set_range(slider_frq1, 0, 20000);
    lv_slider_set_value(slider_frq1, sg_settings[0][0], LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_frq1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    label_frq1 = lv_label_create(cont);
    lv_label_set_text_fmt(label_frq1, "%dHz", sg_settings[0][0]);
    lv_obj_align_to(label_frq1, slider_frq1, LV_ALIGN_OUT_TOP_MID, 0, 0);

    slider_phs1 = lv_slider_create(cont);
    lv_obj_set_width(slider_phs1, 180);
    lv_obj_align(slider_phs1, LV_ALIGN_TOP_MID, 0, 150);
    lv_slider_set_range(slider_phs1, 0, 360);
    lv_slider_set_value(slider_phs1, sg_settings[0][1], LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_phs1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    label_phs1 = lv_label_create(cont);
    lv_label_set_text_fmt(label_phs1, "%d°", sg_settings[0][1]);
    lv_obj_align_to(label_phs1, slider_phs1, LV_ALIGN_OUT_TOP_MID, 0, 0);

    slider_amp1 = lv_slider_create(cont);
    lv_obj_set_width(slider_amp1, 180);
    lv_obj_align(slider_amp1, LV_ALIGN_TOP_MID, 0, 200);
    lv_slider_set_range(slider_amp1, 0, 1000);
    lv_slider_set_value(slider_amp1, sg_settings[0][2], LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_amp1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    label_amp1 = lv_label_create(cont);
    lv_label_set_text_fmt(label_amp1, "%dmV", sg_settings[0][2]);
    lv_obj_align_to(label_amp1, slider_amp1, LV_ALIGN_OUT_TOP_MID, 0, 0);
	
	
	//Right Channel
    cont = lv_obj_create(app_scr);
    lv_obj_set_size(cont, 240, 280);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    roller_wave2 = lv_roller_create(cont);
    lv_roller_set_options(roller_wave2, opts, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_visible_row_count(roller_wave2, 2);
    lv_obj_set_width(roller_wave2, 150);
    lv_obj_set_style_text_align(roller_wave2, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(roller_wave2, LV_ALIGN_TOP_MID, 0, 5);
    lv_roller_set_selected(roller_wave2, sg_settings[1][3], LV_ANIM_OFF);
    lv_obj_add_event_cb(roller_wave2, event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    slider_frq2 = lv_slider_create(cont);
    lv_obj_set_width(slider_frq2, 180);
    lv_obj_align(slider_frq2, LV_ALIGN_TOP_MID, 0, 100);
    lv_slider_set_range(slider_frq2, 0, 20000);
    lv_slider_set_value(slider_frq2, sg_settings[1][0], LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_frq2, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    label_frq2 = lv_label_create(cont);
    lv_label_set_text_fmt(label_frq2, "%dHz", sg_settings[1][0]);
    lv_obj_align_to(label_frq2, slider_frq2, LV_ALIGN_OUT_TOP_MID, 0, 0);

    slider_phs2 = lv_slider_create(cont);
    lv_obj_set_width(slider_phs2, 180);
    lv_obj_align(slider_phs2, LV_ALIGN_TOP_MID, 0, 150);
    lv_slider_set_range(slider_phs2, 0, 360);
    lv_slider_set_value(slider_phs2, sg_settings[1][1], LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_phs2, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    label_phs2 = lv_label_create(cont);
    lv_label_set_text_fmt(label_phs2, "%d°", sg_settings[1][1]);
    lv_obj_align_to(label_phs2, slider_phs2, LV_ALIGN_OUT_TOP_MID, 0, 0);

    slider_amp2 = lv_slider_create(cont);
    lv_obj_set_width(slider_amp2, 180);
    lv_obj_align(slider_amp2, LV_ALIGN_TOP_MID, 0, 200);
    lv_slider_set_range(slider_amp2, 0, 1000);
    lv_slider_set_value(slider_amp2, sg_settings[1][2], LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_amp2, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    label_amp2 = lv_label_create(cont);
    lv_label_set_text_fmt(label_amp2, "%dmV", sg_settings[1][2]);
    lv_obj_align_to(label_amp2, slider_amp2, LV_ALIGN_OUT_TOP_MID, 0, 0);

}

void event_cb(lv_event_t* e){
	if (lv_event_get_target(e) == slider_frq1){
		sg_settings[0][0] = lv_slider_get_value(lv_event_get_target(e));
		lv_label_set_text_fmt(label_frq1, "%dHz", sg_settings[0][0]);
	}
	if (lv_event_get_target(e) == slider_phs1){
		sg_settings[0][1] = lv_slider_get_value(lv_event_get_target(e));
		lv_label_set_text_fmt(label_phs1, "%d°", sg_settings[0][1]);
	}
	if (lv_event_get_target(e) == slider_amp1){
		sg_settings[0][2] = lv_slider_get_value(lv_event_get_target(e));
		lv_label_set_text_fmt(label_amp1, "%dmV", sg_settings[0][2]);
	}
		if (lv_event_get_target(e) == roller_wave1){
		sg_settings[0][3] = lv_roller_get_selected(lv_event_get_target(e));
	}
	
	if (lv_event_get_target(e) == slider_frq2){
		sg_settings[1][0] = lv_slider_get_value(lv_event_get_target(e));
		lv_label_set_text_fmt(label_frq2, "%dHz", sg_settings[1][0]);
	}
	if (lv_event_get_target(e) == slider_phs2){
		sg_settings[1][1] = lv_slider_get_value(lv_event_get_target(e));
		lv_label_set_text_fmt(label_phs2, "%d°", sg_settings[1][1]);
	}
	if (lv_event_get_target(e) == slider_amp2){
		sg_settings[1][2] = lv_slider_get_value(lv_event_get_target(e));
		lv_label_set_text_fmt(label_amp2, "%dmV", sg_settings[1][2]);
	}
		if (lv_event_get_target(e) == roller_wave2){
		sg_settings[1][3] = lv_roller_get_selected(lv_event_get_target(e));
	}
}
