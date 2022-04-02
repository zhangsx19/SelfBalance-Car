#include "BluetoothSerial.h"
#include "esp_camera.h"
#include <WiFi.h>
//#include "communicate.h"
//
// WARNING!!! Make sure that you have either selected ESP32 Wrover Module,
//            or another board which has PSRAM enabled
//

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

const char* ssid = "TEST1";
const char* password = "123456789";

void startCameraServer();

void camSetup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);//flip it back
    s->set_brightness(s, 1);//up the blightness just a bit
    s->set_saturation(s, -2);//lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}


bool testJudge = false;
BluetoothSerial SerialBT;
void setup() {
	Serial.begin(115200);
	Serial.println("cam setup");
	SerialBT.begin("ESP32test2"); //Bluetooth device name
	Serial.println("The device started, now you can pair it with bluetooth!");
	Serial.println("ESP2 setup");
	///*a.BTSetup();*/
	camSetup(); //³õÊ¼»¯ÉãÏñÍ·
	
	/*a.serialSetup();*/

}

void loop() {
  // put your main code here, to run repeatedly:
	if (testJudge==false)
	{
		Serial.println("test1");
	}
	delay(1000);
	/*a.PcToUno();
	a.UnoToPc();*/
	char PcToUnomessage[5]; 
	if (SerialBT.available() > 0) {
		for (int i = 0; i < 5; i++)
		{
			if (SerialBT.available() > 0) {
				PcToUnomessage[i] = (char)SerialBT.read();
			}
			else
			{
				i--;
			}
		}
		Serial.println(PcToUnomessage /*(char)SerialBT.read()*/);
	}
	
	
	
	char UnoToPcmessage[5]; 
	if (Serial.available() > 0) {
		for (int j = 0; j < 5; j++)
		{
			if (SerialBT.available() > 0) {
				UnoToPcmessage[j] = (char)Serial.read();
			}
			else
			{
				j--;
			}
			
		}
		SerialBT.println(UnoToPcmessage/*(char)Serial.read()*/);
	}
	testJudge = true;
}
