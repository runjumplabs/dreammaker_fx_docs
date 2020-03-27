// Copyright (c) 2020 Run Jump Labs LLC.  All right reserved.
// This code is licensed under MIT license (see license.txt for details)

/**
 * @file
 * \defgroup Enumerations
 */

#ifndef AFX_FRAMEWORK_EFFECTS_DMFX_EFFECTS_DEFINES_H_
#define AFX_FRAMEWORK_EFFECTS_DMFX_EFFECTS_DEFINES_H_

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/// Types of effects
typedef enum {
    FX_NONE,
    FX_ADSR_ENVELOPE,
    FX_ALLPASS_FILTER,
    FX_ARPEGGIATOR,
    FX_AMPLITUDE_MODULATOR,
    FX_BIQUAD_FILTER,
    FX_COMPRESSOR,
    FX_DELAY,
    FX_DELAY_MULTITAP,
    FX_DESTRUCTOR,
    FX_ENVELOPE_TRACKER,
    FX_GAIN,
    FX_HARMONIZER,
    FX_IMPULSE_RESPONSE,
    FX_LOOPER,
    FX_MIXER_2,
    FX_MIXER_3,
    FX_MIXER_4,
    FX_OSCILLATOR,
    FX_INSTRUMENT_SYNTH,
    FX_PHASE_SHIFTER,
    FX_PITCH_SHIFT,
    FX_RING_MOD,
    FX_SLICER,
    FX_SPECTRALIZER,
    FX_VARIABLE_DELAY,
    FX_CANVAS = (254),
    FX_UNDEFINED = (255),
} EFFECT_TYPE;

typedef enum {
    T_BOOL, T_INT16, T_INT32, T_FLOAT
} PARAM_TYPES;

typedef enum {
  NODE_FLOAT,
  NODE_BOOL,
  NODE_INT32,
  NODE_NOTE
} CTRL_NODE_TYPE;

#define ARP_MAX_STEPS   (16)

#endif

typedef enum {
    IR_SPRING_LONG,
    IR_SPRING_MED,
    IR_HF_SPRING,
    IR_PLATE_1,
    IR_MAX
} IMPULSE;

/**
 * @brief: A step of the arpeggiator.  Set values not used to zero.
 * \ingroup Enumerations
 */
typedef struct {
    float   freq;       /**< Control frequency (for note or filter) */
    float   vol;        /**< Control volume */
    float   dur;        /**< Duration of this event */
    float   param_1;    /**< Auxiliary parameter #1 */
    float   param_2;    /**< Auxiliary parameter #2 */
} ARP_STEP;


/**
 * @brief: Music keys
 * \ingroup Enumerations
 */
typedef enum {
    KEY_C,
    KEY_CS,
    KEY_D,
    KEY_DS,
    KEY_E,
    KEY_F,
    KEY_FS,
    KEY_G,
    KEY_GS,
    KEY_A,
    KEY_AS,
    KEY_B,
    KEY_MAX
} MUSIC_KEY;

/**
 * @brief: Music modes
 * \ingroup Enumerations
 */
typedef enum {
    IONIAN,
    DORIAN,
    PHRYGIAN,
    LYDIAN,
    MIXOLYDIAN,
    AEOLIAN,
    LOCRIAN,
    MODE_MAX
} MUSIC_MODE;

/**
 * @brief: Types of oscillators
 * \ingroup Enumerations
 */
typedef enum {
    OSC_NONE,
    OSC_SINE,               /**< Sine wave */
    OSC_TRIANGLE,           /**< Triangle wave */
    OSC_TRIANGLE_CLIPPED,   /**< Clipped triangle wave (param1 is the clipping threshold) */
    OSC_SQUARE,             /**< Square wave */
    OSC_SQUARE_SOFT,        /**< Square wave with smoothed transitions so no abrubt changes when used as a control source */
    OSC_PULSE,              /**< Pulse wave (param1 is the duty cycle from 0.0 to 1.0) */
    OSC_RAMP_POS,           /**< Ramp wave with positive slope */
    OSC_RAMP_NEG,           /**< Ramp wave with negative slope */
    OSC_RANDOM,             /**< Random wave */
    OSC_CLIPPED_TRI,
    OSC_TOTAL,
} OSC_TYPES;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Legacy
#define OSC_TRI OSC_TRIANGLE
#endif

/**
 * @brief: Transition time when new parameters are loaded
 * \ingroup Enumerations
 */
typedef enum {
    TRANS_VERY_FAST = (1),      /**< < 1 ms */
    TRANS_FAST = (5),           /**< ~ 3 ms */
    TRANS_MED = (10),           /**< ~ 6 ms */
    TRANS_SLOW = (40),          /**< ~ 25 ms */
    TRANS_VERY_SLOW = (120),    /**< ~ 80 ms */
    TRANS_TOTAL
} EFFECT_TRANSITION_SPEED;

