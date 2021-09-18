#include "gui.h"

void start_synthesizer()
{

}



void start_fft()
{
    for (uint16_t i = 0; i < FFT_POINTS; i++)
    fft_vectors[i] = 500 + 400.0 * sin(i / 200.0);

    sys_status_refresh();
    lv_obj_align(sys_status, LV_ALIGN_TOP_LEFT, 0, 0);

    list = lv_list_create(app_scr);
    lv_obj_set_size(list, 120, 320);
    lv_obj_set_align(list, LV_ALIGN_TOP_RIGHT);

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
    lv_obj_align(chart, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, FFT_POINTS);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 1000);
    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_point_count(chart, FFT_POINTS);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t*)fft_vectors);
    lv_obj_set_style_bg_color(chart, lv_color_make(30, 30, 30), LV_PART_MAIN);
}
