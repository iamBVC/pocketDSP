#include "lvgl/examples/lv_examples.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#define M_PI 3.1415926536

#define DSP_MAX_FX_COUNT 16
#define DSP_MAX_FX_SETTINGS 10
#define FFT_POINTS 2048

static lv_obj_t* label, * btn, * cont, * menu, * list, * slider, * mbox, * chart, * roller, * app_scr;
static lv_obj_t* sys_status;
static lv_chart_series_t* ser;

static lv_obj_t* dsp_fx_scr, * dsp_main_cont;

uint8_t battery_charge = 100;
uint8_t is_active_usb = 1;
uint8_t is_active_sd = 1;
uint8_t dsp_fx_count = 0;

uint16_t dsp_fx_settings[DSP_MAX_FX_COUNT][DSP_MAX_FX_SETTINGS + 1];

static lv_coord_t dsp_filter_response[200], dsp_sidechain_response[100], fft_vectors[FFT_POINTS], oscilloscope_buffer_r[FFT_POINTS], oscilloscope_buffer_l[FFT_POINTS];

const char* fx_list[] = { "None", "IIR Filter", "Reeverb", "Delay", "Sidechain", "Distortion", "FIR Filter" };
const char* app_list[] = { "DSP", "Synthesizer", "Oscilloscope", "FFT", "", "" };
const char* dsp_main_menu[] = { "New project", "Open project", "Save project", "Settings", "Exit" };

static double *impulseResponse;
static double *window;
static double *windowedImpulseResponse;
static double *frequencyVectorHz;
static double *winRespMag;
static lv_coord_t *dsp_fir_response;


void reset_app()
{
    dsp_fx_count = 0;
    memset(dsp_fx_settings, 0, DSP_MAX_FX_COUNT * (DSP_MAX_FX_SETTINGS + 1));
    impulseResponse = realloc(impulseResponse ,0);
    window = realloc(window, 0);
    windowedImpulseResponse = realloc(windowedImpulseResponse, 0);
    frequencyVectorHz = realloc(frequencyVectorHz, 0);
    winRespMag = realloc(winRespMag, 0);
    dsp_fir_response = realloc(dsp_fir_response, 0);
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
    if (id == app_list[3]) start_fft();
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
        static const char* btns[] = { "Delete", "" };
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
        chart = lv_chart_create(dsp_fx_scr);
        lv_obj_set_size(chart, 200, 150);
        lv_obj_align(chart, LV_ALIGN_LEFT_MID, 50, 50);
        lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 20000);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
        lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 21, 1, false, 50);
        lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 50);
        ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
        lv_chart_set_point_count(chart, 200);
        dsp_filter_response_refresh(dsp_fx_settings[id][1], dsp_fx_settings[id][2] + 1, dsp_fx_settings[id][3], dsp_fx_settings[id][4]);
        lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)dsp_filter_response);

        const char* opts1 =
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

        const char* opts2 =
            "1° order\n"
            "2° order\n"
            "3° order\n"
            "4° order\n"
            "5° order\n"
            "6° order\n"
            "7° order\n"
            "8° order\n"
            "9° order\n"
            "10° order\n"
            "11° order\n"
            "12° order\n"
            "13° order\n"
            "14° order\n"
            "15° order\n"
            "16° order\n"
            "17° order\n"
            "18° order\n"
            "19° order\n"
            "20° order";

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
        lv_obj_align_to(label, chart, LV_ALIGN_OUT_RIGHT_MID, 10, -40);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 140);
        lv_slider_set_range(slider, 0, 480);
        lv_obj_align_to(slider, label, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx++);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "HF");
        lv_obj_align_to(label, chart, LV_ALIGN_OUT_RIGHT_MID, 10, 40);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 140);
        lv_slider_set_range(slider, 0, 480);
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

    if (dsp_fx_settings[id][0] == 3) //Delay
    {
        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "Delay");
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
        lv_label_set_text(label, "Decay");
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

    if (dsp_fx_settings[id][0] == 4) //Sidechain
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


    if (dsp_fx_settings[id][0] == 6) //FIR Filter
    {
        if (dsp_fx_settings[id][5] == 0) dsp_fx_settings[id][5] = 2;
        chart = lv_chart_create(dsp_fx_scr);
        lv_obj_set_size(chart, 200, 150);
        lv_obj_align(chart, LV_ALIGN_LEFT_MID, 50, 50);
        lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 20000);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
        lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 21, 1, false, 50);
        lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 50);
        ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
        lv_chart_set_point_count(chart, dsp_fx_settings[id][5]);
        dsp_fir_calc(dsp_fx_settings[id][1], dsp_fx_settings[id][2], dsp_fx_settings[id][5], dsp_fx_settings[id][5]/2.0, 1.0 / 192000.0, dsp_fx_settings[id][3], dsp_fx_settings[id][4]);
        lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)dsp_fir_response);

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
        lv_obj_align_to(label, chart, LV_ALIGN_OUT_RIGHT_MID, 10, -40);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 140);
        lv_slider_set_range(slider, 0, 20000);
        lv_obj_align_to(slider, label, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx++);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "HF");
        lv_obj_align_to(label, chart, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 140);
        lv_slider_set_range(slider, 0, 20000);
        lv_obj_align_to(slider, label, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx++);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);

        label = lv_label_create(dsp_fx_scr);
        lv_label_set_text(label, "Size");
        lv_obj_align_to(label, chart, LV_ALIGN_OUT_RIGHT_MID, 10, 40);
        slider = lv_slider_create(dsp_fx_scr);
        lv_obj_set_width(slider, 140);
        lv_slider_set_range(slider, 2, 1024);
        lv_obj_align_to(slider, label, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        lv_slider_set_value(slider, dsp_fx_settings[id][fx], LV_ANIM_OFF);
        lv_obj_set_user_data(slider, fx++);
        lv_obj_add_event_cb(slider, dsp_refresh_fx_slider, LV_EVENT_VALUE_CHANGED, id);

    }


}

