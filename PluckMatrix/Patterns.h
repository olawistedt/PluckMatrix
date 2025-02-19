#ifndef __PATTERNS_H__
#define __PATTERNS_H__

const int kNumberOfPatterns = 24;
const int kNrOfNotes = 12;
const int kNrOfProps = 2;

//------------------------------------------------------------------------------------------
// PATTERNS
//------------------------------------------------------------------------------------------
class Patterns
{
public:
  enum
  {
    kNoNote = -1,
    kOctaveMediumMask = 0x0,
    kOctaveLowMask = 0x1,
    kOctaveHighMask = 0x2
  };
  Patterns();
  void randomize(int patternNr);

  int mNotes[kNumberOfPatterns][kNrOfNotes];
  int mProps[kNumberOfPatterns][kNrOfProps];
  int mSelectedPattern;
};

#endif  // __PATTERNS_H__
