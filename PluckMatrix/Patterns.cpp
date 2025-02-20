#include <ctime>
#include <cstdlib>
#include <assert.h>
#include "Patterns.h"

Patterns::Patterns() : mSelectedPattern(0) {}

void
Patterns::randomize(int patternNr)
{
  // Use the time as a seed for the random number generator
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  assert(patternNr >= 0 && patternNr <= 24);

  // Create rhythm
  bool rhythm[16];
  int nrUpInRow = 0;
  int nrLowInRow = 0;
  int maxUpInRow = 3;
  int maxLowInRow = 2;

  for (int i = 0; i < 16; i++)
  {
    rhythm[i] = std::rand() % 100 < 50;
    if (rhythm[i])  // up
    {
      nrUpInRow += 1;
      nrLowInRow = 0;
      if (nrUpInRow > maxUpInRow)
      {
        rhythm[i] = false;
        nrUpInRow = 0;
        nrLowInRow = 1;
      }
    }
    else
    // down
    {
      nrLowInRow += 1;
      nrUpInRow = 0;
      if (nrLowInRow > maxLowInRow)
      {
        rhythm[i] = true;
        nrLowInRow = 0;
        nrUpInRow = 1;
      }
    }
  }

  const int kNoteNrForC0 = 24;  // The midi note number for C0
  const int kNoteNrForC1 = 36;  // The midi note number for C1
  const int kNoteNrForC2 = 48;  // The midi note number for C2
  const int kNoteNrForC3 = 60;  // The midi note number for C3

  int baseNote =
      (patternNr % 24) +
      kNoteNrForC0;  // Let the current pattern be the base note for the randomized pattern
  int i = 0;
  int octave = -1;
  const int kNrOf16ths = 32;
  const int kKvint = 7;  // Kvint is the swedish word for "fifth".

  while (i < kNrOf16ths)
  {
    int playedNote = baseNote;
    octave = -1;
    if (rhythm[i % 16])  // If we want play the base note.
    {
      playedNote += 12;
      if (playedNote + kKvint <= kNoteNrForC3)
      {
        playedNote += (std::rand() % 2) == 1 ? kKvint : 0;
      }
    }

    if (i < 16)
    {
      if (playedNote < kNoteNrForC1)
      {
        octave = -1;
      }
      else if (playedNote < kNoteNrForC2)
      {
        octave = 0;
      }
      else if (playedNote <= kNoteNrForC3)
      {
        octave = 1;
      }
      else
      {
        assert(false);
      }
    }
    //else if (i < 32)
    //{
    //  mPattern[i][playedNote + baseNote + 7] = true;
    //}

    mNotes[0][i] = 11 - (playedNote % 12);  // The C is 0 and B is 11
    mProps[0][i] = 0;
    if (octave == -1)
    {
      mProps[0][i] = mProps[0][i] | kOctaveLowMask;
    }
    else if (octave == 1)
    {
      mProps[0][i] = mProps[0][i] | kOctaveHighMask;
    }

    //if ((std::rand() % 100) < 25)  // 25% probability it will be silent
    //{
    //  mNotes[0][i] = kNoNote;
    //}

    //notes[i].octave = octave;
    //notes[i].accent = (std::rand() % 100) < 30;  // 30%
    //notes[i].slide = (std::rand() % 100) < 15;   // 15%

    // Increase step
    i++;
  }

  // A nice trance pluck
  for (int i = 0; i < kNrOf16ths; i++)
  {
    // A,  A#, B,  C,  C#, D,  D#, E,  F, F#, G , G#
    // 45, 46, 47, 48, 49, 50, 51, 52, 53,54, 55, 56
    // 57, 58, 59, 60, 61, 62, 63, 64, 65,66, 67, 68
    mNotes[0][i] = 45;  // Set all to A
    if (i == 2 || i == 5 || i == 8)
    {
      mNotes[0][i] = 64;  // Set to E3
    }
    else if (i == 11 || i == 14 || i == 30)
    {
      mNotes[0][i] = 69;  // Set to A3
    }
    else if (i == 18)
    {
      mNotes[0][i] = 67;  // Set to G3
    }
    else if (i == 21 || i == 24)
    {
      mNotes[0][i] = 60;  // Set to C3
    }
    else if (i == 27)
    {
      mNotes[0][i] = 65;  // Set to F3
    }
    mNotes[0][i] -= 36;  // Compensate for the + 36 done later
  }
}