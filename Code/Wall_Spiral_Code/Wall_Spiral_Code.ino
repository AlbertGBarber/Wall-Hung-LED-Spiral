#define FASTLED_INTERNAL 1  //turns of the pragma messages from FastLED
#include <Pixel_Spork.h>

//IMPORTANT: requires FastLED version 3.10.0 or less (may not work with higher versions)
//Code was originally compiled with FastLED Version 3.9.16 and ESP8266 core 3.1.2
//So use those versions if you run into any bugs

//FastLED vars
#define NUM_LEDS 90 //total number of LEDs
#define DATA_PIN D8

//Effect count
//See "Spare Effect" file for unused effects
#define NUM_EFFECTS 23

//LightSensor Vars
#define LIGHTSENPIN A0
#define LIGHTSEN_ENABLE_PIN D5  //jumper block reading pin, used to enable / disable the light sensor reading at boot
#define LIGHTSEN_THRESHOLD 1000
boolean lightSenDisable = false; //if true, then the light sensor will be ignored 

// Define the array of leds for FastLED
CRGB leds[NUM_LEDS];

//========================================================================
//                          Segment Sets Setups
//========================================================================
//see segDefs file for segment set definitions
#include "segDefs.h"

//========================================================================
//                          Palette Classes Setups
//========================================================================

//Random palettes 
//(initialized with red, green, and blue starting colors, they will be randomized during setup())
CRGB rand3Pallet_Arr[] = {CRGB::Red, CRGB::Blue, CRGB::Green};
palettePS rand3Pallet1 = {rand3Pallet_Arr, SIZE(rand3Pallet_Arr)};

CRGB rand3Pallet_Arr2[] = {CRGB::Red, CRGB::Blue, CRGB::Green};
palettePS rand3Pallet2 = {rand3Pallet_Arr2, SIZE(rand3Pallet_Arr2)};

CRGB rand3Pallet_Arr3[] = {CRGB::Red, CRGB::Blue, CRGB::Green};
palettePS rand3Pallet3 = {rand3Pallet_Arr3, SIZE(rand3Pallet_Arr3)};

CRGB rand2Pallet_Arr[] = {CRGB::Red, CRGB::Blue};
palettePS rand2Pallet1 = {rand2Pallet_Arr, SIZE(rand2Pallet_Arr)};

CRGB rand2Pallet_Arr2[] = {CRGB::Red, CRGB::Blue};
palettePS rand2Pallet2 = {rand2Pallet_Arr2, SIZE(rand2Pallet_Arr2)};

CRGB rand2Pallet_Arr3[] = {CRGB::Red, CRGB::Blue};
palettePS rand2Pallet3 = {rand2Pallet_Arr3, SIZE(rand2Pallet_Arr3)};

CRGB singPal_Arr[] = {CRGB::Red};
palettePS singPal = {singPal_Arr, SIZE(singPal_Arr)};

//========================================================================
//                          Util Classes Setups
//========================================================================
//The classes below either manipulate properties of effects or segments
//For most cases you'll only need one in your code

//The main effect cycle controller, contains 3 spaces:
//the first two for the palette blenders, and the last for the active effects
EffectBasePS *effArray[3];  //an effect array of length 1, we will fill it in later
EffectSetPS effectSet(effArray, SIZE(effArray), 2, 20000);

//Palette blender with 3 colors, 5000ms pause between blends
PaletteBlenderPS Rand3PB(rand3Pallet1, rand3Pallet2, true, true, false, 5000, 20, 50);
palettePS &randPB3Pal = Rand3PB.blendPalette;

//Palette blender with 2 colors, 5000ms pause between blends
PaletteBlenderPS Rand2PB(rand2Pallet1, rand2Pallet2, true, true, false, 5000, 20, 50);
palettePS &randPB2Pal = Rand2PB.blendPalette;

//unused utils
//RandRateCtrlPS randRate(80, -60, 20, 1000, 100);

//RandRateCtrlPS randRate2(15, -10, 10, 1000, 100);

//RateNoisePS rateNoise(10, 40, 10, 80);

//========================================================================
//                          Effect Names
//========================================================================
ColorModeFillPS *ColorFill;
//AddGlitterPS *Glitter;
//ShiftingSeaSL *seaSft;
Twinkle2SLSeg *Twinkl2;
PrideWPalSL2 *Pride2;
GradientCycleFastSL *GCF;
StreamerSL *Strem;
RollingWavesSL *Roll;
RollingWavesFastSL *RollFa;
ParticlesSL *Parti;
RainSL *rainSL;
RainSeg *rainSeg;
//PacificaHueSL *PaciH;
LavaPS *Lav;
NoiseSL *Noise;
//NoiseGradSL *NoiseGrad;
EdgeBurstSL *Edge;
//ColorMeltSL *ColMelt;
SegWaves *Waves;
ColorWipeSLSeg *ColorWipe;

