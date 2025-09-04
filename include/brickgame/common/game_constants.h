/**
 * @file game_constants.h
 * @brief Общие константы для игр проекта BrickGame.
 *
 * Этот заголовочный файл определяет базовые параметры,
 * используемые всеми играми (например, Tetris и Snake).
 * Константы включают размеры игрового поля, параметры панели информации
 * и задержки обновления кадров.
 *
 * Используется как единый источник "магических чисел" для унификации
 * логики и отрисовки игр.
 */
#ifndef BRICKGAME_COMMON_GAME_CONSTANTS_H
#define BRICKGAME_COMMON_GAME_CONSTANTS_H

#ifdef __cplusplus
extern "C" {
#endif

/// Ширина игрового поля (в ячейках).
static const int kGameWidth = 10;

/// Высота игрового поля (в ячейках).
static const int kGameHeight = 20;

/// Ширина панели информации (в символах/ячейках).
static const int kInfoPanelWidth = 15;

/// Задержка между кадрами в миллисекундах.
static const int kFrameDelayMs = 100;

#ifdef __cplusplus
}
#endif

#endif  // BRICKGAME_COMMON_GAME_CONSTANTS_H