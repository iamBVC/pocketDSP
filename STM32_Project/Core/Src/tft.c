/* Includes ------------------------------------------------------------------*/

#include "stm32h7xx_hal.h"
#include "string.h"
#include "stdlib.h"
#include "tft.h"
#include "user_setting.h"

/********************************************** NO CHNAGES AFTER THIS ************************************************/

void PIN_LOW(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

void PIN_HIGH(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

void PIN_INPUT(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void PIN_OUTPUT(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void PIN_ANALOG(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t ADC_Ch) {
	ADC_ChannelConfTypeDef sConfig = { 0 };
	HAL_GPIO_DeInit(GPIOx, GPIO_Pin);
	sConfig.Channel = ADC_Ch;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}
uint16_t analogRead() {
	HAL_ADC_Start(&hadc1);
	while (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) != HAL_OK)
		;
	return HAL_ADC_GetValue(&hadc1);
}

#define RD_ACTIVE  PIN_LOW(RD_PORT, RD_PIN)
#define RD_IDLE    PIN_HIGH(RD_PORT, RD_PIN)
#define RD_OUTPUT  PIN_OUTPUT(RD_PORT, RD_PIN)
#define WR_ACTIVE  PIN_LOW(WR_PORT, WR_PIN)
#define WR_IDLE    PIN_HIGH(WR_PORT, WR_PIN)
#define WR_OUTPUT  PIN_OUTPUT(WR_PORT, WR_PIN)
#define CD_COMMAND PIN_LOW(CD_PORT, CD_PIN)
#define CD_DATA    PIN_HIGH(CD_PORT, CD_PIN)
#define CD_OUTPUT  PIN_OUTPUT(CD_PORT, CD_PIN)
#define CS_ACTIVE  PIN_LOW(CS_PORT, CS_PIN)
#define CS_IDLE    PIN_HIGH(CS_PORT, CS_PIN)
#define CS_OUTPUT  PIN_OUTPUT(CS_PORT, CS_PIN)
#define RESET_ACTIVE  PIN_LOW(RESET_PORT, RESET_PIN)
#define RESET_IDLE    PIN_HIGH(RESET_PORT, RESET_PIN)
#define RESET_OUTPUT  PIN_OUTPUT(RESET_PORT, RESET_PIN)

#define WR_ACTIVE2  {WR_ACTIVE; WR_ACTIVE;}
#define WR_ACTIVE4  {WR_ACTIVE2; WR_ACTIVE2;}
#define WR_ACTIVE8  {WR_ACTIVE4; WR_ACTIVE4;}
#define RD_ACTIVE2  {RD_ACTIVE; RD_ACTIVE;}
#define RD_ACTIVE4  {RD_ACTIVE2; RD_ACTIVE2;}
#define RD_ACTIVE8  {RD_ACTIVE4; RD_ACTIVE4;}
#define RD_ACTIVE16 {RD_ACTIVE8; RD_ACTIVE8;}
#define WR_IDLE2  {WR_IDLE; WR_IDLE;}
#define WR_IDLE4  {WR_IDLE2; WR_IDLE2;}
#define RD_IDLE2  {RD_IDLE; RD_IDLE;}
#define RD_IDLE4  {RD_IDLE2; RD_IDLE2;}

#define WR_STROBE { WR_ACTIVE; WR_IDLE; }         //PWLW=TWRL=50ns
#define RD_STROBE RD_IDLE, RD_ACTIVE, RD_ACTIVE, RD_ACTIVE   //PWLR=TRDL=150ns

#define write8(x)     { write_8(x); WRITE_DELAY; WR_STROBE; WR_IDLE; }
#define write16(x)    { uint8_t h = (x)>>8, l = x; write8(h); write8(l); }
#define READ_8(dst)   { RD_STROBE; READ_DELAY; dst = read_8(); RD_IDLE; RD_IDLE; } // read 250ns after RD_ACTIVE goes low
#define READ_16(dst)  { uint8_t hi; READ_8(hi); READ_8(dst); dst |= (hi << 8); }

#define CTL_INIT()   { RD_OUTPUT; WR_OUTPUT; CD_OUTPUT; CS_OUTPUT; RESET_OUTPUT; }
#define WriteCmd(x)  { CD_COMMAND; write16(x); CD_DATA; }
#define WriteData(x) { write16(x); }
#define SUPPORT_9488_555          //costs +230 bytes, 0.03s / 0.19s
#define SUPPORT_B509_7793         //R61509, ST7793 +244 bytes
#define OFFSET_9327 32            //costs about 103 bytes, 0.08s

/************************************************************************************************************/

uint16_t _width = WIDTH;
uint16_t _height = HEIGHT;

uint16_t width(void) {
	return _width;
}

uint16_t height(void) {
	return _height;
}

void pushColors16b(uint16_t *block, int16_t n, uint8_t first);
void pushColors8b(uint8_t *block, int16_t n, uint8_t first);
void pushColors4n(const uint8_t *block, int16_t n, uint8_t first,
		uint8_t bigend);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

void setAddrWindow(int16_t x, int16_t y, int16_t x1, int16_t y1);
int16_t readGRAM(int16_t x, int16_t y, uint16_t *block, int16_t w, int16_t h);

void setReadDir(void);
void setWriteDir(void);
static uint8_t done_reset, is8347, is555, is9797;
static uint16_t color565_to_555(uint16_t color) {
	return (color & 0xFFC0) | ((color & 0x1F) << 1) | ((color & 0x01)); //lose Green LSB, extend Blue LSB
}
static uint16_t color555_to_565(uint16_t color) {
	return (color & 0xFFC0) | ((color & 0x0400) >> 5) | ((color & 0x3F) >> 1); //extend Green LSB
}
static uint8_t color565_to_r(uint16_t color) {
	return ((color & 0xF800) >> 8);  // transform to rrrrrxxx
}
static uint8_t color565_to_g(uint16_t color) {
	return ((color & 0x07E0) >> 3);  // transform to ggggggxx
}
static uint8_t color565_to_b(uint16_t color) {
	return ((color & 0x001F) << 3);  // transform to bbbbbxxx
}

uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}
uint16_t readPixel(int16_t x, int16_t y) {
	uint16_t color;
	readGRAM(x, y, &color, 1, 1);
	return color;
}

static void pushColors_any(uint16_t cmd, uint8_t *block, int16_t n,
		uint8_t first, uint8_t flags);

static void write24(uint16_t color);

static void writecmddata(uint16_t cmd, uint16_t dat);

void WriteCmdData(uint16_t cmd, uint16_t dat) {
	writecmddata(cmd, dat);
}

static inline void WriteCmdParam4(uint8_t cmd, uint8_t d1, uint8_t d2,
		uint8_t d3, uint8_t d4);

static void init_table(const void *table, int16_t size);

static void WriteCmdParamN(uint16_t cmd, int8_t N, uint8_t *block);

void pushCommand(uint16_t cmd, uint8_t *block, int8_t N) {
	WriteCmdParamN(cmd, N, block);
}

static uint16_t read16bits(void);

uint16_t readReg(uint16_t reg, int8_t index);

uint32_t readReg32(uint16_t reg);

uint32_t readReg40(uint16_t reg);

uint8_t cursor_y = 0, cursor_x = 0;
uint8_t textsize = 1;
uint16_t textcolor = 0xffff, textbgcolor = 0xFFFF;
uint8_t wrap = true;
uint8_t _cp437 = false;
uint8_t rotation = 0;

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }

uint16_t _lcd_xor, _lcd_capable;

uint16_t _lcd_ID, _lcd_rev, _lcd_madctl, _lcd_drivOut, _MC, _MP, _MW, _SC, _EC,
		_SP, _EP;

//extern GFXfont *gfxFont;

void setReadDir(void) {
	PIN_INPUT(D0_PORT, D0_PIN);
	PIN_INPUT(D1_PORT, D1_PIN);
	PIN_INPUT(D2_PORT, D2_PIN);
	PIN_INPUT(D3_PORT, D3_PIN);
	PIN_INPUT(D4_PORT, D4_PIN);
	PIN_INPUT(D5_PORT, D5_PIN);
	PIN_INPUT(D6_PORT, D6_PIN);
	PIN_INPUT(D7_PORT, D7_PIN);
}

void setWriteDir(void) {
	PIN_OUTPUT(D0_PORT, D0_PIN);
	PIN_OUTPUT(D1_PORT, D1_PIN);
	PIN_OUTPUT(D2_PORT, D2_PIN);
	PIN_OUTPUT(D3_PORT, D3_PIN);
	PIN_OUTPUT(D4_PORT, D4_PIN);
	PIN_OUTPUT(D5_PORT, D5_PIN);
	PIN_OUTPUT(D6_PORT, D6_PIN);
	PIN_OUTPUT(D7_PORT, D7_PIN);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawScreen(uint16_t *color_t, lv_area_t *area) {
	if(area->x2 < 0) return;
	if(area->y2 < 0) return;
	if(area->x1 >= 480) return;
	if(area->y1 >= 320) return;

	int16_t act_x1 = area->x1 < 0 ? 0 : area->x1;
	int16_t act_y1 = area->y1 < 0 ? 0 : area->y1;
	int16_t act_x2 = area->x2 > 479 ? 479 : area->x2;
	int16_t act_y2 = area->y2 > 319 ? 319 : area->y2;

	setAddrWindow(act_x1, act_y1, act_x2, act_y2);
	CS_ACTIVE;
	WriteCmd(_MW);
	for (uint16_t y = act_y1; y <= act_y2; y++) {
		for (uint16_t x = act_x1; x <= act_x2; x++) {
			write16(*color_t);
			color_t++;
		}
	}

	CS_IDLE;
}

static void pushColors_any(uint16_t cmd, uint8_t *block, int16_t n,
		uint8_t first, uint8_t flags) {
	uint16_t color;
	uint8_t h, l;
	uint8_t isconst = flags & 1;
	uint8_t isbigend = (flags & 2) != 0;
	CS_ACTIVE;
	if (first) {
		WriteCmd(cmd);
	}

	if (!isconst && !isbigend) {
		uint16_t *block16 = (uint16_t*) block;
		while (n-- > 0) {
			color = *block16++;
			write16(color);
		}
	} else

		while (n-- > 0) {
			if (isconst) {
				h = pgm_read_byte(block++);
				l = pgm_read_byte(block++);
			} else {
				h = (*block++);
				l = (*block++);
			}
			color = (isbigend) ? (h << 8 | l) : (l << 8 | h);
#if defined(SUPPORT_9488_555)
			if (is555)
				color = color565_to_555(color);
#endif
			if (is9797)
				write24(color);
			else
				write16(color);
		}
	CS_IDLE;
}

static void write24(uint16_t color) {
	uint8_t r = color565_to_r(color);
	uint8_t g = color565_to_g(color);
	uint8_t b = color565_to_b(color);
	write8(r);
	write8(g);
	write8(b);
}

static void writecmddata(uint16_t cmd, uint16_t dat) {
	CS_ACTIVE;
	WriteCmd(cmd);
	WriteData(dat);
	CS_IDLE;
}

static void WriteCmdParamN(uint16_t cmd, int8_t N, uint8_t *block) {
	CS_ACTIVE;
	WriteCmd(cmd);
	while (N-- > 0) {
		uint8_t u8 = *block++;
		write8(u8);
		if (N && is8347) {
			cmd++;
			WriteCmd(cmd);
		}
	}
	CS_IDLE;
}

static inline void WriteCmdParam4(uint8_t cmd, uint8_t d1, uint8_t d2,
		uint8_t d3, uint8_t d4) {
	uint8_t d[4];
	d[0] = d1, d[1] = d2, d[2] = d3, d[3] = d4;
	WriteCmdParamN(cmd, 4, d);
}

#define TFTLCD_DELAY 0xFFFF
#define TFTLCD_DELAY8 0x7F
static void init_table(const void *table, int16_t size) {

	uint8_t *p = (uint8_t*) table, dat[24];            //R61526 has GAMMA[22]

	while (size > 0) {
		uint8_t cmd = pgm_read_byte(p++);
		uint8_t len = pgm_read_byte(p++);
		if (cmd == TFTLCD_DELAY8) {
			delay(len);
			len = 0;
		} else {
			for (uint8_t i = 0; i < len; i++)
				dat[i] = pgm_read_byte(p++);
			WriteCmdParamN(cmd, len, dat);
		}
		size -= len + 2;
	}
}

static void init_table16(const void *table, int16_t size) {
	uint16_t *p = (uint16_t*) table;
	while (size > 0) {
		uint16_t cmd = pgm_read_word(p++);
		uint16_t d = pgm_read_word(p++);
		if (cmd == TFTLCD_DELAY)
			delay(d);
		else {
			writecmddata(cmd, d);                      //static function
		}
		size -= 2 * sizeof(int16_t);
	}
}

void readTouch(uint16_t *x, uint16_t *y, uint16_t *z) {

	const uint16_t TS_LEFT = 440, TS_RT = 3880, TS_TOP = 3800, TS_BOT = 550;

	PIN_OUTPUT(RD_PORT, RD_PIN);
	PIN_HIGH(RD_PORT, RD_PIN);
	PIN_OUTPUT(FCS_PORT, FCS_PIN);
	PIN_HIGH(FCS_PORT, FCS_PIN);

	PIN_ANALOG(YP_PORT, YP_PIN, YP_ADC_CHANNEL);
	PIN_INPUT(YM_PORT, YM_PIN);
	PIN_OUTPUT(XP_PORT, XP_PIN);
	PIN_OUTPUT(XM_PORT, XM_PIN);
	PIN_HIGH(XP_PORT, XP_PIN);
	PIN_LOW(XM_PORT, XM_PIN);
	float ypos = WIDTH * (TS_TOP - analogRead()) / (TS_TOP - TS_BOT);
	if (ypos > WIDTH)
		ypos = WIDTH;
	if (ypos < 0)
		ypos = 0;
	*y = ypos;
	//*y = analogRead();

	PIN_ANALOG(XM_PORT, XM_PIN, XM_ADC_CHANNEL);
	PIN_INPUT(XP_PORT, XP_PIN);
	PIN_OUTPUT(YP_PORT, YP_PIN);
	PIN_OUTPUT(YM_PORT, YM_PIN);
	PIN_HIGH(YP_PORT, YP_PIN);
	PIN_LOW(YM_PORT, YM_PIN);
	float xpos = HEIGHT * (analogRead() - TS_LEFT) / (TS_RT - TS_LEFT);
	if (xpos > HEIGHT)
		xpos = HEIGHT;
	if (xpos < 0)
		xpos = 0;
	*x = xpos;
	//*x = analogRead();

	PIN_OUTPUT(XP_PORT, XP_PIN);
	PIN_INPUT(YP_PORT, YP_PIN);
	PIN_HIGH(YM_PORT, YM_PIN);
	PIN_LOW(XP_PORT, XP_PIN);
	uint16_t z1 = analogRead();

	PIN_ANALOG(YP_PORT, YP_PIN, YP_ADC_CHANNEL);
	PIN_INPUT(XM_PORT, XM_PIN);
	uint16_t z2 = analogRead();
	*z = 4095 - (z2 - z1);

	PIN_ANALOG(GPIOC, GPIO_PIN_0, ADC_CHANNEL_10);
	PIN_OUTPUT(XP_PORT, XP_PIN);
	PIN_OUTPUT(XM_PORT, XM_PIN);
	PIN_OUTPUT(YP_PORT, YP_PIN);
	PIN_OUTPUT(YM_PORT, YM_PIN);
	PIN_LOW(FCS_PORT, FCS_PIN);
/*
	char buffer[16];
	sprintf(buffer, "%d,%d,%d\n", *x, *y, *z);
	CDC_Transmit_FS(buffer, sizeof(buffer));
*/
}

void reset(void) {
	done_reset = 1;
	setWriteDir();
	CTL_INIT();
	CS_IDLE;
	RD_IDLE;
	WR_IDLE;
	RESET_IDLE;
	delay(50);
	RESET_ACTIVE;
	delay(100);
	RESET_IDLE;
	delay(100);
	WriteCmdData(0xB0, 0x0000);   //R61520 needs this to read ID
}

static uint16_t read16bits(void) {
	uint16_t ret;
	uint8_t lo;
	READ_8(ret);
	READ_8(lo);
	return (ret << 8) | lo;
}

uint16_t readReg(uint16_t reg, int8_t index) {
	uint16_t ret;
	uint8_t lo;
	if (!done_reset)
		reset();
	CS_ACTIVE;
	WriteCmd(reg);
	setReadDir();
	delay(1);    //1us should be adequate
	//    READ_16(ret);
	do {
		ret = read16bits();
	} while (--index >= 0);  //need to test with SSD1963
	RD_IDLE;
	CS_IDLE;
	setWriteDir();
	return ret;
}

uint32_t readReg32(uint16_t reg) {
	uint16_t h = readReg(reg, 0);
	uint16_t l = readReg(reg, 1);
	return ((uint32_t) h << 16) | (l);
}

uint32_t readReg40(uint16_t reg) {
	uint16_t h = readReg(reg, 0);
	uint16_t m = readReg(reg, 1);
	uint16_t l = readReg(reg, 2);
	return ((uint32_t) h << 24) | (m << 8) | (l >> 8);
}

void tft_init(uint16_t ID) {
	int dummy = 0;
	int16_t *p16;             //so we can "write" to a const protected variable.
	const uint8_t *table8_ads = NULL;
	int16_t table_size;
	_lcd_xor = 0;
	switch (_lcd_ID = ID) {

	case 0x1511:                // Unknown from Levy
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1;   //extra read_8(dummy)
		static const uint8_t R61511_regValues[] = { 0xB0, 1, 0x00, //Command Access Protect
				};
		table8_ads = R61511_regValues, table_size = sizeof(R61511_regValues);
		p16 = (int16_t*) &height;
		*p16 = 480;
		p16 = (int16_t*) &width;
		*p16 = 320;
		break;

	case 0x1520:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | READ_24BITS;
		static const uint8_t R61520_regValues[] = { 0xB0, 1, 0x00, //Command Access Protect
				0xC0, 1, 0x0A,      //DM=1, BGR=1
				};
		table8_ads = R61520_regValues, table_size = sizeof(R61520_regValues);
		break;

	case 0x1526:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | READ_24BITS;
		static const uint8_t R61526_regValues[] = { 0xB0, 1, 0x03, //Command Access
				0xE2, 1, 0x3F,      //Command Write Access
				0xC0, 1, 0x22,      //REV=0, BGR=1, SS=0
				0xE2, 1, 0x00,      //Command Write Protect
				};
		table8_ads = R61526_regValues, table_size = sizeof(R61526_regValues);
		break;

	case 0x5310:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | INVERT_SS
				| INVERT_RGB | READ_24BITS;
		static const uint8_t NT35310_regValues[] = {        //
				TFTLCD_DELAY8, 10,    //just some dummy
				};
		table8_ads = NT35310_regValues, table_size = sizeof(NT35310_regValues);
		p16 = (int16_t*) &height;
		*p16 = 480;
		p16 = (int16_t*) &width;
		*p16 = 320;
		break;

	case 0x7789:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | READ_24BITS;
		static const uint8_t ST7789_regValues[] = { (0xB2), 5, 0x0C, 0x0C, 0x00,
				0x33,
				0x33,    //PORCTRK: Porch setting [08 08 00 22 22] PSEN=0 anyway
				(0xB7), 1,
				0x35,    //GCTRL: Gate Control [35]
				(0xBB), 1,
				0x2B,    //VCOMS: VCOM setting VCOM=1.175 [20] VCOM=0.9
				(0xC0), 1,
				0x04,    //LCMCTRL: LCM Control [2C]
				(0xC2), 2, 0x01,
				0xFF,      //VDVVRHEN: VDV and VRH Command Enable [01 FF]
				(0xC3), 1,
				0x11,    //VRHS: VRH Set VAP=4.4, VAN=-4.4 [0B]
				(0xC4), 1,
				0x20,    //VDVS: VDV Set [20]
				(0xC6), 1,
				0x0F,    //FRCTRL2: Frame Rate control in normal mode [0F]
				(0xD0), 2, 0xA4,
				0xA1,      //PWCTRL1: Power Control 1 [A4 A1]
				(0xE0), 14, 0xD0, 0x00, 0x05, 0x0E, 0x15, 0x0D, 0x37, 0x43,
				0x47, 0x09, 0x15, 0x12, 0x16,
				0x19,     //PVGAMCTRL: Positive Voltage Gamma control
				(0xE1), 14, 0xD0, 0x00, 0x05, 0x0D, 0x0C, 0x06, 0x2D, 0x44,
				0x40, 0x0E, 0x1C, 0x18, 0x16, 0x19, //NVGAMCTRL: Negative Voltage Gamma control
				};
		static const uint8_t ST7789_regValues_arcain6[] = { (0xB2), 5, 0x0C,
				0x0C, 0x00, 0x33,
				0x33,    //PORCTRK: Porch setting [08 08 00 22 22] PSEN=0 anyway
				(0xB7), 1,
				0x35,    //GCTRL: Gate Control [35]
				(0xBB), 1,
				0x35,    //VCOMS: VCOM setting VCOM=??? [20] VCOM=0.9
				(0xC0), 1,
				0x2C,    //LCMCTRL: LCM Control [2C]
				(0xC2), 2, 0x01,
				0xFF,      //VDVVRHEN: VDV and VRH Command Enable [01 FF]
				(0xC3), 1,
				0x13,    //VRHS: VRH Set VAP=???, VAN=-??? [0B]
				(0xC4), 1,
				0x20,    //VDVS: VDV Set [20]
				(0xC6), 1,
				0x0F,    //FRCTRL2: Frame Rate control in normal mode [0F]
				(0xCA), 1,
				0x0F,    //REGSEL2 [0F]
				(0xC8), 1,
				0x08,    //REGSEL1 [08]
				(0x55), 1,
				0x90,    //WRCACE  [00]
				(0xD0), 2, 0xA4,
				0xA1,      //PWCTRL1: Power Control 1 [A4 A1]
				(0xE0), 14, 0xD0, 0x00, 0x06, 0x09, 0x0B, 0x2A, 0x3C, 0x55,
				0x4B, 0x08, 0x16, 0x14, 0x19,
				0x20,     //PVGAMCTRL: Positive Voltage Gamma control
				(0xE1), 14, 0xD0, 0x00, 0x06, 0x09, 0x0B, 0x29, 0x36, 0x54,
				0x4B, 0x0D, 0x16, 0x14, 0x21, 0x20, //NVGAMCTRL: Negative Voltage Gamma control
				};
		table8_ads = ST7789_regValues, table_size = sizeof(ST7789_regValues); //
		break;

	case 0x8031:      //Unknown BangGood thanks PrinceCharles
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | READ_24BITS
				| REV_SCREEN;
		static const uint8_t FK8031_regValues[] = {
				// 0xF2:8.2 = SM, 0xF2:8.0 = REV. invertDisplay(), vertScroll() do not work
				0xF2, 11, 0x16, 0x16, 0x03, 0x08, 0x08, 0x08, 0x08, 0x10, 0x04,
				0x16, 0x16, // f.k. 0xF2:8.2 SM=1
				0xFD, 3, 0x11, 0x02, 0x35, //f.k 0xFD:1.1 creates contiguous scan lins
				};
		table8_ads = FK8031_regValues, table_size = sizeof(FK8031_regValues);
		break;

	case 0x8357:                //BIG CHANGE: HX8357-B is now 0x8357
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | REV_SCREEN;
		goto common_8357;
	case 0x9090:                //BIG CHANGE: HX8357-D was 0x8357
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | REV_SCREEN
				| READ_24BITS;
		common_8357: dummy = 1;
		static const uint8_t HX8357C_regValues[] = {
		TFTLCD_DELAY8, 1,  //dummy table
				};
		table8_ads = HX8357C_regValues, table_size = sizeof(HX8357C_regValues);
		p16 = (int16_t*) &height;
		*p16 = 480;
		p16 = (int16_t*) &width;
		*p16 = 320;
		break;

	case 0x0099:                //HX8357-D matches datasheet
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | REV_SCREEN
				| READ_24BITS;
		static const uint8_t HX8357_99_regValues[] = { (0xB9), 3, 0xFF, 0x83,
				0x57,   // SETEXTC
				TFTLCD_DELAY8, 150,
				TFTLCD_DELAY8, 150, (0xB6), 1, 0x25, // SETCOM [4B 00] -2.5V+37*0.02V=-1.76V [-1.00V]
				(0xC0), 6, 0x50, 0x50, 0x01, 0x3C, 0x1E, 0x08, // SETSTBA [73 50 00 3C C4 08]
				(0xB4), 7, 0x02, 0x40, 0x00, 0x2A, 0x2A, 0x0D, 0x78, // SETCYC [02 40 00 2A 2A 0D 96]

				};
		table8_ads = HX8357_99_regValues, table_size =
				sizeof(HX8357_99_regValues);
		p16 = (int16_t*) &height;
		*p16 = 480;
		p16 = (int16_t*) &width;
		*p16 = 320;
		break;

	case 0x0001:
		_lcd_capable = 0 | REV_SCREEN | INVERT_GS; //no RGB bug. thanks Ivo_Deshev
		goto common_9320;
	case 0x5408:
		_lcd_capable = 0 | REV_SCREEN | READ_BGR; //Red 2.4" thanks jorgenv, Ardlab_Gent
//        _lcd_capable = 0 | REV_SCREEN | READ_BGR | INVERT_GS; //Blue 2.8" might be different
		goto common_9320;
	case 0x1505:    //R61505 thanks Ravi_kanchan2004. R61505V, R61505W different
	case 0x9320:
		_lcd_capable = 0 | REV_SCREEN | READ_BGR;
		common_9320: dummy = 1;
		static const uint16_t ILI9320_regValues[] = { 0x00e5, 0x8000, 0x0000,
				0x0001, 0x0001, 0x100, 0x0002, 0x0700, 0x0003, 0x1030, 0x0004,
				0x0000, 0x0008, 0x0202, 0x0009, 0x0000, 0x000A, 0x0000, 0x000C,
				0x0000, 0x000D, 0x0000, 0x000F, 0x0000,
				//-----Power On sequence-----------------------
				0x0010, 0x0000, 0x0011, 0x0007, 0x0012, 0x0000, 0x0013, 0x0000,
				TFTLCD_DELAY, 50, 0x0010,
				0x17B0,  //SAP=1, BT=7, APE=1, AP=3
				0x0011,
				0x0007,  //DC1=0, DC0=0, VC=7
				TFTLCD_DELAY, 10, 0x0012,
				0x013A,  //VCMR=1, PON=3, VRH=10
				TFTLCD_DELAY, 10, 0x0013,
				0x1A00,  //VDV=26
				0x0029,
				0x000c,  //VCM=12
				TFTLCD_DELAY, 10,
				//-----Gamma control-----------------------
				0x0030, 0x0000, 0x0031, 0x0505, 0x0032, 0x0004, 0x0035, 0x0006,
				0x0036, 0x0707, 0x0037, 0x0105, 0x0038, 0x0002, 0x0039, 0x0707,
				0x003C, 0x0704, 0x003D, 0x0807,
				//-----Set RAM area-----------------------
				0x0060,
				0xA700,     //GS=1
				0x0061, 0x0001, 0x006A, 0x0000, 0x0021, 0x0000, 0x0020, 0x0000,
				//-----Partial Display Control------------
				0x0080, 0x0000, 0x0081, 0x0000, 0x0082, 0x0000, 0x0083, 0x0000,
				0x0084, 0x0000, 0x0085, 0x0000,
				//-----Panel Control----------------------
				0x0090, 0x0010, 0x0092, 0x0000, 0x0093, 0x0003, 0x0095, 0x0110,
				0x0097, 0x0000, 0x0098, 0x0000,
				//-----Display on-----------------------
				0x0007, 0x0173,
				TFTLCD_DELAY, 50, };
		init_table16(ILI9320_regValues, sizeof(ILI9320_regValues));
		break;
	case 0x6809:
		_lcd_capable = 0 | REV_SCREEN | INVERT_GS | AUTO_READINC;
		goto common_93x5;
	case 0x9328:
	case 0x9325:
		_lcd_capable = 0 | REV_SCREEN | INVERT_GS;
		goto common_93x5;
	case 0x9331:
	case 0x9335:
		_lcd_capable = 0 | REV_SCREEN;
		common_93x5: dummy = 1;
		static const uint16_t ILI9325_regValues[] = { 0x00E5,
				0x78F0,     // set SRAM internal timing
				0x0001,
				0x0100,     // set Driver Output Control
				0x0002,
				0x0200,     // set 1 line inversion
				0x0003,
				0x1030,     // set GRAM write direction and BGR=1.
				0x0004,
				0x0000,     // Resize register
				0x0005,
				0x0000,     // .kbv 16bits Data Format Selection
				0x0008,
				0x0207,     // set the back porch and front porch
				0x0009,
				0x0000,     // set non-display area refresh cycle ISC[3:0]
				0x000A,
				0x0000,     // FMARK function
				0x000C,
				0x0000,     // RGB interface setting
				0x000D,
				0x0000,     // Frame marker Position
				0x000F,
				0x0000,     // RGB interface polarity
				// ----------- Power On sequence ----------- //
				0x0010,
				0x0000,     // SAP, BT[3:0], AP, DSTB, SLP, STB
				0x0011,
				0x0007,     // DC1[2:0], DC0[2:0], VC[2:0]
				0x0012,
				0x0000,     // VREG1OUT voltage
				0x0013,
				0x0000,     // VDV[4:0] for VCOM amplitude
				0x0007, 0x0001,
				TFTLCD_DELAY,
				200,  // Dis-charge capacitor power voltage
				0x0010,
				0x1690,     // SAP=1, BT=6, APE=1, AP=1, DSTB=0, SLP=0, STB=0
				0x0011,
				0x0227,     // DC1=2, DC0=2, VC=7
				TFTLCD_DELAY,
				50,   // wait_ms 50ms
				0x0012,
				0x000D,     // VCIRE=1, PON=0, VRH=5
				TFTLCD_DELAY,
				50,   // wait_ms 50ms
				0x0013,
				0x1200,     // VDV=28 for VCOM amplitude
				0x0029,
				0x000A,     // VCM=10 for VCOMH
				0x002B,
				0x000D,     // Set Frame Rate
				TFTLCD_DELAY,
				50,   // wait_ms 50ms
				0x0020,
				0x0000,     // GRAM horizontal Address
				0x0021,
				0x0000,     // GRAM Vertical Address
				// ----------- Adjust the Gamma Curve ----------//

				0x0030, 0x0000, 0x0031, 0x0404, 0x0032, 0x0003, 0x0035, 0x0405,
				0x0036, 0x0808, 0x0037, 0x0407, 0x0038, 0x0303, 0x0039, 0x0707,
				0x003C, 0x0504, 0x003D, 0x0808,

				//------------------ Set GRAM area ---------------//
				0x0060,
				0x2700,     // Gate Scan Line GS=0 [0xA700]
				0x0061,
				0x0001,     // NDL,VLE, REV .kbv
				0x006A,
				0x0000,     // set scrolling line
				//-------------- Partial Display Control ---------//
				0x0080, 0x0000, 0x0081, 0x0000, 0x0082, 0x0000, 0x0083, 0x0000,
				0x0084, 0x0000, 0x0085, 0x0000,
				//-------------- Panel Control -------------------//
				0x0090, 0x0010, 0x0092, 0x0000, 0x0007, 0x0133, // 262K color and display ON
				};
		init_table16(ILI9325_regValues, sizeof(ILI9325_regValues));
		break;

	case 0x9327:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS;
		static const uint8_t ILI9327_regValues[] = { 0xB0, 1, 0x00, //Disable Protect for cmds B1-DF, E0-EF, F0-FF
				//            0xE0, 1, 0x20,      //NV Memory Write [00]
				//            0xD1, 3, 0x00, 0x71, 0x19,  //VCOM control [00 40 0F]
				//            0xD0, 3, 0x07, 0x01, 0x08,  //Power Setting [07 04 8C]
				0xC1, 4, 0x10, 0x10, 0x02, 0x02,  //Display Timing [10 10 02 02]
				0xC0, 6, 0x00, 0x35, 0x00, 0x00, 0x01, 0x02, //Panel Drive [00 35 00 00 01 02 REV=0,GS=0,SS=0
				0xC5, 1, 0x04,      //Frame Rate [04]
				0xD2, 2, 0x01, 0x04,        //Power Setting [01 44]
				//            0xC8, 15, 0x04, 0x67, 0x35, 0x04, 0x08, 0x06, 0x24, 0x01, 0x37, 0x40, 0x03, 0x10, 0x08, 0x80, 0x00,
				//            0xC8, 15, 0x00, 0x77, 0x77, 0x04, 0x04, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
				0xCA, 1, 0x00,      //DGC LUT ???
				0xEA, 1, 0x80,      //3-Gamma Function Enable
				//                     0xB0, 1, 0x03,      //Enable Protect
				};
		table8_ads = ILI9327_regValues, table_size = sizeof(ILI9327_regValues);
		p16 = (int16_t*) &height;
		*p16 = 400;
		p16 = (int16_t*) &width;
		*p16 = 240;
		break;
	case 0x1602:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS; //does not readGRAM
		static const uint8_t XX1602_regValues[] = { 0xB8, 1, 0x01,     //GS [00]
				0xC0, 1, 0x0E,      //??Power [0A]
				};
		table8_ads = XX1602_regValues, table_size = sizeof(XX1602_regValues);
		break;

	case 0x2053:    //weird from BangGood
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | READ_24BITS
				| REV_SCREEN | READ_BGR;
		goto common_9329;
	case 0xAC11:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | READ_24BITS
				| REV_SCREEN; //thanks viliam
		goto common_9329;
	case 0x9302:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS;
		goto common_9329;
	case 0x9338:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | READ_24BITS;
		goto common_9329;
	case 0x9329:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | INVERT_SS
				| REV_SCREEN;
		common_9329: dummy = 1;
		static const uint8_t ILI9329_regValues[] = {
//            0xF6, 3, 0x01, 0x01, 0x00,  //Interface Control needs EXTC=1 MX_EOR=1, TM=0, RIM=0
//            0xB6, 3, 0x0A, 0x82, 0x27,  //Display Function [0A 82 27]
//            0xB7, 1, 0x06,      //Entry Mode Set [06]
				0x36, 1, 0x00, //Memory Access [00] pointless but stops an empty array
				};
		table8_ads = ILI9329_regValues, table_size = sizeof(ILI9329_regValues);
		break;

	case 0x9340:                //ILI9340 thanks Ravi_kanchan2004.
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | READ_24BITS
				| REV_SCREEN;
		goto common_9341;
	case 0x9341:
		common_9341: _lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS
				| READ_24BITS;
		static const uint8_t ILI9341_regValues_2_4[] = {        // BOE 2.4"
				0xF6, 3, 0x01, 0x01,
						0x00, //Interface Control needs EXTC=1 MV_EOR=0, TM=0, RIM=0
						0xCF, 3, 0x00, 0x81,
						0x30,  //Power Control B [00 81 30]
						0xED, 4, 0x64, 0x03, 0x12,
						0x81,    //Power On Seq [55 01 23 01]
						0xE8, 3, 0x85, 0x10,
						0x78,  //Driver Timing A [04 11 7A]
						0xCB, 5, 0x39, 0x2C, 0x00, 0x34,
						0x02,      //Power Control A [39 2C 00 34 02]
						0xF7, 1,
						0x20,      //Pump Ratio [10]
						0xEA, 2, 0x00,
						0x00,        //Driver Timing B [66 00]
						0xB0, 1,
						0x00,      //RGB Signal [00]
						0xB1, 2, 0x00,
						0x1B,        //Frame Control [00 1B]
						//            0xB6, 2, 0x0A, 0xA2, 0x27, //Display Function [0A 82 27 XX]    .kbv SS=1
						0xB4, 1,
						0x00,  //Inversion Control [02] .kbv NLA=1, NLB=1, NLC=1
						0xC0, 1,
						0x21,      //Power Control 1 [26]
						0xC1, 1,
						0x11,      //Power Control 2 [00]
						0xC5, 2, 0x3F,
						0x3C,        //VCOM 1 [31 3C]
						0xC7, 1,
						0xB5,      //VCOM 2 [C0]
						0x36, 1,
						0x48,      //Memory Access [00]
						0xF2, 1,
						0x00,      //Enable 3G [02]
						0x26, 1,
						0x01,      //Gamma Set [01]
						0xE0, 15, 0x0f, 0x26, 0x24, 0x0b, 0x0e, 0x09, 0x54,
						0xa8, 0x46, 0x0c, 0x17, 0x09, 0x0f, 0x07, 0x00, 0xE1,
						15, 0x00, 0x19, 0x1b, 0x04, 0x10, 0x07, 0x2a, 0x47,
						0x39, 0x03, 0x06, 0x06, 0x30, 0x38, 0x0f, };
		static const uint8_t ILI9341_regValues_ada[] = { // Adafruit_TFTLCD only works with EXTC=0
				//                     0xF6, 3, 0x00, 0x01, 0x00,  //Interface Control needs EXTC=1 TM=0, RIM=0
				//            0xF6, 3, 0x01, 0x01, 0x03,  //Interface Control needs EXTC=1 RM=1, RIM=1
						0xF6, 3, 0x09, 0x01, 0x03, //Interface Control needs EXTC=1 RM=0, RIM=1
						0xB0, 1, 0x40,      //RGB Signal [40] RCM=2
						0xB4, 1, 0x00, //Inversion Control [02] .kbv NLA=1, NLB=1, NLC=1
						0xC0, 1, 0x23,      //Power Control 1 [26]
						0xC1, 1, 0x10,      //Power Control 2 [00]
						0xC5, 2, 0x2B, 0x2B,        //VCOM 1 [31 3C]
						0xC7, 1, 0xC0,      //VCOM 2 [C0]
						0x36, 1, 0x88,      //Memory Access [00]
						0xB1, 2, 0x00, 0x1B,        //Frame Control [00 1B]
						0xB7, 1, 0x07,      //Entry Mode [00]
				};
		table8_ads = ILI9341_regValues_2_4, table_size =
				sizeof(ILI9341_regValues_2_4);   //
		break;

	case 0x1581:                   //no BGR in MADCTL.  set BGR in Panel Control
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | READ_24BITS; //thanks zdravke
		goto common_9481;
	case 0x9481:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | READ_BGR;
		common_9481: dummy = 1;
		static const uint8_t ILI9481_regValues[] = {    // Atmel MaxTouch
				0xB0, 1,
						0x00,              // unlocks E0, F0
						0xB3, 4, 0x02, 0x00, 0x00,
						0x00, //Frame Memory, interface [02 00 00 00]
						0xB4, 1,
						0x00,              // Frame mode [00]
						0xD0, 3, 0x07, 0x42,
						0x18,  // Set Power [00 43 18] x1.00, x6, x3
						0xD1, 3, 0x00, 0x07,
						0x10,  // Set VCOM  [00 00 00] x0.72, x1.02
						0xD2, 2, 0x01,
						0x02,        // Set Power for Normal Mode [01 22]
						0xD3, 2, 0x01,
						0x02,        // Set Power for Partial Mode [01 22]
						0xD4, 2, 0x01,
						0x02,        // Set Power for Idle Mode [01 22]
						0xC0, 5, 0x12, 0x3B, 0x00, 0x02,
						0x11, //Panel Driving BGR for 1581 [10 3B 00 02 11]
						0xC1, 3, 0x10, 0x10,
						0x88,  // Display Timing Normal [10 10 88]
						0xC5, 1,
						0x03,      //Frame Rate [03]
						0xC6, 1,
						0x02,      //Interface Control [02]
						0xC8, 12, 0x00, 0x32, 0x36, 0x45, 0x06, 0x16, 0x37,
						0x75, 0x77, 0x54, 0x0C, 0x00, 0xCC, 1, 0x00, //Panel Control [00]
				};
		static const uint8_t ILI9481_CPT29_regValues[] = {    // 320x430
				0xB0, 1, 0x00, 0xD0, 3, 0x07, 0x42,
						0x1C,  // Set Power [00 43 18]
						0xD1, 3, 0x00, 0x02,
						0x0F,  // Set VCOM  [00 00 00] x0.695, x1.00
						0xD2, 2, 0x01,
						0x11,        // Set Power for Normal Mode [01 22]
						0xC0, 5, 0x10, 0x35, 0x00, 0x02,
						0x11,      //Set Panel Driving [10 3B 00 02 11]
						0xC5, 1,
						0x03,      //Frame Rate [03]
						0xC8, 12, 0x00, 0x30, 0x36, 0x45, 0x04, 0x16, 0x37,
						0x75, 0x77, 0x54, 0x0F, 0x00, 0xE4, 1, 0xA0, 0xF0, 1,
						0x01, 0xF3, 2, 0x02, 0x1A, };
		static const uint8_t ILI9481_PVI35_regValues[] = {    // 320x480
				0xB0, 1, 0x00, 0xD0, 3, 0x07, 0x41,
						0x1D,  // Set Power [00 43 18]
						0xD1, 3, 0x00, 0x2B,
						0x1F,  // Set VCOM  [00 00 00] x0.900, x1.32
						0xD2, 2, 0x01,
						0x11,        // Set Power for Normal Mode [01 22]
						0xC0, 5, 0x10, 0x3B, 0x00, 0x02,
						0x11,      //Set Panel Driving [10 3B 00 02 11]
						0xC5, 1,
						0x03,      //Frame Rate [03]
						0xC8, 12, 0x00, 0x14, 0x33, 0x10, 0x00, 0x16, 0x44,
						0x36, 0x77, 0x00, 0x0F, 0x00, 0xE4, 1, 0xA0, 0xF0, 1,
						0x01, 0xF3, 2, 0x40, 0x0A, };
		static const uint8_t ILI9481_AUO317_regValues[] = {    // 320x480
				0xB0, 1, 0x00, 0xD0, 3, 0x07, 0x40,
						0x1D,  // Set Power [00 43 18]
						0xD1, 3, 0x00, 0x18,
						0x13,  // Set VCOM  [00 00 00] x0.805, x1.08
						0xD2, 2, 0x01,
						0x11,        // Set Power for Normal Mode [01 22]
						0xC0, 5, 0x10, 0x3B, 0x00, 0x02,
						0x11,      //Set Panel Driving [10 3B 00 02 11]
						0xC5, 1,
						0x03,      //Frame Rate [03]
						0xC8, 12, 0x00, 0x44, 0x06, 0x44, 0x0A, 0x08, 0x17,
						0x33, 0x77, 0x44, 0x08, 0x0C, 0xE4, 1, 0xA0, 0xF0, 1,
						0x01, };
		static const uint8_t ILI9481_CMO35_regValues[] = {    // 320480
				0xB0, 1, 0x00, 0xD0, 3, 0x07, 0x41,
						0x1D,  // Set Power [00 43 18] 07,41,1D
						0xD1, 3, 0x00, 0x1C,
						0x1F,  // Set VCOM  [00 00 00] x0.825, x1.32 1C,1F
						0xD2, 2, 0x01,
						0x11,        // Set Power for Normal Mode [01 22]
						0xC0, 5, 0x10, 0x3B, 0x00, 0x02,
						0x11,      //Set Panel Driving [10 3B 00 02 11]
						0xC5, 1,
						0x03,      //Frame Rate [03]
						0xC6, 1, 0x83, 0xC8, 12, 0x00, 0x26, 0x21, 0x00, 0x00,
						0x1F, 0x65, 0x23, 0x77, 0x00, 0x0F, 0x00, 0xF0, 1, 0x01, //?
						0xE4, 1, 0xA0,      //?SETCABC on Himax
						0x36, 1, 0x48,      //Memory Access [00]
						0xB4, 1, 0x11, };
		static const uint8_t ILI9481_RGB_regValues[] = {    // 320x480
				0xB0, 1, 0x00, 0xD0, 3, 0x07, 0x41,
						0x1D,  // SETPOWER [00 43 18]
						0xD1, 3, 0x00, 0x2B,
						0x1F,  // SETVCOM  [00 00 00] x0.900, x1.32
						0xD2, 2, 0x01,
						0x11,        // SETNORPOW for Normal Mode [01 22]
						0xC0, 6, 0x10, 0x3B, 0x00, 0x02, 0x11,
						0x00,     //SETPANEL [10 3B 00 02 11]
						0xC5, 1,
						0x03,      //SETOSC Frame Rate [03]
						0xC6, 1,
						0x80,      //SETRGB interface control
						0xC8, 12, 0x00, 0x14, 0x33, 0x10, 0x00, 0x16, 0x44,
						0x36, 0x77, 0x00, 0x0F, 0x00, 0xF3, 2, 0x40, 0x0A, 0xF0,
						1, 0x08, 0xF6, 1, 0x84, 0xF7, 1, 0x80, 0x0C, 2, 0x00,
						0x55, //RDCOLMOD
						0xB4, 1, 0x00,      //SETDISPLAY
//			0xB3, 4, 0x00, 0x01, 0x06, 0x01,  //SETGRAM simple example
						0xB3, 4, 0x00, 0x01, 0x06, 0x30,  //jpegs example
				};
		table8_ads = ILI9481_regValues, table_size = sizeof(ILI9481_regValues);
//        table8_ads = ILI9481_CPT29_regValues, table_size = sizeof(ILI9481_CPT29_regValues);
//        table8_ads = ILI9481_PVI35_regValues, table_size = sizeof(ILI9481_PVI35_regValues);
//        table8_ads = ILI9481_AUO317_regValues, table_size = sizeof(ILI9481_AUO317_regValues);
//        table8_ads = ILI9481_CMO35_regValues, table_size = sizeof(ILI9481_CMO35_regValues);
//        table8_ads = ILI9481_RGB_regValues, table_size = sizeof(ILI9481_RGB_regValues);
		p16 = (int16_t*) &height;
		*p16 = 480;
		p16 = (int16_t*) &width;
		*p16 = 320;
		break;
	case 0x9486:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS; //Red 3.5", Blue 3.5"
//        _lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | REV_SCREEN; //old Red 3.5"
		static const uint8_t ILI9486_regValues[] =
				{
				/*
				 0xF2, 9, 0x1C, 0xA3, 0x32, 0x02, 0xB2, 0x12, 0xFF, 0x12, 0x00,        //f.k
				 0xF1, 2, 0x36, 0xA4,        //
				 0xF8, 2, 0x21, 0x04,        //
				 0xF9, 2, 0x00, 0x08,        //
				 */
				0xC0, 2, 0x0d,
						0x0d,        //Power Control 1 [0E 0E]
						0xC1, 2, 0x43,
						0x00,        //Power Control 2 [43 00]
						0xC2, 1,
						0x00,      //Power Control 3 [33]
						0xC5, 4, 0x00, 0x48, 0x00,
						0x48,    //VCOM  Control 1 [00 40 00 40]
						0xB4, 1,
						0x00,      //Inversion Control [00]
						0xB6, 3, 0x02, 0x02,
						0x3B,  // Display Function Control [02 02 3B]
#define GAMMA9486 4
#if GAMMA9486 == 0
            // default GAMMA terrible
#elif GAMMA9486 == 1
            // GAMMA f.k.	bad
            0xE0, 15, 0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1, 0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00,
            0xE1, 15, 0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f,
#elif GAMMA9486 == 2
            // 1.2 CPT 3.5 Inch Initial Code not bad
			0xE0, 15, 0x0F, 0x1B, 0x18, 0x0B, 0x0E, 0x09, 0x47, 0x94, 0x35, 0x0A, 0x13, 0x05, 0x08, 0x03, 0x00,
			0xE1, 15, 0x0F, 0x3A, 0x37, 0x0B, 0x0C, 0x05, 0x4A, 0x24, 0x39, 0x07, 0x10, 0x04, 0x27, 0x25, 0x00,
#elif GAMMA9486 == 3
            // 2.2 HSD 3.5 Inch Initial Code not bad
			0xE0, 15, 0x0F, 0x1F, 0x1C, 0x0C, 0x0F, 0x08, 0x48, 0x98, 0x37, 0x0A, 0x13, 0x04, 0x11, 0x0D, 0x00,
			0xE1, 15, 0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75, 0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00,
#elif GAMMA9486 == 4
						// 3.2 TM  3.2 Inch Initial Code not bad
						0xE0, 15, 0x0F, 0x21, 0x1C, 0x0B, 0x0E, 0x08, 0x49,
						0x98, 0x38, 0x09, 0x11, 0x03, 0x14, 0x10, 0x00, 0xE1,
						15, 0x0F, 0x2F, 0x2B, 0x0C, 0x0E, 0x06, 0x47, 0x76,
						0x37, 0x07, 0x11, 0x04, 0x23, 0x1E, 0x00,
#elif GAMMA9486 == 5
            // 4.2 WTK 3.5 Inch Initial Code too white
			0xE0, 15, 0x0F, 0x10, 0x08, 0x05, 0x09, 0x05, 0x37, 0x98, 0x26, 0x07, 0x0F, 0x02, 0x09, 0x07, 0x00,
			0xE1, 15, 0x0F, 0x38, 0x36, 0x0D, 0x10, 0x08, 0x59, 0x76, 0x48, 0x0A, 0x16, 0x0A, 0x37, 0x2F, 0x00,
#endif
				};
		table8_ads = ILI9486_regValues, table_size = sizeof(ILI9486_regValues);
		p16 = (int16_t*) &height;
		*p16 = 480;
		p16 = (int16_t*) &width;
		*p16 = 320;
		break;
	case 0x7796:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS; //thanks to safari1
		goto common_9488;
	case 0x9487:                //with thanks to Charlyf
	case 0x9488:
		_lcd_capable = AUTO_READINC | MIPI_DCS_REV1 | MV_AXIS | READ_24BITS;
		common_9488: dummy = 1;
		static const uint8_t ILI9488_regValues_max[] = {       // Atmel MaxTouch
				0xC0, 2, 0x10, 0x10,        //Power Control 1 [0E 0E]
						0xC1, 1, 0x41,      //Power Control 2 [43]
						0xC5, 4, 0x00, 0x22, 0x80, 0x40, //VCOM  Control 1 [00 40 00 40]
						0x36, 1, 0x68,      //Memory Access [00]
						0xB0, 1, 0x00,      //Interface     [00]
						0xB1, 2, 0xB0, 0x11,        //Frame Rate Control [B0 11]
						0xB4, 1, 0x02,      //Inversion Control [02]
						0xB6, 3, 0x02, 0x02, 0x3B, // Display Function Control [02 02 3B] .kbv NL=480
						0xB7, 1, 0xC6,      //Entry Mode      [06]
						0x3A, 1, 0x55,      //Interlace Pixel Format [XX]
						0xF7, 4, 0xA9, 0x51, 0x2C, 0x82, //Adjustment Control 3 [A9 51 2C 82]
				};
		table8_ads = ILI9488_regValues_max, table_size =
				sizeof(ILI9488_regValues_max);
		p16 = (int16_t*) &height;
		*p16 = 480;
		p16 = (int16_t*) &width;
		*p16 = 320;
		break;
	case 0xB505:                //R61505V
	case 0xC505:                //R61505W
		_lcd_capable = 0 | REV_SCREEN | READ_LOWHIGH;
		static const uint16_t R61505V_regValues[] = { 0x0000, 0x0000, 0x0000,
				0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x00A4,
				0x0001,     //CALB=1
				TFTLCD_DELAY, 10, 0x0060,
				0x2700,     //NL
				0x0008,
				0x0808,     //FP & BP
				0x0030,
				0x0214,     //Gamma settings
				0x0031, 0x3715, 0x0032, 0x0604, 0x0033, 0x0E16, 0x0034, 0x2211,
				0x0035, 0x1500, 0x0036, 0x8507, 0x0037, 0x1407, 0x0038, 0x1403,
				0x0039, 0x0020, 0x0090,
				0x0015,     //DIVI & RTNI
				0x0010,
				0x0410,     //BT=4,AP=1
				0x0011,
				0x0237,     //DC1=2,DC0=3, VC=7
				0x0029,
				0x0046,     //VCM1=70
				0x002A,
				0x0046,     //VCMSEL=0,VCM2=70
				// Sleep mode IN sequence
				0x0007, 0x0000,
				//0x0012, 0x0000,   //PSON=0,PON=0
				// Sleep mode EXIT sequence
				0x0012,
				0x0189,     //VCMR=1,PSON=0,PON=0,VRH=9
				0x0013,
				0x1100,     //VDV=17
				TFTLCD_DELAY, 150, 0x0012,
				0x01B9,     //VCMR=1,PSON=1,PON=1,VRH=9 [018F]
				0x0001,
				0x0100,     //SS=1 Other mode settings
				0x0002,
				0x0200,     //BC0=1--Line inversion
				0x0003, 0x1030, 0x0009,
				0x0001,     //ISC=1 [0000]
				0x000A,
				0x0000,     // [0000]
				//            0x000C, 0x0001,   //RIM=1 [0000]
				0x000D,
				0x0000,     // [0000]
				0x000E,
				0x0030,     //VEM=3 VCOM equalize [0000]
				0x0061, 0x0001, 0x006A, 0x0000, 0x0080, 0x0000, 0x0081, 0x0000,
				0x0082, 0x005F, 0x0092, 0x0100, 0x0093, 0x0701,
				TFTLCD_DELAY, 80, 0x0007, 0x0100,     //BASEE=1--Display On
				};
		init_table16(R61505V_regValues, sizeof(R61505V_regValues));
		break;

#if defined(SUPPORT_B509_7793)
	case 0x7793:
	case 0xB509:
		_lcd_capable = REV_SCREEN;
		static const uint16_t R61509V_regValues[] = { 0x0000, 0x0000, 0x0000,
				0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
				TFTLCD_DELAY, 15, 0x0400,
				0x6200,     //NL=0x31 (49) i.e. 400 rows
				0x0008, 0x0808,
				//gamma
				0x0300, 0x0C00, 0x0301, 0x5A0B, 0x0302, 0x0906, 0x0303, 0x1017,
				0x0304, 0x2300, 0x0305, 0x1700, 0x0306, 0x6309, 0x0307, 0x0C09,
				0x0308, 0x100C, 0x0309, 0x2232,

				0x0010, 0x0016,     //69.5Hz         0016
				0x0011, 0x0101, 0x0012, 0x0000, 0x0013, 0x0001,

				0x0100, 0x0330,     //BT,AP
				0x0101, 0x0237,     //DC0,DC1,VC
				0x0103, 0x0D00,     //VDV
				0x0280, 0x6100,     //VCM
				0x0102, 0xC1B0,     //VRH,VCMR,PSON,PON
				TFTLCD_DELAY, 50,

				0x0001, 0x0100, 0x0002, 0x0100, 0x0003, 0x1030,     //1030
				0x0009, 0x0001, 0x000C, 0x0000, 0x0090, 0x8000, 0x000F, 0x0000,

				0x0210, 0x0000, 0x0211, 0x00EF, 0x0212, 0x0000, 0x0213, 0x018F, //432=01AF,400=018F
				0x0500, 0x0000, 0x0501, 0x0000, 0x0502, 0x005F,     //???
				0x0401, 0x0001,     //REV=1
				0x0404, 0x0000,
				TFTLCD_DELAY, 50,

				0x0007, 0x0100,     //BASEE
				TFTLCD_DELAY, 50,

				0x0200, 0x0000, 0x0201, 0x0000, };
		init_table16(R61509V_regValues, sizeof(R61509V_regValues));
		p16 = (int16_t*) &height;
		*p16 = 400;
		break;
#endif

	default:
		p16 = (int16_t*) &width;
		*p16 = 0;       //error value for width
		break;
	}
	_lcd_rev = ((_lcd_capable & REV_SCREEN) != 0);
	if (table8_ads != NULL) {
		static const uint8_t reset_off[] = { 0x01, 0,            //Soft Reset
				TFTLCD_DELAY8, 150, // .kbv will power up with ONLY reset, sleep out, display on
				0x28, 0,            //Display Off
				0x3A, 1, 0x55,      //Pixel read=565, write=565.
				};
		static const uint8_t wake_on[] = { 0x11, 0,            //Sleep Out
				TFTLCD_DELAY8, 150, 0x29, 0,            //Display On
				};
		init_table(&reset_off, sizeof(reset_off));
		init_table(table8_ads, table_size);   //can change PIXFMT
		init_table(&wake_on, sizeof(wake_on));
	}
	setRotation(0);             //PORTRAIT
	invertDisplay(false);
#if defined(SUPPORT_9488_555)
	if (_lcd_ID == 0x9488) {
		is555 = 0;
		drawPixel(0, 0, 0xFFE0);
		if (readPixel(0, 0) == 0xFF1F) {
			uint8_t pixfmt = 0x06;
			pushCommand(0x3A, &pixfmt, 1);
			_lcd_capable &= ~READ_24BITS;
			is555 = 1;
		}
	}
#endif
}

uint16_t readID(void) {
	uint16_t ret, ret2;
	uint8_t msb;
	ret = readReg(0, 0);           //forces a reset() if called before begin()
	if (ret == 0x5408)          //the SPFD5408 fails the 0xD3D3 test.
		return 0x5408;
	if (ret == 0x5420)          //the SPFD5420 fails the 0xD3D3 test.
		return 0x5420;
	if (ret == 0x8989)          //SSD1289 is always 8989
		return 0x1289;
	ret = readReg(0x67, 0);        //HX8347-A
	if (ret == 0x4747)
		return 0x8347;
//#if defined(SUPPORT_1963) && USING_16BIT_BUS
	ret = readReg32(0xA1);      //SSD1963: [01 57 61 01]
	if (ret == 0x6101)
		return 0x1963;
	if (ret == 0xFFFF)          //R61526: [xx FF FF FF]
		return 0x1526;          //subsequent begin() enables Command Access
//    if (ret == 0xFF00)          //R61520: [xx FF FF 00]
//        return 0x1520;          //subsequent begin() enables Command Access
//#endif
	ret = readReg40(0xBF);
	if (ret == 0x8357)          //HX8357B: [xx 01 62 83 57 FF]
		return 0x8357;
	if (ret == 0x9481)          //ILI9481: [xx 02 04 94 81 FF]
		return 0x9481;
	if (ret == 0x1511)          //?R61511: [xx 02 04 15 11] not tested yet
		return 0x1511;
	if (ret == 0x1520)          //?R61520: [xx 01 22 15 20]
		return 0x1520;
	if (ret == 0x1526)          //?R61526: [xx 01 22 15 26]
		return 0x1526;
	if (ret == 0x1581)          //R61581:  [xx 01 22 15 81]
		return 0x1581;
	if (ret == 0x1400)          //?RM68140:[xx FF 68 14 00] not tested yet
		return 0x6814;
	ret = readReg32(0xD4);
	if (ret == 0x5310)          //NT35310: [xx 01 53 10]
		return 0x5310;
	ret = readReg32(0xD7);
	if (ret == 0x8031) //weird unknown from BangGood [xx 20 80 31] PrinceCharles
		return 0x8031;
	ret = readReg40(0xEF);      //ILI9327: [xx 02 04 93 27 FF]
	if (ret == 0x9327)
		return 0x9327;
	ret = readReg32(0xFE) >> 8; //weird unknown from BangGood [04 20 53]
	if (ret == 0x2053)
		return 0x2053;
	uint32_t ret32 = readReg32(0x04);
	msb = ret32 >> 16;
	ret = ret32;
//    if (msb = 0x38 && ret == 0x8000) //unknown [xx 38 80 00] with D3 = 0x1602
	if (msb == 0x00 && ret == 0x8000) { //HX8357-D [xx 00 80 00]
#if 1
		uint8_t cmds[] = { 0xFF, 0x83, 0x57 };
		pushCommand(0xB9, cmds, 3);
		msb = readReg(0xD0, 0);
		if (msb == 0x99)
			return 0x0099; //HX8357-D from datasheet
		if (msb == 0x90)        //HX8357-C undocumented
#endif
			return 0x9090;      //BIG CHANGE: HX8357-D was 0x8357
	}
//    if (msb == 0xFF && ret == 0xFFFF) //R61526 [xx FF FF FF]
//        return 0x1526;          //subsequent begin() enables Command Access
	if (ret == 0x1526)          //R61526 [xx 06 15 26] if I have written NVM
		return 0x1526;          //subsequent begin() enables Command Access
	if (ret == 0x89F0)          //ST7735S: [xx 7C 89 F0]
		return 0x7735;
	if (ret == 0x8552)          //ST7789V: [xx 85 85 52]
		return 0x7789;
	if (ret == 0xAC11)          //?unknown [xx 61 AC 11]
		return 0xAC11;
	ret32 = readReg32(0xD3);      //[xx 91 63 00]
	ret = ret32 >> 8;
	if (ret == 0x9163)
		return ret;
	ret = readReg32(0xD3);      //for ILI9488, 9486, 9340, 9341
	msb = ret >> 8;
	if (msb == 0x93 || msb == 0x94 || msb == 0x98 || msb == 0x77 || msb == 0x16)
		return ret;             //0x9488, 9486, 9340, 9341, 7796
	if (ret == 0x00D3 || ret == 0xD3D3)
		return ret;             //16-bit write-only bus
	/*
	 msb = 0x12;                 //read 3rd,4th byte.  does not work in parallel
	 pushCommand(0xD9, &msb, 1);
	 ret2 = readReg(0xD3);
	 msb = 0x13;
	 pushCommand(0xD9, &msb, 1);
	 ret = (ret2 << 8) | readReg(0xD3);
	 //	if (ret2 == 0x93)
	 return ret2;
	 */
	return readReg(0, 0);          //0154, 7783, 9320, 9325, 9335, B505, B509
}

// independent cursor and window registers.   S6D0154, ST7781 increments.  ILI92320/5 do not.
int16_t readGRAM(int16_t x, int16_t y, uint16_t *block, int16_t w, int16_t h) {
	uint16_t ret, dummy, _MR = _MW;
	int16_t n = w * h, row = 0, col = 0;
	uint8_t r, g, b, tmp;
	if (!is8347 && (_lcd_capable & MIPI_DCS_REV1)) // HX8347 uses same register
		_MR = 0x2E;
	if (_lcd_ID == 0x1602)
		_MR = 0x2E;
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	while (n > 0) {
		if (!(_lcd_capable & MIPI_DCS_REV1)) {
			WriteCmdData(_MC, x + col);
			WriteCmdData(_MP, y + row);
		}
		CS_ACTIVE;
		WriteCmd(_MR);
		setReadDir();
		if (_lcd_capable & READ_NODUMMY) {
			;
		} else if ((_lcd_capable & MIPI_DCS_REV1) || _lcd_ID == 0x1289) {
			READ_8(r);
		} else {
			READ_16(dummy);
		}
		if (_lcd_ID == 0x1511)
			READ_8(r);   //extra dummy for R61511
		while (n) {
			if (_lcd_capable & READ_24BITS) {
				READ_8(r);
				READ_8(g);
				READ_8(b);
				if (_lcd_capable & READ_BGR)
					ret = color565(b, g, r);
				else
					ret = color565(r, g, b);
			} else {
				READ_16(ret);
				if (_lcd_capable & READ_LOWHIGH)
					ret = (ret >> 8) | (ret << 8);
				if (_lcd_capable & READ_BGR)
					ret = (ret & 0x07E0) | (ret >> 11) | (ret << 11);
			}
#if defined(SUPPORT_9488_555)
			if (is555)
				ret = color555_to_565(ret);
#endif
			*block++ = ret;
			n--;
			if (!(_lcd_capable & AUTO_READINC))
				break;
		}
		if (++col >= w) {
			col = 0;
			if (++row >= h)
				row = 0;
		}
		RD_IDLE;
		CS_IDLE;
		setWriteDir();
	}
	if (!(_lcd_capable & MIPI_DCS_REV1))
		setAddrWindow(0, 0, width() - 1, height() - 1);
	return 0;
}

void setRotation(uint8_t r) {
	uint16_t GS, SS_v, ORG, REV = _lcd_rev;
	uint8_t val, d[3];
	rotation = r & 3; // just perform the operation ourselves on the protected variables
	_width = (rotation & 1) ? HEIGHT : WIDTH;
	_height = (rotation & 1) ? WIDTH : HEIGHT;
	switch (rotation) {
	case 0:                    //PORTRAIT:
		val = 0x48;             //MY=0, MX=1, MV=0, ML=0, BGR=1
		break;
	case 1:                    //LANDSCAPE: 90 degrees
		val = 0x28;             //MY=0, MX=0, MV=1, ML=0, BGR=1
		break;
	case 2:                    //PORTRAIT_REV: 180 degrees
		val = 0x98;             //MY=1, MX=0, MV=0, ML=1, BGR=1
		break;
	case 3:                    //LANDSCAPE_REV: 270 degrees
		val = 0xF8;             //MY=1, MX=1, MV=1, ML=1, BGR=1
		break;
	}
	if (_lcd_capable & INVERT_GS)
		val ^= 0x80;
	if (_lcd_capable & INVERT_SS)
		val ^= 0x40;
	if (_lcd_capable & INVERT_RGB)
		val ^= 0x08;
	if (_lcd_capable & MIPI_DCS_REV1) {
		if (_lcd_ID == 0x6814) {  //.kbv my weird 0x9486 might be 68140
			GS = (val & 0x80) ? (1 << 6) : 0;   //MY
			SS_v = (val & 0x40) ? (1 << 5) : 0;   //MX
			val &= 0x28;        //keep MV, BGR, MY=0, MX=0, ML=0
			d[0] = 0;
			d[1] = GS | SS_v | 0x02;      //MY, MX
			d[2] = 0x3B;
			WriteCmdParamN(0xB6, 3, d);
			goto common_MC;
		} else if (_lcd_ID == 0x1963 || _lcd_ID == 0x9481
				|| _lcd_ID == 0x1511) {
			if (val & 0x80)
				val |= 0x01;    //GS
			if ((val & 0x40))
				val |= 0x02;    //SS
			if (_lcd_ID == 0x1963)
				val &= ~0xC0;
			if (_lcd_ID == 0x9481)
				val &= ~0xD0;
			if (_lcd_ID == 0x1511) {
				val &= ~0x10;   //remove ML
				val |= 0xC0;    //force penguin 180 rotation
			}
//            val &= (_lcd_ID == 0x1963) ? ~0xC0 : ~0xD0; //MY=0, MX=0 with ML=0 for ILI9481
			goto common_MC;
		} else if (is8347) {
			_MC = 0x02, _MP = 0x06, _MW = 0x22, _SC = 0x02, _EC = 0x04, _SP =
					0x06, _EP = 0x08;
			if (_lcd_ID == 0x0065) {             //HX8352-B
				val |= 0x01;    //GS=1
				if ((val & 0x10))
					val ^= 0xD3;  //(ML) flip MY, MX, ML, SS, GS
				if (r & 1)
					_MC = 0x82, _MP = 0x80;
				else
					_MC = 0x80, _MP = 0x82;
			}
			if (_lcd_ID == 0x5252) {             //HX8352-A
				val |= 0x02;   //VERT_SCROLLON
				if ((val & 0x10))
					val ^= 0xD4;  //(ML) flip MY, MX, SS. GS=1
			}
			goto common_BGR;
		}
		common_MC: _MC = 0x2A, _MP = 0x2B, _MW = 0x2C, _SC = 0x2A, _EC = 0x2A, _SP =
				0x2B, _EP = 0x2B;
		common_BGR: WriteCmdParamN(is8347 ? 0x16 : 0x36, 1, &val);
		_lcd_madctl = val;
//	    if (_lcd_ID	== 0x1963) WriteCmdParamN(0x13, 0, NULL);   //NORMAL mode
	}
	// cope with 9320 variants
	else {
		switch (_lcd_ID) {

		case 0x5420:
		case 0x7793:
		case 0x9326:
		case 0xB509:
			_MC = 0x200, _MP = 0x201, _MW = 0x202, _SC = 0x210, _EC = 0x211, _SP =
					0x212, _EP = 0x213;
			GS = (val & 0x80) ? (1 << 15) : 0;
			uint16_t NL;
			NL = ((432 / 8) - 1) << 9;
			if (_lcd_ID == 0x9326 || _lcd_ID == 0x5420)
				NL >>= 1;
			WriteCmdData(0x400, GS | NL);
			goto common_SS;
		default:
			_MC = 0x20, _MP = 0x21, _MW = 0x22, _SC = 0x50, _EC = 0x51, _SP =
					0x52, _EP = 0x53;
			GS = (val & 0x80) ? (1 << 15) : 0;
			WriteCmdData(0x60, GS | 0x2700);    // Gate Scan Line (0xA700)
			common_SS: SS_v = (val & 0x40) ? (1 << 8) : 0;
			WriteCmdData(0x01, SS_v);     // set Driver Output Control
			common_ORG: ORG = (val & 0x20) ? (1 << 3) : 0;

			if (val & 0x08)
				ORG |= 0x1000;  //BGR
			_lcd_madctl = ORG | 0x0030;
			WriteCmdData(0x03, _lcd_madctl); // set GRAM write direction and BGR=1.
			break;

		}
	}
	if ((rotation & 1) && ((_lcd_capable & MV_AXIS) == 0)) {
		uint16_t x;
		x = _MC, _MC = _MP, _MP = x;
		x = _SC, _SC = _SP, _SP = x;    //.kbv check 0139
		x = _EC, _EC = _EP, _EP = x;    //.kbv check 0139
	}
	setAddrWindow(0, 0, width() - 1, height() - 1);
	vertScroll(0, HEIGHT, 0);   //reset scrolling after a rotation
}

void drawPixel(int16_t x, int16_t y, uint16_t color) {
	// MCUFRIEND just plots at edge if you try to write outside of the box:
	if (x < 0 || y < 0 || x >= width() || y >= height())
		return;
#if defined(SUPPORT_9488_555)
	if (is555)
		color = color565_to_555(color);
#endif
	setAddrWindow(x, y, x, y);
//    CS_ACTIVE; WriteCmd(_MW); write16(color); CS_IDLE; //-0.01s +98B
	if (is9797) {
		CS_ACTIVE;
		WriteCmd(_MW);
		write24(color);
		CS_IDLE;
	} else
		WriteCmdData(_MW, color);
}

void setAddrWindow(int16_t x, int16_t y, int16_t x1, int16_t y1) {
#if defined(OFFSET_9327)
	if (_lcd_ID == 0x9327) {
		if (rotation == 2)
			y += OFFSET_9327, y1 += OFFSET_9327;
		if (rotation == 3)
			x += OFFSET_9327, x1 += OFFSET_9327;
	}
#endif
#if 1
	if (_lcd_ID == 0x1526 && (rotation & 1)) {
		int16_t dx = x1 - x, dy = y1 - y;
		if (dy == 0) {
			y1++;
		} else if (dx == 0) {
			x1 += dy;
			y1 -= dy;
		}
	}
#endif
	if (_lcd_capable & MIPI_DCS_REV1) {
		WriteCmdParam4(_SC, x >> 8, x, x1 >> 8, x1); //Start column instead of _MC
		WriteCmdParam4(_SP, y >> 8, y, y1 >> 8, y1);   //
		if (is8347 && _lcd_ID == 0x0065) {      //HX8352-B has separate _MC, _SC
			uint8_t d[2];
			d[0] = x >> 8;
			d[1] = x;
			WriteCmdParamN(_MC, 2, d);                 //allows !MV_AXIS to work
			d[0] = y >> 8;
			d[1] = y;
			WriteCmdParamN(_MP, 2, d);
		}
	} else {
		WriteCmdData(_MC, x);
		WriteCmdData(_MP, y);
		if (!(x == x1 && y == y1)) {  //only need MC,MP for drawPixel
			if (_lcd_capable & XSA_XEA_16BIT) {
				if (rotation & 1)
					y1 = y = (y1 << 8) | y;
				else
					x1 = x = (x1 << 8) | x;
			}
			WriteCmdData(_SC, x);
			WriteCmdData(_SP, y);
			WriteCmdData(_EC, x1);
			WriteCmdData(_EP, y1);
		}
	}
}

void vertScroll(int16_t top, int16_t scrollines, int16_t offset) {
#if defined(OFFSET_9327)
	if (_lcd_ID == 0x9327) {
		if (rotation == 2 || rotation == 3)
			top += OFFSET_9327;
	}
#endif
	int16_t bfa = HEIGHT - top - scrollines;  // bottom fixed area
	int16_t vsp;
	int16_t sea = top;
	if (_lcd_ID == 0x9327)
		bfa += 32;
	if (offset <= -scrollines || offset >= scrollines)
		offset = 0; //valid scroll
	vsp = top + offset; // vertical start position
	if (offset < 0)
		vsp += scrollines;          //keep in unsigned range
	sea = top + scrollines - 1;
	if (_lcd_capable & MIPI_DCS_REV1) {
		uint8_t d[6];           // for multi-byte parameters
		d[0] = top >> 8;        //TFA
		d[1] = top;
		d[2] = scrollines >> 8; //VSA
		d[3] = scrollines;
		d[4] = bfa >> 8;        //BFA
		d[5] = bfa;
		WriteCmdParamN(is8347 ? 0x0E : 0x33, 6, d);
//        if (offset == 0 && rotation > 1) vsp = top + scrollines;   //make non-valid
		d[0] = vsp >> 8;        //VSP
		d[1] = vsp;
		WriteCmdParamN(is8347 ? 0x14 : 0x37, 2, d);
		if (is8347) {
			d[0] = (offset != 0) ? (_lcd_ID == 0x8347 ? 0x02 : 0x08) : 0;
			WriteCmdParamN(_lcd_ID == 0x8347 ? 0x18 : 0x01, 1, d);  //HX8347-D
		} else if (offset == 0 && (_lcd_capable & MIPI_DCS_REV1)) {
			WriteCmdParamN(0x13, 0, NULL);    //NORMAL i.e. disable scroll
		}
		return;
	}
	// cope with 9320 style variants:
	switch (_lcd_ID) {
	case 0x7783:
		WriteCmdData(0x61, _lcd_rev);   //!NDL, !VLE, REV
		WriteCmdData(0x6A, vsp);        //VL#
		break;
#ifdef SUPPORT_0139
    case 0x0139:
        WriteCmdData(0x07, 0x0213 | (_lcd_rev << 2));  //VLE1=1, GON=1, REV=x, D=3
        WriteCmdData(0x41, vsp);  //VL# check vsp
        break;
#endif
#if defined(SUPPORT_0154) || defined(SUPPORT_9225)  //thanks tongbajiel
    case 0x9225:
	case 0x0154:
        WriteCmdData(0x31, sea);        //SEA
        WriteCmdData(0x32, top);        //SSA
        WriteCmdData(0x33, vsp - top);  //SST
        break;
#endif
#ifdef SUPPORT_1289
    case 0x1289:
        WriteCmdData(0x41, vsp);        //VL#
        break;
#endif
	case 0x5420:
	case 0x7793:
	case 0x9326:
	case 0xB509:
		WriteCmdData(0x401, (1 << 1) | _lcd_rev);       //VLE, REV
		WriteCmdData(0x404, vsp);       //VL#
		break;
	default:
		// 0x6809, 0x9320, 0x9325, 0x9335, 0xB505 can only scroll whole screen
		WriteCmdData(0x61, (1 << 1) | _lcd_rev);        //!NDL, VLE, REV
		WriteCmdData(0x6A, vsp);        //VL#
		break;
	}
}

void pushColors16b(uint16_t *block, int16_t n, uint8_t first) {
	pushColors_any(_MW, (uint8_t*) block, n, first, 0);
}
void pushColors8b(uint8_t *block, int16_t n, uint8_t first) {
	pushColors_any(_MW, (uint8_t*) block, n, first, 2);   //regular bigend
}
void pushColors4n(const uint8_t *block, int16_t n, uint8_t first,
		uint8_t bigend) {
	pushColors_any(_MW, (uint8_t*) block, n, first, bigend ? 3 : 1);
}

void fillScreen(uint16_t color) {
	fillRect(0, 0, _width, _height, color);
}

void invertDisplay(uint8_t i) {
	uint8_t val;
	_lcd_rev = ((_lcd_capable & REV_SCREEN) != 0) ^ i;
	if (_lcd_capable & MIPI_DCS_REV1) {
		if (is8347) {
			// HX8347D: 0x36 Panel Characteristic. REV_Panel
			// HX8347A: 0x36 is Display Control 10
			if (_lcd_ID == 0x8347 || _lcd_ID == 0x5252) // HX8347-A, HX5352-A
				val = _lcd_rev ? 6 : 2;       //INVON id bit#2,  NORON=bit#1
			else
				val = _lcd_rev ? 8 : 10; //HX8347-D, G, I: SCROLLON=bit3, INVON=bit1
			// HX8347: 0x01 Display Mode has diff bit mapping for A, D
			WriteCmdParamN(0x01, 1, &val);
		} else
			WriteCmdParamN(_lcd_rev ? 0x21 : 0x20, 0, NULL);
		return;
	}
	// cope with 9320 style variants:
	switch (_lcd_ID) {
#ifdef SUPPORT_0139
    case 0x0139:
#endif
	case 0x9225:                              //REV is in reg(0x07) like Samsung
	case 0x0154:
		WriteCmdData(0x07, 0x13 | (_lcd_rev << 2));     //.kbv kludge
		break;
#ifdef SUPPORT_1289
    case 0x1289:
        _lcd_drivOut &= ~(1 << 13);
        if (_lcd_rev)
            _lcd_drivOut |= (1 << 13);
        WriteCmdData(0x01, _lcd_drivOut);
        break;
#endif
	case 0x5420:
	case 0x7793:
	case 0x9326:
	case 0xB509:
		WriteCmdData(0x401, (1 << 1) | _lcd_rev);       //.kbv kludge VLE
		break;
	default:
		WriteCmdData(0x61, _lcd_rev);
		break;
	}
}

void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
	fillRect(x, y, 1, h, color);
}
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
	fillRect(x, y, w, 1, color);
}

