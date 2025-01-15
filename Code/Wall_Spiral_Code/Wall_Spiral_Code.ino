#define FASTLED_INTERNAL 1  //turns of the pragma messages from FastLED
#include <Pixel_Spork.h>
#include "effectStore.h"

//FastLED vars
#define NUM_LEDS 90
#define DATA_PIN D8

//Effect count
#define NUM_EFFECTS 22

//===============================================================
//WARNING, must be compiled using ESP8266 Version 2.7.4 or lower,
//due FastLED generating noise on the LED data line for higher versions.
//This specifically affects "bullet" WS2811 LEDs, but not the typical 5050 package WS2812* LEDs,
//and causes the bullets to flicker for certain effects.
//===============================================================

//LightSensor Vars
#define LIGHTSENPIN A0
#define LIGHTSEN_ENABLE_PIN D5  //jumper block reading pin, used to enable / disable the light sensor reading at boot
#define LIGHTSEN_THRESHOLD 1000
boolean lightSenDisable = false;

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
//                           Patterns
//========================================================================
//uint8_t christStreamPat_arr = {0, 0, 0, 255, 255, 255, 255, 255};
//patternPS christStreamPat = {christStreamPat_arr, SIZE(christStreamPat_arr)};

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

//===================================================================================
ColorModeFillPS *ColorFill;
AddGlitterPS *Glitter;
//CrossFadeCyclePS *CFC;
ShiftingSeaSL *seaSft;
TwinkleSL *Twinkl;
//TwinkleFastSL *CFC;
Twinkle2SLSeg *Twinkl2;
FairyLightsSLSeg *FL;
//DissolveSL *Dis;
//SoftTwinkleSL *CFC;
//GlimmerSL *Glim;
ShimmerSL *Shim;
PrideWPalSL *Pride;
PrideWPalSL2 *Pride2;
//TheaterChaseSL *CFC;
GradientCycleSL *GC;
GradientCycleFastSL *GCF;
StreamerSL *Strem;
//StreamerFastSL *CFC;
RollingWavesSL *Roll;
RollingWavesFastSL *RollFa;
ParticlesSL *Parti;
//LarsonScannerSL *CFC;
//PoliceStrobeSL *CFC;
//StrobeSeg *CFC;
//Fire2012Seg *Fire;
Fire2012SL *Fire;
RainSL *rainSL;
RainSeg *rainSeg;
//PacificaPS *CFC;
PacificaHueSL *PaciH;
LavaPS *Lav;
//PlasmaSL *CFC;
//Noise16PS *CFC;
NoiseSL *Noise;
FireworksPS *FW;
FirefliesSL *FF;
//NoiseWavesSL *CFC;
NoiseGradSL *NoiseGrad;
EdgeBurstSL *Edge;
ColorMeltSL *ColMelt;
//RainbowFontsSL *RainFont;
SegWaves *Waves;
//SegWavesFast *WavesFast;
//ScannerSL *Scan;
//BreathPS *CFC;
//BreathEyeSL *CFC;
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

//--------------------------------------------------
//       Effects Depot (contains used and spare effects)
//---------------------------------------------------

//Particles
//--------------
//Parti = new ParticlesSL(CWspiralLineSeg, randPB3Pal, 0, 4, 2, 40, 40, 1, 0, 1, 9, 18, 2, randPB3Pal.length, true);
//Parti->colorMode = 1;

//Parti = new ParticlesSL(CWspiralLineSeg, randPB3Pal, 0, 7, 2, 70, 50, 1, 0, 1, 2, 5, 2, randPB3Pal.length, true);

//Parti = new ParticlesSL(joinedRingSegments, randPB3Pal, 0, 3, 2, 80, 50, 1, 0, 2, 0, 2, 2, randPB3Pal.length, true);
//Parti->colorMode = 3;
//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(Parti, 0);

