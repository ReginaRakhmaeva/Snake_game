/**
 * @file timermanager.cpp
 * @brief Реализация менеджера таймера игры.
 */

#include "../../include/gui/desktop/timermanager.h"

TimerManager::TimerManager(QObject* parent)
    : QObject(parent), m_timer(new QTimer(this)) {
  connect(m_timer, &QTimer::timeout, this, &TimerManager::timeout);
}

void TimerManager::start(int interval) { m_timer->start(interval); }

void TimerManager::stop() { m_timer->stop(); }

void TimerManager::setInterval(int interval) { m_timer->setInterval(interval); }

int TimerManager::getInterval() const { return m_timer->interval(); }

bool TimerManager::isActive() const { return m_timer->isActive(); }
