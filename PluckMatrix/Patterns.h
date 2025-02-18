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
  Patterns();
  void randomize(int patternNr);

  int mNotes[kNumberOfPatterns][kNrOfNotes];
  int mProps[kNumberOfPatterns][kNrOfProps];
  int mSelectedPattern;
};

#endif  // __PATTERNS_H__