/**
 * @brief: Type of biquad filter
 *
 * Here's a nice video explaining the various types of filters: https://www.youtube.com/watch?v=rkwS6vigSyE
 *
 * \ingroup Enumerations
 */
typedef enum {
    BIQUAD_TYPE_LPF,            /**< Low pass filter – cuts high frequencies, allows low frequencies to pass */
    BIQUAD_TYPE_HPF,            /**< High pass filter – cuts low frequencies, allows high frequencies to pass */
    BIQUAD_TYPE_BPF,            /**< Band pass filter – cuts low and high frequencies outside of center/cutoff frequency of filter (like wah pedal) */
    BIQUAD_TYPE_NOTCH,          /**< Notch filter – cuts a specific frequency and lets other frequencies pass (basically opposite of a band-pass filter)  */
    BIQUAD_TYPE_PEAKING,        /**< Peaking filter – increases or decreases gain at a specific frequency (like those used in parametric filters) */
    BIQUAD_TYPE_L_SHELF,        /**< Low shelf filter – lets all frequencies pass but increases or reduces frequencies below the shelf frequency by specified amount */
    BIQUAD_TYPE_H_SHELF,        /**< High shelf filter – lets all frequencies pass but increases or reduces frequencies above the shelf frequency by specified amount */
    BIQUAD_TYPE_TOTAL
} BIQUAD_FILTER_TYPE;


/**
 * @brief: Biquad filter order.  The higher the order, the more intense the filtering effect
 * \ingroup Enumerations
 */
typedef enum {
    BIQUAD_ORDER_2=1,           /**< 2nd order  (less intense filtering effect) */
    BIQUAD_ORDER_4=2,           /**< 4th order  */
    BIQUAD_ORDER_6=3,           /**< 6th order  */
    BIQUAD_ORDER_8=4,           /**< 8th order  (more intense filtering effect) */
    BIQUAD_ORDER_TOTAL
} BIQUAD_FILTER_ORDER;

typedef enum {
    FILTER_WIDTH_VERY_NARROW,     // Q = 8
    FILTER_WIDTH_NARROW,          // Q = 4
    FILTER_WIDTH_MEDIUM,          // Q = 1
    FILTER_WIDTH_WIDE,            // Q = 0.7071
    FILTER_WIDTH_VERY_WIDE,       // Q = 0.5
    FILTER_WIDTH_TOTAL
} BIQUAD_FILTER_WIDTH;

/**
 * @brief: Destructor functions
 * \ingroup Enumerations
 */
typedef enum {
    SMOOTH_CLIP,                /**< A warm clipping function (based on polynomial expansion) that mimics the analog warmth of a tube amp. Parameters: `param_1` : The Clipping threshold is typically (0.01 to 1.0).  Values closer to 0.0 are more aggressive.  Start with 0.1. `param_2` : input drive The input drive is a multiplier that determines how input signal is multiplied.  Values between 2 and 6 will be warmer, softer clipping whereas values greater than 10 will heavier distortions. */
    SMOOTHER_CLIP,              /**< A warmer clipping function (based on polynomial expansion) that mimics the analog warmth of a tube amp.Parameters `param_1` : The Clipping threshold is typically (0.01 to 1.0).  Values closer to 0.0 are more aggressive.  Start with 0.1.  `param_2` : input drive The input drive is a multiplier that determines how input signal is multiplied.  Values between 2 and 6 will be warmer, softer clipping whereas values greater than 10 will heavier distortions. */
    SMOOTH_FUZZ,                /**< A warm clipping function (based on polynomial expansion) that mimics the analog warmth of a tube amp and also rectifies the signal creating an octave multiplying effect.Parameters `param_1` : he Clipping threshold is typically (0.01 to 1.0).  Values closer to 0.0 are more aggressive.  Start with 0.1. `param_2` : input drive is a multiplier that determines how input signal is multiplied.  Values between 2 and 6 will be warmer, softer clipping whereas values greater than 10 will heavier distortions. */
    BIT_CRUSHER,                /**< A bit crusher simulates reduction in the number of bits used to store audio samples and creates quantization noise, a very “digital” sounding distortion. Parameters `param_1` : bit shift coefficient determines how extreme the effect is.  0.0 provides a gentle bit reduction while 1.0 is an aggressive bit reduction.  `param_2` : (not used) */
    SAMPLE_RATE_CRUSHER,        /**< A sample rate crusher reduces the sample rate without any anti-aliasing.  The result is a remapping of upper harmonics into different audio bands.  Parameters : `param_1` : sample rate reduction coefficient - determines how extreme the effect is.  0.0 provides a gentle bit reduction while 1.0 is an aggressive sample rate reduction. `param_2` : (not used) */
    SHAPER,                      /**< A shaper uses a sine function to “wrap” values outside of the clipping threshold.  The result is louder notes end up with new higher-order harmonics.Parameters: `param_1` : scale determines how extreme the effect is.  0.0 provides a gentle bit reduction while 1.0 (or higher in this case) is an aggressive clipping function. `param_2` : (not used) */
    DIGITAL_PULVERIZER,         /**< Creates nasty, beautiful digital destruction of the incoming waveform.Parameters `param_1` : death - pla around with it – typically between 0.0 and 1.0 `param_2` : and destruction - play around with it – typically between 0.0 and 1.0 */
    FLIP_FLOP_OCTAVE,           /**< Emulates a flip-flop circuit used in early synth and octave pedals.Parameters `param_1` : octaves to drop down - a value of 1.0 is same frequency, 2.0 is one octave down, 3.0 is two octaves down, etc.  Doesn’t do anything with fractional values – input rounded to whole number. `param_2` : (not used)  */
    POLY_TOTAL
} DESTRUCTOR_TYPE;


