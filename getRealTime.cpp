

//----------------------------------------
// 
// v1.5
// 
//----------------------------------------




#include <Arduino.h>
#include <getRealTime.h>

#include <debug.h>


//----- CONSTANTS --------------



//------------------------------------
//GetRealTimeClass GetRealTime;
//------------------------------------

bool GetRealTimeClass::get_httpData(char *data, uint16_t length, bool terminateConnection=true){

String tempArray;


if (connectWifi() == 0)
	{
		
		return 0;

	}	//end if failed to connect to wifi

	HTTPClient http;

	http.begin(timeData_hostNameJSON);


	int HttpResult = http.GET();


	
	if (HttpResult > 0)
	{
		

		tempArray = http.getString();

		http.end();

		memcpy(data, tempArray.c_str() ,length-2);

		//ensure a null char to terminate the string
		data[length-2] = 0;

	}   //end if got a response




	if(terminateConnection == 1)
	{
		WiFi.mode(WIFI_OFF);
	}
	return 1;



}
//--------------------------------------------






bool GetRealTimeClass::get_unixtime(int32_t *unixtime, bool terminateConnection=true){

if (connectWifi() == 0)
	{
		
		return 0;

	}	//end if failed to connect to wifi

	HTTPClient http;

	http.begin(timeData_hostNameJSON);


	int HttpResult = http.GET();


	
	if (HttpResult > 0)
	{
		

		// Allocate a temporary JsonDocument
		// Don't forget to change the capacity to match your requirements.
		// Use arduinojson.org/v6/assistant to compute the capacity.
		StaticJsonDocument<612> doc;



		#ifdef		__DEBUG_INJECT_JSON

		String injectJson = __DEBUG_WORLD_API;


		// Deserialize the JSON document
		DeserializationError error = deserializeJson(doc, injectJson);
		if (error)
		{
			Serial.println(F("Failed to deserialize"));
			
			
			http.end();
			return 0;
		}


		#else
		// Deserialize the JSON document
		DeserializationError error = deserializeJson(doc, http.getString());
		if (error)
		{
			Serial.println(F("Failed to deserialize"));
			
			
			http.end();
			return 0;
		}

		#endif

		// Copy values from the JsonDocument to the structure



		Serial.println(F("deserialized OK"));

		
		http.end();



		//covert the day of the week to a numeric value
		 *unixtime = doc["unixtime"];

	}   //end if got a response


	else
	{

		#ifdef __DEBUG_SHOW_TIME

		Serial.println("an error occured when using GET");

		#endif // __DEBUG_SHOW_TIME
		
		http.end();

	}


	



	if(terminateConnection == 1)
	{
		WiFi.mode(WIFI_OFF);
	}
	return 1;



}
//--------------------------------------------


bool GetRealTimeClass::getTime(char *timeData, bool terminateConnection){


	if (connectWifi() == 0)
	{
		*timeData = 0;
		return 0;

	}	//end if failed to connect to wifi

	HTTPClient http;

	http.begin(timeData_hostName);


	int HttpResult = http.GET();


	
	if (HttpResult > 0)
	{
		String payload = http.getString();


		payload.toCharArray(timeData, 100);

		//strcpy(timeData, payload.c_str());


		#ifdef __DEBUG_SHOW_TIME

			Serial.println("=======================");
			Serial.println("Got result: ");
			Serial.println(timeData);

		#endif // __DEBUG_SHOW_TIME

		
			




	}   //end if got a response


	else
	{

		#ifdef __DEBUG_SHOW_TIME

		Serial.println("an error occured when using GET");

		#endif // __DEBUG_SHOW_TIME
		


	}


	http.end();


	if(terminateConnection == 1)
	{
		WiFi.mode(WIFI_OFF);
	}
	return 1;


}	//end function
//---------------------------------------