void dsp_change_fx(lv_event_t* e)
{
    uint8_t id = lv_event_get_user_data(e);
    dsp_fx_settings[id][0] = lv_dropdown_get_selected(lv_event_get_target(e));
    for (uint8_t i = 1; i <= DSP_MAX_FX_SETTINGS; i++)
        dsp_fx_settings[id][i] = 0;
    dsp_open_edit(e);
}

void dsp_refresh_fx_slider(lv_event_t* e)
{
    lv_obj_t* target = lv_event_get_target(e);
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
void dsp_refresh_fx_roller(lv_event_t* e)
{
    lv_obj_t* target = lv_event_get_target(e);
    uint8_t id = lv_event_get_user_data(e);
    uint8_t fx = lv_obj_get_user_data(target);
    dsp_fx_settings[id][fx] = lv_roller_get_selected(target);
    dsp_open_edit(e);
}

void dsp_close_edit()
{
    lv_obj_del_async(dsp_fx_scr);
    reset_scr();
    start_dsp();
}

void dsp_filter_response_refresh(uint8_t type, uint8_t order, double frq1, double frq2)
{
    for (uint8_t s = 0; s < 200; s++)
    {
        if (type == 0) dsp_filter_response[s] = 100.0 / sqrt(1.0 + pow(s / frq2, 2.0 * order));
        if (type == 1) dsp_filter_response[s] = 100.0 / (sqrt(1.0 + pow(s / frq2, 2.0 * order)) * sqrt(1.0 + pow(frq1 / s, 2.0 * order)));
        if (type == 2) dsp_filter_response[s] = 100.0 / sqrt(1.0 + pow(frq1 / s, 2.0 * order));
        if (type == 3) dsp_filter_response[s] = 100.0 * (pow(s, 2) + pow((frq2 - frq1) / 2, 2)) / sqrt(pow(pow(s, 2) + pow((frq2 - frq1) / 2, 2), 2) + pow(s * (frq2 - frq1), 2));
    }
}

void dsp_sidechain_response_refresh(uint8_t type, uint8_t width, uint8_t slope)
{
    for (uint8_t t = 0; t < 100; t++)
    {
        if (type == 0) dsp_sidechain_response[t] = 100.0 / (1.0 + pow(width / (0.1 + t), slope));
        if (type == 1) dsp_sidechain_response[t] = 100.0 / (1.0 + pow(t / (0.1 + width), slope));
    }
}










void start_oscilloscope()
{
    for (uint16_t i = 0; i < FFT_POINTS; i++)
    {
        oscilloscope_buffer_r[i] = (rand() % 40) + 500 + 400.0 * sin(i / 50.0);
        oscilloscope_buffer_l[i] = (rand() % 40) + 500 + 50.0 * cos(i / 20.0);
    }


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
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, FFT_POINTS);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 1000);
    lv_chart_set_zoom_x(chart, 256 * 1);
    lv_chart_set_point_count(chart, FFT_POINTS);
    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)oscilloscope_buffer_r);
    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_YELLOW), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)oscilloscope_buffer_l);
    lv_obj_set_style_bg_color(chart, lv_color_make(30, 30, 30), LV_PART_MAIN);

}