// Envelope tracker types
typedef enum {
    ENV_TRACK_ENVELOPE,
    ENV_TRACK_TOTAL
} ENV_TRACKER_TYPE;




#ifndef DOXYGEN_SHOULD_SKIP_THIS


/*********************************************************
 * STATE
 ********************************************************/

// Bit fields
#define     SYS_VALID       (0x500A)
#define     SYS_INITIALIZED (0x0010)
#define     SYS_HF_AUDIO    (0x0020)
#define     SYS_LF_AUDIO    (0x0040)
#define     SYS_CANVAS_OK   (0x0080)
#define     SYS_ERR_ALLOC   (0x0100)
#define     SYS_ERR_PARAM   (0x0200)
#define     SYS_ERR_CRPT    (0x0400)
#define     SYS_ERR_OTHER   (0x0800)

typedef enum {
    SPI_DSP_STAT_FIRMWARE_MAJ,
    SPI_DSP_STAT_FIRMWARE_MIN,
    SPI_DSP_STAT_MIPS_PERCENT,
    SPI_DSP_STAT_NEW_NOTE,
    SPI_DSP_STAT_AMPLITUDE,
    SPI_DSP_STAT_SYS_STATE,
    SPI_DSP_STAT_NOTE_1_FREQ,
    SPI_DSP_STAT_NOTE_1_AMP,
    SPI_DSP_STAT_NOTE_1_DUR,
    SPI_DSP_STAT_NOTE_2_FREQ,
    SPI_DSP_STAT_NOTE_2_AMP,
    SPI_DSP_STAT_NOTE_2_DUR,
    SPI_DSP_STAT_NOTE_3_FREQ,
    SPI_DSP_STAT_NOTE_3_AMP,
    SPI_DSP_STAT_NOTE_3_DUR,
    SPI_DSP_STAT_NOTE_4_FREQ,
    SPI_DSP_STAT_NOTE_4_AMP,
    SPI_DSP_STAT_NOTE_4_DUR,
    SPI_DSP_STAT_FRAME_SIZE
} SPI_STATUS_FRAME_OFFSETs;

/*********************************************************
 * UNIVERSAL
 ********************************************************/
#define  FX_PARAM_ID_ENABLED                       0
#define  FX_PARAM_OFFSET_ENABLED                   0

/*********************************************************
 * CANVAS
 ********************************************************/

#define  FX_CANVAS_PARAM_ID_NOTE_FREQ                1
#define  FX_CANVAS_PARAM_ID_NOTE_DURATION            2
#define  FX_CANVAS_PARAM_ID_NOTE_NEW_NOTE            3
#define  FX_CANVAS_PARAM_ID_NEW_FREQ_LOCK            4


/*********************************************************
 * Allpass Filter
 ********************************************************/

// Parameters
#define  FX_ALLPASS_PARAM_ID_ENABLED          0
#define  FX_ALLPASS_PARAM_ID_GAIN             1

// Offsets
#define  FX_ALLPASS_OFFSET_ENABLED            0
#define  FX_ALLPASS_OFFSET_GAIN               1
#define  FX_ALLPASS_OFFSET_LENGTH_MS          3


/*********************************************************
 * ADSR Envelope
 ********************************************************/

// Parameters
#define  FX_ADSR_PARAM_ID_ENABLED          0
#define  FX_ADSR_PARAM_ID_ATK_MS           1
#define  FX_ADSR_PARAM_ID_DEC_MS           2
#define  FX_ADSR_PARAM_ID_SUS_MS           3
#define  FX_ADSR_PARAM_ID_RLS_MS           4
#define  FX_ADSR_PARAM_ID_PEAK_RATIO       5
#define  FX_ADSR_PARAM_ID_SUSTAIN_RATIO    6
#define  FX_ADSR_PARAM_ID_OUT_VOL          7
#define  FX_ADSR_PARAM_ID_VALUE            8
#define  FX_ADSR_PARAM_ID_START            9
#define  FX_ADSR_PARAM_ID_STOP             10

