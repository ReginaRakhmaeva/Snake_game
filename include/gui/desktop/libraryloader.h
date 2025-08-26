#ifndef LIBRARYLOADER_H
#define LIBRARYLOADER_H

#include <QObject>
#include <QString>
#include <dlfcn.h>
#include "../../brickgame/common/types.h"

enum class GameType {
    TETRIS,
    SNAKE
};

struct GameAPI {
    void* lib_handle = nullptr;
    void (*userInput)(UserAction_t action, bool hold) = nullptr;
    GameInfo_t (*updateState)(void) = nullptr;
    bool (*isOver)(void) = nullptr;
    bool valid = false;
    QString error;
};

class LibraryLoader : public QObject
{
    Q_OBJECT

public:
    explicit LibraryLoader(QObject *parent = nullptr);
    ~LibraryLoader();

    bool loadGame(GameType gameType);
    void unloadGame();
    GameAPI getAPI() const { return m_api; }
    bool isLoaded() const { return m_api.valid; }

private:
    GameAPI m_api;
    QString getLibraryPath(GameType gameType) const;
};

#endif // LIBRARYLOADER_H
