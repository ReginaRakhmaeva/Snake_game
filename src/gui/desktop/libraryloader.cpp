#include "../../../include/gui/desktop/libraryloader.h"
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QFile>

LibraryLoader::LibraryLoader(QObject *parent)
    : QObject(parent)
{
}

LibraryLoader::~LibraryLoader()
{
    unloadGame();
}

bool LibraryLoader::loadGame(GameType gameType)
{
    unloadGame();

    QString libraryPath = getLibraryPath(gameType);
    
    m_api.lib_handle = dlopen(libraryPath.toUtf8().constData(), RTLD_LAZY);
    if (!m_api.lib_handle) {
        m_api.error = QString("Failed to load library: %1").arg(dlerror());
        qDebug() << "Error loading library:" << m_api.error;
        return false;
    }

    m_api.userInput = (void(*)(UserAction_t, bool))dlsym(m_api.lib_handle, "userInput");
    m_api.updateState = (GameInfo_t(*)())dlsym(m_api.lib_handle, "updateCurrentState");
    m_api.isOver = (bool(*)())dlsym(m_api.lib_handle, "isGameOver");

    if (!m_api.userInput || !m_api.updateState || !m_api.isOver) {
        m_api.error = "Failed to load required functions";
        qDebug() << "Error loading functions:" << m_api.error;
        unloadGame();
        return false;
    }

    m_api.valid = true;
    qDebug() << "Game library loaded successfully:" << libraryPath;
    return true;
}

void LibraryLoader::unloadGame()
{
    if (m_api.lib_handle) {
        dlclose(m_api.lib_handle);
        m_api.lib_handle = nullptr;
    }
    
    m_api.userInput = nullptr;
    m_api.updateState = nullptr;
    m_api.isOver = nullptr;
    m_api.valid = false;
    m_api.error.clear();
}

QString LibraryLoader::getLibraryPath(GameType gameType) const
{
    QString libraryName;
    switch (gameType) {
        case GameType::TETRIS:
            libraryName = "libtetris.so";
            break;
        case GameType::SNAKE:
            libraryName = "libsnake.so";
            break;
    }

    QString currentPath = QDir::currentPath() + "/" + libraryName;
    if (QFile::exists(currentPath)) {
        return currentPath;
    }

    QString appPath = QCoreApplication::applicationDirPath() + "/" + libraryName;
    if (QFile::exists(appPath)) {
        return appPath;
    }

    return libraryName;
}
