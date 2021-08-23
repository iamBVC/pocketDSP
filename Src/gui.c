﻿#include "../lv_examples.h"
#include <stdio.h>
#include <math.h>

#define DSP_MAX_FX_COUNT 16
#define DSP_MAX_FX_SETTINGS 10

static lv_obj_t *label, *btn, *cont, *menu, *list, *slider, *mbox, *chart, *app_scr;
static lv_obj_t* sys_status;
static lv_chart_series_t* ser;

static lv_obj_t * dsp_fx_scr, * dsp_main_cont;

uint8_t battery_charge = 100;
uint8_t is_active_usb = 1;
uint8_t is_active_sd = 1;
uint8_t dsp_fx_count = 0;

uint16_t dsp_fx_settings[DSP_MAX_FX_COUNT][DSP_MAX_FX_SETTINGS + 1];

static lv_coord_t dsp_filter_response[200];

const char *fx_list[] = { "None", "Filter", "Reeverb", "Delay", "Sidechain", "Distortion" };
const char *app_list[] = { "DSP", "Synthesizer", "Oscilloscope", "FFT", "", "" };
const char *dsp_main_menu[] = { "New project", "Open project", "Save project", "Settings", "Exit" };

void reset_app()
{
    dsp_fx_count = 0;
    for (uint8_t i = 0; i < DSP_MAX_FX_COUNT; i++)
        for (uint8_t j = 0; j <= DSP_MAX_FX_SETTINGS; j++)
            dsp_fx_settings[i][j] = 0;
    
    reset_scr();
    start_main();
}

void reset_scr()
{
    if (lv_obj_is_valid(app_scr)) lv_obj_del_async(app_scr);
    app_scr = lv_obj_create(NULL);
    lv_scr_load(app_scr);
    //lv_scr_load_anim(app_scr, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, 1);
}


void start_main()
{
    sys_status_refresh();
    lv_obj_align(sys_status, LV_ALIGN_TOP_LEFT, 0, 0);
    cont = lv_obj_create(app_scr);
    lv_obj_set_size(cont, 480, 250);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    for (uint8_t i = 0; i < 6; i++) {
        btn = lv_btn_create(cont);
        lv_obj_set_size(btn, 140, 50);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_add_event_cb(btn, start_app, LV_EVENT_CLICKED, app_list[i]);
        label = lv_label_create(btn);
        lv_label_set_text(label, app_list[i]);
        lv_obj_center(label);
    }
    label = lv_label_create(app_scr);
    lv_label_set_text(label, "pocketDSP by BVC");
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_set_style_bg_color(cont, lv_color_make(200, 200, 200), LV_PART_MAIN);
}

void start_app(lv_event_t* e)
{
    char* id = lv_event_get_user_data(e);
    reset_scr();
    if (id == app_list[0]) start_dsp();
    if (id == app_list[1]) start_synthesizer();
    if (id == app_list[2]) start_oscilloscope();
}






void start_dsp()
{
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

    for (uint8_t i = 0; i < dsp_fx_count; i++)
    {
        btn = lv_btn_create(dsp_main_cont);
        lv_obj_set_size(btn, 150, 150);
        lv_obj_add_event_cb(btn, dsp_open_edit, LV_EVENT_CLICKED, i);
        label = lv_label_create(btn);
        //lv_label_set_text(dsp_fx_label[i], fx_list[(uint8_t)dsp_fx_settings[i][0]]);
        lv_label_set_text_fmt(label, "FX %d\n\n%s", i, fx_list[(uint8_t)dsp_fx_settings[i][0]]);
        lv_obj_center(label);
    }

    btn = lv_btn_create(app_scr);
    lv_obj_set_size(btn, 50, 50);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_FLOATING);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    lv_obj_add_event_cb(btn, dsp_add_fx, LV_EVENT_CLICKED, 0);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_set_style_text_font(btn, lv_theme_get_font_large(btn), 0);

    menu = lv_dropdown_create(app_scr);
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
    LV_IMG_DECLARE(img_caret_down) lv_dropdown_set_symbol(menu, &img_caret_down);
    lv_obj_set_style_transform_angle(menu, 1800, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_dropdown_set_selected_highlight(menu, false);
    lv_obj_add_event_cb(menu, dsp_menu_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_set_style_bg_color(app_scr, lv_color_make(220, 220, 255), LV_PART_MAIN);
}

void dsp_add_fx(lv_event_t* e)
{
    if (dsp_fx_count < DSP_MAX_FX_COUNT)
    {
        uint8_t id = lv_event_get_user_data(e);
        btn = lv_btn_create(dsp_main_cont);
        lv_obj_set_size(btn, 150, 150);
        lv_obj_add_event_cb(btn, dsp_open_edit, LV_EVENT_CLICKED, dsp_fx_count);
        label = lv_label_create(btn);
        lv_label_set_text_fmt(label, "FX %d\n\n%s", dsp_fx_count, fx_list[id]);
        lv_obj_center(label);
        dsp_fx_count++;
    }
}

void dsp_menu_cb(lv_event_t* e)
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
    if (selected == 4) reset_app();
}

