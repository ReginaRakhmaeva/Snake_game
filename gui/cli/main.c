/**
 * @file main.c
 * @brief Точка входа в приложение BrickGame.
 *
 * Минимальная реализация main(), которая просто запускает
 * контроллер приложения. Все логика вынесена в app_controller.
 */

#include "../../include/gui/cli/app_controller.h"

/**
 * @brief Точка входа в программу.
 *
 * @return int Код завершения программы
 */
int main(void) {
  run_app();
  return 0;
}