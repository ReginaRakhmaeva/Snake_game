/**
 * @file main.cpp
 * @brief Точка входа в приложение BrickGame.
 *
 * Создает объект QApplication и главное окно MainWindow, после чего запускает
 * главный цикл обработки событий Qt.
 */

#include <QApplication>
#include <QStyleFactory>

#include "../../../include/gui/desktop/mainwindow.h"
/**
 * @brief Основная функция приложения.
 * @param argc Количество аргументов командной строки
 * @param argv Массив аргументов командной строки
 * @return Код завершения приложения
 *
 * Создает QApplication, главное окно MainWindow, показывает его и запускает
 * цикл обработки событий.
 */
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MainWindow window;
  window.show();

  return app.exec();
}
