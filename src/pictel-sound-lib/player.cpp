//
//  pictel_sound_file.cpp
//  Pictel-sound
//
// Copyright (c) 2022 Krzysztof Pawłowski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "player.hpp"

using namespace PictelSound;

Player::Player(DecoderI *decoder, SystemAudioI *systemAudio)
:   __PlayerI(decoder, systemAudio)
,   m_decoder(decoder)
,   m_systemAudio(systemAudio)
{
}

Player::~Player()
{
    Close();
}

bool Player::Open()
{
    if (!m_decoder->Open())
    {   return false;
    }

    m_systemAudio->SetDecoder(m_decoder);
    m_systemAudio->PrepareToPlay();
    return true;
}

void Player::Play()
{
    m_systemAudio->Play();
}

void Player::Pause()
{
    m_systemAudio->Pause();
}

void Player::Stop()
{
    m_systemAudio->Stop();
}

void Player::Close()
{
    if (m_systemAudio != nullptr)
    {   m_systemAudio->Stop();
        m_systemAudio->Free();
        delete m_systemAudio;
    }

    if (m_decoder != nullptr)
    {   m_decoder->Close();
        delete m_decoder;
        m_decoder = NULL;
    }
}

bool Player::QueryIsPlaging()
{
    if (m_systemAudio != nullptr)
    {   return m_systemAudio->QueryIsRunning();
    }
    else
    {   return false;
    }
}

double Player::QueryDuration()
{
    if (m_systemAudio == nullptr)
    {   return -1;
    }
    return m_systemAudio->GetDuration();
}

double Player::QueryPosition()
{
    if (m_systemAudio == nullptr)
    {   return -1;
    }
    return m_systemAudio->QueryPosition();
}

void Player::SetVolume(double value)
{
    m_systemAudio->SetVolume(value);
}

void Player::SetLoops(bool loops)
{
    m_systemAudio->SetLoops(loops);
}

PlayerState Player::GetState()
{
    return PLAYER_STOPPED;
}

PlayerCallbackI* Player::AddCallback(std::function<void(PlayerState)> lambda)
{
    return m_systemAudio->AddCallback(lambda);
}

void Player::RemoveCallback(PlayerCallbackI* callback)
{
    m_systemAudio->RemoveCallback(callback);
}
