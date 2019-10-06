uint16_t upperCount1;
int32_t Upper1;   
int32_t  Lower1;          
uint16_t lowerCount1;     
uint32_t RunningSum1;
uint8_t DummyCount1;

void QT4_ISR1() {
  
}



void startPDB(uint32_t freq=44100) {
  CCM_CCGR6 |= (0x3 << 16);  //Clock is on during all modes except STOP.
    
    //USE TIMER4, CHANNEL 0 FOR AS ARBITRARY CHOICE
    TMR4_CTRL0 &= (0xFFFF ^ (0x7 << 13)); //Stops the timer
    TMR4_ENBL &= (0xFFFF ^ 0xF); //Disables the timer on all channels.
   
    uint32_t QTimerClock = 150000000; //Can I pull this from one of the core*.h?
    uint32_t MagFact = 300000000; //A large number at least twice as large as QTimerClock to magnify a decimal and turn it into an int.  The size makes it so we won't lose more resolution on our calculations than exists in the clock.
    // Of course, the current size of MagFact only works for 150MHz.  The Clock Divide will eat into the precision.
    uint8_t clockDivide = 1;
    
    if (freq > QTimerClock) { //Clearly this will work at 150MHz...
        return;
    } else if (freq < 20){
        return; //Too slow
    } else if (freq < 40) {
        //TMR4_CTRL0[9:12] = 1111
        clockDivide = 128;
        TMR4_CTRL0 &= (0xFFFF ^ (0xF << 9)); //Clear Bits
        TMR4_CTRL0 |= (0xF << 9); // Set Bits
    } else if (freq < 80) {
        //TMR4_CTRL0[9:12] = 1110
        //Clock Divide to 64
        clockDivide = 64;
        TMR4_CTRL0 &= (0xFFFF ^ (0xF << 9));
        TMR4_CTRL0 |= (0xE << 9);
    } else if (freq < 150) {
        //TMR4_CTRL0[9:12] = 1101
        //Clock Divide to 32
        clockDivide = 32;
        TMR4_CTRL0 &= (0xFFFF ^ (0xF << 9));
        TMR4_CTRL0 |= (0xD << 9);
    } else if (freq < 300) {
        //TMR4_CTRL0[9:12] = 1100
        //Clock Divide to 16
        clockDivide = 16;
        TMR4_CTRL0 &= (0xFFFF ^ (0xF << 9));
        TMR4_CTRL0 |= (0xC << 9);
    } else if (freq < 600) {
        //TMR4_CTRL0[9:12] = 1011
        //Clock Divide to 8
        clockDivide = 8;
        TMR4_CTRL0 &= (0xFFFF ^ (0xF << 9));
        TMR4_CTRL0 |= (0xB << 9);
    } else if (freq < 1200) {
        //TMR4_CTRL0[9:12] = 1010
        //Clock Divide to 4
        clockDivide = 4;
        TMR4_CTRL0 &= (0xFFFF ^ (0xF << 9));
        TMR4_CTRL0 |= (0xA << 9);
    } else if (freq < 2500){
        //TMR4_CTRL0[9:12] = 1001
        //Clock Divide to 2
        clockDivide = 2;
        TMR4_CTRL0 &= (0xFFFF ^ (0xF << 9));
        TMR4_CTRL0 |= (0x9 << 9);
    } else {
        //TMR4_CTRL0[9:12] = 1000
        //Clock Divide to 1
        TMR4_CTRL0 &= (0xFFFF ^ (0xF << 9));
        TMR4_CTRL0 |= (0x8 << 9);
    }

    double wholeRatio = ((double) QTimerClock) / ((double) freq * (double) clockDivide);  // Ratio of the clock and the desired frequency.
        upperCount1 = floor(wholeRatio);                                // The integer part of the ratio gives the upper bound of clock ticks for the desired frequency. (fast)
        double ratio = wholeRatio - upperCount1;                        // The decimal remainder of the ratio.  This is the target we will aim to achieve with our alternating timer counts
        Upper1 = round(MagFact * ratio);                                // Get the Upper Int
        Lower1 = Upper1 - MagFact;                                       // The Lower Int
        lowerCount1 = upperCount1 + 1;                                   // The lower bound of clock ticks (slow)
        TMR4_COMP10 = upperCount1;                                      // Compare Register 1  0xFFFF, it always starts off as fast
        TMR4_CMPLD10 = lowerCount1;                                     // Compare 1 Load Register 0xFFFF, the second one will always be slow.
        RunningSum1 = Upper1 + Lower1;                                    // Current Running sum to modify.

        //setHardwareTrigger(); // trigger ADC with hardware, still using swtrigger for T4 fake PDB.  Can try HW if this doesn't work.
        attachInterruptVector(IRQ_QTIMER4, QT4_ISR1);

        NVIC_ENABLE_IRQ(IRQ_QTIMER4);
    
    //To Find in i.MX RT1060 Reference, search "TMRx_CSCTRLn", "TMRx_FILTn", "TMRx_DMAn", "TMRx_SCTRL", "TMRx_CTRLn", or "TMRx_ENBL"  Currently starts on Page 3125
    TMR4_CSCTRL0 &= (0xFFFF ^ (0x3 << 14));     //normal Operations during debug 
    TMR4_CSCTRL0 &= (0xFFFF ^ (0x1 << 13));     //Fault input disabled
    TMR4_CSCTRL0 &= (0xFFFF ^ (0x1 << 12));     //Alt-Load disabled
    TMR4_CSCTRL0 &= (0xFFFF ^ (0x1 << 11));     //Reload on Capture disabled, doesn't really matter
    TMR4_CSCTRL0 &= (0xFFFF ^ (0x1 << 7));      //Compare2 Interrupt Disabled
    TMR4_CSCTRL0 &= (0xFFFF ^ (0x1 << 6));      //Compare1 Interrupt Disabled
    TMR4_CSCTRL0 &= (0xFFFF ^ (0x1 << 5));      //Compare2 Interrupt Flag
    TMR4_CSCTRL0 &= (0xFFFF ^ (0x1 << 4));      //Compare1 Interrupt Flag
    TMR4_CSCTRL0 &= (0xFFFF ^ (0x3 << 2));      //Compare load 2 is never preloaded 
    TMR4_CSCTRL0 &= (0xFFFF ^ (0x1 << 1));      //See Bit 0
    TMR4_CSCTRL0 |= (0x1);                      //Compare load 1 is preloaded on successful compare1
    
    TMR4_FILT0 &= (0xFFFF ^ (0xFF));            //Bypass input filter
    
    TMR4_DMA0 &= (0xFFFF ^ (0x7));              //Disables DMA transfers to CMPLD.  Might want to look at this in the future
    
    TMR4_SCTRL0 &= (0xFFFF ^ 0x3FFF);            //Various interrupts and options disabled
    TMR4_SCTRL0 &= (0xFFFF ^ (0x1 << 15));       //Compare Flag, high is compare occured
    TMR4_SCTRL0 |= (0x1 << 14);                  //Compare Interrupt Enable
    
    TMR4_CTRL0 &= (0xFFFF ^ (0x1 << 6));        //Continuous Operation
    TMR4_CTRL0 |= (0x1 << 5);                   //Count until Compare, rollover, load new compare from load register
    TMR4_CTRL0 &= (0xFFFF ^ (0x1 << 4));        //Count up
    TMR4_CTRL0 &= (0xFFFF ^ (0x1 << 3));        //Disable CoChannel Init
    TMR4_CTRL0 &= (0xFFFF ^ (0x7));             //Output is high
    
    TMR4_ENBL |= 0x1;                           //Enables the timer on channel 0.  This means channels 1-3 have been disabled by this module.  May be unwanted.
    TMR4_CTRL0 |= (0x1 << 13);                  //Starts the timer
}
