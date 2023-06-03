#define LED0_PIN 4
#define LED0_CHANNEL 0

#define LED1_PIN 2
#define LED1_CHANNEL 1

#define LED2_PIN 12
#define LED2_CHANNEL 2

#define LED3_PIN 15
#define LED3_CHANNEL 3

#define LED_FREQUENCY 5000
#define LED_RESOLUTION 8 

void setup()
{
  // Put your setup code here, to run once:

  // Initialize the LEDs
  ledcSetup(LED0_CHANNEL, LED_FREQUENCY, LED_RESOLUTION);
  ledcAttachPin(LED0_PIN, LED0_CHANNEL);

  ledcSetup(LED1_CHANNEL, LED_FREQUENCY, LED_RESOLUTION);
  ledcAttachPin(LED1_PIN, LED1_CHANNEL);

  ledcSetup(LED2_CHANNEL, LED_FREQUENCY, LED_RESOLUTION);
  ledcAttachPin(LED2_PIN, LED2_CHANNEL);

  ledcSetup(LED3_CHANNEL, LED_FREQUENCY, LED_RESOLUTION);
  ledcAttachPin(LED3_PIN, LED3_CHANNEL);
}

void loop()
{
  // Put your main code here, to run repeatedly:
  
  for(int i = 200; i >= 0; i--)
  {
    ledcWrite(LED0_CHANNEL, i);
    ledcWrite(LED1_CHANNEL, i);
    ledcWrite(LED2_CHANNEL, i);
    ledcWrite(LED3_CHANNEL, i);
    delay(20);
  }
  for(int i = 0; i <= 200; i++)
  {
    ledcWrite(LED0_CHANNEL, i);
    ledcWrite(LED1_CHANNEL, i);
    ledcWrite(LED2_CHANNEL, i);
    ledcWrite(LED3_CHANNEL, i);
    delay(20);
  }

  delay(1000);
}