//=====================================================================================
//                              General Vars
//=====================================================================================

uint8_t normalBrightness = 200;  //note that this is very bright, so it shows through the paper!

int lightLevel;

uint8_t count = 0;
uint8_t prevCycleNum = 0;
bool def = false;
bool lightsTurnedOn = false;

void setup() {

    //Setup FastLed
    FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(normalBrightness);
    segDrawUtils::turnSegSetOff(mainSegments);  //clear the LEDs, just in case
    //Serial.begin(9600);

    //set a random seed for the esp
    randomSeed(ESP.getCycleCount());
    random16_add_entropy(ESP.getCycleCount());

    //assign the effect utility classes to the effect controller
    effectSet.setEffect(&Rand3PB, 1);
    effectSet.setEffect(&Rand2PB, 0);

    //Randomize the colors in the random color palettes
    paletteUtilsPS::randomize(rand3Pallet1);
    paletteUtilsPS::randomize(rand3Pallet2);
    paletteUtilsPS::randomize(rand3Pallet3);

    paletteUtilsPS::randomize(rand2Pallet1);
    paletteUtilsPS::randomize(rand2Pallet2);
    paletteUtilsPS::randomize(rand2Pallet3);

    //Turn on the runOffset setting for all the segment sets
    //This allows the color mode rainbows to shift
    ringSegments.runOffset = true;
    ringSegments.offsetDirect = false;

    joinedRingSegments.runOffset = true;
    halfRingSegments.runOffset = true;

    CCWspiralSegs.runOffset = true;
    CCWspiralLineSeg.runOffset = true;

    CWringSegments.runOffset = true;
    CWspiralSegs.runOffset = true;
    CWspiralLineSeg.runOffset = true;

    lotusSegs.runOffset = true;
}

//=====================================================================================
//                              Main Loop Code
//=====================================================================================
//To remove an effect, simply set its effectSet.runTime = 0 and it will be skipped over

//To add a new effect, add the following code block to the large switch statement below,
//filling in the case number, effect constructor, and effect name.
//If using a new type of effect you must add it in the "effect names" section above.
//You'll also need to increment NUM_EFFECTS above.
/* 
case <<case number>>: {
    if(!def) {
        <<effect constructor>>
        effectSet.runTime = 20000; //runtime in ms
        effectSet.setEffect(<<effect name>>, 2);
    } else {
        //
    }
} break;
*/

