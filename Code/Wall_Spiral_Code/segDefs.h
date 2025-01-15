//========================================================================
//                          Segment Sets Setups
//========================================================================
//Segment sets re-arrange the LEDs virtually, allowing us to create complex shapes.
//See Pixel Spork wiki for more info on segment sets.

//Note that due to the physical layout of the LEDs, we mainly used mixed sections in the segment sets
//as it is easier to just list all the LEDs in each section.

//-----------------------------------------------------

//As connected:
    //Here the LEDs are arranged to match how they are physically connected in a single line.

    const PROGMEM segmentSecCont mainSec[] = { {0, NUM_LEDS} };
    SegmentPS mainSegment = { mainSec, SIZE(mainSec), true };
    SegmentPS *main_arr[] = { &mainSegment };
    SegmentSetPS mainSegments = {leds, NUM_LEDS, main_arr, SIZE(main_arr)};

//-----------------------------------------------------

//Rings: 
    //The LEDs are arranged into rings around the spiral, starting with the outermost ring and going inward

    const PROGMEM uint16_t ringSec0_arr[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
    const PROGMEM segmentSecMix ringSec0[] = { { ringSec0_arr, SIZE(ringSec0_arr) } };  //Outer Ring 0
    SegmentPS ringSegment0 = {ringSec0, SIZE(ringSec0), true};

    const PROGMEM uint16_t ringSec1_arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    const PROGMEM segmentSecMix ringSec1[] = { { ringSec1_arr, SIZE(ringSec1_arr) } };  //Ring 1
    SegmentPS ringSegment1 = {ringSec1, SIZE(ringSec1), true};

    const PROGMEM uint16_t ringSec2_arr[] = {20, 22, 24, 26, 28, 30, 32, 34, 36, 38};
    const PROGMEM segmentSecMix ringSec2[] = { { ringSec2_arr, SIZE(ringSec2_arr) } };  //Ring 2
    SegmentPS ringSegment2 = {ringSec2, SIZE(ringSec2), true};

    const PROGMEM uint16_t ringSec3_arr[] = {21, 23, 25, 27, 29, 31, 33, 35, 37, 39};
    const PROGMEM segmentSecMix ringSec3[] = { { ringSec3_arr, SIZE(ringSec3_arr) } };  //Ring 3
    SegmentPS ringSegment3 = {ringSec3, SIZE(ringSec3), true};

    const PROGMEM uint16_t ringSec4_arr[] = {40, 42, 44, 46, 48, 50, 52, 54, 56, 58};
    const PROGMEM segmentSecMix ringSec4[] = { { ringSec4_arr, SIZE(ringSec4_arr) } };  //Ring 4
    SegmentPS ringSegment4 = {ringSec4, SIZE(ringSec4), true};

    const PROGMEM uint16_t ringSec5_arr[] = {41, 43, 45, 47, 49, 51, 53, 55, 57, 59};
    const PROGMEM segmentSecMix ringSec5[] = { { ringSec5_arr, SIZE(ringSec5_arr) } };  //Ring 5
    SegmentPS ringSegment5 = {ringSec5, SIZE(ringSec5), true};

    const PROGMEM uint16_t ringSec6_arr[] = {60, 62, 64, 66, 68, 70, 72, 74, 76, 78};
    const PROGMEM segmentSecMix ringSec6[] = { { ringSec6_arr, SIZE(ringSec6_arr) } };  //Ring 6
    SegmentPS ringSegment6 = {ringSec6, SIZE(ringSec6), true};

    const PROGMEM uint16_t ringSec7_arr[] = {61, 63, 65, 67, 69, 71, 73, 75, 77, 79};
    const PROGMEM segmentSecMix ringSec7[] = { { ringSec7_arr, SIZE(ringSec7_arr) } };  //Ring 7
    SegmentPS ringSegment7 = {ringSec7, SIZE(ringSec7), true};

    const PROGMEM segmentSecCont ringSec8[] = { {80, 10} };  //Ring 8 (we can use a continuous section here b/c the ring isn't broken up)
    SegmentPS ringSegment8 = {ringSec8, SIZE(ringSec8), true};

    //the ring segments are grouped together into one segment set, so that effects can act across all of them
    SegmentPS *rings_arr[] = { &ringSegment0, &ringSegment1, &ringSegment2, &ringSegment3, &ringSegment4, 
                               &ringSegment5, &ringSegment6, &ringSegment7, &ringSegment8 };
    SegmentSetPS ringSegments = {leds, NUM_LEDS, rings_arr, SIZE(rings_arr)};

//-----------------------------------------------------

//Joined Rings: 
    //The LEDs are arranged into joined rings around the spiral, 
    //so that the rings are more continuous that the previous set of rings.
    //The rings are formed by combining pairs of rings together from the above "ringSegments", 
    //so that rings 0 and 1, 2 and 3, 4 and 5, 6 and 7 are combined. The final ring 8, remains the same.
    //All rings are 20 LEDs long, except ring 8, which is 10.

    const PROGMEM segmentSecCont joinedRingSec0[] = { {0, 20} };   //ring 0
    SegmentPS joinedRingSegment0 = {joinedRingSec0, SIZE(joinedRingSec0), true};

    const PROGMEM segmentSecCont joinedRingSec1[] = { {20, 20} };  //ring 1
    SegmentPS joinedRingSegment1 = {joinedRingSec1, SIZE(joinedRingSec1), true};

    const PROGMEM segmentSecCont joinedRingSec2[] = { {40, 20} };  //ring 2
    SegmentPS joinedRingSegment2 = {joinedRingSec2, SIZE(joinedRingSec2), true};

    const PROGMEM segmentSecCont joinedRingSec3[] = { {60, 20} };  //ring 3
    SegmentPS joinedRingSegment3 = {joinedRingSec3, SIZE(joinedRingSec3), true};

    //the ring segments are grouped together into one segment set, so that effects can act across all of them
    //note that we re-use the "ringSegment8" from the previous rings set,as it is the same
    SegmentPS *joinedRings_arr[] = { &joinedRingSegment0, &joinedRingSegment1, &joinedRingSegment2, &joinedRingSegment3, &ringSegment8 };
    SegmentSetPS joinedRingSegments = {leds, NUM_LEDS, joinedRings_arr, SIZE(joinedRings_arr)};

//-----------------------------------------------------

//Half Ring Segments
    //The same as the joined rings, but each ring is split in half vertically
    //The half directions are also set to run from top to bottom
    //The top and bottom LEDs of each half pair overlap

    const PROGMEM segmentSecCont halfRingSec0_0[] = { {0, 11} };           //ring 0, half 0
    SegmentPS halfRingSegment0_0 = {halfRingSec0_0, SIZE(halfRingSec0_0), true};

    const PROGMEM segmentSecCont halfRingSec0_1[] = { {10, 10}, {0, 1} };  //ring 0, half 1
    SegmentPS halfRingSegment0_1 = {halfRingSec0_1, SIZE(halfRingSec0_1), false};

    const PROGMEM segmentSecCont halfRingSec1_0[] = { {20, 11} };           //ring 1, half 0
    SegmentPS halfRingSegment1_0 = {halfRingSec1_0, SIZE(halfRingSec1_0), true};

    const PROGMEM segmentSecCont halfRingSec1_1[] = { {30, 10}, {20, 1} };  //ring 1, half 1
    SegmentPS halfRingSegment1_1 = {halfRingSec1_1, SIZE(halfRingSec1_1), false};

    const PROGMEM segmentSecCont halfRingSec2_0[] = { {40, 11} };           //ring 2, half 0
    SegmentPS halfRingSegment2_0 = {halfRingSec2_0, SIZE(halfRingSec2_0), true};

    const PROGMEM segmentSecCont halfRingSec2_1[] = { {50, 10}, {40, 1} };  //ring 2, half 1
    SegmentPS halfRingSegment2_1 = {halfRingSec2_1, SIZE(halfRingSec2_1), false};

    const PROGMEM segmentSecCont halfRingSec3_0[] = { {60, 11} };           //ring 3, half 0
    SegmentPS halfRingSegment3_0 = {halfRingSec3_0, SIZE(halfRingSec3_0), true};

    const PROGMEM segmentSecCont halfRingSec3_1[] = { {70, 10}, {60, 1} };  //ring 3, half 1
    SegmentPS halfRingSegment3_1 = {halfRingSec3_1, SIZE(halfRingSec3_1), false};

    const PROGMEM segmentSecCont halfRingSec4_0[] = { {80, 6} };           //ring 4, half 0
    SegmentPS halfRingSegment4_0 = {halfRingSec4_0, SIZE(halfRingSec4_0), true};

    const PROGMEM segmentSecCont halfRingSec4_1[] = { {85, 5}, {80, 1} };  //ring 4, half 1
    SegmentPS halfRingSegment4_1 = {halfRingSec4_1, SIZE(halfRingSec4_1), false};

    SegmentPS *halfRings_arr[] = { &halfRingSegment0_0, &halfRingSegment0_1, 
                                   &halfRingSegment1_0, &halfRingSegment1_1, &halfRingSegment2_0, &halfRingSegment2_1,
                                   &halfRingSegment3_0, &halfRingSegment3_1, &halfRingSegment4_0, &halfRingSegment4_1 };
    SegmentSetPS halfRingSegments = {leds, NUM_LEDS, halfRings_arr, SIZE(halfRings_arr)};

//-----------------------------------------------------

//Spiral Arms Counter Clockwise 
    //A segment set that splits the LEDs into a series of spiral arms twisting around the center
    //The arms spiral outwards in a counter clockwise direction
    //Note that the default segment direction has the spirals starting at the center and going outwards

    //Arm 0 
    const PROGMEM uint16_t CCWspiralSec0_LEDs[] = { 0, 19, 38, 37, 56, 55, 74, 73, 86 }; 
    const PROGMEM segmentSecMix CCWspiralSec0_arr[] = { { CCWspiralSec0_LEDs, SIZE(CCWspiralSec0_LEDs) } };
    SegmentPS CCWspiralSegment0 = {CCWspiralSec0_arr, SIZE(CCWspiralSec0_arr), false};

    //Arm 1
    const PROGMEM uint16_t CCWspiralSec1_LEDs[] = { 18, 17, 36, 35, 54, 53, 72, 71, 85 }; 
    const PROGMEM segmentSecMix CCWspiralSec1_arr[] = { { CCWspiralSec1_LEDs, SIZE(CCWspiralSec1_LEDs) } };
    SegmentPS CCWspiralSegment1 = {CCWspiralSec1_arr, SIZE(CCWspiralSec1_arr), false};

    //Arm 2
    const PROGMEM uint16_t CCWspiralSec2_LEDs[] = { 16, 15, 34, 33, 52, 51, 70, 69, 84 }; 
    const PROGMEM segmentSecMix CCWspiralSec2_arr[] = { { CCWspiralSec2_LEDs, SIZE(CCWspiralSec2_LEDs) } };
    SegmentPS CCWspiralSegment2 = {CCWspiralSec2_arr, SIZE(CCWspiralSec2_arr), false};

    //Arm 3
    const PROGMEM uint16_t CCWspiralSec3_LEDs[] = { 14, 13, 32, 31, 50, 49, 68, 67, 83 };
    const PROGMEM segmentSecMix CCWspiralSec3_arr[] = { { CCWspiralSec3_LEDs, SIZE(CCWspiralSec3_LEDs) } };
    SegmentPS CCWspiralSegment3 = {CCWspiralSec3_arr, SIZE(CCWspiralSec3_arr), false};

    //Arm 4
    const PROGMEM uint16_t CCWspiralSec4_LEDs[] = { 12, 11, 30, 29, 48, 47, 66, 65, 82 };
    const PROGMEM segmentSecMix CCWspiralSec4_arr[] = { { CCWspiralSec4_LEDs, SIZE(CCWspiralSec4_LEDs) } };
    SegmentPS CCWspiralSegment4 = {CCWspiralSec4_arr, SIZE(CCWspiralSec4_arr), false};

    //Arm 5
    const PROGMEM uint16_t CCWspiralSec5_LEDs[] = { 10, 9, 28, 27, 46, 45, 64, 63, 81 };
    const PROGMEM segmentSecMix CCWspiralSec5_arr[] = { { CCWspiralSec5_LEDs, SIZE(CCWspiralSec5_LEDs) } };
    SegmentPS CCWspiralSegment5 = {CCWspiralSec5_arr, SIZE(CCWspiralSec5_arr), false};

    //Arm 6
    const PROGMEM uint16_t CCWspiralSec6_LEDs[] = { 8, 7, 26, 25, 44, 43, 62, 61, 80 };
    const PROGMEM segmentSecMix CCWspiralSec6_arr[] = { { CCWspiralSec6_LEDs, SIZE(CCWspiralSec6_LEDs) } };
    SegmentPS CCWspiralSegment6 = {CCWspiralSec6_arr, SIZE(CCWspiralSec6_arr), false};

    //Arm 7
    const PROGMEM uint16_t CCWspiralSec7_LEDs[] = { 6, 5, 24, 23, 42, 41, 60, 79, 89 };
    const PROGMEM segmentSecMix CCWspiralSec7_arr[] = { { CCWspiralSec7_LEDs, SIZE(CCWspiralSec7_LEDs) } };
    SegmentPS CCWspiralSegment7 = {CCWspiralSec7_arr, SIZE(CCWspiralSec7_arr), false};

    //Arm 8
    const PROGMEM uint16_t CCWspiralSec8_LEDs[] = { 4, 3, 22, 21, 40, 59, 78, 77, 88 };
    const PROGMEM segmentSecMix CCWspiralSec8_arr[] = { { CCWspiralSec8_LEDs, SIZE(CCWspiralSec8_LEDs) } };
    SegmentPS CCWspiralSegment8 = {CCWspiralSec8_arr, SIZE(CCWspiralSec8_arr), false};

    //Arm 9 
    const PROGMEM uint16_t CCWspiralSec9_LEDs[] = { 2, 1, 20, 39, 58, 57, 76, 75, 87 }; 
    const PROGMEM segmentSecMix CCWspiralSec9_arr[] = { { CCWspiralSec9_LEDs, SIZE(CCWspiralSec9_LEDs) } };
    SegmentPS CCWspiralSegment9 = {CCWspiralSec9_arr, SIZE(CCWspiralSec9_arr), false};

    //Final segment set
    SegmentPS *CCWspiral_arr[] = { &CCWspiralSegment0, &CCWspiralSegment1, &CCWspiralSegment2, &CCWspiralSegment3, &CCWspiralSegment4, 
                                   &CCWspiralSegment5, &CCWspiralSegment6, &CCWspiralSegment7, &CCWspiralSegment8, &CCWspiralSegment9 };
    SegmentSetPS CCWspiralSegs = {leds, NUM_LEDS, CCWspiral_arr, SIZE(CCWspiral_arr)};

//-----------------------------------------------------

//Counter Clockwise Spiral Line
    //Takes the counter clockwise spiral arms from above and combines them into one "continuous" line,
    //starting from the outer point of arm 0, and zig-zagging counter clockwise
    const PROGMEM uint16_t CCWSpiralLine_arr[] = { 0, 19, 38, 37, 56, 55, 74, 73, 86,   //arm 0
                                                   85, 71, 72, 53, 54, 35, 36, 17, 18,  //arm 1
                                                   16, 15, 34, 33, 52, 51, 70, 69, 84,  //arm 2
                                                   83, 67, 68, 49, 50, 31, 32, 13, 14,  //arm 3
                                                   12, 11, 30, 29, 48, 47, 66, 65, 82,  //arm 4
                                                   81, 63, 64, 45, 46, 27, 28, 9, 10,   //arm 5
                                                   8, 7, 26, 25, 44, 43, 62, 61, 80,    //arm 6
                                                   89, 79, 60, 41, 42, 23, 24, 5, 6,    //arm 7
                                                   4, 3, 22, 21, 40, 59, 78, 77, 88,    //arm 8
                                                   87, 75, 76, 57, 58, 39, 20, 1, 2 };  //arm 9
    const PROGMEM segmentSecMix CCWspiralLineSec[] = { { CCWSpiralLine_arr, SIZE(CCWSpiralLine_arr) } };
    SegmentPS CCWspiralLine = {CCWspiralLineSec, SIZE(CCWspiralLineSec), true};

    //Final segment set
    SegmentPS *CCWspiralLine_arr[] = { &CCWspiralLine };
    SegmentSetPS CCWspiralLineSeg = {leds, NUM_LEDS, CCWspiralLine_arr, SIZE(CCWspiralLine_arr)};

//-----------------------------------------------------

//Spiral Arms Clockwise 
    //A segment set that splits the LEDs into a series of spiral arms twisting around the center
    //The arms spiral outwards in a clockwise direction
    //Note that the default segment direction has the spirals starting at the center and going outwards

    //Arm 0
    const PROGMEM uint16_t CWspiralSec0_LEDs[] = {0, 1, 22, 23, 44, 45, 66, 67, 84};
    const PROGMEM segmentSecMix CWspiralSec0_arr[] = { { CWspiralSec0_LEDs, SIZE(CWspiralSec0_LEDs) } };
    SegmentPS CWspiralSegment0 = {CWspiralSec0_arr, SIZE(CWspiralSec0_arr), false};

    //Arm 1
    const PROGMEM uint16_t CWspiralSec1_LEDs[] = {2, 3, 24, 25, 46, 47, 68, 69, 85};
    const PROGMEM segmentSecMix CWspiralSec1_arr[] = { { CWspiralSec1_LEDs, SIZE(CWspiralSec1_LEDs) } };
    SegmentPS CWspiralSegment1 = {CWspiralSec1_arr, SIZE(CWspiralSec1_arr), false};

    //Arm 2
    const PROGMEM uint16_t CWspiralSec2_LEDs[] = {4, 5, 26, 27, 48, 49, 70, 71, 86};
    const PROGMEM segmentSecMix CWspiralSec2_arr[] = { { CWspiralSec2_LEDs, SIZE(CWspiralSec2_LEDs) } };
    SegmentPS CWspiralSegment2 = {CWspiralSec2_arr, SIZE(CWspiralSec2_arr), false};

    //Arm 3
    const PROGMEM uint16_t CWspiralSec3_LEDs[] = {6, 7, 28, 29, 50, 51, 72, 73, 87};
    const PROGMEM segmentSecMix CWspiralSec3_arr[] = { { CWspiralSec3_LEDs, SIZE(CWspiralSec3_LEDs) } };
    SegmentPS CWspiralSegment3 = {CWspiralSec3_arr, SIZE(CWspiralSec3_arr), false};

    //Arm 4
    const PROGMEM uint16_t CWspiralSec4_LEDs[] = {8, 9, 30, 31, 52, 53, 74, 75, 88};
    const PROGMEM segmentSecMix CWspiralSec4_arr[] = { { CWspiralSec4_LEDs, SIZE(CWspiralSec4_LEDs) } };
    SegmentPS CWspiralSegment4 = {CWspiralSec4_arr, SIZE(CWspiralSec4_arr), false};

    //Arm 5
    const PROGMEM uint16_t CWspiralSec5_LEDs[] = {10, 11, 32, 33, 54, 55, 76, 77, 89};
    const PROGMEM segmentSecMix CWspiralSec5_arr[] = { { CWspiralSec5_LEDs, SIZE(CWspiralSec5_LEDs) } };
    SegmentPS CWspiralSegment5 = {CWspiralSec5_arr, SIZE(CWspiralSec5_arr), false};

    //Arm 6
    const PROGMEM uint16_t CWspiralSec6_LEDs[] = {12, 13, 34, 35, 56, 57, 78, 79, 80};
    const PROGMEM segmentSecMix CWspiralSec6_arr[] = { { CWspiralSec6_LEDs, SIZE(CWspiralSec6_LEDs) } };
    SegmentPS CWspiralSegment6 = {CWspiralSec6_arr, SIZE(CWspiralSec6_arr), false};

    //Arm 7
    const PROGMEM uint16_t CWspiralSec7_LEDs[] = {14, 15, 36, 37, 58, 59, 60, 61, 81};
    const PROGMEM segmentSecMix CWspiralSec7_arr[] = { { CWspiralSec7_LEDs, SIZE(CWspiralSec7_LEDs) } };
    SegmentPS CWspiralSegment7 = {CWspiralSec7_arr, SIZE(CWspiralSec7_arr), false};

    //Arm 8
    const PROGMEM uint16_t CWspiralSec8_LEDs[] = {16, 17, 38, 39, 40, 41, 62, 63, 82};
    const PROGMEM segmentSecMix CWspiralSec8_arr[] = { { CWspiralSec8_LEDs, SIZE(CWspiralSec8_LEDs) } };
    SegmentPS CWspiralSegment8 = {CWspiralSec8_arr, SIZE(CWspiralSec8_arr), false};

    //Arm 9
    const PROGMEM uint16_t CWspiralSec9_LEDs[] = {18, 19, 20, 21, 42, 43, 64, 65, 83};
    const PROGMEM segmentSecMix CWspiralSec9_arr[] = { { CWspiralSec9_LEDs, SIZE(CWspiralSec9_LEDs) } };
    SegmentPS CWspiralSegment9 = {CWspiralSec9_arr, SIZE(CWspiralSec9_arr), false};

    //Final segment set
    SegmentPS *CWspiral_arr[] = { &CWspiralSegment0, &CWspiralSegment1, &CWspiralSegment2, &CWspiralSegment3, &CWspiralSegment4, 
                                  &CWspiralSegment5, &CWspiralSegment6, &CWspiralSegment7, &CWspiralSegment8, &CWspiralSegment9 };
    SegmentSetPS CWspiralSegs = {leds, NUM_LEDS, CWspiral_arr, SIZE(CWspiral_arr)};

//-----------------------------------------------------

//Clockwise Spiral Line
    //Takes the clockwise spiral arms from above and combines them into one "continuous" line,
    //starting from the outer point of arm 0, and zig-zagging clockwise.
    const PROGMEM uint16_t CWSpiralLine_arr[] = {   0, 1, 22, 23, 44, 45, 66, 67, 84,     //arm 0
                                                    85, 69, 68, 47, 46, 25, 24, 3, 2,     //arm 1
                                                    4, 5, 26, 27, 48, 49, 70, 71, 86,     //arm 2
                                                    87, 73, 72, 51, 50, 29, 28, 7, 6,     //arm 3
                                                    8, 9, 30, 31, 52, 53, 74, 75, 88,     //arm 4
                                                    89, 77, 76, 55, 54, 33, 32, 11, 10,   //arm 5
                                                    12, 13, 34, 35, 56, 57, 78, 79, 80,   //arm 6
                                                    81, 61, 60, 59, 58, 37, 36, 15, 14,   //arm 7
                                                    16, 17, 38, 39, 40, 41, 62, 63, 82,   //arm 8
                                                    83, 65, 64, 43, 42, 21, 20, 19, 18 }; //arm 9
    const PROGMEM segmentSecMix CWspiralLineSec[] = { { CWSpiralLine_arr, SIZE(CWSpiralLine_arr) } };
    SegmentPS CWspiralLine = {CWspiralLineSec, SIZE(CWspiralLineSec), true};

    //Final segment set
    SegmentPS *CWspiralLine_arr[] = { &CWspiralLine };
    SegmentSetPS CWspiralLineSeg = {leds, NUM_LEDS, CWspiralLine_arr, SIZE(CWspiralLine_arr)};

//-----------------------------------------------------

//Spiral Rings Clockwise

    //The LEDs are arranged to form "rings" that will create segment line spirals
    //runnning clockwise. 
    //This is basically the orthogonal version of Spiral Arms Clockwise Below
    //So that each **segment line** forms a clockwise spiral arm
    //To create this, it's basically the same as the "Rings" segment set, but we stagger the starting point of each ring

    //Ring 0
    const PROGMEM uint16_t CWringSec0_arr[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
    const PROGMEM segmentSecMix CWringSec0[] = { { CWringSec0_arr, SIZE(CWringSec0_arr) } };  
    SegmentPS CWringSegment0 = {CWringSec0, SIZE(CWringSec0), true}; 

    //Ring 1
    const PROGMEM uint16_t CWringSec1_arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    const PROGMEM segmentSecMix CWringSec1[] = { { CWringSec1_arr, SIZE(CWringSec1_arr) } };  
    SegmentPS CWringSegment1 = {CWringSec1, SIZE(CWringSec1), true}; 

    //Ring 2
    const PROGMEM uint16_t CWringSec2_arr[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 20};
    const PROGMEM segmentSecMix CWringSec2[] = { { CWringSec2_arr, SIZE(CWringSec2_arr) } };  
    SegmentPS CWringSegment2 = {CWringSec2, SIZE(CWringSec2), true}; 

    //Ring 3
    const PROGMEM uint16_t CWringSec3_arr[] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 21};
    const PROGMEM segmentSecMix CWringSec3[] = { { CWringSec3_arr, SIZE(CWringSec3_arr) } };  
    SegmentPS CWringSegment3 = {CWringSec3, SIZE(CWringSec3), true}; 

    //Ring 4
    const PROGMEM uint16_t CWringSec4_arr[] = {44, 46, 48, 50, 52, 54, 56, 58, 40, 42};
    const PROGMEM segmentSecMix CWringSec4[] = { { CWringSec4_arr, SIZE(CWringSec4_arr) } };  
    SegmentPS CWringSegment4 = {CWringSec4, SIZE(CWringSec4), true}; 

    //Ring 5
    const PROGMEM uint16_t CWringSec5_arr[] = {45, 47, 49, 51, 53, 55, 57, 59, 41, 43};
    const PROGMEM segmentSecMix CWringSec5[] = { { CWringSec5_arr, SIZE(CWringSec5_arr) } };  
    SegmentPS CWringSegment5 = {CWringSec5, SIZE(CWringSec5), true}; 

    //Ring 6
    const PROGMEM uint16_t CWringSec6_arr[] = {66, 68, 70, 72, 74, 76, 78, 60, 62, 64};
    const PROGMEM segmentSecMix CWringSec6[] = { { CWringSec6_arr, SIZE(CWringSec6_arr) } };  
    SegmentPS CWringSegment6 = {CWringSec6, SIZE(CWringSec6), true}; 

    //Ring 7
    const PROGMEM uint16_t CWringSec7_arr[] = {67, 69, 71, 73, 75, 77, 79, 61, 63, 65};
    const PROGMEM segmentSecMix CWringSec7[] = { { CWringSec7_arr, SIZE(CWringSec7_arr) } };  
    SegmentPS CWringSegment7 = {CWringSec7, SIZE(CWringSec7), true}; 

    //Ring 8
    const PROGMEM uint16_t CWringSec8_arr[] = {84, 85, 86, 87, 88, 89, 80, 81, 82, 83};
    const PROGMEM segmentSecMix CWringSec8[] = { { CWringSec8_arr, SIZE(CWringSec8_arr) } };  
    SegmentPS CWringSegment8 = {CWringSec8, SIZE(CWringSec8), true}; 

    SegmentPS *CWRings_arr[] = { &CWringSegment0, &CWringSegment1, &CWringSegment2, &CWringSegment3, &CWringSegment4,
                                 &CWringSegment5, &CWringSegment6, &CWringSegment7, &CWringSegment8 };
    SegmentSetPS CWringSegments = {leds, NUM_LEDS, CWRings_arr, SIZE(CWRings_arr)};

//-----------------------------------------------------

//Lotus
    //An expanding lotus shape, starting with the inner most vertical LED (80) 
    //and expanding outwards around the wall spiral
    //max segment length: 17

    //Part 0
    const PROGMEM uint16_t lotusSec0_LEDs[] = { 80 };
    const PROGMEM segmentSecMix lotusSec0_arr[] = { { lotusSec0_LEDs, SIZE(lotusSec0_LEDs) } };
    SegmentPS lotusSegment0 = {lotusSec0_arr, SIZE(lotusSec0_arr), true};

    //Part 1
    const PROGMEM uint16_t lotusSec1_LEDs[] = { 89, 79, 60, 61, 81 };
    const PROGMEM segmentSecMix lotusSec1_arr[] = { { lotusSec1_LEDs, SIZE(lotusSec1_LEDs) } };
    SegmentPS lotusSegment1 = {lotusSec1_arr, SIZE(lotusSec1_arr), true};

    //Part 2
    const PROGMEM uint16_t lotusSec2_LEDs[] = { 88, 77, 78, 59, 40, 41, 62, 63, 82 };
    const PROGMEM segmentSecMix lotusSec2_arr[] = { { lotusSec2_LEDs, SIZE(lotusSec2_LEDs) } };
    SegmentPS lotusSegment2 = {lotusSec2_arr, SIZE(lotusSec2_arr), true};

    //Part 3
    const PROGMEM uint16_t lotusSec3_LEDs[] = { 87, 75, 76, 57, 58, 39, 20, 21, 42, 43, 64, 65, 83 };
    const PROGMEM segmentSecMix lotusSec3_arr[] = { { lotusSec3_LEDs, SIZE(lotusSec3_LEDs) } };
    SegmentPS lotusSegment3 = {lotusSec3_arr, SIZE(lotusSec3_arr), true};

    //Part 4
    const PROGMEM uint16_t lotusSec4_LEDs[] = { 86, 73, 74, 55, 56, 37, 38, 19, 0, 1, 22, 23, 44, 45, 66, 67, 84 };
    const PROGMEM segmentSecMix lotusSec4_arr[] = { { lotusSec4_LEDs, SIZE(lotusSec4_LEDs) } };
    SegmentPS lotusSegment4 = {lotusSec4_arr, SIZE(lotusSec4_arr), true};

    //Part 5
    const PROGMEM uint16_t lotusSec5_LEDs[] = { 85, 71, 72, 53, 54, 35, 36, 17, 18, 2, 3, 24, 25, 46, 47, 68, 69};
    const PROGMEM segmentSecMix lotusSec5_arr[] = { { lotusSec5_LEDs, SIZE(lotusSec5_LEDs) } };
    SegmentPS lotusSegment5 = {lotusSec5_arr, SIZE(lotusSec5_arr), true};

    //Part 6
    const PROGMEM uint16_t lotusSec6_LEDs[] = { 70, 51, 52, 33, 34, 15, 16, 4, 5, 26, 27, 48, 49 };
    const PROGMEM segmentSecMix lotusSec6_arr[] = { { lotusSec6_LEDs, SIZE(lotusSec6_LEDs) } };
    SegmentPS lotusSegment6 = {lotusSec6_arr, SIZE(lotusSec6_arr), true};

    //Part 7
    const PROGMEM uint16_t lotusSec7_LEDs[] = { 50, 31, 32, 13, 14, 6, 7, 28, 29 };
    const PROGMEM segmentSecMix lotusSec7_arr[] = { { lotusSec7_LEDs, SIZE(lotusSec7_LEDs) } };
    SegmentPS lotusSegment7 = {lotusSec7_arr, SIZE(lotusSec7_arr), true};

    //Part 8
    const PROGMEM uint16_t lotusSec8_LEDs[] = { 30, 11, 12, 8, 9 };
    const PROGMEM segmentSecMix lotusSec8_arr[] = { { lotusSec8_LEDs, SIZE(lotusSec8_LEDs) } };
    SegmentPS lotusSegment8 = {lotusSec8_arr, SIZE(lotusSec8_arr), true};

    //Part 9
    const PROGMEM uint16_t lotusSec9_LEDs[] = { 10 };
    const PROGMEM segmentSecMix lotusSec9_arr[] = { { lotusSec9_LEDs, SIZE(lotusSec9_LEDs) } };
    SegmentPS lotusSegment9 = {lotusSec9_arr, SIZE(lotusSec9_arr), true};

    //Final segment set
    SegmentPS *lotus_arr[] = { &lotusSegment0, &lotusSegment1, &lotusSegment2, &lotusSegment3, &lotusSegment4, 
                               &lotusSegment5, &lotusSegment6, &lotusSegment7, &lotusSegment8, &lotusSegment9 };
    SegmentSetPS lotusSegs = {leds, NUM_LEDS, lotus_arr, SIZE(lotus_arr)};

//-----------------------------------------------------   