void writePixel(int16_t x, int16_t y, uint16_t color) {
	drawPixel(x, y, color);
}

void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		_swap_int16_t(x0, y0);
		_swap_int16_t(x1, y1);
	}

	if (x0 > x1) {
		_swap_int16_t(x0, x1);
		_swap_int16_t(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			writePixel(y0, x0, color);
		} else {
			writePixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
	if (x0 == x1) {
		if (y0 > y1)
			_swap_int16_t(y0, y1);
		drawFastVLine(x0, y0, y1 - y0 + 1, color);
	} else if (y0 == y1) {
		if (x0 > x1)
			_swap_int16_t(x0, x1);
		drawFastHLine(x0, y0, x1 - x0 + 1, color);
	} else {
		writeLine(x0, y0, x1, y1, color);
	}
}

void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	writePixel(x0, y0 + r, color);
	writePixel(x0, y0 - r, color);
	writePixel(x0 + r, y0, color);
	writePixel(x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		writePixel(x0 + x, y0 + y, color);
		writePixel(x0 - x, y0 + y, color);
		writePixel(x0 + x, y0 - y, color);
		writePixel(x0 - x, y0 - y, color);
		writePixel(x0 + y, y0 + x, color);
		writePixel(x0 - y, y0 + x, color);
		writePixel(x0 + y, y0 - x, color);
		writePixel(x0 - y, y0 - x, color);
	}
}