//particles single and inf trails effects
//--------------
//Parti = new ParticlesSL(CWspiralLineSeg, randPB3Pal, 0, 6, 2, 60, 30, 1, 0, 0, 0, 0, 2, randPB3Pal.length, true);
//particleUtilsPS::setAllTrailRand(*Parti->particleSet, true, false, false, false, true);
//rainSeg = new RainSeg(CCWspiralSegs, randPB3Pal, 0, true, 10, 2, 1, 0, 1, 3, true, false, false, false, true, 80, 50);
//rainSL = new RainSL(joinedRingSegments, randPB3Pal, 0, true, 10, 1, 1, 0, 1, 3, false, true, false, false, true, 70, 50, false);

//Rains
//--------------
//rainSeg = new RainSeg(CCWspiralSegs, randPB3Pal, 0, true, 10, 2, 1, 0, 1, 3, true, false, false, false, false, 80, 50);

//rainSeg = new RainSeg(CCWspiralSegs, CRGB::White, 0, false, 10, 2, 1, 0, 1, 3, true, false, false, false, false, 80, 50);
//rainSeg->bgColorMode = 3;
//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(rainSeg, 0);

//Pacifica
//--------------
//PaciH = new PacificaHueSL(CWspiralLineSeg, false, 0, 120, 60); //joinedRingSegments
//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(PaciH, 0);

//Edge Burst
//--------------
//Edge = new EdgeBurstSL(joinedRingSegments, 0, true, true, 15, 80);
//Edge = new EdgeBurstSL(CWspiralSegs, 2, true, true, 20, 100);
//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(Edge, 0);

//Streamer
//--------------
//Strem = new StreamerSL(CCWspiralLineSeg, randPB2Pal, 0, 9, 0, 10, 20);
//Strem = new StreamerSL(CCWspiralLineSeg, randPB3Pal, 0, 9, 9, 10, 20);
//Strem = new StreamerSL(joinedRingSegments, CRGB::White, 0, 2, 3, 10, 20);
//Strem->bgColorMode = 3;

//Strem = new StreamerSL(joinedRingSegments, randPB3Pal, 0, 3, 2, 10, 20);
//Strem->colorMode = 3;

//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(Strem, 0);

//Color Melt
//--------------
//ColMelt = new ColorMeltSL(CCWspiralSegs, randPB2Pal, 15, 3, true, 80);
//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(ColMelt, 0);

//Lava
//--------------
//Lav = new LavaPS(CWspiralLineSeg, 20, 50, 50);
//Lav = new LavaPS(CWspiralLineSeg, randPB3Pal, 20, 30, 50);
//Lav->hueCycle = true;

//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(Lav, 0);

//Waves
//--------------
//Waves = new SegWaves(ringSegments, randPB3Pal, 0, 2, 3, 10, true, 60);
//Waves = new SegWaves(ringSegments, randPB3Pal, 0, 5, 0, 10, true, 60);
//Waves->randMode = 1;

//Waves = new SegWaves(CWspiralSegs, randPB2Pal, 0, 5, 0, 5, false, 50); //add glitter?
//Waves->randMode = 1;

//Waves = new SegWaves(lotusSegs, randPB2Pal, 0, 1, 2, 10, false, 65);
//Waves->colorMode = 4;

//Waves = new SegWaves(lotusSegs, randPB2Pal, 0, 5, 2, 10, false, 65); //or 0 spacing
//Waves->randMode = 1;
//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(Waves, 0);

//Pride
//--------------
//Pride2 = new PrideWPalSL2(joinedRingSegments, true, true, 80);
//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(Pride2, 0);

//Rolling Waves
//--------------
//Roll = new RollingWavesSL(mainSegments, randPB3Pal, 0, 5, 1, 2, 120);
//Roll = new RollingWavesSL(joinedRingSegments, 2, 0, 5, 1, 0, 130); //<meh
//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(Roll, 0);

//RollFa = new RollingWavesFastSL(CWspiralSegs, 3, 0, 9, 1, 0, 130);
//RollFa->randMode = 1;
//effectSet.setEffect(RollFa, 0);

