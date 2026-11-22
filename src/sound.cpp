// SOUND and PLAY (sound.hpp).
#include "wfc/brun71/sound.hpp"

#include <cmath>
#include <cstddef>
#include <optional>

#include "wfc/brun71/error.hpp"

namespace wfc::brun71 {
namespace {

// Clock ticks a second (the PC timer).
constexpr double kTicksPerSecond = 18.2;

[[noreturn]] void refuse() {
    throw BasicError(kIllegalFunctionCall);
}

char upper(const char c) {
    return c >= 'a' && c <= 'z' ? static_cast<char>(c - 'a' + 'A') : c;
}

// The semitone of a note letter in its octave (C 0 ... B 11).
int semitone(const char letter) {
    switch (letter) {
        case 'C':
            return 0;
        case 'D':
            return 2;
        case 'E':
            return 4;
        case 'F':
            return 5;
        case 'G':
            return 7;
        case 'A':
            return 9;
        default:
            return 11;  // B
    }
}

class Reader {
public:
    explicit Reader(const std::string_view text) : text_(text) {}

    void skip_blanks() {
        while (at_ < text_.size() && text_[at_] == ' ') {
            ++at_;
        }
    }
    bool done() {
        skip_blanks();
        return at_ >= text_.size();
    }
    char next() { return upper(text_[at_++]); }
    bool accept(const char c) {
        skip_blanks();
        if (at_ < text_.size() && upper(text_[at_]) == c) {
            ++at_;
            return true;
        }
        return false;
    }
    // A number, or nullopt when no digit follows.
    std::optional<int> number() {
        skip_blanks();
        if (at_ >= text_.size() || text_[at_] < '0' || text_[at_] > '9') {
            return std::nullopt;
        }
        long value = 0;
        while (at_ < text_.size() && text_[at_] >= '0' && text_[at_] <= '9') {
            value = value * 10 + (text_[at_] - '0');
            if (value > 32767) {
                refuse();
            }
            ++at_;
        }
        return static_cast<int>(value);
    }
    int dots() {
        int count = 0;
        while (accept('.')) {
            ++count;
        }
        return count;
    }
    int required(const int low, const int high) {
        const auto value = number();
        if (!value || *value < low || *value > high) {
            refuse();
        }
        return *value;
    }

private:
    std::string_view text_;
    std::size_t at_ = 0;
};

}  // namespace

Tone sound_tone(const double frequency, const double duration) {
    if (!(frequency >= 37.0 && frequency <= 32767.0) ||
        !(duration >= 0.0 && duration <= 65535.0)) {
        refuse();
    }
    Tone tone;
    tone.frequency = static_cast<int>(std::lround(frequency));
    tone.milliseconds =
        static_cast<int>(std::lround(duration * 1000.0 / kTicksPerSecond));
    return tone;
}

void MusicPlayer::note(std::vector<Tone>& tones, const int number,
                       const int length, const int dots,
                       const bool rest) const {
    // A whole note is four beats of 60000 / tempo milliseconds.
    double time = 240000.0 / (tempo_ * static_cast<double>(length));
    for (int i = 0; i < dots; ++i) {
        time *= 1.5;
    }
    const int total = static_cast<int>(std::lround(time));
    if (rest || number == 0) {
        tones.push_back(Tone{0, total});
        return;
    }
    // N46 is the A above middle C, 440 Hz.
    const double frequency = 440.0 * std::pow(2.0, (number - 46) / 12.0);
    const int sounding = total * style_ / 8;
    tones.push_back(Tone{static_cast<int>(std::lround(frequency)), sounding});
    if (total > sounding) {
        tones.push_back(Tone{0, total - sounding});
    }
}

std::vector<Tone> MusicPlayer::play(const std::string_view music) {
    std::vector<Tone> tones;
    Reader reader(music);
    while (!reader.done()) {
        const char command = reader.next();
        switch (command) {
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G': {
                int number = octave_ * 12 + semitone(command) + 1;
                if (reader.accept('#') || reader.accept('+')) {
                    ++number;
                } else if (reader.accept('-')) {
                    --number;
                }
                const auto length = reader.number();
                if (length && (*length < 1 || *length > 64)) {
                    refuse();
                }
                const int dots = reader.dots();
                if (number < 1 || number > 84) {
                    refuse();
                }
                note(tones, number, length.value_or(length_), dots, false);
                break;
            }
            case 'N': {
                const int number = reader.required(0, 84);
                note(tones, number, length_, reader.dots(), false);
                break;
            }
            case 'P': {
                const int length = reader.required(1, 64);
                note(tones, 0, length, reader.dots(), true);
                break;
            }
            case 'O':
                octave_ = reader.required(0, 6);
                break;
            case '<':
                if (octave_ > 0) {
                    --octave_;
                }
                break;
            case '>':
                if (octave_ < 6) {
                    ++octave_;
                }
                break;
            case 'L':
                length_ = reader.required(1, 64);
                break;
            case 'T':
                tempo_ = reader.required(32, 255);
                break;
            case 'M':
                if (reader.accept('N')) {
                    style_ = 7;
                } else if (reader.accept('L')) {
                    style_ = 8;
                } else if (reader.accept('S')) {
                    style_ = 6;
                } else if (!reader.accept('F') && !reader.accept('B')) {
                    refuse();
                }
                break;
            default:
                refuse();
        }
    }
    return tones;
}

}  // namespace wfc::brun71