void dsp_new_project()
{
    reset_app();
    reset_scr();
    start_dsp();
}

void dsp_open_project()
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

void dsp_open_edit(lv_event_t* e)
{
    uint8_t id = lv_event_get_user_data(e);
    uint8_t fx = 1;

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
    char buf1[5];
    lv_snprintf(buf1, sizeof(buf1), "FX %d", id);
    lv_label_set_text(label, buf1);

    list = lv_dropdown_create(dsp_fx_scr);
    char buf2[128];
    sprintf(buf2, "");
    for (uint8_t i = 0; i < sizeof(fx_list) / sizeof(fx_list[0]); i++)
    {
        sprintf(buf2 + strlen(buf2), "%s", fx_list[i]);
        if (i + 1 < sizeof(fx_list) / sizeof(fx_list[0])) sprintf(buf2 + strlen(buf2), "\n");
    }
    lv_dropdown_set_options(list, buf2);
    lv_dropdown_set_selected(list, dsp_fx_settings[id][0]);
    lv_obj_add_event_cb(list, dsp_change_fx, LV_EVENT_VALUE_CHANGED, id);
    lv_obj_align(list, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_style_bg_color(dsp_fx_scr, lv_color_make(220, 255, 220), LV_PART_MAIN);
 

    if (dsp_fx_settings[id][0] == 1) //Filter
    {
        float type = dsp_fx_settings[id][1];
        float order = dsp_fx_settings[id][2] + 1;
        float frq1 = dsp_fx_settings[id][3] * 4;
        float frq2 = dsp_fx_settings[id][4];
        
        chart = lv_chart_create(dsp_fx_scr);
        lv_obj_set_size(chart, 200, 150);
        lv_obj_align(chart, LV_ALIGN_LEFT_MID, 50, 0);
        //lv_chart_set_type(chart,LV_CHART_TYPE_SCATTER);
        lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
        //lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 3, 10, true, 50);
        lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 50);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 20000);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
        ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
        lv_chart_set_point_count(chart, 200);
        dsp_filter_response_refresh(type, order, frq1 * 2, frq2 * 2);
        lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)dsp_filter_response);


        static const char* opts1 =
            "Low Pass\n"
            "Band Pass\n"
            "High Pass\n"
            "Band Stop";

        list = lv_dropdown_create(dsp_fx_scr);
        lv_dropdown_set_options_static(list, opts1);
        lv_obj_set_size(list, 110, 40);
        lv_obj_align_to(list, chart, LV_ALIGN_OUT_TOP_RIGHT, 0, 0);
        lv_dropdown_set_selected(list, dsp_fx_settings[id][fx]);
        lv_obj_set_user_data(list, fx++);
        lv_obj_add_event_cb(list, dsp_refresh_fx_list, LV_EVENT_VALUE_CHANGED, id);

        static const char* opts2 =
            "1° order\n"
            "2° order\n"
            "3° order\n"
            "4° order\n"
            "5° order\n"
            "6° order";

        list = lv_dropdown_create(dsp_fx_scr);
        lv_dropdown_set_options_static(list, opts2);
        lv_obj_set_size(list,90,40);
        lv_obj_align_to(list, chart, LV_ALIGN_OUT_TOP_LEFT, 0, 0);
        lv_dropdown_set_selected(list, dsp_fx_settings[id][fx]);
        lv_obj_set_user_data(list, fx++);
        lv_obj_add_event_cb(list, dsp_refresh_fx_list, LV_EVENT_VALUE_CHANGED, id);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "FRQ1");
        lv_obj_align_to(label, chart, LV_ALIGN_OUT_RIGHT_MID, 20, -40);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 150);
        lv_obj_align_to(slider, label, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx++);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "FRQ2");
        lv_obj_align_to(label, chart, LV_ALIGN_OUT_RIGHT_MID, 20, 40);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 150);
        lv_obj_align_to(slider, label, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx++);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);

    }


    if (dsp_fx_settings[id][0] == 2) //Reeverb
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
        lv_label_set_text_fmt(label, "%d%%", dsp_fx_settings[id][fx++]);
        lv_obj_align_to(label, slider, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "Delay");
        lv_obj_align(label, LV_ALIGN_LEFT_MID, 20, -40);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 200);
        lv_obj_align(slider, LV_ALIGN_LEFT_MID, 100, -40);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text_fmt(label, "%d%%", dsp_fx_settings[id][fx++]);
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


}

void dsp_change_fx(lv_event_t* e)
{
    uint8_t id = lv_event_get_user_data(e);
    dsp_fx_settings[id][0] = lv_dropdown_get_selected(lv_event_get_target(e));
    for (uint8_t i = 1; i <= DSP_MAX_FX_SETTINGS; i++) dsp_fx_settings[id][i] = 0;
    dsp_open_edit(e);
}

