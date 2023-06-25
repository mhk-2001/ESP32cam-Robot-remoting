#include <esp_camera.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

//motors pins
#define motorRight_1 12
#define motorRight_2 13
#define motorLeft_1 15
#define motorLeft_2 14

//catcher pins
#define catcher_1 0
#define catcher_2 0

//shooter pin
#define shooter 2

//flash pin & setups
#define LED_PIN 4
//#define LED_CHANNEL 0 //شماره کانال که قرار است تنظیم شود
//#define LED_FREQUENCY 5000 // فرکانس  که باید بین 1 و 40 کیلو هرتز باشد.
//#define LED_RESOLUTION 8 // دقت که باید بین 1 و 16 بیت باشد

// Network address and password
const char* ssid = "oscole";
const char* password = "111oscole";

// State strings
int arr[5];

// HTML web page & JavaScript function
const char* htmlContent = "<html>\
                          	<head>\
                          		<title>ESP32-CAM</title>\
                          	</head>\
                          	<body>\
                          		<h1>ESP32-CAM</h1>\
                          		<br>\
                          		<form method='get' action='/showImage'>\
		                          <button type='/submit'>Show Image</button>\
		                          </form>\
                          		<br><br>\
                          		<form method='post' action='/submit'>\
                          			Enter text: <input type='text' name='input'>\
                          			<br>\
                          			<input type='submit' value='Submit'>\
                          		</form>\
                          		<script type=\"text/javascript\">\
                          			function showImage()\
                          			{\
                                  console.log(\"Test image\");\
                          				var xhr = new XMLHttpRequest();\
                          				xhr.onload = function ()\
                          				{\
                          					document.getElementById(\"image\").src = \"data:image/jpeg,\" + xhr.responseText;\
                          				};\
                          				xhr.open(\"GET\", \"/image\", true);\
                          				xhr.send();\
                          			}\
                          		</script>\
                          	</body>\
                          </html>";

// Define server
AsyncWebServer server(80);

// Convert string to array
void convert(String str)
{
  //int arr_length = sizeof(arr) / sizeof(arr[0]);
  for(byte i = 0; i < 5; i++)
  {
    arr[i] = (byte)str[i] - 48;
  }
  arr[0] = arr[0] - 4;
  arr[1] = arr[1] - 4;
}

void showArr()
{
  byte arr_len = sizeof(arr) / sizeof(arr[0]);
  for(byte i = 0; i < arr_len; i++)
  {
    Serial.print(arr[i]);
  }
  Serial.println();
}

// Joystick module code block
void motorRightForward(byte sequence)
{
  digitalWrite(motorRight_1, HIGH);
  digitalWrite(motorRight_2, LOW);
  delay(sequence * 30);
  digitalWrite(motorRight_1, LOW);
  digitalWrite(motorRight_2, LOW);
}

void motorRightBackward(byte sequence)
{
  digitalWrite(motorRight_1, LOW);
  digitalWrite(motorRight_2, HIGH);
  delay(sequence * 30);
  digitalWrite(motorRight_1, LOW);
  digitalWrite(motorRight_2, LOW);
}

void motorLeftForward(byte sequence)
{
  digitalWrite(motorLeft_1, HIGH);
  digitalWrite(motorLeft_2, LOW);
  delay(sequence * 30);
  digitalWrite(motorLeft_1, LOW);
  digitalWrite(motorLeft_2, LOW);
}

void motorLeftBackward(byte sequence)
{
  digitalWrite(motorLeft_1, LOW);
  digitalWrite(motorLeft_2, HIGH);
  delay(sequence * 30);
  digitalWrite(motorLeft_1, LOW);
  digitalWrite(motorLeft_2, LOW);
}

void stopMotors()
{
  digitalWrite(motorRight_1, LOW);
  digitalWrite(motorRight_2, LOW);
  digitalWrite(motorLeft_1, LOW);
  digitalWrite(motorLeft_2, LOW);
}

