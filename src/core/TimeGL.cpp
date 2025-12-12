#include "TimeGL.h"

TimeGL::TimeGL() {
    lastTime = std::chrono::steady_clock::now();
}

void TimeGL::Update() {
    auto now = std::chrono::steady_clock::now();
    deltaTime = std::chrono::duration<float>(now - lastTime).count();

    if (!paused)
        totalTime += deltaTime;

    lastTime = now;
}

float TimeGL::GetDeltaTime() const {
    return deltaTime;
}

float TimeGL::GetTotalTime() const {
    return totalTime;
}

void TimeGL::Pause() {
    paused = true;
}

void TimeGL::Resume() {
    paused = false;
    lastTime = std::chrono::steady_clock::now(); // чтобы не накапливалось время паузы
}

bool TimeGL::IsPaused() const {
    return paused;
}