void dsp_refresh_fx_slider(lv_event_t* e)
{
    lv_obj_t *target = lv_event_get_target(e);
    uint8_t id = lv_event_get_user_data(e);
    uint8_t fx = lv_obj_get_user_data(target);
    dsp_fx_settings[id][fx] = lv_slider_get_value(target);
    dsp_open_edit(e);
}
void dsp_refresh_fx_list(lv_event_t* e)
{
    lv_obj_t* target = lv_event_get_target(e);
    uint8_t id = lv_event_get_user_data(e);
    uint8_t fx = lv_obj_get_user_data(target);
    dsp_fx_settings[id][fx] = lv_dropdown_get_selected(target);
    dsp_open_edit(e);
}

void dsp_close_edit()
{
    lv_obj_del_async(dsp_fx_scr);
    reset_scr();
    start_dsp();
}










void start_oscilloscope()
{
    sys_status_refresh();
    lv_obj_align(sys_status, LV_ALIGN_TOP_RIGHT, 0, 0);

    list = lv_list_create(app_scr);
    lv_obj_set_size(list, 120, 300);
    lv_obj_set_align(list, LV_ALIGN_BOTTOM_RIGHT);

    lv_list_add_text(list, "Channels");
    btn = lv_list_add_btn(list, LV_SYMBOL_VOLUME_MAX, "R CH");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "config_r_ch");
    btn = lv_list_add_btn(list, LV_SYMBOL_VOLUME_MAX, "L CH");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "config_l_ch");

    lv_list_add_text(list, "Trigger");
    btn = lv_list_add_btn(list, NULL, "Thresh");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_trigger_threshold");
    btn = lv_list_add_btn(list, NULL, "Type");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_trigger_type");
    btn = lv_list_add_btn(list, NULL, "Mode");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_trigger_mode");

    lv_list_add_text(list, "Cursors");
    btn = lv_list_add_btn(list, NULL, "t1");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_t1");
    btn = lv_list_add_btn(list, NULL, "t2");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_t1");
    btn = lv_list_add_btn(list, NULL, "v1");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_v1");
    btn = lv_list_add_btn(list, NULL, "v2");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_v2");

    lv_list_add_text(list, "Signal Gen");
    btn = lv_list_add_btn(list, NULL, "R CH");
    btn = lv_list_add_btn(list, NULL, "L CH");

    lv_list_add_text(list, "System");
    btn = lv_list_add_btn(list, LV_SYMBOL_SETTINGS, NULL);
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "osc_settings");
    btn = lv_list_add_btn(list, LV_SYMBOL_POWER, NULL);
    lv_obj_add_event_cb(btn, reset_app, LV_EVENT_CLICKED, NULL);

    chart = lv_chart_create(app_scr);
    lv_obj_set_size(chart, 360, 300);
    lv_obj_align_to(chart, list, LV_ALIGN_OUT_LEFT_TOP, 0, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_SCATTER);
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 1000);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 1000);
    lv_chart_set_zoom_x(chart,256*1);
    lv_chart_set_point_count(chart, 1000);
    lv_chart_series_t* ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    for (uint16_t i = 0; i < 1000; i++) lv_chart_set_next_value2(chart, ser, i, 500+400*sin(i/50.0));
    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    for (uint16_t i = 0; i < 1000; i++) lv_chart_set_next_value2(chart, ser, i, 500 + 100 * cos(i / 50.0));

}

void start_synthesizer()
{

}

void sys_status_refresh()
{
    if(lv_obj_is_valid(sys_status)) lv_obj_del_async(sys_status);
    sys_status = lv_obj_create(app_scr);
    lv_obj_set_size(sys_status, 20, 120);
    lv_obj_remove_style_all(sys_status);

    label = lv_label_create(sys_status);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 20, 2);
    if (battery_charge >= 0 && battery_charge < 20) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_EMPTY " %d%%", battery_charge);
    else if (battery_charge >= 20 && battery_charge < 40) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_1 " %d%%", battery_charge);
    else if (battery_charge >= 40 && battery_charge < 60) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_2 " %d%%", battery_charge);
    else if (battery_charge >= 60 && battery_charge < 80) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_3 " %d%%", battery_charge);
    else if (battery_charge >= 80 && battery_charge <= 100) lv_label_set_text_fmt(label, LV_SYMBOL_BATTERY_FULL " %d%%", battery_charge);

    label = lv_label_create(sys_status);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 80, 2);
    if (is_active_sd) lv_label_set_text(label, LV_SYMBOL_SD_CARD);

    label = lv_label_create(sys_status);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 100, 2);
    if (is_active_usb) lv_label_set_text(label, LV_SYMBOL_USB);
}


void dsp_filter_response_refresh(uint8_t type, uint8_t order, float frq1, float frq2)
{

        for (uint8_t s = 0; s < 200; s++)
        {
            if (type == 0) dsp_filter_response[s] = 100.0 / (1.0 + pow(s / frq1, order));
            if (type == 1) dsp_filter_response[s] = 100.0 / ((1.0 + pow(s / frq2, order))*(1.0 + 1.0 / pow(s / frq1, order)));
            if (type == 2) dsp_filter_response[s] = 100.0 / (1.0 + 1.0 / pow(s / frq1, order));
            if (type == 3) dsp_filter_response[s] = 100.0 * (pow(s,order)+pow(frq1,order)) / (pow(s, order)+(frq1*s)+pow(frq1,order));
        }

}