void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			writePixel(x0 + x, y0 + y, color);
			writePixel(x0 + y, y0 + x, color);
		}
		if (cornername & 0x2) {
			writePixel(x0 + x, y0 - y, color);
			writePixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			writePixel(x0 - y, y0 + x, color);
			writePixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			writePixel(x0 - y, y0 - x, color);
			writePixel(x0 - x, y0 - y, color);
		}
	}
}

void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	drawFastVLine(x0, y0 - r, 2 * r + 1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}

void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners,
		int16_t delta, uint16_t color) {

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;
	int16_t px = x;
	int16_t py = y;

	delta++; // Avoid some +1's in the loop

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		// These checks avoid double-drawing certain lines, important
		// for the SSD1306 library which has an INVERT drawing mode.
		if (x < (y + 1)) {
			if (corners & 1)
				drawFastVLine(x0 + x, y0 - y, 2 * y + delta, color);
			if (corners & 2)
				drawFastVLine(x0 - x, y0 - y, 2 * y + delta, color);
		}
		if (y != py) {
			if (corners & 1)
				drawFastVLine(x0 + py, y0 - px, 2 * px + delta, color);
			if (corners & 2)
				drawFastVLine(x0 - py, y0 - px, 2 * px + delta, color);
			py = y;
		}
		px = x;
	}
}