//Each loop, we're either updating and cycling effects,
//or checking the light level to start an effect
//Once the light level has been reached, we run the next effect to completion
//and check the light level again once the effect has finished.
void loop() {
 
    //If the lights are on, we run effects as normal,
    //if not, we check the light level (all the way at the end of this big if)
    if(lightsTurnedOn) {
        //This switch statement sets what effect is active based on the cycle "count"
        //When the effect is first defined, it hits the "!def" part of each case to define it
        //Otherwise it does whatever's in the "else" part of the if with each update
        switch(count) {
            case 0: {
                if(!def) {
                    ColorWipe = new ColorWipeSLSeg(CWringSegments, singPal, 10, 0, true, false, true, false, 140);
                    ColorWipe->setUpLoop(true, 2, false, 1, true, false, true, false, false);
                    effectSet.runTime = 15000;
                    effectSet.setEffect(ColorWipe, 2);
                } else {
                    if(ColorWipe->loopCount != prevCycleNum) {
                        prevCycleNum = ColorWipe->loopCount;
                        paletteUtilsPS::randomize(singPal);
                    }
                }
            } break;
            case 1: {
                if(!def) {
                    rainSeg = new RainSeg(CCWspiralSegs, randPB3Pal, 0, true, 10, 2, 1, 0, 1, 3, true, false, false, false, false, 80, 50);
                    effectSet.runTime = 20000;
                    effectSet.setEffect(rainSeg, 2);
                } else {
                    //
                }
            } break;
            case 2: {
                if(!def) {
                    rainSeg = new RainSeg(CCWspiralSegs, CRGB::White, 0, false, 10, 2, 1, 0, 1, 3, true, false, false, false, false, 80, 50);
                    rainSeg->bgColorMode = 3;
                    effectSet.runTime = 10000;
                    effectSet.setEffect(rainSeg, 2);
                } else {
                    //
                }
            } break;
            case 3: {
                if(!def) {
                    RollFa = new RollingWavesFastSL(CWspiralSegs, 3, 0, 9, 1, 0, 120);
                    RollFa->randMode = 1;
                    effectSet.runTime = 10000;
                    effectSet.setEffect(RollFa, 2);
                } else {
                    //
                }
            } break;
            case 4: {
                if(!def) {
                    Lav = new LavaPS(CWspiralLineSeg, randPB3Pal, 20, 30, 50);
                    Lav->hueCycle = true;
                    effectSet.runTime = 15000;
                    effectSet.setEffect(Lav, 2);
                } else {
                    //
                }
            } break;
            case 5: {
                if(!def) {
                    Edge = new EdgeBurstSL(joinedRingSegments, 0, true, true, 15, 80);
                    effectSet.runTime = 10000;
                    effectSet.setEffect(Edge, 2);
                } else {
                    //
                }
            } break;
            case 6: {
                if(!def) {
                    Strem = new StreamerSL(CCWspiralLineSeg, randPB3Pal, 0, 9, 9, 8, 20);
                    effectSet.runTime = 20000;
                    effectSet.setEffect(Strem, 2);
                } else {
                    //
                }
            } break;
            case 7: {
                if(!def) {
                    rainSL = new RainSL(joinedRingSegments, randPB3Pal, 0, true, 10, 1, 1, 0, 1, 3, false, true, false, false, true, 70, 50, false);
                    effectSet.runTime = 20000;
                    effectSet.setEffect(rainSL, 2);
                } else {
                    //
                }
            } break;
            case 8: {
                if(!def) {
                    GCF = new GradientCycleFastSL(CWringSegments, randPB3Pal, 5, 100);
                    GCF->randMode = 1;
                    effectSet.runTime = 15000;
                    effectSet.setEffect(GCF, 2);
                } else {
                    //
                }
            } break;
            case 9: {
                if(!def) {
                    Roll = new RollingWavesSL(mainSegments, randPB3Pal, 0, 5, 1, 2, 120);
                    effectSet.runTime = 15000;
                    effectSet.setEffect(Roll, 2);
                } else {
                    //
                }
            } break;
            case 10: {
                if(!def) {
                    Parti = new ParticlesSL(CWspiralLineSeg, randPB3Pal, 0, 7, 2, 70, 50, 1, 0, 1, 2, 7, 2, randPB3Pal.length, true);
                    effectSet.runTime = 15000;
                    effectSet.setEffect(Parti, 2);
                } else {
                    //
                }
            } break;
            case 11: {
                if(!def) {
                    Parti = new ParticlesSL(CWspiralLineSeg, randPB3Pal, 0, 4, 2, 50, 40, 1, 0, 1, 9, 18, 2, randPB3Pal.length, true);
                    Parti->colorMode = 1;
                    effectSet.runTime = 15000;
                    effectSet.setEffect(Parti, 2);
                } else {
                    //
                }
            } break;
            case 12: {
                if(!def) {
                    Twinkl2 = new Twinkle2SLSeg(halfRingSegments, randPB3Pal, 0, 5, 500, 5, 5, 5, 5, 0, 70);
                    Twinkl2->colorMode = 2;
                    effectSet.runTime = 15000;
                    effectSet.setEffect(Twinkl2, 2);
                } else {
                    //
                }
            } break;
            case 13: {
                if(!def) {
                    ColorWipe = new ColorWipeSLSeg(ringSegments, rand3Pallet3, 9, 2, true, false, false, true, 140);
                    ColorWipe->setUpLoop(true, 2, false, 1, true, false, true, false, false);
                    effectSet.runTime = 15000;
                    effectSet.setEffect(ColorWipe, 2);
                } else {
                    //change the colors after every wipe cycle
                    if(ColorWipe->loopCount != prevCycleNum) {
                        prevCycleNum = ColorWipe->loopCount;
                        paletteUtilsPS::randomize(rand3Pallet3);
                    }
                }
            } break;
            case 14: {
                if(!def) {
                    ColorFill = new ColorModeFillPS(ringSegments, 2, 60);
                    effectSet.runTime = 10000;
                    effectSet.setEffect(ColorFill, 2);
                } else {
                    //
                }
            } break;
            case 15: {
                if(!def) {
                    Noise = new NoiseSL(joinedRingSegments, randPB3Pal, 20, 30, 40, 10, 0, 80);  //20 steps for mode 0
                    effectSet.runTime = 20000;
                    effectSet.setEffect(Noise, 2);
                } else {
                    //
                }
            } break;
            case 16: {
                if(!def) {
                    Twinkl2 = new Twinkle2SLSeg(joinedRingSegments, randPB3Pal, 0, 25, 300, 2, 6, 2, 6, 2, 60);
                    effectSet.runTime = 15000;
                    effectSet.setEffect(Twinkl2, 2);
                } else {
                    //
                }
            } break;
            case 17: {
                if(!def) {
                    Edge = new EdgeBurstSL(CWspiralSegs, 2, true, true, 20, 100);
                    effectSet.runTime = 15000;
                    effectSet.setEffect(Edge, 2);
                } else {
                    //
                }
            } break;
            case 18: {
                if(!def) {
                    ColorFill = new ColorModeFillPS(lotusSegs, 2, 60);
                    effectSet.runTime = 10000;
                    effectSet.setEffect(ColorFill, 2);
                } else {
                    //
                }
            } break;
            case 19: {
                if(!def) {
                    Waves = new SegWaves(lotusSegs, randPB2Pal, 0, 1, 2, 10, false, 65);
                    Waves->colorMode = 4;
                    effectSet.runTime = 15000;
                    effectSet.setEffect(Waves, 2);
                } else {
                    //
                }
            } break;
            case 20: {
                if(!def) {
                    ColorWipe = new ColorWipeSLSeg(lotusSegs, rand2Pallet3, 10, 2, true, false, true, true, 140);
                    ColorWipe->setUpLoop(true, 2, false, 1, true, false, true, false, false);
                    effectSet.runTime = 17000;
                    effectSet.setEffect(ColorWipe, 2);
                } else {
                    if(ColorWipe->loopCount != prevCycleNum) {
                        prevCycleNum = ColorWipe->loopCount;
                        paletteUtilsPS::randomize(rand2Pallet3);
                    }
                }
            } break;
            case 21: {
                if(!def) {
                    Pride2 = new PrideWPalSL2(joinedRingSegments, true, true, 80);
                    effectSet.runTime = 60000;
                    effectSet.setEffect(Pride2, 2);
                } else {
                    //
                }
            } break;
            case 22: {
                if(!def) {
                    ColorFill = new ColorModeFillPS(CWspiralSegs, 2, 60);
                    effectSet.runTime = 10000;
                    effectSet.setEffect(ColorFill, 2);
                } else {
                    //
                }
            } break;
        }

        //We use "def" to track if an effect has been created
        //If def is false, an effect will always be created in the above switch statement,
        //so we can set def to false here
        if(!def) {
            def = true;
        }

        //With each cycle, we either want to keep updating the current effect,
        //or swap to the next one when the time is right
        if(effectSet.done) {
            //Once the current effect's time is up, we reset and cycle to the next effect
            count = (count + 1) % NUM_EFFECTS;  //advance the count to the next effect
            //set flags, free the old effect's memory, and reset other vars to defaults
            def = false;
            effectSet.destructEffsAftLim();
            effectSet.reset();
            FastLED.setBrightness(normalBrightness);
            resetLoopSettings();
            segDrawUtils::turnSegSetOff(mainSegments);  //clear the spiral of any leftover colors
            checkLightSen();                            //check the light sensor
        } else {
            //Update the current effect
            yield();
            effectSet.update();
        }
    } else {
        //If lights not on, check the light level
        checkLightSen();
    }
}

