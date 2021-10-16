/*
 * dsp.c
 *
 *  Created on: 3 set 2021
 *      Author: Brian
 */


#include "system.h"

static Distortion dist __attribute__ ((section(".RAMD2"),used));
static Phaser phs __attribute__ ((section(".RAMD2"),used));

static FIRFilter fir __attribute__ ((section(".RAMD2"),used));
static Delay dly __attribute__ ((section(".RAMD2"),used));
static Reverb rvb __attribute__ ((section(".RAMD2"),used));

void start_dsp()
{
	dsp_fx_init();
	sample_callback = &DSP_sample_callback;

    sys_status_refresh();
    lv_obj_align(sys_status, LV_ALIGN_TOP_LEFT, 0, 0);

    label = lv_label_create(app_scr);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 5, 45);
    lv_label_set_text_fmt(label, LV_SYMBOL_LEFT "INPUT");

    label = lv_label_create(app_scr);
    lv_obj_align(label, LV_ALIGN_TOP_RIGHT, -5, 45);
    lv_label_set_text_fmt(label, "OUTPUT" LV_SYMBOL_RIGHT);

    dsp_main_cont = lv_obj_create(app_scr);
    lv_obj_set_size(dsp_main_cont, 480, 200);
    lv_obj_set_scroll_snap_x(dsp_main_cont, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(dsp_main_cont, LV_FLEX_FLOW_ROW);
    lv_obj_align(dsp_main_cont, LV_ALIGN_CENTER, 0, 0);

    for (uint8_t i = 0; i < sizeof(fx_list)/sizeof(fx_list[0]); i++)
    {
        btn = lv_btn_create(dsp_main_cont);
        lv_obj_set_size(btn, 150, 150);
        lv_obj_add_event_cb(btn, dsp_open_edit, LV_EVENT_CLICKED, i);
        label = lv_label_create(btn);
        lv_label_set_text_fmt(label, "%s", fx_list[i]);
        lv_obj_center(label);
    }

    menu = lv_dropdown_create(app_scr);
    lv_obj_set_size(menu, 140, 40);
    lv_obj_align(menu, LV_ALIGN_TOP_RIGHT, 0, 0);
    char buf[200];
    sprintf(buf, "");
    for (uint8_t i = 0; i < sizeof(dsp_main_menu) / sizeof(dsp_main_menu[0]); i++)
    {
        sprintf(buf + strlen(buf), "%s", dsp_main_menu[i]);
        if (i + 1 < sizeof(dsp_main_menu) / sizeof(dsp_main_menu[0])) sprintf(buf + strlen(buf), "\n");
    }
    lv_dropdown_set_options(menu, buf);
    lv_dropdown_set_text(menu, "Menu");
    lv_obj_set_style_text_align(menu, LV_TEXT_ALIGN_LEFT, 0);
    LV_IMG_DECLARE(img_caret_down) lv_dropdown_set_symbol(menu, &img_caret_down);
    lv_obj_set_style_transform_angle(menu, 1800, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_dropdown_set_selected_highlight(menu, false);
    lv_obj_add_event_cb(menu, dsp_menu_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_set_style_bg_color(app_scr, lv_color_make(220, 220, 255), LV_PART_MAIN);
}

static void dsp_menu_cb(lv_event_t* e)
{
    uint16_t selected = lv_dropdown_get_selected(lv_event_get_target(e));
    if (selected == 0)
    {
        static const char* btns[] = { "Delete", ""};
        mbox = lv_msgbox_create(app_scr, "Warning", "Delete the current project?", btns, true);
        lv_obj_add_event_cb(mbox, dsp_new_project, LV_EVENT_VALUE_CHANGED, NULL);
        lv_obj_center(mbox);
    }
    if (selected == 1) dsp_open_project();
    if (selected == 2);
    if (selected == 3);
    if (selected == 4){
    	reset_dsp();
    	reset_app();
    }
}

static void reset_dsp(){
    for (uint8_t i = 0; i < DSP_MAX_FX_COUNT; i++)
        for (uint8_t j = 0; j <= DSP_MAX_FX_SETTINGS; j++)
            dsp_fx_settings[i][j] = 0;
}

static void dsp_new_project()
{
	reset_dsp();
	reset_scr();
    start_dsp();
}

static void dsp_open_project()
{
    /*
    dsp_fx_count = 1;
    dsp_fx_settings[0][0] = 2;
    dsp_fx_settings[0][1] = 10;
    dsp_fx_settings[0][2] = 50;
    */
    reset_scr();
    start_dsp();
}

static void dsp_open_edit(lv_event_t* e)
{

    uint8_t id = lv_event_get_user_data(e);
    dsp_fx_setup(id);
    uint8_t fx = 0;

    if (lv_obj_is_valid(dsp_fx_scr)) lv_obj_del_async(dsp_fx_scr);
    dsp_fx_scr = lv_obj_create(NULL);
    lv_obj_set_size(dsp_fx_scr, 480, 320);
    lv_scr_load(dsp_fx_scr);

    btn = lv_btn_create(dsp_fx_scr);
    lv_obj_set_size(btn, 50, 50);
    lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_event_cb(btn, dsp_close_edit, LV_EVENT_CLICKED, NULL);
    label = lv_label_create(btn);
    lv_label_set_text(label, "Close");
    lv_obj_center(label);

    label = lv_label_create(dsp_fx_scr);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 15);
    char buf1[20];
    lv_snprintf(buf1, sizeof(buf1), "%s", fx_list[id]);
    lv_label_set_text(label, buf1);


    if (fx_list[id] == "Distortion") //Distortion
        {
            label = lv_label_create(dsp_fx_scr);
            lv_label_set_text(label, "InGain");
            lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, -80);
            slider = lv_slider_create(dsp_fx_scr);
            lv_obj_set_width(slider, 200);
            lv_obj_align(slider, LV_ALIGN_LEFT_MID, 100, -80);
            lv_slider_set_range(slider, 0, 1000);
            lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
            lv_obj_set_user_data(slider, fx);
            lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
            label = lv_label_create(dsp_fx_scr);
            lv_label_set_text_fmt(label, "%.2f", dsp_fx_settings[id][fx++]/100.0);
            lv_obj_align_to(label, slider, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

            label = lv_label_create(dsp_fx_scr);
            lv_label_set_text(label, "Threshold");
            lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, -40);
            slider = lv_slider_create(dsp_fx_scr);
            lv_obj_set_width(slider, 200);
            lv_obj_align(slider, LV_ALIGN_LEFT_MID, 100, -40);
            lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
            lv_obj_set_user_data(slider, fx);
            lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
            label = lv_label_create(dsp_fx_scr);
            lv_label_set_text_fmt(label, "%.2f", dsp_fx_settings[id][fx++]/100.0);
            lv_obj_align_to(label, slider, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

            label = lv_label_create(dsp_fx_scr);
            lv_label_set_text(label, "OutGain");
            lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, 0);
            slider = lv_slider_create(dsp_fx_scr);
            lv_obj_set_width(slider, 200);
            lv_obj_align(slider, LV_ALIGN_LEFT_MID, 100, 0);
            lv_slider_set_range(slider, 0, 1000);
            lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
            lv_obj_set_user_data(slider, fx);
            lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
            label = lv_label_create(dsp_fx_scr);
            lv_label_set_text_fmt(label, "%.2f", dsp_fx_settings[id][fx++]/100.0);
            lv_obj_align_to(label, slider, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
        }


    if (fx_list[id] == "Reeverb") //Reeverb
    {
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "Decay");
        lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, -80);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 200);
        lv_obj_align(slider, LV_ALIGN_LEFT_MID, 100, -80);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text_fmt(label, "%.2f", dsp_fx_settings[id][fx++]/100.0);
        lv_obj_align_to(label, slider, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "Size");
        lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, -40);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 200);
        lv_obj_align(slider, LV_ALIGN_LEFT_MID, 100, -40);
        lv_slider_set_range(slider, 0, 5000);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text_fmt(label, "%d Samples", dsp_fx_settings[id][fx++]);
        lv_obj_align_to(label, slider, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "Wet");
        lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, 0);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 200);
        lv_obj_align(slider, LV_ALIGN_LEFT_MID, 100, 0);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text_fmt(label, "%d%%", dsp_fx_settings[id][fx++]);
        lv_obj_align_to(label, slider, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "Dry");
        lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, 40);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 200);
        lv_obj_align(slider, LV_ALIGN_LEFT_MID, 100, 40);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text_fmt(label, "%d%%", dsp_fx_settings[id][fx++]);
        lv_obj_align_to(label, slider, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    }

    if (fx_list[id] == "Delay") //Delay
    {
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "Delay");
        lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, -80);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 200);
        lv_obj_align(slider, LV_ALIGN_LEFT_MID, 100, -80);
        lv_slider_set_range(slider, 0, 24000);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text_fmt(label, "%d Samples", dsp_fx_settings[id][fx++]);
        lv_obj_align_to(label, slider, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "Decay");
        lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, -40);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 200);
        lv_obj_align(slider, LV_ALIGN_LEFT_MID, 100, -40);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text_fmt(label, "%.2f", dsp_fx_settings[id][fx++]/100.0);
        lv_obj_align_to(label, slider, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "Mix");
        lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, 0);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 200);
        lv_obj_align(slider, LV_ALIGN_LEFT_MID, 100, 0);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text_fmt(label, "%d%%", dsp_fx_settings[id][fx++]);
        lv_obj_align_to(label, slider, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    }

    if (fx_list[id] == "Sidechain") //Sidechain
    {
        uint8_t type = dsp_fx_settings[id][1];
        uint8_t width = dsp_fx_settings[id][2];
        uint8_t slope = dsp_fx_settings[id][3];

        chart = lv_chart_create(dsp_fx_scr);
        lv_obj_set_size(chart, 200, 150);
        lv_obj_align(chart, LV_ALIGN_LEFT_MID, 50, 50);
        lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
        lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 50);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 100);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
        ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
        lv_chart_set_point_count(chart, 100);
        dsp_sidechain_response_refresh(type, width, slope);
        lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)dsp_sidechain_response);


        const char* opts1 =
            "Type 0\n"
            "Type 1\n"
            "Type 2\n"
            "Type 3";
        list = lv_dropdown_create(dsp_fx_scr);
        lv_dropdown_set_options_static(list, opts1);
        lv_obj_set_size(list, 110, 40);
        lv_obj_align_to(list, chart, LV_ALIGN_OUT_TOP_RIGHT, 0, 0);
        lv_dropdown_set_selected(list, dsp_fx_settings[id][fx]);
        lv_obj_set_user_data(list, fx++);
        lv_obj_add_event_cb(list, dsp_refresh_fx_list, LV_EVENT_VALUE_CHANGED, id);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "Width");
        lv_obj_align_to(label, chart, LV_ALIGN_OUT_RIGHT_MID, 10, -40);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 140);
        lv_obj_align_to(slider, label, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx++);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "Slope");
        lv_obj_align_to(label, chart, LV_ALIGN_OUT_RIGHT_MID, 10, 40);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 140);
        lv_obj_align_to(slider, label, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx++);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);

    }


    if (fx_list[id] == "FIR Filter") //FIR Filter
    {
        chart = lv_chart_create(dsp_fx_scr);
        lv_obj_set_size(chart, 200, 150);
        lv_obj_align(chart, LV_ALIGN_LEFT_MID, 50, 50);
        lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 20000);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
        lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 21, 1, false, 50);
        lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 50);
        ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
        lv_chart_set_point_count(chart, dsp_fx_settings[id][4]);
        for (uint16_t n = 0; n < dsp_fx_settings[id][4]; n++) dsp_filter_response[n] = fir.FIR_FREQUENCY_RESPONSE[n];
        lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)dsp_filter_response);

        const char* opts1 =
            "Low Pass\n"
            "High Pass\n"
            "Band Pass\n"
            "Band Stop";
        list = lv_dropdown_create(dsp_fx_scr);
        lv_dropdown_set_options_static(list, opts1);
        lv_obj_set_size(list, 110, 40);
        lv_obj_align_to(list, chart, LV_ALIGN_OUT_TOP_RIGHT, 0, -15);
        lv_dropdown_set_selected(list, dsp_fx_settings[id][fx]);
        lv_obj_set_user_data(list, fx++);
        lv_obj_add_event_cb(list, dsp_refresh_fx_list, LV_EVENT_VALUE_CHANGED, id);

        const char* opts2 =
            "Rectangular\n"
            "Triangular\n"
            "Welch\n"
            "Sine\n"
            "Hann\n"
            "Hamming\n"
            "Blackman\n"
            "Nuttall\n"
            "BlackmanNuttall\n"
            "BlackmanHarris\n"
            "FlatTop";

        roller = lv_roller_create(dsp_fx_scr);
        lv_roller_set_options(roller, opts2, LV_ROLLER_MODE_NORMAL);
        lv_roller_set_visible_row_count(roller, 2);
        lv_obj_set_width(roller, 90);
        lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
        lv_obj_align_to(roller, chart, LV_ALIGN_OUT_TOP_LEFT, 0, 0);
        lv_roller_set_selected(roller, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(roller, fx++);
        lv_obj_add_event_cb(roller, dsp_refresh_fx_roller, LV_EVENT_VALUE_CHANGED, id);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "LF");
        lv_obj_align_to(label, chart, LV_ALIGN_OUT_RIGHT_TOP, 10, 0);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 140);
        lv_slider_set_range(slider, 1, 20000);
        lv_obj_align_to(slider, label, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text_fmt(label, "%d Hz", dsp_fx_settings[id][fx++]);
        lv_obj_align_to(label, slider, LV_ALIGN_OUT_TOP_MID, 0, 0);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "HF");
        lv_obj_align_to(label, chart, LV_ALIGN_OUT_RIGHT_TOP, 10, 40);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 140);
        lv_slider_set_range(slider, 1, 20000);
        lv_obj_align_to(slider, label, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text_fmt(label, "%d Hz", dsp_fx_settings[id][fx++]);
        lv_obj_align_to(label, slider, LV_ALIGN_OUT_TOP_MID, 0, 0);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "Size");
        lv_obj_align_to(label, chart, LV_ALIGN_OUT_RIGHT_TOP, 10, 80);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 140);
        lv_slider_set_range(slider, 2, FIR_FILTER_LENGHT);
        lv_obj_align_to(slider, label, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text_fmt(label, "%d Samples", dsp_fx_settings[id][fx++]);
        lv_obj_align_to(label, slider, LV_ALIGN_OUT_TOP_MID, 0, 0);

    }


}