//Shifting Sea
//--------------
//seaSft = new ShiftingSeaSL(mainSegments, 3, 20, 0, 8, 0, 60);
//seaSft = new ShiftingSeaSL(CWringSegments, 3, 20, 0, 0, 0, 80);
//seaSft = new ShiftingSeaSL(joinedRingSegments, 3, 20, 0, 0, 1, 80);

//seaSft->randomShift = true;

//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(seaSft, 0);

//Noise
//--------------
//NoiseGrad = new NoiseGradSL(halfRingSegments, randPB3Pal, 0, 3, 7, 20, 40, 40, 5000, 80);
//NoiseGrad->doBrightness = false;

//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(NoiseGrad, 0);

//Noise = new NoiseSL(CWringSegments, randPB3Pal, 20, 30, 40, 10, 0, 80);
//Noise = new NoiseSL(joinedRingSegments, randPB3Pal, 80, 30, 40, 10, 1, 80); //20 steps for mode 0
//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(Noise, 0);

//Grad Cycle
//--------------
//GCF = new GradientCycleFastSL(halfRingSegments, randPB3Pal, 5, 110);
//GCF->randMode = 1;

//GCF = new GradientCycleFastSL(CWringSegments, randPB3Pal, 6, 100);
//GCF->randMode = 1;

//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(GCF, 0);

//Twinkle2
//--------------
//Twinkl2 = new Twinkle2SLSeg(halfRingSegments, randPB3Pal, 0, 5, 500, 5, 5, 5, 5, 0, 70);
//Twinkl2->colorMode = 2;

//Twinkl2 = new Twinkle2SLSeg(joinedRingSegments, randPB3Pal, 0, 10, 500, 2, 5, 5, 5, 0, 70);
//Twinkl2->colorMode = 3;

//Twinkl2 = new Twinkle2SLSeg(joinedRingSegments, randPB3Pal, 0, 25, 300, 2, 6, 2, 6, 2, 60);
//Twinkl2->colorMode = 3;

//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(Twinkl2, 0);

//Color Wipe
//--------------
//ColorWipe = new ColorWipeSLSeg(ringSegments, rand3Pallet3, 9, 2, true, false, false, true, 140);
//ColorWipe->setUpLoop(true, 2, false, 1, true, false, true, false, false);

//ColorWipe = new ColorWipeSLSeg(ringSegments, rand3Pallet3, 10, 2, true, false, false, true, 140);
//ColorWipe->setUpLoop(true, 2, true, 1, true, false, true, false, false);

//ColorWipe = new ColorWipeSLSeg(halfRingSegments, singPal, 11, 0, true, false, true, false, 140);
//ColorWipe->setUpLoop(true, 2, true, 1, true, true, true, false, false);

//if(ColorWipe->loopCount != prevCycleNum){
//prevCycleNum = ColorWipe->loopCount;
//paletteUtilsPS::randomize(singPal);
//}

//ColorWipe = new ColorWipeSLSeg(lotusSegs, rand2Pallet3, 10, 2, true, false, true, true, 140);
//ColorWipe->setUpLoop(true, 2, false, 1, true, false, true, false, false);

//ColorWipe = new ColorWipeSLSeg(CWringSegments, singPal, 10, 0, true, false, true, false, 140);
//ColorWipe->setUpLoop(true, 2, false, 1, true, false, true, false, false);
//if(ColorWipe->loopCount != prevCycleNum){
//prevCycleNum = ColorWipe->loopCount;
//paletteUtilsPS::randomize(singPal);
//}

//effectSet.runTime = 15000;  //20000
//effectSet.setEffect(ColorWipe, 0);

//Extra
//--------------
//Glitter = new AddGlitterPS(joinedRingSegments, CRGB::White, 10, 1, 50, 50);
//&randRate2.outputRate;
//Waves->rate = &rateNoise.outputRate;

//rateNoise.update();
//randRate2.update();
//Glitter->update();

//----------------------------------------------------