//reads the light sensor, and turns the effects on/off using the lightsTurnedOn flag
void checkLightSen() {
    lightLevel = analogRead(A0);
    //Serial.println(lightLevel);
    if(lightSenDisable || (lightLevel >= LIGHTSEN_THRESHOLD)) {
        lightsTurnedOn = true;
    } else {
        if(lightsTurnedOn) {
            //turn the lights off
            segDrawUtils::turnSegSetOff(mainSegments);
            lightsTurnedOn = false;
            FastLED.show();
        }
    }
}

//Resets any segment set settings and other loop variables to there starting states
//So that any changes made for/by the current effect are reset
void resetLoopSettings() {
    prevCycleNum = 0;

    //Reset the direction of all the segment sets
    //mainSegments.setAllSegDirection(true);
    //ringSegments.setAllSegDirection(true);
    //joinedRingSegments.setAllSegDirection(true);

    //halfRingSegments.setSegDirectionEvery(2, true, true);
    //halfRingSegments.setSegDirectionEvery(2, false, false);

    //CCWspiralSegs.setAllSegDirection(false);
    //CCWspiralLineSeg.setAllSegDirection(true);

    //CWspiralSegs.setAllSegDirection(false);
    //CWspiralLineSeg.setAllSegDirection(true);
    //CWringSegments.setAllSegDirection(true);

    //lotusSegs.setAllSegDirection(true);
}