// Offsets
#define  FX_ADSR_OFFSET_ENABLED                0
#define  FX_ADSR_OFFSET_ATK_MS                 1
#define  FX_ADSR_OFFSET_DEC_MS                 3
#define  FX_ADSR_OFFSET_SUS_MS                 5
#define  FX_ADSR_OFFSET_RLS_MS                 7
#define  FX_ADSR_OFFSET_RATIO_PEAK             9
#define  FX_ADSR_OFFSET_RATIO_SUSTAIN          11
#define  FX_ADSR_OFFSET_VOL_OUT                13
#define  FX_ADSR_OFFSET_LOOKAHEAD              15


/*********************************************************
 * AMPLITUDE MODULATOR
 ********************************************************/

// Parameters
#define  FX_AMP_MOD_PARAM_ID_ENABLED                 0
#define  FX_AMP_MOD_PARAM_ID_MOD_FREQ                1
#define  FX_AMP_MOD_PARAM_ID_MOD_PHASE               2
#define  FX_AMP_MOD_PARAM_ID_MOD_DEPTH               3
#define  FX_AMP_MOD_PARAM_ID_MOD_TYPE                4
#define  FX_AMP_MOD_PARAM_ID_EXT_MOD                 5

// Offsets
#define  FX_AMP_MOD_OFFSET_ENABLED                   0
#define  FX_AMP_MOD_OFFSET_MOD_FREQ                  1
#define  FX_AMP_MOD_OFFSET_MOD_PHASE                 3
#define  FX_AMP_MOD_OFFSET_MOD_DEPTH                 5
#define  FX_AMP_MOD_OFFSET_MOD_TYPE                  7
#define  FX_AMP_MOD_OFFSET_EXT_MOD                   8


/*********************************************************
 * ARPEGGIATOR
 ********************************************************/

#define FX_ARPEGGIATOR_PARAM_ID_ENABLE              0
#define FX_ARPEGGIATOR_PARAM_ID_TIME_SCALE          1
#define FX_ARPEGGIATOR_PARAM_ID_PERIOD              2
#define FX_ARPEGGIATOR_PARAM_ID_FREQ                3
#define FX_ARPEGGIATOR_PARAM_ID_VOL                 4
#define FX_ARPEGGIATOR_PARAM_ID_PARAM_1             5
#define FX_ARPEGGIATOR_PARAM_ID_PARAM_2             6
#define FX_ARPEGGIATOR_PARAM_ID_RESTART             7


#define FX_ARPEGGIATOR_OFFSET_EN                    0
#define FX_ARPEGGIATOR_OFFSET_TOTAL_STEPS           1
#define FX_ARPEGGIATOR_OFFSET_STEPS                 2


/*********************************************************
 * BIQUAD
 ********************************************************/

// Parameters
#define  FX_BIQUAD_PARAM_ID_ENABLED                   0
#define  FX_BIQUAD_PARAM_ID_TYPE                      1
#define  FX_BIQUAD_PARAM_ID_SPEED                     2
#define  FX_BIQUAD_PARAM_ID_FREQ                      3
#define  FX_BIQUAD_PARAM_ID_Q                         4
#define  FX_BIQUAD_PARAM_ID_GAIN                      5
#define  FX_BIQUAD_PARAM_ID_ORDER                     6

// Offsets
#define     FX_BIQUAD_PARAM_OFFSET_EN                 0
#define     FX_BIQUAD_PARAM_OFFSET_TYPE               1
#define     FX_BIQUAD_PARAM_OFFSET_SPEED              2
#define     FX_BIQUAD_PARAM_OFFSET_FREQ               3
#define     FX_BIQUAD_PARAM_OFFSET_Q                  5
#define     FX_BIQUAD_PARAM_OFFSET_GAIN               7
#define     FX_BIQUAD_PARAM_OFFSET_ORDER              9


/*********************************************************
 * COMPRESSOR
 ********************************************************/

// Parameters
#define  FX_COMPRESSOR_PARAM_ID_ENABLED               0
#define  FX_COMPRESSOR_PARAM_ID_THRESH                1
#define  FX_COMPRESSOR_PARAM_ID_RATIO                 2
#define  FX_COMPRESSOR_PARAM_ID_ATTACK                3
#define  FX_COMPRESSOR_PARAM_ID_RELEASE               4
#define  FX_COMPRESSOR_PARAM_ID_OUT_GAIN              5

// Offsets
#define     FX_COMPRESSOR_PARAM_OFFSET_EN             0
#define     FX_COMPRESSOR_PARAM_OFFSET_THRESH         1
#define     FX_COMPRESSOR_PARAM_OFFSET_RATIO          3
#define     FX_COMPRESSOR_PARAM_OFFSET_ATTACK         5
#define     FX_COMPRESSOR_PARAM_OFFSET_RELEASE        7
#define     FX_COMPRESSOR_PARAM_OFFSET_OUT_GAIN       9

