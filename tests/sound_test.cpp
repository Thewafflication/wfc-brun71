// TC-0008: SOUND and PLAY (tests/tc-0008-sound.tex).
#include <vector>

#include "check.hpp"
#include "wfc/brun71/sound.hpp"

using wfc::brun71::MusicPlayer;
using wfc::brun71::sound_tone;
using wfc::brun71::Tone;

namespace {

using Tones = std::vector<Tone>;

void sound() {
    CHECK(sound_tone(440.0, 18.2) == (Tone{440, 1000}));
    CHECK(sound_tone(37.0, 0.0) == (Tone{37, 0}));
    CHECK(sound_tone(32767.0, 65535.0).frequency == 32767);
    CHECK_ERROR(5, sound_tone(36.0, 1.0));
    CHECK_ERROR(5, sound_tone(32768.0, 1.0));
    CHECK_ERROR(5, sound_tone(440.0, -1.0));
    CHECK_ERROR(5, sound_tone(440.0, 65536.0));
}

void notes() {
    MusicPlayer player;
    // O4 L4 T120 MN: a quarter note is 500 ms, 7/8 of it sounds.
    CHECK(player.play("C") == (Tones{{523, 437}, {0, 63}}));
    CHECK(player.play("o3a") == (Tones{{440, 437}, {0, 63}}));
    CHECK(player.octave() == 3);
    CHECK(player.play("O4C#") == player.play("O4D-"));
    CHECK(player.play("C4.") == (Tones{{523, 656}, {0, 94}}));
    CHECK(player.play("P4") == (Tones{{0, 500}}));
    CHECK(player.play("N0") == (Tones{{0, 500}}));
    CHECK(player.play("N46") == (Tones{{440, 437}, {0, 63}}));
    CHECK(player.play("") == Tones{});
    CHECK(player.play("  ") == Tones{});
}

void state() {
    MusicPlayer player;
    CHECK(player.play("ML L8 O3 A") == (Tones{{440, 250}}));
    CHECK(player.length() == 8);
    CHECK(player.play("MS L4 A") == (Tones{{440, 375}, {0, 125}}));
    CHECK(player.play("MN T240 L2 A") == (Tones{{440, 437}, {0, 63}}));
    CHECK(player.tempo() == 240);
    CHECK(player.play("MF MB") == Tones{});
    player.play("O6 >");
    CHECK(player.octave() == 6);
    player.play("O0 <");
    CHECK(player.octave() == 0);
}

void errors() {
    MusicPlayer player;
    CHECK_ERROR(5, player.play("O7"));
    CHECK_ERROR(5, player.play("N85"));
    CHECK_ERROR(5, player.play("L0"));
    CHECK_ERROR(5, player.play("L65"));
    CHECK_ERROR(5, player.play("T31"));
    CHECK_ERROR(5, player.play("P"));
    CHECK_ERROR(5, player.play("C65"));
    CHECK_ERROR(5, player.play("H"));
    CHECK_ERROR(5, player.play("X"));
    CHECK_ERROR(5, player.play("MX"));
    CHECK_ERROR(5, player.play("O0 C-"));
}

}  // namespace

int main() {
    sound();
    notes();
    state();
    errors();
    return wfc::brun71::test::finish("sound");
}