static void dsp_refresh_fx_slider(lv_event_t* e)
{
    lv_obj_t *target = lv_event_get_target(e);
    uint8_t id = lv_event_get_user_data(e);
    uint8_t fx = lv_obj_get_user_data(target);
    dsp_fx_settings[id][fx] = lv_slider_get_value(target);
    dsp_open_edit(e);
}
static void dsp_refresh_fx_list(lv_event_t* e)
{
    lv_obj_t* target = lv_event_get_target(e);
    uint8_t id = lv_event_get_user_data(e);
    uint8_t fx = lv_obj_get_user_data(target);
    dsp_fx_settings[id][fx] = lv_dropdown_get_selected(target);
    dsp_open_edit(e);
}

static void dsp_refresh_fx_roller(lv_event_t* e)
{
    lv_obj_t* target = lv_event_get_target(e);
    uint8_t id = lv_event_get_user_data(e);
    uint8_t fx = lv_obj_get_user_data(target);
    dsp_fx_settings[id][fx] = lv_roller_get_selected(target);
    dsp_open_edit(e);
}

static void dsp_close_edit()
{
    lv_obj_del_async(dsp_fx_scr);
    lv_scr_load(app_scr);
}

static void dsp_sidechain_response_refresh(uint8_t type, uint8_t width, uint8_t slope)
{
    for (uint8_t t = 0; t < 100; t++)
    {
        if (type == 0) dsp_sidechain_response[t] = 100.0 / (1.0 + pow(width / (0.1 + t), slope));
        if (type == 1) dsp_sidechain_response[t] = 100.0 / (1.0 + pow(t / (0.1 + width), slope));
    }
}

