#ifndef AUDIO_CUSTOM_H
#define AUDIO_CUSTOM_H

/* define Gain For Normal */
/* Normal volume: TON, SPK, MIC, FMR, SPH, SID, MED */
#define GAIN_NOR_TON_VOL        8     // reserved
#define GAIN_NOR_KEY_VOL       43    // TTY_CTM_Mic
#define GAIN_NOR_MIC_VOL       26    // IN_CALL BuiltIn Mic gain
// GAIN_NOR_FMR_VOL is used as idle mode record volume
#define GAIN_NOR_FMR_VOL        0     // Normal BuiltIn Mic gain
#define GAIN_NOR_SPH_VOL       20     // IN_CALL EARPIECE Volume
#define GAIN_NOR_SID_VOL      100  // IN_CALL EARPICE sidetone
#define GAIN_NOR_MED_VOL       25   // reserved

/* define Gain For Headset */
/* Headset volume: TON, SPK, MIC, FMR, SPH, SID, MED */
#define GAIN_HED_TON_VOL        8     // reserved
#define GAIN_HED_KEY_VOL       24    // reserved
#define GAIN_HED_MIC_VOL       20    // IN_CALL BuiltIn headset gain
#define GAIN_HED_FMR_VOL       24     // reserved
#define GAIN_HED_SPH_VOL       12     // IN_CALL Headset volume
#define GAIN_HED_SID_VOL      100  // IN_CALL Headset sidetone
#define GAIN_HED_MED_VOL       12   // Idle, headset Audio Buf Gain setting

/* define Gain For Handfree */
/* Handfree volume: TON, SPK, MIC, FMR, SPH, SID, MED */
/* GAIN_HND_TON_VOL is used as class-D Amp gain*/
#define GAIN_HND_TON_VOL       15     // use for ringtone volume
#define GAIN_HND_KEY_VOL       24    // reserved
#define GAIN_HND_MIC_VOL       20    // IN_CALL LoudSpeak Mic Gain = BuiltIn Gain
#define GAIN_HND_FMR_VOL       24     // reserved
#define GAIN_HND_SPH_VOL        6     // IN_CALL LoudSpeak
#define GAIN_HND_SID_VOL      100  // IN_CALL LoudSpeak sidetone
#define GAIN_HND_MED_VOL       12   // Idle, loudSPK Audio Buf Gain setting
    /* 0: Input FIR coefficients for 2G/3G Normal mode */
    /* 1: Input FIR coefficients for 2G/3G/VoIP Headset mode */
    /* 2: Input FIR coefficients for 2G/3G Handfree mode */
    /* 3: Input FIR coefficients for 2G/3G/VoIP BT mode */
    /* 4: Input FIR coefficients for VoIP Normal mode */
    /* 5: Input FIR coefficients for VoIP Handfree mode */
#define SPEECH_INPUT_FIR_COEFF \
      304,   220,   552,   140,   678,\
     -604,    31,  -222,  -120,   421,\
     -540,   134, -2213,  2684, -1770,\
     2237, -1845,  -167, -5241,  7361,\
   -14774, 23197, 23197,-14774,  7361,\
    -5241,  -167, -1845,  2237, -1770,\
     2684, -2213,   134,  -540,   421,\
     -120,  -222,    31,  -604,   678,\
      140,   552,   220,   304,     0,\
                                      \
    32767,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
                                      \
      580,   149,  1158,   203,  1240,\
     -560, -1696,   437,   -67,   199,\
     -449, -1460, -1945,  3830, -1440,\
     3723, -3088,   280, -7040,  5376,\
   -17497, 32767, 32767,-17497,  5376,\
    -7040,   280, -3088,  3723, -1440,\
     3830, -1945, -1460,  -449,   199,\
      -67,   437, -1696,  -560,  1240,\
      203,  1158,   149,   580,     0,\
                                      \
    32767,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
                                      \
    32767,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
                                      \
    32767,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0
    /* 0: Output FIR coefficients for 2G/3G Normal mode */
    /* 1: Output FIR coefficients for 2G/3G/VoIP Headset mode */
    /* 2: Output FIR coefficients for 2G/3G Handfree mode */
    /* 3: Output FIR coefficients for 2G/3G/VoIP BT mode */
    /* 4: Output FIR coefficients for VoIP Normal mode */
    /* 5: Output FIR coefficients for VoIP Handfree mode */
