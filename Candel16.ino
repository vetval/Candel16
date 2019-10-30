#include <FastLED.h>
#include <Wire.h>

#define LED_PIN     5
#define COLOR_ORDER GRB
#define CHIPSET     WS2812
#define NUM_LEDS    6
#define HEIGHT    6
#define FRAMES_PER_SECOND 60


CRGB leds[NUM_LEDS];
uint8_t brightness = 190;
LEDColorCorrection colorCorr = TypicalLEDStrip;


static byte heat[NUM_LEDS];
void setup() {
  delay(1000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( colorCorr );
  FastLED.setBrightness( brightness );
}

uint8_t cooling=10;
uint8_t sparking=120;
unsigned long start=0;
uint8_t delayMS=30;
void Fire16(void);


void loop(){
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random8());
  Fire16();
  start = millis();
  FastLED.show(); // display this frame
  FastLED.delay(delayMS);

  
}



#define MAX_HEAT 130

void Fire16(){
// Array of temperature readings at each simulation cell
  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(4+cooling, (MAX_HEAT / HEIGHT)+ cooling));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= HEIGHT - 1; k > 0; k--) {
      heat[k] = qsub8(heat[k - 1], random8(0,8));
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    heat[0]     = random8(80, MAX_HEAT);
    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < HEIGHT; j++) {
      leds[j] = HeatColor( heat[j]);
    }
}
