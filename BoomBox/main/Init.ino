// random RNG
#define PRREG(x) Serial.print(#x" 0x"); Serial.println(x,HEX)

#define REPS 50 //not needed in this variant
#define MYDLY 500 //loop delay in milliseconds

#define RNG_CR_GO_MASK                           0x1u
#define RNG_CR_HA_MASK                           0x2u
#define RNG_CR_INTM_MASK                         0x4u
#define RNG_CR_CLRI_MASK                         0x8u
#define RNG_CR_SLP_MASK                          0x10u
#define RNG_SR_OREG_LVL_MASK                     0xFF00u
#define RNG_SR_OREG_LVL_SHIFT                    8
#define RNG_SR_OREG_LVL(x)                       (((uint32_t)(((uint32_t)(x))<<RNG_SR_OREG_LVL_SHIFT))&RNG_SR_OREG_LVL_MASK)
#define SIM_SCGC6_RNGA    ((uint32_t)0x00000200)

//#define Serial Serial1
uint32_t trng(){
    RNG_CR |= RNG_CR_GO_MASK;
    while((RNG_SR & RNG_SR_OREG_LVL(0xF)) == 0); // wait
    return RNG_OR;
}

void rngsetup() {
    SIM_SCGC6 |= SIM_SCGC6_RNGA; // enable RNG
    PRREG(SIM_SCGC6);
    RNG_CR &= ~RNG_CR_SLP_MASK;
    RNG_CR |= RNG_CR_HA_MASK;  // high assurance, not needed
    PRREG(RNG_CR);
    PRREG(RNG_SR);
}

/*--------------------------------------------------*/
//-------------------------Initializers-------------------------
/*
We need an interrupt timer for our morse code so we can provide exact timings
needed in a morse code message. We will be using the built in teensy interrupt
objects to accomplish this
*/
const uint8_t morseLED1 = 29;
const uint8_t morseLED2 = 30;
const uint32_t timeUnit = 750000; //Value is in microseconds
IntervalTimer morseTimer;
char morseWord1[] = {'p','e','a','r'}; 
char morseWord2[] = {'p','a','i','r'};
char* morse1[4];
char* morse2[4];
bool stateChanged = false;
uint8_t currentTimeUnits = 0;
