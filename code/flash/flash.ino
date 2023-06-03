#define LED_PIN 4
#define LED_CHANNEL 0 //شماره کانال که قرار است تنظیم شود
#define LED_FREQUENCY 5000 // فرکانس  که باید بین 1 و 40 کیلو هرتز باشد.
#define LED_RESOLUTION 8 // دقت که باید بین 1 و 16 بیت باشد

void setup()
{
  // Put your setup code here, to run once:

  // Initialize the LED
  ledcSetup(LED_CHANNEL, LED_FREQUENCY, LED_RESOLUTION);
  ledcAttachPin(LED_PIN, LED_CHANNEL);
}

void loop()
{
  // Put your main code here, to run repeatedly:
  
  for(int i = 200; i >= 0; i--)
  {
    ledcWrite(LED_CHANNEL, i);
    delay(20);
  }
  
  for(int i = 0; i <= 200; i++)
  {
    ledcWrite(LED_CHANNEL, i);
    delay(20);
  }


  // Turn on the flash
  //ledcWrite(LED_CHANNEL, 255);

  delay(1000);

  // Turn off the flash
  //ledcWrite(LED_CHANNEL, 0);
}