bool GetRealTimeClass::getTime(timeFormat *timeData, bool terminateConnection){



char strData[50], *token;
uint8_t dayOfWeek;

	if (connectWifi() == 0)
	{
		timeData->dateValue[0] = 0;
		timeData->dayOfWeek[0] = 0;
		timeData->dayOfYear = 0;
		timeData->timeValue[0] = 0;
		timeData->timeZone[0] = 0;
		return 0;

	}	//end if failed to connect to wifi

	HTTPClient http;

	http.begin(timeData_hostNameJSON);


	int HttpResult = http.GET();


	
	if (HttpResult > 0)
	{
		

		// Allocate a temporary JsonDocument
		// Don't forget to change the capacity to match your requirements.
		// Use arduinojson.org/v6/assistant to compute the capacity.
		StaticJsonDocument<512> doc;

		// Deserialize the JSON document
		DeserializationError error = deserializeJson(doc, http.getString());
		if (error)
		{
			Serial.println(F("Failed to deserialize"));
			
			
			http.end();
			return 0;
		}


		// Copy values from the JsonDocument to the structure



		Serial.println(F("deserialized OK"));

		
		http.end();



		//covert the day of the week to a numeric value
		dayOfWeek = doc["day_of_week"];


		switch(dayOfWeek)
		{
			case 0:
				sprintf(timeData->dayOfWeek, "Sunday");
				break;

			case 1:
				sprintf(timeData->dayOfWeek, "Monday");
				break;

			case 2:
				sprintf(timeData->dayOfWeek, "Tuesday");
				break;

			case 3:
				sprintf(timeData->dayOfWeek, "Wednesday");
				break;

			case 4:
				sprintf(timeData->dayOfWeek, "Thursday");
				break;

			case 5:
				sprintf(timeData->dayOfWeek, "Friday");
				break;

			case 6:
				sprintf(timeData->dayOfWeek, "Saturday");
				break;	

		}		//end switch statement

		#ifdef	__DEBUG_SHOW_TIME
			Serial.println(timeData->dayOfWeek);
		#endif



		
		strcpy(timeData->timeZone, doc["timezone"]);

		#ifdef	__DEBUG_SHOW_TIME
			Serial.println(timeData->timeZone);
		#endif


		timeData->dayOfYear = doc["day_of_year"];


		#ifdef	__DEBUG_SHOW_TIME
			Serial.println(timeData->dayOfYear);
		#endif


		strcpy(strData, doc["datetime"]);


		#ifdef	__DEBUG_SHOW_TIME
			Serial.println("complete date time");
			Serial.println(strData);
		#endif

		
		token = strtok(strData, "T.");

		strcpy(timeData->dateValue, token);

		#ifdef	__DEBUG_SHOW_TIME
			Serial.println("date component of string");
			Serial.println(timeData->dateValue);
		#endif


		token = strtok(NULL, "T.");

		strcpy(timeData->timeValue, token);

		#ifdef	__DEBUG_SHOW_TIME
			Serial.println("time component of string");
			Serial.println(timeData->timeValue);
		#endif



		#ifdef __DEBUG_SHOW_TIME

			Serial.println("=======================");
			Serial.println("Got result: ");
			Serial.println(timeData->timeValue);

			Serial.println(timeData->dateValue);
			Serial.println(timeData->dayOfWeek);
			Serial.println(timeData->dayOfYear);
			Serial.println(timeData->timeZone);


		#endif // __DEBUG_SHOW_TIME

		
			




	}   //end if got a response


	else
	{

		#ifdef __DEBUG_SHOW_TIME

		Serial.println("an error occured when using GET");

		#endif // __DEBUG_SHOW_TIME
		


	}


	http.end();


	if(terminateConnection == 1)
	{
		WiFi.mode(WIFI_OFF);
	}
	return 1;


}	//end function
//---------------------------------------



//---------------------------------------






bool GetRealTimeClass::connectWifi(void) {

	uint8_t retryCnt;
	char ssid[30], password[30];

	configDevice WIFI_deviceConfig;




#ifdef __DEBUG_USE_FIXED_WIFI


	sprintf(ssid, "%s",SSID_FIXED );
	sprintf(password, "%s", PASSWORD_FIXED);
#else



	WIFI_deviceConfig.begin();


	if(WIFI_deviceConfig.get_wifi_ssid(ssid) == 0)
	{

		Serial.println("critial error reading wifi SSID");
		return 0;

	}


	if (WIFI_deviceConfig.get_wifi_pass(password) == 0)
		{

			Serial.println("critial error reading wifi password");
			return 0;
		}


#endif // __DEBUG_USE_FIXED_WIFI




	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);


	#ifdef __DEBUG_SHOW_TIME

		Serial.print("Connecting WiFi");

	#endif


		retryCnt = WIFI_RETRY;

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		
		#ifdef __DEBUG_SHOW_TIME
			Serial.print(".");
		#endif


			retryCnt--;

			if (!retryCnt)
			{
				//exit the loop if failed to connect 
				
				#ifdef __DEBUG_SHOW_TIME
					Serial.println("failed to connect to wifi");
				
				#endif
				return 0;



			}

	}

	#ifdef __DEBUG_SHOW_TIME
		Serial.println();

		Serial.print("Connected, IP address: ");
		Serial.println(WiFi.localIP());
	#endif


		return 1;

}	//end function

//---------------------------------------


