// The sound statements of BASIC PDS 7.1: SOUND and PLAY. They make a list of
// tones (a frequency and a time); the host plays them (on Windows with the
// console's Beep) or, when there is nothing to play them on, skips them.
//
// SOUND frequency, duration: 37 to 32767 Hz for duration clock ticks (18.2 a
// second, 0 to 65535).
//
// PLAY music$: the music macro language. A to G a note (then # or + sharp,
// - flat, a length 1-64 and dots); O0-O6 the octave (O3 starts at middle C;
// a program starts in O4), < and > one octave down and up; Ln the length of
// the notes (1 a whole note, 4 a quarter note); Tn the tempo, 32-255 quarter
// notes a minute; Pn a pause of length n; Nn note n (1-84, 0 a rest); MN, ML,
// MS normal (7/8 of the length sounds), legato (all) and staccato (3/4); MF
// and MB foreground and background (played the same). Blanks are ignored.
#pragma once

#include <string_view>
#include <vector>

namespace wfc::brun71 {

struct Tone {
    // Hertz, or 0 for silence.
    int frequency = 0;
    int milliseconds = 0;

    bool operator==(const Tone&) const = default;
};

// SOUND: the tone; error 5 when an argument is out of range. A duration
// below one millisecond gives 0 milliseconds (nothing to play).
Tone sound_tone(double frequency, double duration);

// PLAY: the music state (octave, length, tempo, style) lasts from one PLAY
// to the next, as in BASIC.
class MusicPlayer {
public:
    // The tones of the music string; error 5 for a command BASIC refuses
    // (an unknown letter, a number out of range, X with VARPTR$).
    std::vector<Tone> play(std::string_view music);

    int octave() const noexcept { return octave_; }
    int length() const noexcept { return length_; }
    int tempo() const noexcept { return tempo_; }

private:
    void note(std::vector<Tone>& tones, int number, int length, int dots,
              bool rest) const;

    int octave_ = 4;
    int length_ = 4;
    int tempo_ = 120;
    // Eighths of a note's length that sound: 7 normal, 8 legato, 6 staccato.
    int style_ = 7;
};

}  // namespace wfc::brun71