void DSP_sample_callback(){
	HAL_I2S_Receive(&hi2s2, (uint16_t*)adc_output, 2, 0);
	HAL_I2S_Transmit(&hi2s1, (uint16_t*)dac_input, 2, 0);

	dsp_fx_output[0] = dsp_fx_sample(0, (int24_to_float(adc_output[0])+int24_to_float(adc_output[1]))/2.0);
	for (uint8_t i = 1; i < DSP_MAX_FX_COUNT; i++) dsp_fx_output[i] = dsp_fx_sample(i, dsp_fx_output[i-1]);

	dac_input[0] = float_to_int24(dsp_fx_output[DSP_MAX_FX_COUNT-1]);
	dac_input[1] = dac_input[0];
}

static float dsp_fx_sample(uint8_t id, float input){
	float output = input;

	if(fx_list[id] == "Distortion") output = Distortion_Update(&dist, input);
	if(fx_list[id] == "FIR Filter") output = FIRFilter_Update(&fir, input);
	if(fx_list[id] == "Delay") output = Delay_Update(&dly, input);
	if(fx_list[id] == "Reeverb") output = Reverb_Update(&rvb, input);

	return output;
}

static void dsp_fx_init(){

	Distortion_Init(&dist, 1.0, 1.0, 1.0);
	dsp_fx_settings[0][0] = 100;
	dsp_fx_settings[0][1] = 100;
	dsp_fx_settings[0][2] = 100;

	FIRFilter_Init(&fir);
	dsp_fx_settings[3][0] = 2;
	dsp_fx_settings[3][1] = 5;
	dsp_fx_settings[3][2] = 1;
	dsp_fx_settings[3][3] = 20000;
	dsp_fx_settings[3][4] = FIR_FILTER_LENGHT;
	FIRFilter_CalcCoeff(&fir, dsp_fx_settings[3][0], dsp_fx_settings[3][1], dsp_fx_settings[3][4], dsp_fx_settings[3][4]/2, 1.0/SAMPLE_FREQ, dsp_fx_settings[3][2], dsp_fx_settings[3][3]);

	Delay_Init(&dly, 24000, 0.0, 0.0);
	dsp_fx_settings[4][0] = 24000;
	dsp_fx_settings[4][1] = 0;
	dsp_fx_settings[4][2] = 0;

	Reverb_Init(&rvb, 5000, 0.7, 1.0, 0.0);
	dsp_fx_settings[5][0] = 70;
	dsp_fx_settings[5][1] = 5000;
	dsp_fx_settings[5][2] = 0;
	dsp_fx_settings[5][3] = 100;

}