void start_synthesizer()
{

}

void sys_status_refresh()
{
    if (lv_obj_is_valid(sys_status)) lv_obj_del_async(sys_status);
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

void start_fft()
{
    for (uint16_t i = 0; i < FFT_POINTS; i++)
        fft_vectors[i] = (rand() % 50) + 500000.0 / (1000 + pow(500 - i, 2));
    for (uint16_t i = 0; i < FFT_POINTS; i++)
        oscilloscope_buffer_r[i] = 500 + 400.0 * sin(i / 50.0);

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

    lv_list_add_text(list, "Cursors");
    btn = lv_list_add_btn(list, NULL, "t1");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_t1");
    btn = lv_list_add_btn(list, NULL, "t2");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_t1");
    btn = lv_list_add_btn(list, NULL, "v1");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_v1");
    btn = lv_list_add_btn(list, NULL, "v2");
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "set_cursor_v2");

    lv_list_add_text(list, "System");
    btn = lv_list_add_btn(list, LV_SYMBOL_SETTINGS, NULL);
    //lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, "osc_settings");
    btn = lv_list_add_btn(list, LV_SYMBOL_POWER, NULL);
    lv_obj_add_event_cb(btn, reset_app, LV_EVENT_CLICKED, NULL);

    chart = lv_chart_create(app_scr);
    lv_obj_set_size(chart, 360, 300);
    lv_obj_align_to(chart, list, LV_ALIGN_OUT_LEFT_TOP, 0, 0);
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, FFT_POINTS);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 1000);
    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_point_count(chart, FFT_POINTS);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)oscilloscope_buffer_r);
    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_YELLOW), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_point_count(chart, FFT_POINTS);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)fft_vectors);
    lv_obj_set_style_bg_color(chart, lv_color_make(30, 30, 30), LV_PART_MAIN);
}