/*********************************************************
 * DELAY MULTITAP
 ********************************************************/

// Parameters
#define FX_MULTITAP_DELAY_PARAM_ID_ENABLED            0
#define FX_MULTITAP_DELAY_PARAM_ID_WET_MIX            1
#define FX_MULTITAP_DELAY_PARAM_ID_DRY_MIX            2

// Offsets
#define FX_MULTITAP_DELAY_OFFSET_ENABLED              0
#define FX_MULTITAP_DELAY_OFFSET_TAP_1_MS             1
#define FX_MULTITAP_DELAY_OFFSET_TAP_1_GAIN           3
#define FX_MULTITAP_DELAY_OFFSET_TAP_2_MS             5
#define FX_MULTITAP_DELAY_OFFSET_TAP_2_GAIN           7
#define FX_MULTITAP_DELAY_OFFSET_TAP_3_MS             9
#define FX_MULTITAP_DELAY_OFFSET_TAP_3_GAIN           11
#define FX_MULTITAP_DELAY_OFFSET_TAP_4_MS             13
#define FX_MULTITAP_DELAY_OFFSET_TAP_4_GAIN           15
#define FX_MULTITAP_DELAY_OFFSET_CLEAN_MIX            17
#define FX_MULTITAP_DELAY_OFFSET_WET_MIX              19

/*********************************************************
 * DELAY
 ********************************************************/

// Parameters
#define  FX_DELAY_PARAM_ID_ENABLED                    0
#define  FX_DELAY_PARAM_ID_LEN_MS                     1
#define  FX_DELAY_PARAM_ID_LEN_MAX_MS                 2
#define  FX_DELAY_PARAM_ID_FEEDBACK                   3
#define  FX_DELAY_PARAM_ID_DRY_MIX                    4
#define  FX_DELAY_PARAM_ID_WET_MIX                    5
#define  FX_DELAY_PARAM_ID_EXT_FB                     6

// Offsets
#define     FX_DELAY_PARAM_OFFSET_EN                  0
#define     FX_DELAY_PARAM_OFFSET_DELAY_LEN           1
#define     FX_DELAY_PARAM_OFFSET_DELAY_LEN_MAX       3
#define     FX_DELAY_PARAM_OFFSET_DELAY_FB            5
#define     FX_DELAY_PARAM_OFFSET_DELAY_DRY           7
#define     FX_DELAY_PARAM_OFFSET_DELAY_WET           9
#define     FX_DELAY_PARAM_OFFSET_EXT_LOOP            11


/*********************************************************
 * DESTRUCTOR
 ********************************************************/

// Parameters
#define  FX_DESTRUCTOR_PARAM_ID_ENABLED               0
#define  FX_DESTRUCTOR_PARAM_ID_TYPE                  1
#define  FX_DESTRUCTOR_PARAM_ID_PARAM_1               2
#define  FX_DESTRUCTOR_PARAM_ID_PARAM_2               3
#define  FX_DESTRUCTOR_PARAM_ID_OUT_GAIN              4
#define  FX_DESTRUCTOR_PARAM_ID_UPSAMPLE              5

// Offsets
#define  FX_DESTRUCTOR_OFFSET_ENABLED                 0
#define  FX_DESTRUCTOR_OFFSET_POLY_TYPE               1
#define  FX_DESTRUCTOR_OFFSET_PARAM_1                 2
#define  FX_DESTRUCTOR_OFFSET_PARAM_2                 4
#define  FX_DESTRUCTOR_OFFSET_OUT_GAIN                6
#define  FX_DESTRUCTOR_OFFSET_UPSAMPLE                8

/*********************************************************
 * ENV TRACKER
 ********************************************************/

// Parameters
#define  FX_ENV_TRACKER_PARAM_ID_ENABLED              0
#define  FX_ENV_TRACKER_PARAM_ID_ATTACK_MS            1
#define  FX_ENV_TRACKER_PARAM_ID_DECAY_MS             2
#define  FX_ENV_TRACKER_PARAM_ID_TYPE                 3
#define  FX_ENV_TRACKER_PARAM_ID_SCALE                4
#define  FX_ENV_TRACKER_PARAM_ID_OFFSET               5
#define  FX_ENV_TRACKER_PARAM_ID_TRIGGERED            6
#define  FX_ENV_TRACKER_PARAM_ID_VALUE                7

// Offsets
#define     FX_ENV_TRACKER_OFFSET_EN                  0
#define     FX_ENV_TRACKER_OFFSET_ATTACK_MS           1
#define     FX_ENV_TRACKER_OFFSET_DECAY_MS            3
#define     FX_ENV_TRACKER_OFFSET_SCALE               5
#define     FX_ENV_TRACKER_OFFSET_OFFSET              7
#define     FX_ENV_TRACKER_OFFSET_TYPE                9
#define     FX_ENV_TRACKER_OFFSET_TRIGGERED           10


