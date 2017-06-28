#include <utils/Log.h>
#include <fcntl.h>
#include <math.h>

#include "camera_custom_nvram.h"
#include "camera_custom_sensor.h"
#include "image_sensor.h"
#include "kd_imgsensor_define.h"
#include "camera_AE_PLineTable_ov8858mipiraw.h"
#include "camera_info_ov8858mipiraw.h"
#include "camera_custom_AEPlinetable.h"
#include "camera_custom_tsf_tbl.h"
const NVRAM_CAMERA_ISP_PARAM_STRUCT CAMERA_ISP_DEFAULT_VALUE =
{{
    //Version
    Version: NVRAM_CAMERA_PARA_FILE_VERSION,
    //SensorId
    SensorId: SENSOR_ID,
    ISPComm:{
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    },
    ISPPca:{
        #include INCLUDE_FILENAME_ISP_PCA_PARAM
    },
    ISPRegs:{
        #include INCLUDE_FILENAME_ISP_REGS_PARAM
        },
    ISPMfbMixer:{{
        {//00: MFB mixer for ISO 100
            0x00000000, 0x00000000
        },
        {//01: MFB mixer for ISO 200
            0x00000000, 0x00000000
        },
        {//02: MFB mixer for ISO 400
            0x00000000, 0x00000000
        },
        {//03: MFB mixer for ISO 800
            0x00000000, 0x00000000
        },
        {//04: MFB mixer for ISO 1600
            0x00000000, 0x00000000
        },
        {//05: MFB mixer for ISO 2400
            0x00000000, 0x00000000
        },
        {//06: MFB mixer for ISO 3200
            0x00000000, 0x00000000
        }
    }},
    ISPCcmPoly22:{
        70475,    // i4R_AVG
        14991,    // i4R_STD
        91950,    // i4B_AVG
        20464,    // i4B_STD
        {  // i4P00[9]
            4282500, -1930000, 205000, -665000, 3465000, -245000, 12500, -2350000, 4895000
        },
        {  // i4P10[9]
            1703841, -1869988, 170458, 1760, -389881, 370638, 230378, 488878, -717873
        },
        {  // i4P01[9]
            1512205, -1656860, 145323, -186071, -430217, 603171, 47312, -268756, 227305
        },
        {  // i4P20[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P11[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        },
        {  // i4P02[9]
            0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    }
}};

const NVRAM_CAMERA_3A_STRUCT CAMERA_3A_NVRAM_DEFAULT_VALUE =
{
    NVRAM_CAMERA_3A_FILE_VERSION, // u4Version
    SENSOR_ID, // SensorId

    // AE NVRAM
    {
        // rDevicesInfo
        {
            1152,    // u4MinGain, 1024 base = 1x
            10240,    // u4MaxGain, 16x
            100,    // u4MiniISOGain, ISOxx  
            128,    // u4GainStepUnit, 1x/8 
            27,    // u4PreExpUnit 
            30,    // u4PreMaxFrameRate
            18,    // u4VideoExpUnit  
            30,    // u4VideoMaxFrameRate 
            1024,    // u4Video2PreRatio, 1024 base = 1x 
            14,    // u4CapExpUnit 
            30,    // u4CapMaxFrameRate
            1024,    // u4Cap2PreRatio, 1024 base = 1x
            20,    // u4LensFno, Fno = 2.8
            350    // u4FocusLength_100x
        },
        // rHistConfig
        {
            2,    // u4HistHighThres
            40,    // u4HistLowThres
            2,    // u4MostBrightRatio
            1,    // u4MostDarkRatio
            160,    // u4CentralHighBound
            20,    // u4CentralLowBound
            {240, 230, 220, 210, 200},    // u4OverExpThres[AE_CCT_STRENGTH_NUM] 
            {86, 108, 128, 148, 170},    // u4HistStretchThres[AE_CCT_STRENGTH_NUM] 
            {18, 22, 26, 30, 34}    // u4BlackLightThres[AE_CCT_STRENGTH_NUM] 
        },
        // rCCTConfig
        {
            TRUE,    // bEnableBlackLight
            TRUE,    // bEnableHistStretch
            FALSE,    // bEnableAntiOverExposure
            TRUE,    // bEnableTimeLPF
            TRUE,    // bEnableCaptureThres
            TRUE,    // bEnableVideoThres
            TRUE,    // bEnableStrobeThres
            47,    // u4AETarget
            0,    // u4StrobeAETarget
            50,    // u4InitIndex
            4,    // u4BackLightWeight
            32,    // u4HistStretchWeight
            4,    // u4AntiOverExpWeight
            2,    // u4BlackLightStrengthIndex
            2,    // u4HistStretchStrengthIndex
            2,    // u4AntiOverExpStrengthIndex
            2,    // u4TimeLPFStrengthIndex
            {1, 3, 5, 7, 8},    // u4LPFConvergeTable[AE_CCT_STRENGTH_NUM] 
            90,    // u4InDoorEV = 9.0, 10 base 
            -1,    // i4BVOffset delta BV = value/10 
            64,    // u4PreviewFlareOffset
            64,    // u4CaptureFlareOffset
            4,    // u4CaptureFlareThres
            64,    // u4VideoFlareOffset
            4,    // u4VideoFlareThres
            64,    // u4StrobeFlareOffset
            4,    // u4StrobeFlareThres
            200,    // u4PrvMaxFlareThres
            32,    // u4PrvMinFlareThres
            200,    // u4VideoMaxFlareThres
            32,    // u4VideoMinFlareThres
            18,    // u4FlatnessThres    // 10 base for flatness condition.
            60    // u4FlatnessStrength
        }
    },
    // AWB NVRAM
    {
        // AWB calibration data
        {
            // rUnitGain (unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rGoldenGain (golden sample gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rTuningUnitGain (Tuning sample unit gain: 1.0 = 512)
            {
                0,    // i4R
                0,    // i4G
                0    // i4B
            },
            // rD65Gain (D65 WB gain: 1.0 = 512)
            {
                910,    // i4R
                512,    // i4G
                627    // i4B
            }
        },
        // Original XY coordinate of AWB light source
        {
           // Strobe
            {
                0,    // i4X
                0    // i4Y
            },
            // Horizon
            {
                -351,    // i4X
                -321    // i4Y
            },
            // A
            {
                -265,    // i4X
                -310    // i4Y
            },
            // TL84
            {
                -120,    // i4X
                -301    // i4Y
            },
            // CWF
            {
                -94,    // i4X
                -352    // i4Y
            },
            // DNP
            {
                -20,    // i4X
                -330    // i4Y
            },
            // D65
            {
                137,    // i4X
                -287    // i4Y
            },
            // DF
            {
                88,    // i4X
                -350    // i4Y
            }
        },
        // Rotated XY coordinate of AWB light source
        {
            // Strobe
            {
                0,    // i4X
                0    // i4Y
            },
            // Horizon
            {
                -372,    // i4X
                -295    // i4Y
            },
            // A
            {
                -286,    // i4X
                -290    // i4Y
            },
            // TL84
            {
                -141,    // i4X
                -291    // i4Y
            },
            // CWF
            {
                -118,    // i4X
                -344    // i4Y
            },
            // DNP
            {
                -43,    // i4X
                -327    // i4Y
            },
            // D65
            {
                116,    // i4X
                -296    // i4Y
            },
            // DF
            {
                63,    // i4X
                -355    // i4Y
            }
        },
        // AWB gain of AWB light source
        {
            // Strobe 
            {
                512,    // i4R
                512,    // i4G
                512    // i4B
            },
            // Horizon 
            {
                512,    // i4R
                533,    // i4G
                1326    // i4B
            },
            // A 
            {
                544,    // i4R
                512,    // i4G
                1115    // i4B
            },
            // TL84 
            {
                655,    // i4R
                512,    // i4G
                905    // i4B
            },
            // CWF 
            {
                726,    // i4R
                512,    // i4G
                937    // i4B
            },
            // DNP 
            {
                778,    // i4R
                512,    // i4G
                822    // i4B
            },
            // D65 
            {
                910,    // i4R
                512,    // i4G
                627    // i4B
            },
            // DF 
            {
                926,    // i4R
                512,    // i4G
                731    // i4B
            }
        },
        // Rotation matrix parameter
        {
            4,    // i4RotationAngle
            255,    // i4Cos
            18    // i4Sin
        },
        // Daylight locus parameter
        {
            -145,    // i4SlopeNumerator
            128    // i4SlopeDenominator
        },
        // AWB light area
        {
            // Strobe:FIXME
            {
            0,    // i4RightBound
            0,    // i4LeftBound
            0,    // i4UpperBound
            0    // i4LowerBound
            },
            // Tungsten
            {
            -191,    // i4RightBound
            -841,    // i4LeftBound
            -242,    // i4UpperBound
            -342    // i4LowerBound
            },
            // Warm fluorescent
            {
            -191,    // i4RightBound
            -841,    // i4LeftBound
            -342,    // i4UpperBound
            -462    // i4LowerBound
            },
            // Fluorescent
            {
            -93,    // i4RightBound
            -191,    // i4LeftBound
            -226,    // i4UpperBound
            -317    // i4LowerBound
            },
            // CWF
            {
            -93,    // i4RightBound
            -191,    // i4LeftBound
            -317,    // i4UpperBound
            -394    // i4LowerBound
            },
            // Daylight
            {
            141,    // i4RightBound
            -93,    // i4LeftBound
            -216,    // i4UpperBound
            -376    // i4LowerBound
            },
            // Shade
            {
            501,    // i4RightBound
            141,    // i4LeftBound
            -216,    // i4UpperBound
            -376    // i4LowerBound
            },
            // Daylight Fluorescent
            {
            141,    // i4RightBound
            -93,    // i4LeftBound
            -376,    // i4UpperBound
            -500    // i4LowerBound
            }
        },
        // PWB light area
        {
            // Reference area
            {
            501,    // i4RightBound
            -841,    // i4LeftBound
            0,    // i4UpperBound
            -500    // i4LowerBound
            },
            // Daylight
            {
            166,    // i4RightBound
            -93,    // i4LeftBound
            -216,    // i4UpperBound
            -376    // i4LowerBound
            },
            // Cloudy daylight
            {
            266,    // i4RightBound
            91,    // i4LeftBound
            -216,    // i4UpperBound
            -376    // i4LowerBound
            },
            // Shade
            {
            366,    // i4RightBound
            91,    // i4LeftBound
            -216,    // i4UpperBound
            -376    // i4LowerBound
            },
            // Twilight
            {
            -93,    // i4RightBound
            -253,    // i4LeftBound
            -216,    // i4UpperBound
            -376    // i4LowerBound
            },
            // Fluorescent
            {
            166,    // i4RightBound
            -241,    // i4LeftBound
            -241,    // i4UpperBound
            -394    // i4LowerBound
            },
            // Warm fluorescent
            {
            -186,    // i4RightBound
            -386,    // i4LeftBound
            -241,    // i4UpperBound
            -394    // i4LowerBound
            },
            // Incandescent
            {
            -186,    // i4RightBound
            -386,    // i4LeftBound
            -216,    // i4UpperBound
            -376    // i4LowerBound
            },
            // Gray World
            {
            5000,    // i4RightBound
            -5000,    // i4LeftBound
            5000,    // i4UpperBound
            -5000    // i4LowerBound
            }
        },
        // PWB default gain	
        {
            // Daylight
            {
            823,    // i4R
            512,    // i4G
            705    // i4B
            },
            // Cloudy daylight
            {
            984,    // i4R
            512,    // i4G
            574    // i4B
            },
            // Shade
            {
            1047,    // i4R
            512,    // i4G
            534    // i4B
            },
            // Twilight
            {
            632,    // i4R
            512,    // i4G
            954    // i4B
            },
            // Fluorescent
            {
            773,    // i4R
            512,    // i4G
            806    // i4B
            },
            // Warm fluorescent
            {
            566,    // i4R
            512,    // i4G
            1155    // i4B
            },
            // Incandescent
            {
            549,    // i4R
            512,    // i4G
            1124    // i4B
            },
            // Gray World
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        // AWB preference color	
        {
            // Tungsten
            {
            0,    // i4SliderValue
            6352    // i4OffsetThr
            },
            // Warm fluorescent	
            {
            0,    // i4SliderValue
            5191    // i4OffsetThr
            },
            // Shade
            {
            22,    // i4SliderValue
            901    // i4OffsetThr
            },
            // Daylight WB gain
            {
            745,    // i4R
            512,    // i4G
            790    // i4B
            },
            // Preference gain: strobe
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: tungsten
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: warm fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: CWF
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: shade
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            },
            // Preference gain: daylight fluorescent
            {
            512,    // i4R
            512,    // i4G
            512    // i4B
            }
        },
        {// CCT estimation
            {// CCT
                2300,    // i4CCT[0]
                2850,    // i4CCT[1]
                4100,    // i4CCT[2]
                5100,    // i4CCT[3]
                6500    // i4CCT[4]
            },
            {// Rotated X coordinate
                -488,    // i4RotatedXCoordinate[0]
                -402,    // i4RotatedXCoordinate[1]
                -257,    // i4RotatedXCoordinate[2]
                -159,    // i4RotatedXCoordinate[3]
                0    // i4RotatedXCoordinate[4]
            }
        }
    },
    {0}
};

#include INCLUDE_FILENAME_ISP_LSC_PARAM
//};  //  namespace
const CAMERA_TSF_TBL_STRUCT CAMERA_TSF_DEFAULT_VALUE =
{
    #include INCLUDE_FILENAME_TSF_PARA
    #include INCLUDE_FILENAME_TSF_DATA
};



typedef NSFeature::RAWSensorInfo<SENSOR_ID> SensorInfoSingleton_T;


namespace NSFeature {
template <>
UINT32
SensorInfoSingleton_T::
impGetDefaultData(CAMERA_DATA_TYPE_ENUM const CameraDataType, VOID*const pDataBuf, UINT32 const size) const
{
    UINT32 dataSize[CAMERA_DATA_TYPE_NUM] = {sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT),
                                             sizeof(NVRAM_CAMERA_3A_STRUCT),
                                             sizeof(NVRAM_CAMERA_SHADING_STRUCT),
                                             sizeof(NVRAM_LENS_PARA_STRUCT),
                                             sizeof(AE_PLINETABLE_T),
                                             0,
                                             sizeof(CAMERA_TSF_TBL_STRUCT)};

    if (CameraDataType > CAMERA_DATA_TSF_TABLE || NULL == pDataBuf || (size < dataSize[CameraDataType]))
    {
        return 1;
    }

    switch(CameraDataType)
    {
        case CAMERA_NVRAM_DATA_ISP:
            memcpy(pDataBuf,&CAMERA_ISP_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_ISP_PARAM_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_3A:
            memcpy(pDataBuf,&CAMERA_3A_NVRAM_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_3A_STRUCT));
            break;
        case CAMERA_NVRAM_DATA_SHADING:
            memcpy(pDataBuf,&CAMERA_SHADING_DEFAULT_VALUE,sizeof(NVRAM_CAMERA_SHADING_STRUCT));
            break;
        case CAMERA_DATA_AE_PLINETABLE:
            memcpy(pDataBuf,&g_PlineTableMapping,sizeof(AE_PLINETABLE_T));
            break;
        case CAMERA_DATA_TSF_TABLE:
            memcpy(pDataBuf,&CAMERA_TSF_DEFAULT_VALUE,sizeof(CAMERA_TSF_TBL_STRUCT));
            break;
        default:
            break;
    }
    return 0;
}}; // NSFeature


