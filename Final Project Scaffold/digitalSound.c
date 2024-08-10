#include "gba.h"
#include "digitalSound.h"

void setupSounds() {

    REG_SOUNDCNT_X = SND_ENABLED;

	REG_SOUNDCNT_H = SND_OUTPUT_RATIO_100 |
                     DSA_OUTPUT_RATIO_100 |
                     DSA_OUTPUT_TO_BOTH |
                     DSA_TIMER0 |
                     DSA_FIFO_RESET |
                     DSB_OUTPUT_RATIO_100 |
                     DSB_OUTPUT_TO_BOTH |
                     DSB_TIMER1 |
                     DSB_FIFO_RESET;

	REG_SOUNDCNT_L = 0;

}

void playSoundA(const signed char* sound, int length, int loops) {

    DMANow(1, sound, REG_FIFO_A, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);

    int cyclesPerSample = PROCESSOR_CYCLES_PER_SECOND / SOUND_FREQ;

    REG_TM0CNT = 0;
    REG_TM0D = 65536 - cyclesPerSample;
    REG_TM0CNT = TIMER_ON;

    soundA.data = sound;
    soundA.dataLength = length;
    soundA.looping = loops;
    soundA.isPlaying = 1;
    soundA.durationInVBlanks = (VBLANK_FREQ / SOUND_FREQ) * length;
    soundA.vBlankCount = 0;

}


void playSoundB(const signed char* sound, int length, int loops) {

    DMANow(2, sound, REG_FIFO_B, DMA_DESTINATION_FIXED | DMA_AT_REFRESH | DMA_REPEAT | DMA_32);

    int cyclesPerSample = PROCESSOR_CYCLES_PER_SECOND / SOUND_FREQ;

    REG_TM1CNT = 0;
    REG_TM1D = 65536 - cyclesPerSample;
    REG_TM1CNT = TIMER_ON;

    soundB.data = sound;
    soundB.dataLength = length;
    soundB.looping = loops;
    soundB.isPlaying = 1;
    soundB.durationInVBlanks = (VBLANK_FREQ / SOUND_FREQ) * length;
    soundB.vBlankCount = 0;

}

void pauseSounds() {
    soundA.isPlaying = 0;
    soundB.isPlaying = 0;
    REG_TM0CNT = TIMER_OFF;
    REG_TM1CNT = TIMER_OFF;
}

void unpauseSounds() {
    soundA.isPlaying = 1;
    soundB.isPlaying = 1;
    REG_TM0CNT = TIMER_ON;
    REG_TM1CNT = TIMER_ON;
}

void stopSounds() {
    soundA.isPlaying = 0;
    soundB.isPlaying = 0;
    REG_TM0CNT = TIMER_OFF;
    REG_TM1CNT = TIMER_OFF;
    dma[1].cnt = 0;
    dma[2].cnt = 0;
}