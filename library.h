#ifndef MUTH_LIBRARY_H
#define MUTH_LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

const char *muthMessage();

// 音程
typedef enum EMuthInterval {
  // 第2字节表示类型：纯10h、大20h、小30h、增40h、减50h
  MUTH_INTERVAL_QUALITY_MASK = 0xf0,
  MUTH_INTERVAL_QUALITY_PERFECT = 0x10,
  MUTH_INTERVAL_QUALITY_MAJOR = 0x20,
  MUTH_INTERVAL_QUALITY_MINOR = 0x30,
  MUTH_INTERVAL_QUALITY_AUGMENTED = 0x40,
  MUTH_INTERVAL_QUALITY_DIMINISHED = 0x50,
  // 第一字节表示度数
  MUTH_INTERVAL_NUMBER_MASK = 0xf,
  MUTH_INTERVAL_PERF_1 = MUTH_INTERVAL_QUALITY_PERFECT | 1, // 纯1
  MUTH_INTERVAL_MIN_2 = MUTH_INTERVAL_QUALITY_MINOR | 2, // 小2
  MUTH_INTERVAL_MAJ_2 = MUTH_INTERVAL_QUALITY_MAJOR | 2, // 大2
  MUTH_INTERVAL_AUG_2 = MUTH_INTERVAL_QUALITY_AUGMENTED | 2, // 增2
  MUTH_INTERVAL_DIM_4 = MUTH_INTERVAL_QUALITY_DIMINISHED | 3, // 减3
  MUTH_INTERVAL_MIN_3 = MUTH_INTERVAL_QUALITY_MINOR | 3, // 小3
  MUTH_INTERVAL_MAJ_3 = MUTH_INTERVAL_QUALITY_MAJOR | 3, // 大3
  MUTH_INTERVAL_AUG_4 = MUTH_INTERVAL_QUALITY_AUGMENTED | 3, // 增3
  MUTH_INTERVAL_PERF_4 = MUTH_INTERVAL_QUALITY_PERFECT | 4, // 纯4
  MUTH_INTERVAL_PERF_5 = MUTH_INTERVAL_QUALITY_PERFECT | 5, // 纯5
  MUTH_INTERVAL_PERF_8 = MUTH_INTERVAL_QUALITY_PERFECT | 8, // 纯8
} EMuthInterval;

// 升降记号
typedef enum EMuthAccidentalCode {
  MUTH_ACCIDENTAL_NONE, // 无记号
  MUTH_ACCIDENTAL_NATURAL, // 自然或还原记号
  MUTH_ACCIDENTAL_SHARP, // #
  MUTH_ACCIDENTAL_FLAT, // b
  MUTH_ACCIDENTAL_DOUBLE_SHARP, // ##
  MUTH_ACCIDENTAL_DOUBLE_FLAT, // x
} EMuthAccidentalCode;

// 音名
typedef enum EMuthPitchName {
  MUTH_PITCH_NAME_C = 'C',
  MUTH_PITCH_NAME_D = 'D',
  MUTH_PITCH_NAME_E = 'E',
  MUTH_PITCH_NAME_F = 'F',
  MUTH_PITCH_NAME_G = 'G',
  MUTH_PITCH_NAME_A = 'A',
  MUTH_PITCH_NAME_B = 'B',
} EMuthPitchName;

typedef struct MuthPitch {
  EMuthPitchName pitchName; // 音名
  EMuthAccidentalCode accidentalCode; // 升降号
} MuthPitch;

// 科学音高记号，中央C为C4。不同的记号可以表示同一个音高
typedef struct MuthSpn {
  MuthPitch pitch; // 音高
  int octave; // 八度
} MuthSpn;

// 调号，C大调/A小调为0，增加升号个数用正数表示，增加降号个数用负数表示
typedef signed char MuthKeySignature;

// MIDI 转音名，只输出一种可能
int muthMidiToPitch1(
    int inMidiCode,
    struct MuthSpn *outMuthSpn
);

// MIDI 转音名，输出N种可能
int muthMidiToPitchN(
    int inMidiCode,
    int inN,
    struct MuthSpn *outMuthSpnArr
);

// 音名转MIDI，成功返回MIDI码，无法转换返回-1
int muthSpnToMidi(
    struct MuthSpn *inSpn
);

// 比较两个音高符号的频率，l==r返回0，l<r返回负数，l>r返回正数。空指针行为未定义。
int muthSpnCmp(
    struct MuthSpn *lhs,
    struct MuthSpn *rhs
);

// 计算音高频率，错误返回-1
float muthSpnFreq(
    struct MuthSpn *spn
);

// 从频率构造音符，构造成功返回0，失败返回-1
int muthSpnFromFreq(
    float inFreq,
    struct MuthSpn *outSpn
);

// 计算音名增减
int muthPit(
    EMuthPitchName inPitchName,
    int inNum,
    EMuthPitchName *outPitchName
);

int muthPitchCal(
    MuthPitch *inPitch,
    int inNum,
    MuthPitch *outPitch
);

// 和弦计算


#ifdef __cplusplus
}; // extern "C"
#endif

#endif //MUTH_LIBRARY_H
