#ifndef TIMEGL_H
#define TIMEGL_H

#include <chrono>
#include <GLFW/glfw3.h>

class TimeGL {
private:
    float deltaTime = 0.0f;  // Время между кадрами (сек)
    float totalTime = 0.0f;  // Общее время работы (сек)
    bool paused = false;      // Пауза
    std::chrono::steady_clock::time_point lastTime;

public:
    TimeGL();

    // Обновление таймера (вызывать каждый кадр)
    void Update();

    // Получить время между кадрами
    float GetDeltaTime() const;

    // Получить общее время работы
    float GetTotalTime() const;

    // Поставить на паузу
    void Pause();

    // Снять с паузы
    void Resume();

    // Проверка, на паузе ли
    bool IsPaused() const;
};

#endif 