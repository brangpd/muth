#include "library.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
static char *sPitchNames = "CDEFGAB";
typedef struct MuthLib {
  size_t messageBufSize;
  char *messageBuf;
} MuthLib;
MuthLib *muthLibInit(size_t messageBufSize) {
  MuthLib *res = malloc(sizeof(MuthLib));
  if (!res) {
    return NULL;
  }
  if (!messageBufSize) {
    res->messageBuf = NULL;
  } else {
    char *messageBuf = malloc(messageBufSize);
    if (!messageBuf) {
      free(res);
      return NULL;
    }
    res->messageBuf = messageBuf;
  }
  res->messageBufSize = messageBufSize;
  return res;
}
void muthLibDestroy(MuthLib *lib) {
  if (!lib) {
    return;
  }
  free(lib->messageBuf);
  lib->messageBuf = NULL;
  lib->messageBufSize = 0;
  free(lib);
}
const char *muthLibMessage(MuthLib *lib) {
  if (lib && lib->messageBuf) {
    return lib->messageBuf;
  }
  return "";
}
static void muthLibPrintMessage(MuthLib *lib, char *fmt, ...) {
  if (!lib || !lib->messageBuf || !lib->messageBufSize) {
    return;
  }
  va_list vl;
      va_start(vl, fmt);
  vsnprintf(lib->messageBuf, lib->messageBufSize, fmt, vl);
      va_end(vl);
}
float muthMidiToFreq(MuthLib *lib, int midiCode) {
  if (midiCode < 0 || midiCode > 127) {
    muthLibPrintMessage(lib, "MIDI码%i超出范围", midiCode);
    return -1;
  }
  return powf(2, (float)(midiCode - 69) / 12.0f) * 440;
}
int muthFreqToMidi(MuthLib *lib, float freq) {
  if (freq < 0) {
    muthLibPrintMessage(lib, "频率%f不合法", freq);
    return -1;
  }
  int res = 69 + (int)roundf(12 * log2f(freq / 440));
  if (res < 0 || res > 127) {
    muthLibPrintMessage(lib, "频率%f超出MIDI码范围", freq);
    return -1;
  }
  return res;
}
int muthSpnToMidi(MuthLib *lib, MuthSpn *inSpn) {
  if (!inSpn) {
    muthLibPrintMessage(lib, "空指针");
    return -1;
  }
  int octave = inSpn->octave;
  if (octave < -1 || octave > 9) {
    muthLibPrintMessage(lib, "八度%i超出范围", octave);
    return -2;
  }
  int midiOctave = (octave + 1) * 12;
  MuthPitchName pitchName = inSpn->pitch.pitchName;
  MuthAccidentalCode accCode = inSpn->pitch.accidentalCode;
  int midiInOctave;
  switch (pitchName) {
  case MUTH_PITCH_NAME_C:
    midiInOctave = 0;
    break;
  case MUTH_PITCH_NAME_D:
    midiInOctave = 2;
    break;
  case MUTH_PITCH_NAME_E:
    midiInOctave = 4;
    break;
  case MUTH_PITCH_NAME_F:
    midiInOctave = 5;
    break;
  case MUTH_PITCH_NAME_G:
    midiInOctave = 7;
    break;
  case MUTH_PITCH_NAME_A:
    midiInOctave = 9;
    break;
  case MUTH_PITCH_NAME_B:
    midiInOctave = 11;
    break;
  default:
    muthLibPrintMessage(lib, "音名%i不合法", pitchName);
    return -1;
  }
  int midiAcc;
  switch (accCode) {
  case MUTH_ACCIDENTAL_NONE:
  case MUTH_ACCIDENTAL_NATURAL:
    midiAcc = 0;
    break;
  case MUTH_ACCIDENTAL_SHARP:
    midiAcc = 1;
    break;
  case MUTH_ACCIDENTAL_FLAT:
    midiAcc = -1;
    break;
  case MUTH_ACCIDENTAL_DOUBLE_SHARP:
    midiAcc = 2;
    break;
  case MUTH_ACCIDENTAL_DOUBLE_FLAT:
    midiAcc = -2;
    break;
  default:
    muthLibPrintMessage(lib, "升降记号%i不合法", pitchName);
    return -1;
  }

  int midi = midiOctave + midiInOctave + midiAcc;
  if (midi >= 0 && midi <= 127) {
    return midi;
  }
  muthLibPrintMessage(lib, "MIDI码%i超出范围", midi);
  return -1;
}
int muthSpnCmp(MuthSpn *lhs, MuthSpn *rhs) {
  return muthSpnToMidi(NULL, lhs) - muthSpnToMidi(NULL, rhs);
}
float muthSpnToFreq(MuthLib *lib, MuthSpn *inSpn) {
  int midi = muthSpnToMidi(lib, inSpn);
  if (midi < 0) {
    return -1;
  }

  return muthMidiToFreq(lib, midi);
}
// 计算音名加减，X+0==X, C+1==D, C+7==C, C+5==A
int muthPitchNameCal(MuthLib *lib, MuthPitchName inPitchName, int inNum, MuthPitchName *outPitchName) {
  if (!outPitchName) {
    muthLibPrintMessage(lib, "空指针");
    return -1;
  }
  if (inNum == 0) {
    *outPitchName = inPitchName;
    return 0;
  }
  char *it = strchr(sPitchNames, inPitchName);
  if (it == NULL) {
    muthLibPrintMessage(lib, "输入音名错误");
    return -2;
  }
  inNum %= 7;
  inNum += 7;
  int oldIndex = (int)(it - sPitchNames);
  char newPitchName = sPitchNames[((oldIndex) + inNum) % 7];
  *outPitchName = (int)newPitchName;

  return 0;
}
