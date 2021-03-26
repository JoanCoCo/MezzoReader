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
#elif
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
    int error;
    string errorS;
    alutInit(0, NULL);

    alListenerfv(AL_POSITION,listenerPosition);
    alListenerfv(AL_VELOCITY,listenerVelocity);
    alListenerfv(AL_ORIENTATION,listenerOrientation);

    alGenBuffers(1, &buffer);
    alGenSources(1, &source);

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
    alDeleteBuffers(1, &buffer);
    alutExit ();
}

void MezzoPlayer::LoadBufferFromWAVFile(ALbyte* file) {
    if(errorFree) {
        alDeleteBuffers(1, &buffer);
        alGenBuffers(1, &buffer);
        alutLoadWAVFile(file, &formatAL, &dataAL, &sizeAL, &freqAL);
        alBufferData(buffer, formatAL, dataAL, sizeAL, freqAL);
        alutUnloadWAV(formatAL, dataAL, sizeAL, freqAL);
        alSourcei(source, AL_BUFFER, buffer);
    }
}

void MezzoPlayer::Play(Note note) {
    if(errorFree) {
        if(!note.isSilence) {
            ALbyte* noteSoundFile = (ALbyte*) GetToneAudioFile(note.tone).c_str();
            LoadBufferFromWAVFile(noteSoundFile);
            float s = (float) *(note.tone_split(note.tone) + 1);
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
        if(!note.isSilence) Stop();
    }
}

string MezzoPlayer::GetToneAudioFile(int tone) {
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

void MezzoPlayer::Stop() {
    if(errorFree && IsPlaying()) alSourceStop(source);
}

bool MezzoPlayer::IsPlaying() {
    if(!errorFree) return false;
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void MezzoPlayer::WaitToFinishPlaying() {
    while (IsPlaying()){}
}