/*********************************************************
 * GAIN
 ********************************************************/

// Parameters
#define  FX_GAIN_PARAM_ID_ENABLED                  0
#define  FX_GAIN_PARAM_ID_GAIN                     1
#define  FX_GAIN_PARAM_ID_SPEED                    2

// Offsets
#define     FX_GAIN_PARAM_OFFSET_EN                0
#define     FX_GAIN_PARAM_OFFSET_GAIN              1
#define     FX_GAIN_PARAM_OFFSET_SPEED             3


/*********************************************************
 * HARMONIZER
 ********************************************************/

// Parameters
#define FX_HARMONIZER_PARAM_ID_ENABLED          0
#define FX_HARMONIZER_PARAM_ID_FREQ_OUT         1
#define FX_HARMONIZER_PARAM_ID_VOL_OUT          2
#define FX_HARMONIZER_PARAM_ID_KEY              3
#define FX_HARMONIZER_PARAM_ID_MODE             4
#define FX_HARMONIZER_PARAM_ID_OFFSET           5
#define FX_HARMONIZER_PARAM_ID_VOL              6


// Offsets
#define  FX_HARMONIZER_OFFSET_ENABLED           0
#define  FX_HARMONIZER_OFFSET_KEY               1
#define  FX_HARMONIZER_OFFSET_MODE              2
#define  FX_HARMONIZER_OFFSET_NOTE_OFFSET       3
#define  FX_HARMONIZER_OFFSET_VOL               4

/*********************************************************
 * IMPULSE RESPONSE
 ********************************************************/

// Parameters
#define FX_IMPULSE_RESPONSE_PARAM_ID_ENABLED        0
#define FX_IMPULSE_RESPONSE_PARAM_ID_IMPULSE_RESP   1

// Offsets
#define  FX_IMPULSE_RESPONSE_OFFSET_ENABLED         0
#define  FX_IMPULSE_RESPONSE_OFFSET_IMPULSE_RESP    1


/*********************************************************
 * INSTRUMENT_SYNTH
 ********************************************************/

// Parameters
#define FX_INSTRUMENT_SYNTH_PARAM_ID_ENABLED             0
#define FX_INSTRUMENT_SYNTH_PARAM_ID_OSC_TYPE            1
#define FX_INSTRUMENT_SYNTH_PARAM_ID_OSC_FM_TYPE         2
#define FX_INSTRUMENT_SYNTH_PARAM_ID_FREQ_RATIO          3
#define FX_INSTRUMENT_SYNTH_PARAM_ID_FM_MOD_RATIO        4
#define FX_INSTRUMENT_SYNTH_PARAM_ID_FM_MOD_DEPTH        5
#define FX_INSTRUMENT_SYNTH_PARAM_ID_ATTACK_MS           6
#define FX_INSTRUMENT_SYNTH_PARAM_ID_FILT_RESONANCE      7
#define FX_INSTRUMENT_SYNTH_PARAM_ID_FILT_RESPONSE       8

// Offsets
#define FX_INSTRUMENT_SYNTH_PARAM_OFFSET_EN              0
#define FX_INSTRUMENT_SYNTH_PARAM_OFFSET_OSC_TYPE        1
#define FX_INSTRUMENT_SYNTH_PARAM_OFFSET_OSC_FM_TYPE     2
#define FX_INSTRUMENT_SYNTH_PARAM_OFFSET_FREQ_RATIO      3
#define FX_INSTRUMENT_SYNTH_PARAM_OFFSET_FM_MOD_RATIO    5
#define FX_INSTRUMENT_SYNTH_PARAM_OFFSET_FM_MOD_DEPTH    7
#define FX_INSTRUMENT_SYNTH_PARAM_OFFSET_ATTACK_MS       9
#define FX_INSTRUMENT_SYNTH_PARAM_OFFSET_FILT_RESONANCE  11
#define FX_INSTRUMENT_SYNTH_PARAM_OFFSET_FILT_RESPONSE   13


/*********************************************************
 * LOOPER
 ********************************************************/

// Parameters
#define     FX_LOOPER_PARAM_OFFSET_EN                  0
#define     FX_LOOPER_PARAM_OFFSET_LOOP_SIZE_S         1
#define     FX_LOOPER_PARAM_OFFSET_DRY_MIX             3
#define     FX_LOOPER_PARAM_OFFSET_LOOP_MIX            5
#define     FX_LOOPER_PARAM_OFFSET_RATE                7
#define     FX_LOOPER_PARAM_OFFSET_EXT_PP              9
#define     FX_LOOPER_PARAM_OFFSET_START               10
#define     FX_LOOPER_PARAM_OFFSET_STOP                11

