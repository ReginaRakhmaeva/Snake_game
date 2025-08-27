/**
 * @file gameselectiondialog.h
 * @brief Заголовочный файл диалога выбора игры для Desktop GUI.
 *
 * Этот класс предоставляет интерфейс диалога выбора игры в приложении
 * BrickGame. Диалог отображает кнопки для выбора игры Tetris или Snake и
 * сигнализирует контроллеру (GameController) о выборе пользователя. Также
 * поддерживает сигнал при закрытии окна вручную.
 *
 * Основные возможности:
 *  - Отображение кнопок Tetris и Snake.
 *  - Сигналы для выбранной игры и отклонения диалога.
 *  - Настройка интерфейса с помощью QVBoxLayout и QHBoxLayout.
 */
#ifndef GAMESELECTIONDIALOG_H
#define GAMESELECTIONDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "gamecontroller.h"

/**
 * @brief Диалог выбора игры.
 * Отображает две кнопки для выбора Tetris или Snake.
 * Сигнализирует контроллеру выбранную игру.
 */
class GameSelectionDialog : public QDialog {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор диалога выбора игры.
   * @param parent Родительский виджет.
   */
  explicit GameSelectionDialog(QWidget* parent = nullptr);
  /**
   * @brief Деструктор диалога.
   */
  ~GameSelectionDialog();

 protected:
  /**
   * @brief Обработчик закрытия окна.
   * Генерирует сигнал dialogRejected(), если пользователь закрыл окно вручную.
   * @param event Событие закрытия окна.
   */
  void closeEvent(QCloseEvent* event) override;

 signals:
  /**
   * @brief Сигнал выбора игры.
   * @param gameType Выбранная игра (TETRIS или SNAKE).
   */

  void gameSelected(GameType gameType);
  /**
   * @brief Сигнал, который испускается при отклонении диалога.
   */
  void dialogRejected();

 private slots:
  /**
   * @brief Слот для выбора игры Tetris.
   * Генерирует сигнал gameSelected(GameType::TETRIS) и закрывает диалог.
   */
  void onTetrisSelected();
  /**
   * @brief Слот для выбора игры Snake.
   * Генерирует сигнал gameSelected(GameType::SNAKE) и закрывает диалог.
   */
  void onSnakeSelected();

 private:
  /**
   * @brief Настраивает интерфейс диалога.
   */
  void setupUI();
  /** @brief Кнопка для выбора игры Tetris. */
  QPushButton* m_tetrisButton;
  /** @brief Кнопка для выбора игры Snake. */
  QPushButton* m_snakeButton;
  /** @brief Метка с заголовком диалога ("Select a Game"). */
  QLabel* m_titleLabel;
};

#endif  // GAMESELECTIONDIALOG_H