static void dsp_fx_setup(uint8_t id){

	sample_callback = &empty_void;

	if (fx_list[id] == "Distortion"){
		dist.InputGain = dsp_fx_settings[id][0]/100.0;
		dist.Threshold = dsp_fx_settings[id][1]/100.0;
		dist.OutputGain = dsp_fx_settings[id][2]/100.0;
	}

	if (fx_list[id] == "FIR Filter"){
		fir.filterLenght = dsp_fx_settings[id][4];
		FIRFilter_CalcCoeff(&fir, dsp_fx_settings[id][0], dsp_fx_settings[id][1], dsp_fx_settings[id][4], dsp_fx_settings[id][4]/2, 1.0/SAMPLE_FREQ, dsp_fx_settings[id][2], dsp_fx_settings[id][3]);
	}

	if (fx_list[id] == "Delay"){
		dly.cbf_dly.Ndelay = dsp_fx_settings[id][0];
		dly.cbf_dly.gain = dsp_fx_settings[id][1]/100.0;
		dly.DryWet = dsp_fx_settings[id][2]/100.0;
	}
	if (fx_list[id] == "Reeverb"){
		//Reverb_Init(&rvb, dsp_fx_settings[id][1], dsp_fx_settings[id][0]/100.0, dsp_fx_settings[id][3]/100.0, dsp_fx_settings[id][2]/100.0);
		/*
		rvb.cbf_rvb_1.Ndelay = dsp_fx_settings[id][1]*1.0/100.0;
		rvb.cbf_rvb_2.Ndelay = dsp_fx_settings[id][1]*0.967/100.0;
		rvb.cbf_rvb_3.Ndelay = dsp_fx_settings[id][1]*0.923/100.0;
		rvb.cbf_rvb_4.Ndelay = dsp_fx_settings[id][1]*0.871/100.0;
		rvb.apf_rvb_1.Ndelay = dsp_fx_settings[id][1]*0.171/100.0;
		rvb.apf_rvb_2.Ndelay = dsp_fx_settings[id][1]*0.057/100.0;
		*/
		rvb.apf_rvb_1.gain = dsp_fx_settings[id][0]/100.0;
		rvb.apf_rvb_2.gain = dsp_fx_settings[id][0]/100.0;
		rvb.cbf_rvb_1.gain = dsp_fx_settings[id][0]/100.0;
		rvb.cbf_rvb_2.gain = dsp_fx_settings[id][0]/100.0;
		rvb.cbf_rvb_3.gain = dsp_fx_settings[id][0]/100.0;
		rvb.cbf_rvb_4.gain = dsp_fx_settings[id][0]/100.0;
		rvb.wet = dsp_fx_settings[id][2]/100.0;
		rvb.dry = dsp_fx_settings[id][3]/100.0;

	}

	sample_callback = &DSP_sample_callback;
}