// Parameter and status IDs
#define  FX_LOOPER_PARAM_ID_ENABLED                    0
#define  FX_LOOPER_PARAM_ID_LOOP_SIZE_S                1
#define  FX_LOOPER_PARAM_ID_DRY_MIX                    2
#define  FX_LOOPER_PARAM_ID_LOOP_MIX                   3
#define  FX_LOOPER_PARAM_ID_RATE                       4
#define  FX_LOOPER_PARAM_ID_EXT_FB                     5
#define  FX_LOOPER_PARAM_ID_START                      6
#define  FX_LOOPER_PARAM_ID_STOP                       7
#define  FX_LOOPER_PARAM_ID_LOOP_LEN_S                 8
#define  FX_LOOPER_PARAM_ID_LOOP_LEN_S_SET             9


/*********************************************************
 * OSCILLATOR
 ********************************************************/

// Parameters
#define  FX_OSCILLATOR_PARAM_ID_ENABLED                 0
#define  FX_OSCILLATOR_PARAM_ID_FREQ                    1
#define  FX_OSCILLATOR_PARAM_ID_AMP                     2
#define  FX_OSCILLATOR_PARAM_ID_OFFSET                  3
#define  FX_OSCILLATOR_PARAM_ID_TYPE                    4
#define  FX_OSCILLATOR_PARAM_ID_OSC_PARAM1              5
#define  FX_OSCILLATOR_PARAM_ID_OSC_PARAM2              6
#define  FX_OSCILLATOR_PARAM_ID_OSC_INITIAL_PHASE       7
#define  FX_OSCILLATOR_PARAM_ID_FM_OSC_TYPE             8
#define  FX_OSCILLATOR_PARAM_ID_FM_DEPTH                9
#define  FX_OSCILLATOR_PARAM_ID_FM_FREQ_RATIO           10
#define  FX_OSCILLATOR_PARAM_ID_OSC_CTRL_VAL            11


// Offsets
#define  FX_OSCILLATOR_PARAM_OFFSET_OFFSET_EN           0
#define  FX_OSCILLATOR_PARAM_OFFSET_FREQ                1
#define  FX_OSCILLATOR_PARAM_OFFSET_AMP                 3
#define  FX_OSCILLATOR_PARAM_OFFSET_OFFSET              5
#define  FX_OSCILLATOR_PARAM_OFFSET_TYPE                7
#define  FX_OSCILLATOR_PARAM_OFFSET_OSC_PARAM1          8
#define  FX_OSCILLATOR_PARAM_OFFSET_OSC_PARAM2          10
#define  FX_OSCILLATOR_PARAM_OFFSET_OSC_INITIAL_PHASE   12
#define  FX_OSCILLATOR_PARAM_OFFSET_FM_OSC_TYPE         14
#define  FX_OSCILLATOR_PARAM_OFFSET_FM_DEPTH            15
#define  FX_OSCILLATOR_PARAM_OFFSET_FM_FREQ_RATIO       17


/*********************************************************
 * PHASE SHIFTER
 ********************************************************/

// Parameters
#define  FX_PHASE_SHIFTER_PARAM_ID_ENABLED            0
#define  FX_PHASE_SHIFTER_PARAM_ID_RATE_HZ            1
#define  FX_PHASE_SHIFTER_PARAM_ID_DEPTH              2
#define  FX_PHASE_SHIFTER_PARAM_ID_FEEDBACK           3
#define  FX_PHASE_SHIFTER_PARAM_ID_INITIAL_PHASE      4
#define  FX_PHASE_SHIFTER_PARAM_ID_MOD_TYPE           5
#define  FX_PHASE_SHIFTER_PARAM_ID_POLE_POS_0         6
#define  FX_PHASE_SHIFTER_PARAM_ID_POLE_POS_1         7
#define  FX_PHASE_SHIFTER_PARAM_ID_POLE_POS_2         8
#define  FX_PHASE_SHIFTER_PARAM_ID_POLE_POS_3         9
#define  FX_PHASE_SHIFTER_PARAM_ID_POLE_RNG_0         10
#define  FX_PHASE_SHIFTER_PARAM_ID_POLE_RNG_1         11
#define  FX_PHASE_SHIFTER_PARAM_ID_POLE_RNG_2         12
#define  FX_PHASE_SHIFTER_PARAM_ID_POLE_RNG_3         13

// Offsets
#define     FX_PHASE_SHIFTER_PARAM_OFFSET_EN              0
#define     FX_PHASE_SHIFTER_PARAM_OFFSET_RATE_HZ         1
#define     FX_PHASE_SHIFTER_PARAM_OFFSET_DEPTH           3
#define     FX_PHASE_SHIFTER_PARAM_OFFSET_FEEDBACK        5
#define     FX_PHASE_SHIFTER_PARAM_OFFSET_INITIAL_PHASE   7
#define     FX_PHASE_SHIFTER_PARAM_OFFSET_MOD_TYPE        9



/*********************************************************
 * PITCH SHIFTER
 ********************************************************/
