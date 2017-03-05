#include "all.hpp"

#include "stk/RtAudio.h"

// loop instruments
#include "stk/Flute.h"
#include "stk/Clarinet.h"
#include "stk/Saxofony.h"
#include "stk/Bowed.h"
// non-loop instruments
#include "stk/Plucked.h"
#include "stk/Mandolin.h"
#include "stk/Rhodey.h"

stk::StkFloat clamp(stk::StkFloat x) {
    if (x > 1.0) {
        return 1.0;
    } else if (x < -1.0) {
        return -1.0;
    } else {
        return x;
    }
}

int tick(
    void *buffer, void *, unsigned int buffer_size, double,
    RtAudioStreamStatus, void *
) {
    static std::map<uint64_t, stk::Instrmnt *> instruments;

    stk::StkFloat *samples = (stk::StkFloat *) buffer;

    for (unsigned int i = 0; i < buffer_size; ++i, ++samples) {
        const std::map<uint64_t, Note> &notes {
            apply_vibe(
                smooth_notes(
                    fetch_notes(),
                    1000 / 44100. * buffer_size, 100, 200, 100 // TODO: use fp timestamp?
                ),
                smooth_vibe(
                    fetch_vibe(),
                    1000 / 44100. * buffer_size, 100, 100
                )
            )
        };

        for (const auto &note: notes) {
            if (instruments.find(note.first) == instruments.end()) {
                instruments.insert({note.first, {
                    new stk::Saxofony(200)
                }});

                instruments.at(note.first)->noteOn(
                    440 * pow(2, note.second.pitch / 12), 1
                );
            }
        }

        *samples = 0;

        for (auto &instrument: instruments) {
            if (notes.find(instrument.first) == notes.end()) {
                instrument.second->noteOff(1);
                delete instrument.second;

                instruments.erase(instrument.first);
            } else {
                instrument.second->setFrequency(
                    440 * pow(2, notes.at(instrument.first).pitch / 12)
                );

                *samples += notes.at(instrument.first).volume
                    * instrument.second->tick();
            }
        }

        *samples /= sqrt(instruments.size() + 0.00001); // FIXME: 

        *samples = clamp(*samples);
    }

    return 0;
}

int main() {
    stk::Stk::setSampleRate(44100.);

    RtAudio out;
    RtAudio::StreamParameters parameters;

    parameters.deviceId = out.getDefaultOutputDevice();
    parameters.nChannels = 1;

    RtAudioFormat format = sizeof(stk::StkFloat) == 8
        ? RTAUDIO_FLOAT64
        : RTAUDIO_FLOAT32;
    unsigned int buffer_size = stk::RT_BUFFER_SIZE;

    out.openStream(
        &parameters, nullptr, format, (unsigned int) stk::Stk::sampleRate(),
        &buffer_size, &tick, nullptr
    );
    out.startStream();

    while (true) {
        stk::Stk::sleep(1000);
    }
}
