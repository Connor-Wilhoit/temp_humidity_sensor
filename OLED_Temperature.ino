#include <U8g2lib.h>
#include "dht11.h"

dht11 DHT11;
const int DHT11PIN = 2;
//#define DHT11PIN 2

// OLED-screen setup
const int clk	= 9;
const int mosi	= 10;
const int res	= 11;
const int dc	= 12;
const int cs	= 13;
U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, clk, mosi, cs, dc, res);

double Fahrenheit(double c)
{
	return 1.8 * c + 32;
}

double Kelvin(double c)
{
	return c + 273.15;
}

double dew_point(double celcius, double humidity)
{
	double A0	= 373.15 / (273.15 + celcius);
	double sum	= -7.90298 * (A0 - 1);
	sum += 5.02808 * log10(A0);
	sum += -1.3816e-7 * (pow(10, (11.344 * (1-A0))) -  1);
	sum +=  8.1328e-3 * (pow(10, (-3.49149 * (A0-1))) - 1); 
	sum += log10(1013.246);
	double vp = pow(10, sum-3) * humidity;
	double tmp = log(vp/0.61078);
	return (241.88 * tmp) / (17.558-tmp);
}

double dew_point_fast(double celcius, double humidity)
{
	double x = 17.271;
	double y = 237.7;
	double temp = (x * celcius) / (y + celcius) + log(humidity/100);
	double xy = (y * temp) / (x - temp);
	return xy;
}


void setup(void)
{
	u8g2.begin();
	u8g2.setContrast(255);
	u8g2.setFont(u8g2_font_profont11_tr);
}

void loop(void)
{
	DHT11.read(DHT11PIN);
	u8g2.clear();
	u8g2.setFont(u8g2_font_profont11_tr);
	u8g2.firstPage();
	do
	{
		// First output is temp in degrees Celcius
		u8g2.drawStr(10, 10, "Temp(C): ");
		u8g2.setCursor(65, 10);
		u8g2.print(static_cast<double>(DHT11.temperature));
		u8g2.drawStr(100, 10, "C");
		
		// Second output is the humidity in %
		u8g2.drawStr(10, 20, "Humidity: ");
		u8g2.setCursor(70, 20);
		u8g2.print(static_cast<double>(DHT11.humidity));
		u8g2.drawStr(102, 20, "%");

		// Third output is temp in degrees Fahrenheit
		u8g2.drawStr(10, 30, "Temp(F): ");
		u8g2.setCursor(65, 30);
		u8g2.print(Fahrenheit(DHT11.temperature));
		u8g2.drawStr(100, 30, "F");
		
		// Fourth output is temp in degrees Kelvin
		u8g2.drawStr(10, 40, "Temp(K): ");
		u8g2.setCursor(65, 40);
		u8g2.print(Kelvin(DHT11.temperature));
		u8g2.drawStr(105, 40, "K");

		// Fifth and final output is of the two dew_point calculations
		u8g2.drawStr(10, 50, "Dew Point: ");
  		u8g2.setCursor(75, 50);
		u8g2.print(dew_point(DHT11.temperature, DHT11.humidity));
		u8g2.drawStr(108, 50, "oC");

  		u8g2.drawStr(10, 60, "[fast]: ");
		u8g2.setCursor(65, 60);
  		u8g2.print(dew_point_fast(DHT11.temperature, DHT11.humidity));
  		u8g2.drawStr(100, 60, "oC");
		
	} while (u8g2.nextPage());
	delay(5000);
}
