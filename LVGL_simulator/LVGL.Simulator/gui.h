#ifndef GUI_H
#define GUI_H

#ifdef __cplusplus
extern "C" {
#endif

    void reset_scr();
    void reset_app();
    void sys_status_refresh();
    void start_app(lv_event_t* e);
    void start_main();
    void start_dsp();
    void start_oscilloscope();
    void start_synthesizer();
    void start_fft();

    void dsp_menu_cb(lv_event_t* e);
    void dsp_new_project();
    void dsp_open_project();
    void dsp_add_fx(lv_event_t* e);
    void dsp_change_fx(lv_event_t* e);
    void dsp_refresh_fx_slider(lv_event_t* e);
    void dsp_refresh_fx_list(lv_event_t* e);
    void dsp_refresh_fx_roller(lv_event_t* e);
    void dsp_open_edit(lv_event_t* e);
    void dsp_close_edit();
    void dsp_filter_response_refresh(uint8_t type, uint8_t order, double frq1, double frq2);
    void dsp_sidechain_response_refresh(uint8_t type, uint8_t width, uint8_t slope);

    void dsp_fir_calc(uint8_t FILT_TYPE, uint8_t WIN_TYPE, uint16_t NUM_TOTAL_SAMPLES, uint16_t NUM_SHIFT_SAMPLES, double SAMPLE_TIME_S, double CUTOFF_FREQUENCY_HZ, double CUTOFF_FREQUENCY2_HZ);

#ifdef __cplusplus
}
#endif


#endif /*GUI_H*/