// Parameters
#define  FX_PITCH_SHIFT_PARAM_ID_ENABLED              0
#define  FX_PITCH_SHIFT_PARAM_ID_FREQ_SHIFT           1

// Offsets
#define  FX_PITCH_SHIFT_PARAM_OFFSET_OFFSET_EN        0
#define  FX_PITCH_SHIFT_PARAM_OFFSET_FREQ_SHIFT       1



/*********************************************************
 * RING MODULATOR
 ********************************************************/

// Parameters
#define  FX_RING_MOD_PARAM_ID_ENABLED                 0
#define  FX_RING_MOD_PARAM_ID_FREQ                    1
#define  FX_RING_MOD_PARAM_ID_DEPTH                   2
#define  FX_RING_MOD_PARAM_ID_EN_FILTER               3

// Offsets
#define  FX_RING_MOD_PARAM_OFFSET_OFFSET_EN           0
#define  FX_RING_MOD_PARAM_OFFSET_FREQ                1
#define  FX_RING_MOD_PARAM_OFFSET_DEPTH               3
#define  FX_RING_MOD_PARAM_OFFSET_EN_FILTER           5



/*********************************************************
 * SLICER
 ********************************************************/

// Parameters
#define  FX_SLICER_PARAM_ID_ENABLED                  0
#define  FX_SLICER_PARAM_ID_PERIOD                   1
#define  FX_SLICER_PARAM_ID_CHANNELS                 2
#define  FX_SLICER_PARAM_ID_START                    3

// Offsets
#define     FX_SLICER_PARAM_OFFSET_EN                0
#define     FX_SLICER_PARAM_OFFSET_PERIOD            1
#define     FX_SLICER_PARAM_OFFSET_CHANNELS          3


/*********************************************************
 * SPECTRALIZER
 ********************************************************/

// Parameters
#define  FX_SPECTRALIZER_PARAM_OFFSET_OFFSET_EN         0
#define  FX_SPECTRALIZER_PARAM_OFFSET_FREQ_SHIFT_1      1
#define  FX_SPECTRALIZER_PARAM_OFFSET_FREQ_SHIFT_2      3
#define  FX_SPECTRALIZER_PARAM_OFFSET_VOL_1             5
#define  FX_SPECTRALIZER_PARAM_OFFSET_VOL_2             7
#define  FX_SPECTRALIZER_PARAM_OFFSET_VOL_CLEAN         9

// Offsets
#define  FX_SPECTRALIZER_PARAM_ID_ENABLED               0
#define  FX_SPECTRALIZER_PARAM_ID_FREQ_SHIFT_1          1
#define  FX_SPECTRALIZER_PARAM_ID_FREQ_SHIFT_2          2
#define  FX_SPECTRALIZER_PARAM_ID_VOL_1                 3
#define  FX_SPECTRALIZER_PARAM_ID_VOL_2                 4
#define  FX_SPECTRALIZER_PARAM_ID_VOL_CLEAN             5


/*********************************************************
 * VARIABLE DELAY
 ********************************************************/

// Parameters
#define  FX_VAR_DELAY_PARAM_ID_ENABLED                 0
#define  FX_VAR_DELAY_PARAM_ID_MOD_FREQ                1
#define  FX_VAR_DELAY_PARAM_ID_MOD_DEPTH               2
#define  FX_VAR_DELAY_PARAM_ID_MOD_PHASE               3
#define  FX_VAR_DELAY_PARAM_ID_FEEDBACK                4
#define  FX_VAR_DELAY_PARAM_ID_MOD_TYPE                5
#define  FX_VAR_DELAY_PARAM_ID_EXT_MOD                 6
#define  FX_VAR_DELAY_PARAM_ID_DELAY_LEN_MS            7
#define  FX_VAR_DELAY_PARAM_ID_MIX_CLEAN               8
#define  FX_VAR_DELAY_PARAM_ID_MIX_DELAYED             9


// Offsets
#define  FX_VAR_DELAY_OFFSET_ENABLED                   0
#define  FX_VAR_DELAY_OFFSET_MOD_FREQ                  1
#define  FX_VAR_DELAY_OFFSET_MOD_DEPTH                 3
#define  FX_VAR_DELAY_OFFSET_MOD_PHASE                 5
#define  FX_VAR_DELAY_OFFSET_FEEDBACK                  7
#define  FX_VAR_DELAY_OFFSET_MOD_TYPE                  9
#define  FX_VAR_DELAY_OFFSET_EXT_MOD                   10
#define  FX_VAR_DELAY_OFFSET_DELAY_LEN_MS              11
#define  FX_VAR_DELAY_OFFSET_MIX_CLEAN                 13
#define  FX_VAR_DELAY_OFFSET_MIX_DELAYED               15

#endif // DOXYGEN_SHOULD_SKIP_THIS

#endif /* AFX_FRAMEWORK_EFFECTS_DMFX_EFFECTS_DEFINES_H_ */