void dsp_fir_calc(uint8_t FILT_TYPE, uint8_t WIN_TYPE, uint16_t NUM_TOTAL_SAMPLES, uint16_t NUM_SHIFT_SAMPLES, double SAMPLE_TIME_S, double CUTOFF_FREQUENCY_HZ, double CUTOFF_FREQUENCY2_HZ){

    impulseResponse = (double*)calloc(NUM_TOTAL_SAMPLES, sizeof(double));
    window = (double*)calloc(NUM_TOTAL_SAMPLES, sizeof(double));
    windowedImpulseResponse = (double*)calloc(NUM_TOTAL_SAMPLES, sizeof(double));
    frequencyVectorHz = (double*)calloc(NUM_TOTAL_SAMPLES, sizeof(double));
    winRespMag = (double*)calloc(NUM_TOTAL_SAMPLES, sizeof(double));
    dsp_fir_response = (lv_coord_t*)calloc(NUM_TOTAL_SAMPLES, sizeof(lv_coord_t));

	double df = (20000.0) / ((double)NUM_TOTAL_SAMPLES - 1.0);

    for (uint16_t n = 0; n < NUM_TOTAL_SAMPLES; n++){

		if (n != NUM_SHIFT_SAMPLES){
            if (FILT_TYPE == 0) impulseResponse[n] = sin(2.0 * M_PI * CUTOFF_FREQUENCY2_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES)) / (M_PI * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES));
            if (FILT_TYPE == 1) impulseResponse[n] = (sin(M_PI * (n - NUM_SHIFT_SAMPLES)) - sin(2.0 * M_PI * CUTOFF_FREQUENCY_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES))) / (M_PI * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES));
            if (FILT_TYPE == 2) impulseResponse[n] = (sin(2.0 * M_PI * CUTOFF_FREQUENCY2_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES)) - sin(2.0 * M_PI * CUTOFF_FREQUENCY_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES))) / (M_PI * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES));
            if (FILT_TYPE == 3) impulseResponse[n] = (sin(2.0 * M_PI * CUTOFF_FREQUENCY_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES)) - sin(2.0 * M_PI * CUTOFF_FREQUENCY2_HZ * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES)) + sin(M_PI * (n - NUM_SHIFT_SAMPLES))) / (M_PI * SAMPLE_TIME_S * (n - NUM_SHIFT_SAMPLES));
        }else{
            if (FILT_TYPE == 0) impulseResponse[n] = 2.0 * CUTOFF_FREQUENCY2_HZ;
            if (FILT_TYPE == 1) impulseResponse[n] = 1.0 / SAMPLE_TIME_S - 2.0 * CUTOFF_FREQUENCY_HZ;
            if (FILT_TYPE == 2) impulseResponse[n] = 2.0 * CUTOFF_FREQUENCY2_HZ - 2.0 * CUTOFF_FREQUENCY_HZ;
            if (FILT_TYPE == 3) impulseResponse[n] = 2.0 * CUTOFF_FREQUENCY_HZ - 2.0 * CUTOFF_FREQUENCY2_HZ + 1.0 / SAMPLE_TIME_S;
        }

		impulseResponse[n] *= SAMPLE_TIME_S;
		
		if(WIN_TYPE <= 0 || WIN_TYPE >= 11) window[n] = 1.0;
		if(WIN_TYPE == 1) window[n] = 1.0 - abs((n - 0.5 * NUM_TOTAL_SAMPLES) / (0.5 * NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 2) window[n] = 1.0 - pow((n - 0.5 * NUM_TOTAL_SAMPLES) / (0.5 * NUM_TOTAL_SAMPLES), 2.0);
		if(WIN_TYPE == 3) window[n] = sin(M_PI * n / ((double)NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 4) window[n] = 0.5 * (1 - cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)));
		if(WIN_TYPE == 5) window[n] = (25.0 / 46.0) - (21.0 / 46.0) * cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 6) window[n] = 0.42 - 0.5 * cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) + 0.08 * cos(4.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 7) window[n] = 0.355768 - 0.487396 * cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) + 0.144232 * cos(4.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) - 0.012604 * cos(6.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 8) window[n] = 0.3635819 - 0.4891775 * cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) + 0.1365995 * cos(4.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) - 0.0106411 * cos(6.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 9) window[n] = 0.35875 - 0.48829 * cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) + 0.14128 * cos(4.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) - 0.01168 * cos(6.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES));
		if(WIN_TYPE == 10) window[n] = 0.21557895 - 0.41663158 * cos(2.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) + 0.277263158 * cos(4.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) - 0.083578947 * cos(6.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES)) + 0.006947368 * cos(8.0 * M_PI * n / ((double)NUM_TOTAL_SAMPLES));
	
		windowedImpulseResponse[n] = impulseResponse[n] * window[n];
		frequencyVectorHz[n] = n * df;
    }


    for (uint16_t fIndex = 0; fIndex < NUM_TOTAL_SAMPLES; fIndex++){
        double reWin = 0.0;
        double imWin = 0.0;
        for (uint16_t n = 0; n < NUM_TOTAL_SAMPLES; n++){
            reWin = reWin + windowedImpulseResponse[n] * cos(2.0 * M_PI * frequencyVectorHz[fIndex] * n * SAMPLE_TIME_S);
            imWin = imWin - windowedImpulseResponse[n] * sin(2.0 * M_PI * frequencyVectorHz[fIndex] * n * SAMPLE_TIME_S);
        }
        winRespMag[fIndex] = 100.0 * sqrt(reWin * reWin + imWin * imWin);
        dsp_fir_response[fIndex] = winRespMag[fIndex];
    }

}
