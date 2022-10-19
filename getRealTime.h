/*********************

 getRealTime.h

 11/04/19


 v1.5

 ---------------
 Description
 ---------------

 get the current time from the web server: 


 will connect to the internet via the wifi.

 request the current time based on the public IP address.

"http://worldtimeapi.org/api/ip"


 return the time



---------------
versions:
---------------

v1.5
	18/10/22
	-increase the size of the buffer in get_unixTime() for decoding json

v1.4
	04/05/22
	-add get_unixtime()

V1.3
	25/06/19
	-change wifi.disconnect() to wifi.mode(WIFI_OFF)


v1.2
	28/05/19
	-add method of returing the JSON formatted data into a structure


**********************/


#ifndef _GETREALTIME_h
#define _GETREALTIME_h

//=============================================

#include <arduino.h>

#include <configDevice.h>

#include <debug.h>


#ifdef	ESP32

	
	#include <HTTPClient.h>
	#include <WiFi.h>
	#include <ArduinoJson.h>
#endif

#ifdef		ESP8266
	#include <ESP8266WiFi.h>
	#include <ESP8266HTTPClient.h>
	#include <ArduinoJson.h>
#endif

//------------ defines -------------



//allow 1 minute to connect to wifi
#define	WIFI_RETRY		120


//--------------- debug ----------------

//use the data below instead of the data in the config file

//#define		__DEBUG_SHOW_TIME


//#define	    __DEBUG_USE_FIXED_WIFI


#define SSID_FIXED		"INTEGRALE"
#define PASSWORD_FIXED	"0penAllnight"


//-----------------------------------------

	struct timeFormat{


		char timeZone[20];
		uint16_t dayOfYear;
		char dayOfWeek[12];
		char dateValue[20];
		char timeValue[20];


	};



//---------------------------------------------

class GetRealTimeClass
{
 


 public:
	bool getTime(char *timeData, bool terminateConnection);
	bool getTime(timeFormat *timeData, bool terminateConnection);

	bool get_unixtime(int32_t *unixtime, bool terminateConnection);

	/**
	 * @brief capture the return string for debug
	 * 
	 * @param data - an array to hold the string
	 * @param length - number of bytes to capture
	 * @param terminateConnection - set true (default) if the wifi connection should be closed after task complete
	 * @return true - task complete successfully
	 * @return false -task failed to complete
	 */
	bool get_httpData(char *data, uint16_t length, bool terminateConnection);
	
private:


	


	//get the pass word and SSID from the configDevice API.
	//connect to the local wifi
	bool connectWifi(void);

	const char* timeData_hostName = "http://worldtimeapi.org/api/ip.txt";
	const int timeData_httpsPort = 80;

	const char *timeData_hostNameJSON = "http://worldtimeapi.org/api/ip";

	

};

//extern GetRealTimeClass GetRealTime;


//=============================================
#endif