void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y + h - 1, w, color);
	drawFastVLine(x, y, h, color);
	drawFastVLine(x + w - 1, y, h, color);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	int16_t end;
#if defined(SUPPORT_9488_555)
	if (is555)
		color = color565_to_555(color);
#endif
	if (w < 0) {
		w = -w;
		x -= w;
	}                           //+ve w
	end = x + w;
	if (x < 0)
		x = 0;
	if (end > width())
		end = width();
	w = end - x;
	if (h < 0) {
		h = -h;
		y -= h;
	}                           //+ve h
	end = y + h;
	if (y < 0)
		y = 0;
	if (end > height())
		end = height();
	h = end - y;
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	CS_ACTIVE;
	WriteCmd(_MW);
	if (h > w) {
		end = h;
		h = w;
		w = end;
	}
	uint8_t hi = color >> 8, lo = color & 0xFF;
	while (h-- > 0) {
		end = w;

		do {
			write8(hi);
			write8(lo);
		} while (--end != 0);
	}
	CS_IDLE;
	if (!(_lcd_capable & MIPI_DCS_REV1)
			|| ((_lcd_ID == 0x1526) && (rotation & 1)))
		setAddrWindow(0, 0, width() - 1, height() - 1);
}

void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,
		uint16_t color) {
	int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
	if (r > max_radius)
		r = max_radius;
	// smarter version
	drawFastHLine(x + r, y, w - 2 * r, color); // Top
	drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
	drawFastVLine(x, y + r, h - 2 * r, color); // Left
	drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
	// draw four corners
	drawCircleHelper(x + r, y + r, r, 1, color);
	drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
	drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
	drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,
		uint16_t color) {
	int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
	if (r > max_radius)
		r = max_radius;
	// smarter version
	fillRect(x + r, y, w - 2 * r, h, color);
	// draw four corners
	fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
	fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
		int16_t y2, uint16_t color) {
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}

