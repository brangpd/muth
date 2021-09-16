#ifndef MUTH_LIBRARY_H
#define MUTH_LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

// 库初始化、销毁、错误消息API
typedef struct MuthLib MuthLib;
MuthLib *muthLibInit(size_t messageBufSize);
void muthLibDestroy(MuthLib *lib);
const char *muthLibMessage(MuthLib *lib);

// 升降记号
typedef enum EMuthAccidentalCode {
  MUTH_ACCIDENTAL_NONE, // 无记号
  MUTH_ACCIDENTAL_NATURAL, // 自然或还原记号
  MUTH_ACCIDENTAL_SHARP, // #
  MUTH_ACCIDENTAL_FLAT, // b
  MUTH_ACCIDENTAL_DOUBLE_SHARP, // ##
  MUTH_ACCIDENTAL_DOUBLE_FLAT, // x
} MuthAccidentalCode;

// 音名
typedef enum EMuthPitchName {
  MUTH_PITCH_NAME_C = 'C',
  MUTH_PITCH_NAME_D = 'D',
  MUTH_PITCH_NAME_E = 'E',
  MUTH_PITCH_NAME_F = 'F',
  MUTH_PITCH_NAME_G = 'G',
  MUTH_PITCH_NAME_A = 'A',
  MUTH_PITCH_NAME_B = 'B',
} MuthPitchName;

typedef struct MuthPitch {
  MuthPitchName pitchName; // 音名
  MuthAccidentalCode accidentalCode; // 升降号
} MuthPitch;

// 科学音高记号，中央C为C4。不同的记号可以表示同一个音高
typedef struct MuthSpn {
  MuthPitch pitch; // 音高
  int octave; // 八度
} MuthSpn;

// 调号，C大调/A小调为0，增加升号个数用正数表示，增加降号个数用负数表示
typedef signed char MuthKeySignature;

// MIDI 转频率，MIDI码超出范围返回-1
float muthMidiToFreq(MuthLib *lib, int midiCode);

// 频率转 MIDI，返回最接近该频率的MIDI码，频率错误输出-1
int muthFreqToMidi(MuthLib *lib, float freq);

// MIDI 转音名，只输出一种可能
int muthMidiToPitch1(
    int inMidiCode,
    MuthSpn *outMuthSpn
);

// MIDI 转音名，输出至多N种可能，返回输出的可能种数，出错返回-1
int muthMidiToPitchN(
    int inMidiCode,
    int inN,
    MuthSpn *outMuthSpnArr
);

// 音名转MIDI，成功返回MIDI码，空指针返回-1，无法转换返回-2
int muthSpnToMidi(MuthLib *lib, MuthSpn *inSpn);

// 比较两个音高符号，l==r返回0，l<r返回负数，l>r返回正数。空指针行为未定义。
int muthSpnCmp(MuthSpn *lhs, MuthSpn *rhs);

// 计算音高频率，空指针返回-1，音符错误返回-2
float muthSpnToFreq(MuthLib *lib, MuthSpn *inSpn);

// 从频率构造音符，构造成功返回0，失败返回-1
int muthFreqToSpn(MuthLib *lib, float inFreq, MuthSpn *outSpn);

// 计算音名增减
int muthPitchNameCal(MuthLib *lib, MuthPitchName inPitchName, int inNum, MuthPitchName *outPitchName);

int muthPitchCal(MuthLib *lib, MuthPitch *inPitch, int inNum, MuthPitch *outPitch);

#ifdef __cplusplus
}; // extern "C"
#endif

#endif //MUTH_LIBRARY_H
