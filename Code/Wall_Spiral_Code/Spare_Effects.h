//--------------------------------------------------
//     Effects Depot For Unused Effects
//---------------------------------------------------
/*
If you want to use these effects, you may also have to uncomment 
the effect pointer in the "Effects Names" section of the main code.
Particles
--------------
Parti = new ParticlesSL(joinedRingSegments, randPB3Pal, 0, 3, 2, 80, 50, 1, 0, 2, 0, 2, 2, randPB3Pal.length, true);
Parti->colorMode = 3;
effectSet.setEffect(Parti, 2);

particles single and inf trails effects
--------------
Parti = new ParticlesSL(CWspiralLineSeg, randPB3Pal, 0, 6, 2, 60, 30, 1, 0, 0, 0, 0, 2, randPB3Pal.length, true);
particleUtilsPS::setAllTrailRand(*Parti->particleSet, true, false, false, false, true);
effectSet.setEffect(Parti, 2);
rainSeg = new RainSeg(CCWspiralSegs, randPB3Pal, 0, true, 10, 2, 1, 0, 1, 3, true, false, false, false, true, 80, 50);
effectSet.setEffect(rainSeg, 2);

Pacifica
--------------
PaciH = new PacificaHueSL(CWspiralLineSeg, false, 0, 120, 60); //joinedRingSegments
effectSet.setEffect(PaciH, 2);

Streamer
--------------
Strem = new StreamerSL(CCWspiralLineSeg, randPB2Pal, 0, 9, 0, 10, 20);
Strem = new StreamerSL(joinedRingSegments, CRGB::White, 0, 2, 3, 10, 20);
Strem->bgColorMode = 3;

Strem = new StreamerSL(joinedRingSegments, randPB3Pal, 0, 3, 2, 10, 20);
Strem->colorMode = 3;

effectSet.setEffect(Strem, 2);

Color Melt
--------------
ColMelt = new ColorMeltSL(CCWspiralSegs, randPB2Pal, 15, 3, true, 80);
effectSet.setEffect(ColMelt, 2);

Lava
--------------
Lav = new LavaPS(CWspiralLineSeg, 20, 50, 50); //default fire colors
effectSet.setEffect(Lav, 2);

Waves
--------------
Waves = new SegWaves(ringSegments, randPB3Pal, 0, 2, 3, 10, true, 60);
Waves = new SegWaves(ringSegments, randPB3Pal, 0, 5, 0, 10, true, 60);
Waves->randMode = 1;

Waves = new SegWaves(CWspiralSegs, randPB2Pal, 0, 5, 0, 5, false, 50); //add glitter?
Waves->randMode = 1;

Waves = new SegWaves(lotusSegs, randPB2Pal, 0, 5, 2, 10, false, 65); //or 0 spacing
Waves->randMode = 1;

effectSet.setEffect(Waves, 2);

Rolling Waves
--------------
Roll = new RollingWavesSL(joinedRingSegments, 2, 0, 5, 1, 0, 130);
effectSet.setEffect(Roll, 2);

Shifting Sea
--------------
seaSft = new ShiftingSeaSL(mainSegments, 3, 20, 0, 8, 0, 60);
seaSft = new ShiftingSeaSL(CWringSegments, 3, 20, 0, 0, 0, 80);
seaSft = new ShiftingSeaSL(joinedRingSegments, 3, 20, 0, 0, 1, 80);

seaSft->randomShift = true;

effectSet.setEffect(seaSft, 2);

Noise and Noise Gradient
--------------
NoiseGrad = new NoiseGradSL(halfRingSegments, randPB3Pal, 0, 3, 7, 20, 40, 40, 5000, 80);
NoiseGrad->doBrightness = false;
effectSet.setEffect(NoiseGrad, 2);

Noise = new NoiseSL(CWringSegments, randPB3Pal, 20, 30, 40, 10, 0, 80);
effectSet.setEffect(Noise, 2);

Grad Cycle
--------------
GCF = new GradientCycleFastSL(halfRingSegments, randPB3Pal, 5, 110);
GCF->randMode = 1;
effectSet.setEffect(GCF, 2);

Twinkle2
--------------
Twinkl2 = new Twinkle2SLSeg(joinedRingSegments, randPB3Pal, 0, 10, 500, 2, 5, 5, 5, 0, 70);
Twinkl2->colorMode = 3;
effectSet.setEffect(Twinkl2, 2);

Color Wipe
--------------
ColorWipe = new ColorWipeSLSeg(ringSegments, rand3Pallet3, 10, 2, true, false, false, true, 140);
ColorWipe->setUpLoop(true, 2, true, 1, true, false, true, false, false);

ColorWipe = new ColorWipeSLSeg(halfRingSegments, singPal, 11, 0, true, false, true, false, 140);
ColorWipe->setUpLoop(true, 2, true, 1, true, true, true, false, false);

if(ColorWipe->loopCount != prevCycleNum){
    prevCycleNum = ColorWipe->loopCount;
    paletteUtilsPS::randomize(singPal);
}

effectSet.setEffect(ColorWipe, 2);

Extra Utils
--------------
Glitter = new AddGlitterPS(joinedRingSegments, CRGB::White, 10, 1, 50, 50);

&randRate2.outputRate;
Waves->rate = &rateNoise.outputRate;

rateNoise.update();
randRate2.update();
Glitter->update();

//----------------------------------------------------
*/