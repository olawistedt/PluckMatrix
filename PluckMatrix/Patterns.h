#ifndef __PATTERNS_H__
#define __PATTERNS_H__

const int kNumberOfPatterns = 24;
const int kNumberOfNotes = 32 * 12;

//------------------------------------------------------------------------------------------
// PATTERNS
//------------------------------------------------------------------------------------------
class Patterns
{
public:
  Patterns();
  void randomize(int patternNr);

  int mNotes[kNumberOfPatterns][kNumberOfNotes];
  int mSelectedPattern;
};

#endif  // __PATTERNS_H__