void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
		int16_t y2, uint16_t color) {

	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		_swap_int16_t(y0, y1);
		_swap_int16_t(x0, x1);
	}
	if (y1 > y2) {
		_swap_int16_t(y2, y1);
		_swap_int16_t(x2, x1);
	}
	if (y0 > y1) {
		_swap_int16_t(y0, y1);
		_swap_int16_t(x0, x1);
	}

	if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;
		drawFastHLine(a, y0, b - a + 1, color);
		return;
	}

	int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
			dx12 = x2 - x1, dy12 = y2 - y1;
	int32_t sa = 0, sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
		last = y1;   // Include y1 scanline
	else
		last = y1 - 1; // Skip it

	for (y = y0; y <= last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		 a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		 b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			_swap_int16_t(a, b);
		drawFastHLine(a, y, b - a + 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = (int32_t) dx12 * (y - y1);
	sb = (int32_t) dx02 * (y - y0);
	for (; y <= y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		 a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		 b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			_swap_int16_t(a, b);
		drawFastHLine(a, y, b - a + 1, color);
	}
}

void setTextWrap(uint8_t w) {
	wrap = w;
}

void setTextColor(uint16_t color) {
	textcolor = color;
}

void setTextSize(uint8_t size) {
	textsize = size;
}

void setCursor(int16_t x, int16_t y) {
	cursor_x = x;
	cursor_y = y;
}

uint8_t getRotation(void) {
	return rotation;
}

void scrollup(uint16_t speed) {
	uint16_t maxscroll;
	if (getRotation() & 1)
		maxscroll = width();
	else
		maxscroll = height();
	for (uint16_t i = 1; i <= maxscroll; i++) {
		vertScroll(0, maxscroll, i);
		if (speed < 655)
			delay(speed * 100);
		else
			HAL_Delay(speed);
	}

}

void scrolldown(uint16_t speed) {
	uint16_t maxscroll;
	if (getRotation() & 1)
		maxscroll = width();
	else
		maxscroll = height();
	for (uint16_t i = 1; i <= maxscroll; i++) {
		vertScroll(0, maxscroll, 0 - (int16_t) i);
		if (speed < 655)
			delay(speed * 100);
		else
			HAL_Delay(speed);
	}
}