#define SPEECH_OUTPUT_FIR_COEFF \
      120,  -276,   175,  -463,   275,\
     -486,   185,  -405,  -196,   110,\
     -973,   327, -1761,  3095, -2384,\
     5840, -2885,  4790, -6883, 10042,\
    -6817, 32767, 32767, -6817, 10042,\
    -6883,  4790, -2885,  5840, -2384,\
     3095, -1761,   327,  -973,   110,\
     -196,  -405,   185,  -486,   275,\
     -463,   175,  -276,   120,     0,\
                                      \
    32767,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
                                      \
      911,  -234,  -227,  1161,  2395,\
      878, -1911, -3416, -3048, -2037,\
    -2689, -4817, -8555, -8558, -4933,\
     2331,  3916,  7242,  7075,  7076,\
    11351, 32767, 32767, 11351,  7076,\
     7075,  7242,  3916,  2331, -4933,\
    -8558, -8555, -4817, -2689, -2037,\
    -3048, -3416, -1911,   878,  2395,\
     1161,  -227,  -234,   911,     0,\
                                      \
    32767,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
                                      \
    32767,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
                                      \
    32767,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0
#define   DG_DL_Speech    0xe3d
#define   DG_Microphone    0x1400
#define   FM_Record_Vol    6     /* 0 is smallest. each step increase 1dB.
                            Be careful of distortion when increase too much.
                            Generally, it's not suggested to tune this parameter */
/*
* The Bluetooth DAI Hardware COnfiguration Parameter
*/
#define   DEFAULT_BLUETOOTH_SYNC_TYPE    0
#define   DEFAULT_BLUETOOTH_SYNC_LENGTH    1
    /* 0: Input FIR coefficients for 2G/3G Normal mode */
    /* 1: Input FIR coefficients for 2G/3G/VoIP Headset mode */
    /* 2: Input FIR coefficients for 2G/3G Handfree mode */
    /* 3: Input FIR coefficients for 2G/3G/VoIP BT mode */
    /* 4: Input FIR coefficients for VoIP Normal mode */
    /* 5: Input FIR coefficients for VoIP Handfree mode */
#define WB_Speech_Input_FIR_Coeff \
      132,   108,     4,   -72,  -169,   -46,    13,    -8,   -20,   -35,\
       36,  -243,  -125,  -270,    41,    29,  -349,  -437,  -290,   210,\
       23,  -361,  -642,    81,  -178,  -180,  -763,   304,  -151,  -144,\
      -92,   403,  1423, -1016,   179, -1677,  -299, -1108,   948, -2141,\
     4920, -7086,  2701, -5960, 23197, 23197, -5960,  2701, -7086,  4920,\
    -2141,   948, -1108,  -299, -1677,   179, -1016,  1423,   403,   -92,\
     -144,  -151,   304,  -763,  -180,  -178,    81,  -642,  -361,    23,\
      210,  -290,  -437,  -349,    29,    41,  -270,  -125,  -243,    36,\
      -35,   -20,    -8,    13,   -46,  -169,   -72,     4,   108,   132,\
                                       \
    32767,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
                                       \
    32767,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
                                       \
    32767,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
                                       \
    32767,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
                                       \
    32767,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0
    /* 0: Output FIR coefficients for 2G/3G Normal mode */
    /* 1: Output FIR coefficients for 2G/3G/VoIP Headset mode */
    /* 2: Output FIR coefficients for 2G/3G Handfree mode */
    /* 3: Output FIR coefficients for 2G/3G/VoIP BT mode */
    /* 4: Output FIR coefficients for VoIP Normal mode */
    /* 5: Output FIR coefficients for VoIP Handfree mode */
#define WB_Speech_Output_FIR_Coeff \
     -165,  -255,  -422,  -597,  -760,  -865, -1078, -1111, -1293, -1550,\
    -1456, -1675, -1513, -1877, -1792, -1917, -1702, -1664, -1852, -1768,\
    -1792, -1210, -1135,  -573,  -779,  -554,  -607,   698,   171,  1476,\
      932,   846,  3446,  2706,  4407,  1954,  1241,   357,  8014,  5989,\
    11426,  1196, 10008, -7272, 32767, 32767, -7272, 10008,  1196, 11426,\
     5989,  8014,   357,  1241,  1954,  4407,  2706,  3446,   846,   932,\
     1476,   171,   698,  -607,  -554,  -779,  -573, -1135, -1210, -1792,\
    -1768, -1852, -1664, -1702, -1917, -1792, -1877, -1513, -1675, -1456,\
    -1550, -1293, -1111, -1078,  -865,  -760,  -597,  -422,  -255,  -165,\
                                       \
    32767,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
                                       \
    32767,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
                                       \
    32767,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
                                       \
    32767,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
                                       \
    32767,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,\
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0
#endif
