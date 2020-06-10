
#include "oled.h"
#include <math.h>

float light_sensor;


uint8_t oled_init()
{
	return sd1306_init();
}


void update_display(float fs, float ls, float rs, float bs, int disp_flag)
{
	uint32_t i;
	uint8_t string_data[10];

	if (disp_flag == 1) {
		// Clear OLED buffer
		clear_oled_buffer();

		// Draw a label at line 1
		sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, "Most Secure", FONT_SIZE_LINE, white_pixel);
		// Draw the title
		sd1306_draw_string(OLED_LINE_1_X, OLED_LINE_1_Y, "SUPER CLEAN", FONT_SIZE_TITLE, white_pixel);
		sd1306_draw_string(OLED_LINE_3_X + 40, OLED_LINE_3_Y, "BOT", FONT_SIZE_TITLE, white_pixel);
		// Draw the title
		sd1306_draw_string(OLED_LINE_5_X, OLED_LINE_5_Y, "AzureSphere Powered", FONT_SIZE_LINE, white_pixel);
		// Send the buffer to OLED RAM
		sd1306_refresh();

	}

	else if (disp_flag == 2) {
		
		// Strings for labels
		uint8_t str_sF[] = "Front:";
		uint8_t str_sL[] = "Left :";
		uint8_t str_sR[] = "Right:";
		uint8_t str_sB[] = "Rear:";


		// Clear OLED buffer
		clear_oled_buffer(); 

		// Draw the title
		sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, "Sensor", FONT_SIZE_TITLE, white_pixel);


		ftoa(fs, string_data, 2);

		// Draw a label at line 1
		sd1306_draw_string(OLED_LINE_1_X, OLED_LINE_1_Y, str_sF, FONT_SIZE_LINE, white_pixel);
		// Draw the value of x
		sd1306_draw_string(sizeof(str_sF) * 6, OLED_LINE_1_Y, string_data, FONT_SIZE_LINE, white_pixel);
		// Draw the units of x
		sd1306_draw_string(sizeof(str_sF) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_1_Y, "cm", FONT_SIZE_LINE, white_pixel);


		ftoa(ls, string_data, 2);

		// Draw a label at line 2
		sd1306_draw_string(OLED_LINE_2_X, OLED_LINE_2_Y, str_sL, FONT_SIZE_LINE, white_pixel);
		// Draw the value of y
		sd1306_draw_string(sizeof(str_sL) * 6, OLED_LINE_2_Y, string_data, FONT_SIZE_LINE, white_pixel);
		// Draw the units of y
		sd1306_draw_string(sizeof(str_sL) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_2_Y, "cm", FONT_SIZE_LINE, white_pixel);


		ftoa(rs, string_data, 2);

		// Draw a label at line 3
		sd1306_draw_string(OLED_LINE_3_X, OLED_LINE_3_Y, str_sR, FONT_SIZE_LINE, white_pixel);
		// Draw the value of z
		sd1306_draw_string(sizeof(str_sR) * 6, OLED_LINE_3_Y, string_data, FONT_SIZE_LINE, white_pixel);
		// Draw the units of z
		sd1306_draw_string(sizeof(str_sR) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_3_Y, "cm", FONT_SIZE_LINE, white_pixel);


		ftoa(bs, string_data, 2);

		// Draw a label at line 3
		sd1306_draw_string(OLED_LINE_4_X, OLED_LINE_4_Y, str_sB, FONT_SIZE_LINE, white_pixel);
		// Draw the value of z
		sd1306_draw_string(sizeof(str_sB) * 6, OLED_LINE_4_Y, string_data, FONT_SIZE_LINE, white_pixel);
		// Draw the units of z
		sd1306_draw_string(sizeof(str_sB) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_4_Y, "cm", FONT_SIZE_LINE, white_pixel);


		// Send the buffer to OLED RAM
		sd1306_refresh();

	}

	else if (disp_flag == 3) {

		// Strings for labels
		uint8_t str_ax[] = "Axis X:";
		uint8_t str_ay[] = "Axis Y:";
		uint8_t str_az[] = "Axis Z:";

		// Clear OLED buffer
		clear_oled_buffer();

		// Draw the title
		sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, "   Accel.", FONT_SIZE_TITLE, white_pixel);

		// Convert x value to string
		ftoa(ls, string_data, 2);

		// Draw a label at line 1
		sd1306_draw_string(OLED_LINE_1_X, OLED_LINE_1_Y, str_ax, FONT_SIZE_LINE, white_pixel);
		// Draw the value of x
		sd1306_draw_string(sizeof(str_ax) * 6, OLED_LINE_1_Y, string_data, FONT_SIZE_LINE, white_pixel);
		// Draw the units of x
		sd1306_draw_string(sizeof(str_ax) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_1_Y, "mg", FONT_SIZE_LINE, white_pixel);

		// Convert y value to string
		ftoa(rs, string_data, 2);

		// Draw a label at line 2
		sd1306_draw_string(OLED_LINE_2_X, OLED_LINE_2_Y, str_ay, FONT_SIZE_LINE, white_pixel);
		// Draw the value of y
		sd1306_draw_string(sizeof(str_az) * 6, OLED_LINE_2_Y, string_data, FONT_SIZE_LINE, white_pixel);
		// Draw the units of y
		sd1306_draw_string(sizeof(str_ay) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_2_Y, "mg", FONT_SIZE_LINE, white_pixel);

		// Convert z value to string
		ftoa(bs, string_data, 2);

		// Draw a label at line 3
		sd1306_draw_string(OLED_LINE_3_X, OLED_LINE_3_Y, str_az, FONT_SIZE_LINE, white_pixel);
		// Draw the value of z
		sd1306_draw_string(sizeof(str_az) * 6, OLED_LINE_3_Y, string_data, FONT_SIZE_LINE, white_pixel);
		// Draw the units of z
		sd1306_draw_string(sizeof(str_az) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_3_Y, "mg", FONT_SIZE_LINE, white_pixel);

		// Send the buffer to OLED RAM
		sd1306_refresh();
	}

	else if (disp_flag == 4) {


	// Strings for labels
	uint8_t str_gx[] = "GX:";
	uint8_t str_gy[] = "GY:";
	uint8_t str_gz[] = "GZ:";

	// Clear OLED buffer
	clear_oled_buffer();

	// Draw the title
	sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, "   Gyro.", FONT_SIZE_TITLE, white_pixel);

	// Convert x value to string
	ftoa(ls, string_data, 2);

	// Draw a label at line 1
	sd1306_draw_string(OLED_LINE_1_X, OLED_LINE_1_Y, str_gx, FONT_SIZE_LINE, white_pixel);
	// Draw the value of x
	sd1306_draw_string(sizeof(str_gx) * 6, OLED_LINE_1_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of x
	sd1306_draw_string(sizeof(str_gx) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_1_Y, "dps", FONT_SIZE_LINE, white_pixel);

	// Convert y value to string
	ftoa(rs, string_data, 2);

	// Draw a label at line 2
	sd1306_draw_string(OLED_LINE_2_X, OLED_LINE_2_Y, str_gy, FONT_SIZE_LINE, white_pixel);
	// Draw the value of y
	sd1306_draw_string(sizeof(str_gy) * 6, OLED_LINE_2_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of y
	sd1306_draw_string(sizeof(str_gy) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_2_Y, "dps", FONT_SIZE_LINE, white_pixel);

	// Convert z value to string
	ftoa(bs, string_data, 2);

	// Draw a label at line 3
	sd1306_draw_string(OLED_LINE_3_X, OLED_LINE_3_Y, str_gz, FONT_SIZE_LINE, white_pixel);
	// Draw the value of z
	sd1306_draw_string(sizeof(str_gz) * 6, OLED_LINE_3_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of z
	sd1306_draw_string(sizeof(str_gz) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_3_Y, "dps", FONT_SIZE_LINE, white_pixel);

	// Send the buffer to OLED RAM
	sd1306_refresh();
	
	}

	else if (disp_flag == 5) {

	// Strings for labels
	uint8_t str_temp1[] = "Temp1:";
	uint8_t str_temp2[] = "Temp2:";
	uint8_t str_atm[] = "Barom:";
	uint8_t str_altitude[] = "Elev :";

	// Clear OLED buffer
	clear_oled_buffer();

	// Draw the title
	sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, "  Environ.", FONT_SIZE_TITLE, white_pixel);

	// Convert temp1 value to string
	ftoa(fs, string_data, 2);

	// Draw a label at line 1
	sd1306_draw_string(OLED_LINE_1_X, OLED_LINE_1_Y, str_temp1, FONT_SIZE_LINE, white_pixel);
	// Draw the value of temp1
	sd1306_draw_string(sizeof(str_temp1) * 6, OLED_LINE_1_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of temp1
	sd1306_draw_string(sizeof(str_temp1) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_1_Y, "°C", FONT_SIZE_LINE, white_pixel);

	// Convert temp2 value to string
	ftoa(ls, string_data, 2);

	// Draw a label at line 2
	sd1306_draw_string(OLED_LINE_2_X, OLED_LINE_2_Y, str_temp2, FONT_SIZE_LINE, white_pixel);
	// Draw the value of temp2
	sd1306_draw_string(sizeof(str_temp2) * 6, OLED_LINE_2_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the value of temp2
	sd1306_draw_string(sizeof(str_temp2) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_2_Y, "°C", FONT_SIZE_LINE, white_pixel);

	// Convert atm value to string
	ftoa(rs, string_data, 2);

	// Draw a label at line 3
	sd1306_draw_string(OLED_LINE_3_X, OLED_LINE_3_Y, str_atm, FONT_SIZE_LINE, white_pixel);
	// Draw the value of atm
	sd1306_draw_string(sizeof(str_atm) * 6, OLED_LINE_3_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of atm
	sd1306_draw_string(sizeof(str_atm) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_3_Y, "hPa", FONT_SIZE_LINE, white_pixel);

	// Convert altitude value to string
	ftoa(bs, string_data, 2);

	// Draw a label at line 4
	sd1306_draw_string(OLED_LINE_4_X, OLED_LINE_4_Y, str_altitude, FONT_SIZE_LINE, white_pixel);
	// Draw the value of altitude
	sd1306_draw_string(sizeof(str_altitude) * 6, OLED_LINE_4_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of altitude
	sd1306_draw_string(sizeof(str_altitude) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_4_Y, "m", FONT_SIZE_LINE, white_pixel);

	// Send the buffer to OLED RAM
	sd1306_refresh();

	}

	else if (disp_flag == 6) {

	// Strings for labels
	uint8_t str_light[] = "  Light:";
	/*uint8_t str_tbd1[] = "TBD 1:";
	uint8_t str_tbd2[] = "TBD 2:";*/

	// Clear OLED buffer
	clear_oled_buffer();

	// Draw the title
	sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, " Visibility", FONT_SIZE_TITLE, white_pixel);

	// Convert x value to string
	ftoa(bs, string_data, 2);

	// Draw a label at line 1
	sd1306_draw_string(OLED_LINE_2_X, OLED_LINE_2_Y, str_light, FONT_SIZE_LINE, white_pixel);
	// Draw the value of x
	sd1306_draw_string(sizeof(str_light) * 6, OLED_LINE_2_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of x
	sd1306_draw_string(sizeof(str_light) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_2_Y, "Lux", FONT_SIZE_LINE, white_pixel);

	// Send the buffer to OLED RAM
	sd1306_refresh();


	}

	else if (disp_flag == 7) {

	// Clear OLED buffer
	clear_oled_buffer();

	// Draw a label at line 1
	sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, "Auto Mode", FONT_SIZE_LINE, white_pixel);
	// Draw the title
	sd1306_draw_string(OLED_LINE_1_X, OLED_LINE_1_Y, "Sanitizing", FONT_SIZE_TITLE, white_pixel);

	ftoa(bs, string_data, 2);
	sd1306_draw_string(OLED_LINE_3_X + 5, OLED_LINE_3_Y, string_data, FONT_SIZE_TITLE, white_pixel);
	// Draw the title

	sd1306_draw_string((get_str_size(string_data) + 1) * 11, OLED_LINE_4_Y, "sec left", FONT_SIZE_LINE, white_pixel);

	sd1306_draw_string(OLED_LINE_5_X, OLED_LINE_5_Y, "Press Reset to stop", FONT_SIZE_LINE, white_pixel);
	// Send the buffer to OLED RAM
	sd1306_refresh();

	}

	
}


/**
  * @brief  Template to show a logo
  * @param  None.
  * @retval None.
  */
void oled_draw_logo(void)
{
	// Copy image_avnet to OLED buffer
	sd1306_draw_img(Image_SuperCleanBot_logo);

	// Send the buffer to OLED RAM
	sd1306_refresh();
}

// reverses a string 'str' of length 'len' 
static void reverse(uint8_t *str, int32_t len)
{
	int32_t i = 0;
	int32_t j = len - 1;
	int32_t temp;

	while (i < j)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++; j--;
	}
}

/**
  * @brief  Converts a given integer x to string uint8_t[]
  * @param  x: x integer input
  * @param  str: uint8_t array output
  * @param  d: Number of zeros added
  * @retval i: number of digits
  */
int32_t intToStr(int32_t x, uint8_t str[], int32_t d)
{
	int32_t i = 0;
	uint8_t flag_neg = 0;

	if (x < 0)
	{
		flag_neg = 1;
		x *= -1;
	}
	while (x)
	{
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}

	// If number of digits required is more, then 
	// add 0s at the beginning 
	while (i < d)
	{
		str[i++] = '0';
	}

	if (flag_neg)
	{
		str[i] = '-';
		i++;
	}

	reverse(str, i);
	str[i] = '\0';
	return i;
}

/**
  * @brief  Converts a given integer x to string uint8_t[]
  * @param  n: float number to convert
  * @param  res:
  * @param  afterpoint:
  * @retval None.
  */
void ftoa(float n, uint8_t *res, int32_t afterpoint)
{
	// Extract integer part 
	int32_t ipart = (int32_t)n;

	// Extract floating part 
	float fpart = n - (float)ipart;

	int32_t i;

	if (ipart < 0)
	{
		res[0] = '-';
		res++;
		ipart *= -1;
	}

	if (fpart < 0)
	{
		fpart *= -1;

		if (ipart == 0)
		{
			res[0] = '-';
			res++;
		}
	}

	// convert integer part to string 
	i = intToStr(ipart, res, 1);

	// check for display option after point 
	if (afterpoint != 0)
	{
		res[i] = '.';  // add dot 

		// Get the value of fraction part upto given no. 
		// of points after dot. The third parameter is needed 
		// to handle cases like 233.007 
		fpart = fpart * pow(10, afterpoint);

		intToStr((int32_t)fpart, res + i + 1, afterpoint);
	}
}

// AVNET logo

const unsigned char Image_avnet_bmp[BUFFER_SIZE] =
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,128,240,240,240,240, 48,  0,  0,112,
  240,240,240,224,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,112,
  240,240,240,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,224,240,240,
  240, 16,  0,  0,  0,  0,  0,  0,  0,  0,240,240,240,240,224,128,
	0,  0,  0,  0,  0,  0,  0,  0,240,240,240,240,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,240,240,240,240,112,112,112,112,112,112,
  112,112,112,112,112,  0,  0,  0,  0,  0,  0,  0,  0,112,112,112,
  112,112,112,112,240,240,240,240,112,112,112,112,112,112,  0,  0,
	0,  0,  0,  0,  0,224,252,255,255,255, 15,  1,  0,  0,  0,  0,
	3, 15,127,255,255,248,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	7, 31,255,255,254,240,  0,  0,  0,  0,224,248,255,255,127,  7,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255, 15, 31,
  127,252,248,224,224,128,  0,  0,255,255,255,255,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,255,255,255,255,224,224,224,224,224,224,
  224,224,224,224,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,255,255,255,255,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,128,240,254,255,127, 15,  1,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  3, 31,255,255,252,224,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  7, 63,255,255,248,240,254,255,255, 31,  3,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255,  0,  0,
	0,  1,  3, 15, 15, 63,126,252,255,255,255,255,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,255,255,255,255,129,129,129,129,129,129,
  129,129,129,129,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,255,255,255,255,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  7,  7,  7,  3,  0,  0,  0, 12, 14, 14, 14, 14, 14, 14,
   14, 14, 12,  0,  0,  0,  7,  7,  7,  7,  4,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  1,  7,  7,  7,  7,  1,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,  7,  7,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  1,  3,  7,  7,  7,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
	7,  7,  7,  7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  7,  7,  7,  7,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};


// 'SuperCleanBot-Logo', 118x40px
const unsigned char Image_SuperCleanBot_logo[BUFFER_SIZE] =
{
0xcc, 0xff, 0xf7, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xc8,
0xef, 0x67, 0xdf, 0xbc, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xc0, 0x96,
0xa7, 0xc7, 0x48, 0x12, 0x25, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xd0, 0xb2, 0x27,
0xe2, 0x0b, 0x92, 0x61, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xd4, 0xb3, 0xb7, 0xfa,
0x7b, 0x92, 0x67, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xdc, 0xc4, 0x33, 0xc3, 0x08,
0xc2, 0x71, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xfc, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff,
0xff, 0xff, 0xfc, 0x80, 0x63, 0xc7, 0x00, 0xe0, 0x1c, 0x03, 0xff, 0x00, 0xc7, 0xf0, 0x0f, 0x87,
0xc3, 0xcc, 0x0c, 0x63, 0xc7, 0x00, 0x60, 0x1c, 0x01, 0xfe, 0x00, 0xc7, 0xf0, 0x0f, 0x07, 0xc1,
0xcc, 0x1f, 0xe3, 0xc7, 0x1c, 0x63, 0xfc, 0x71, 0xfe, 0x3e, 0xc7, 0xf1, 0xff, 0x03, 0xc1, 0xcc,
0x0f, 0xe3, 0xc7, 0x1c, 0x63, 0xfc, 0x71, 0xfc, 0x3f, 0xc7, 0xf1, 0xff, 0x23, 0xc0, 0xcc, 0x80,
0xe3, 0xc7, 0x1c, 0x60, 0x1c, 0x63, 0xfc, 0x7f, 0xc7, 0xf0, 0x1e, 0x23, 0xc4, 0x4c, 0xc0, 0x63,
0xc7, 0x00, 0xe0, 0x1c, 0x07, 0xfc, 0x7f, 0xc7, 0xf0, 0x1e, 0x31, 0xc4, 0x0c, 0xf0, 0x63, 0xc7,
0x01, 0xe3, 0xfc, 0x07, 0xfc, 0x7f, 0xc7, 0xf1, 0xfe, 0x01, 0xc6, 0x0c, 0xfc, 0x63, 0xc7, 0x1f,
0xe3, 0xfc, 0x43, 0xfe, 0x3f, 0xc7, 0xf1, 0xfc, 0x01, 0xc7, 0x0c, 0x3c, 0x71, 0x87, 0x1f, 0xe3,
0xfc, 0x61, 0xfe, 0x1c, 0xc7, 0xf1, 0xfc, 0x70, 0xc7, 0x0c, 0x00, 0x70, 0x0f, 0x1f, 0xe0, 0x1c,
0x71, 0xff, 0x00, 0xc0, 0x30, 0x08, 0xf8, 0xc7, 0x8c, 0x81, 0xfc, 0x1f, 0x1f, 0xe0, 0x1c, 0xf8,
0xff, 0xc1, 0xc0, 0x30, 0x08, 0xfc, 0xc7, 0x8c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xf1, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x07, 0xc0,
0x70, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x07, 0x80, 0x30,
0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xc7, 0x1e, 0x1f, 0x1c,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xc7, 0x1f, 0x1f, 0x1c, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0x1f, 0x1f, 0x1c, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x07, 0x1f, 0x1f, 0x1c, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xe3, 0x1f, 0x1f, 0x1c, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xe3, 0x1f, 0x1f, 0x1c, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xc3, 0x0e, 0x3f, 0x1c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xf8, 0x07, 0x80, 0x7f, 0x1c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xf8, 0x0f, 0xe0, 0xff, 0x3c
};

uint8_t get_str_size(uint8_t * str)
{
	uint8_t legth = 0;
	while (*(str) != NULL)
	{
		str++;
		legth++;
	}
	return legth;
}