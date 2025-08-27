/**
 * @file gameoverdialog.h
 * @brief Диалоговое окно окончания игры (победа/проигрыш) для Desktop GUI.
 *
 * Этот класс реализует модальное окно, которое появляется по завершению игры.
 * Отображает сообщение "Game Over" или "You Won!" в зависимости от результата.
 * Содержит кнопки для перезапуска игры или выхода из приложения.
 */

#ifndef GAMEOVERDIALOG_H
#define GAMEOVERDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class GameOverDialog : public QDialog {
  Q_OBJECT

 public:
  /**
   * @brief Конструктор диалогового окна.
   * @param isWon Флаг, указывающий на победу (true) или проигрыш (false)
   * @param parent Родительский виджет (по умолчанию nullptr)
   */
  explicit GameOverDialog(bool isWon = false, QWidget* parent = nullptr);
  /**
   * @brief Деструктор диалогового окна.
   */
  ~GameOverDialog();

 signals:
  /**
   * @brief Сигнал для запроса перезапуска игры.
   * Вызывается, когда пользователь нажимает кнопку Restart.
   */
  void restartRequested();
  /**
   * @brief Сигнал для запроса выхода из игры.
   * Вызывается, когда пользователь нажимает кнопку Quit.
   */

  void quitRequested();

 private slots:
  /**
   * @brief Слот вызывается при нажатии кнопки Restart.
   * Генерирует сигнал restartRequested() и закрывает окно.
   */
  void onRestartClicked();
  /**
   * @brief Слот вызывается при нажатии кнопки Quit.
   * Генерирует сигнал quitRequested() и закрывает окно.
   */

  void onQuitClicked();

 private:
  /**
   * @brief Настройка пользовательского интерфейса.
   * @param isWon Флаг победы/проигрыша для отображения соответствующего
   * сообщения.
   */
  void setupUI(bool isWon);
  /**
   * @brief Кнопка "Restart" для перезапуска игры.
   * Подключена к слоту onRestartClicked().
   */
  QPushButton* m_restartButton;
  /**
   * @brief Кнопка "Quit" для выхода из игры.
   * Подключена к слоту onQuitClicked().
   */
  QPushButton* m_quitButton;
  /**
   * @brief Метка с сообщением о результате игры.
   * Отображает "Game Over" или "You Won!" в зависимости от результата.
   */
  QLabel* m_messageLabel;
};

#endif  // GAMEOVERDIALOG_H