// motors control
void moduleJoystick()
{
  if(arr[1] > 0)
  {
    if(arr[0] > 0)
    {
      motorRightForward(arr[1]);
      motorLeftForward(arr[1] + arr[0]);
    }
    else if(arr[0] < 0)
    {
      motorRightForward(arr[1] + (-1 * arr[0]));
      motorLeftForward(arr[1]);
    }
    else
    {
      motorRightForward(arr[1]);
      motorLeftForward(arr[1]);
    }
    //Serial.println("forwarding");
    //return "forwarding";
  }
  else if(arr[1] < 0)
  {
    if(arr[0] > 0)
    {
      motorRightBackward(-1 * arr[1]);
      motorLeftBackward((-1 * arr[1]) + arr[0]);
    }
    else if(arr[0] < 0)
    {
      motorRightBackward((-1 * arr[1]) + (-1 * arr[0]));
      motorLeftBackward(-1 * arr[1]);
    }
    else
    {
      motorRightBackward(-1 * arr[1]);
      motorLeftBackward(-1 * arr[1]);
    }
    //Serial.println("backwaring");
    //return "backwaring";
  }
  else
  {
    if(arr[0] > 0)
    {
      motorRightBackward(arr[0]);
      motorLeftForward(arr[0]);
      //Serial.println("spining");
      //return "spining";
    }
    else if(arr[0] < 0)
    {
      motorRightForward(-1 * arr[0]);
      motorLeftBackward(-1 * arr[0]);
      //Serial.println("spining");
      //return "spining";
    }
    else
    {
      stopMotors();
      //Serial.println("stop");
      //return "stop";
    }
  }
}

// Catcher module code block
// Open catcher
void catcherOpen()
{
  digitalWrite(catcher_1, HIGH);
  digitalWrite(catcher_2, HIGH);
  delay(80);
  digitalWrite(catcher_1, LOW);
  digitalWrite(catcher_2, LOW);
}

// Close catcher
void catcherClose()
{
  digitalWrite(catcher_1, LOW);
  digitalWrite(catcher_2, LOW);
  delay(80);
  digitalWrite(catcher_1, LOW);
  digitalWrite(catcher_2, LOW);
}

// Catcher control
void moduleCatcher()
{
  if(arr[2] == 1)
  {
    catcherOpen();
  }
  else if(arr[2] == 2)
  {
    catcherClose();
  }
}

// Shooter control
void moduleShooter()
{
  if(arr[3] == 1)
  {
    digitalWrite(shooter, HIGH);
    delay(50);
    digitalWrite(shooter, LOW);
  }
  else
  {
    digitalWrite(shooter, LOW);
  }
}

// Flash control
void moduleFlash()
{
  if(arr[4] == 1)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
}

// Handeling all modules
void handel(String str)
{
  convert(str);
  //showArr();
  moduleJoystick();
  //moduleCatcher();
  //moduleShooter();
  moduleFlash();
}

void setup()
{
  // Put your setup code here, to run once:

  //setup all pins
  
  //motors
  pinMode(motorRight_1, OUTPUT);
  pinMode(motorRight_2, OUTPUT);
  pinMode(motorLeft_1, OUTPUT);
  pinMode(motorLeft_2, OUTPUT);

  digitalWrite(motorRight_1, LOW);
  digitalWrite(motorRight_2, LOW);
  digitalWrite(motorLeft_1, LOW);
  digitalWrite(motorLeft_2, LOW);

  //catcher
  //pinMode(catcher_1, OUTPUT);
  //pinMode(catcher_2, OUTPUT);
  
  //digitalWrite(catcher_1, LOW);
  //digitalWrite(catcher_2, LOW);

  //shooter
  //pinMode(shooter, OUTPUT);

  //digitalWrite(shooter, LOW);

  //flash
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  //ledcSetup(LED_CHANNEL, LED_FREQUENCY, LED_RESOLUTION);
  //ledcAttachPin(LED_PIN, LED_CHANNEL);

  // Setup serial
  Serial.begin(115200);

  // Connect to Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Connection established
  Serial.println("Connected to WiFi");

  // Config & start the server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send_P(200, "text/html; charset=utf-8", htmlContent);
    }
  );

  server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send_P(200, "text/plain", "Test connection");
    }
  );

  server.on("/submit", HTTP_POST, [](AsyncWebServerRequest *request)
    {
      if(request->hasParam("input", true))
      {
        //Serial.println(request->getParam("input", true)->value().c_str());
        handel(request->getParam("input", true)->value().c_str());
        request->send_P(200, "text/plain", "Data received");
      }
      else
      {
        request->send_P(500, "text/plain", "Data not received");
      }
    }
  );

  server.on("/showImage", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      camera_fb_t* fb = esp_camera_fb_get();
      if (!fb)
      {
        Serial.println("Camera capture failed");
        request->send(500, "text/plain", "Camera capture failed");
        return;
      }
      request->send_P(200, "image/jpeg", (const uint8_t*)fb->buf, fb->len);
      esp_camera_fb_return(fb);
    }
  );

  server.begin();
  Serial.println("Server started successfuly");

  // Camera configs
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA; // Set video frame size
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // Initialize camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  else
  {
    Serial.printf("Camera init success");
  }

  // Initialize ip adderss
  Serial.println();
  Serial.print("your Local IP: ");
  Serial.println(WiFi.localIP());

}

void loop()
{
  // Put your main code here, to run repeatedly:

}
