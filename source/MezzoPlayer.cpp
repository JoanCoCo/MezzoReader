/**
 * MezzoPlayer.cpp
 *  
 * SMII 2020/2021
 * Joan Colom Colom
 *  
 * Implementation of the class described in MezzoPlayer.h.
 */

#if __APPLE__
    #include <OpenAL/alut.h>
#else
    #include <AL/alut.h>
#endif
#include <iostream>
#include <chrono>
#include "../include/Note.h"
#include "../include/MezzoPlayer.h"

using namespace std;

const ALfloat MezzoPlayer::listenerPosition[] = {0.0f, 0.0f, 0.0f};
const ALfloat MezzoPlayer::listenerVelocity[] = {0.0f, 0.0f, 0.0f};
const ALfloat MezzoPlayer::listenerOrientation[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};

const ALfloat MezzoPlayer::sourcePosition[] = {0.0f, 0.0f, 0.0f};
const ALfloat MezzoPlayer::sourceVelocity[] = {0.0f, 0.0f, 0.0f};

MezzoPlayer::MezzoPlayer() {
    alutInit(0, NULL);

    alListenerfv(AL_POSITION,listenerPosition);
    alListenerfv(AL_VELOCITY,listenerVelocity);
    alListenerfv(AL_ORIENTATION,listenerOrientation);

    alGenBuffers(7, buffers);
    alGenSources(1, &source);

    for(int i = DO; i <= SI; i++) {
        load_buffer_from_wav_file(i, (ALbyte*) get_tone_audio_file(i - 2).c_str());
    }

    alSourcef(source, AL_GAIN, 1.0f);
    alSourcefv(source, AL_POSITION, sourcePosition);
    alSourcefv(source, AL_VELOCITY, sourceVelocity);
    alSourcei(source, AL_LOOPING, AL_TRUE);

    error = alGetError();
    if(error) {
        errorFree = false;
    } else {
        errorFree = true; 
    }
}

MezzoPlayer::~MezzoPlayer() {
    alDeleteSources(1, &source);
    alDeleteBuffers(7, buffers);
    alutExit ();
}

void MezzoPlayer::load_buffer_from_wav_file(int buffer, ALbyte* file) {
    #if __APPLE__
        alutLoadWAVFile(file, &formatAL, &dataAL, &sizeAL, &freqAL);
    #else
        alutLoadWAVFile(file, &formatAL, &dataAL, &sizeAL, &freqAL, nullptr);
    #endif
    alBufferData(buffers[buffer], formatAL, dataAL, sizeAL, freqAL);
    alutUnloadWAV(formatAL, dataAL, sizeAL, freqAL);
}

void MezzoPlayer::play(Note note) {
    if(errorFree) {
        if(!note.isSilence) {
            ALbyte* noteSoundFile = (ALbyte*) get_tone_audio_file(note.tone).c_str();
            //load_buffer_from_wav_file(noteSoundFile);
            float s = (float) *(note.tone_split(note.tone) + 1);
            int t = *(note.tone_split(note.tone) + 0);
            alSourcei(source, AL_BUFFER, buffers[t]);
            if(s >= 0) {
                alSourcef(source, AL_PITCH, 1.0f + s);
            } else {
                alSourcef(source, AL_PITCH, 1.0f / (2.0f * s));
            }
            alSourcePlay(source);
        }
        auto start = chrono::steady_clock::now();
        auto current = start;
        while (chrono::duration_cast<chrono::milliseconds>(current - start).count() < 2000 * (note.duration/4.0f))
        {
            current = chrono::steady_clock::now();
        }
        if(!note.isSilence) stop();
    }
}

string MezzoPlayer::get_tone_audio_file(int tone) {
    string name = "sounds/pad/";
    int* pair = Note::tone_split(tone);
    int t = *(pair+0);
    int s = *(pair+1);
    switch (t)
    {
    case DO:
        name.append("do");
        break;
    case RE:
        name.append("re");
        break;
    case MI:
        name.append("mi");
        break;
    case FA:
        name.append("fa");
        break;
    case SOL:
        name.append("sol");
        break;
    case LA:
        name.append("la");
        break;
    case SI:
        name.append("si");
        break;
    default:
        break;
    }
    name.append(".wav");
    return name;
}

void MezzoPlayer::stop() {
    if(errorFree && is_playing()) alSourceStop(source);
}

bool MezzoPlayer::is_playing() {
    if(!errorFree) return false;
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

bool MezzoPlayer::has_been_correctly_initialized() {
    return errorFree;
}

string MezzoPlayer::get_error() {
    if(!errorFree) {
        return alGetString(error);
    } else {
        return "";
    }
}
