/**
 * @file timermanager.h
 * @brief Менеджер таймера игры.
 *
 * Класс TimerManager отвечает за:
 * - Управление игровым циклом через QTimer
 * - Адаптивную скорость игры
 * - Синхронизацию с состоянием игры
 */
#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include <QObject>
#include <QTimer>

/**
 * @brief Менеджер таймера игры.
 */
class TimerManager : public QObject {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор.
   * @param parent Родительский объект Qt
   */
  explicit TimerManager(QObject* parent = nullptr);

  /**
   * @brief Запускает таймер с заданным интервалом.
   * @param interval Интервал в миллисекундах
   */
  void start(int interval = 600);

  /**
   * @brief Останавливает таймер.
   */
  void stop();

  /**
   * @brief Устанавливает интервал таймера.
   * @param interval Интервал в миллисекундах
   */
  void setInterval(int interval);

  /**
   * @brief Получает текущий интервал.
   * @return Интервал в миллисекундах
   */
  int getInterval() const;

  /**
   * @brief Проверяет, активен ли таймер.
   * @return true если таймер активен
   */
  bool isActive() const;

 signals:
  /**
   * @brief Сигнал срабатывания таймера.
   */
  void timeout();

 private:
  QTimer* m_timer; /**< Qt таймер */
};

#endif  // TIMERMANAGER_H
