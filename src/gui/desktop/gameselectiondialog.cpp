#include "../../../include/gui/desktop/gameselectiondialog.h"

#include <QApplication>
#include <QCloseEvent>

GameSelectionDialog::GameSelectionDialog(QWidget* parent)
    : QDialog(parent),
      m_tetrisButton(new QPushButton("Tetris", this)),
      m_snakeButton(new QPushButton("Snake", this)),
      m_titleLabel(new QLabel("Select a Game", this)) {
  setupUI();

  // Подключение сигналов
  connect(m_tetrisButton, &QPushButton::clicked, this,
          &GameSelectionDialog::onTetrisSelected);
  connect(m_snakeButton, &QPushButton::clicked, this,
          &GameSelectionDialog::onSnakeSelected);
}

GameSelectionDialog::~GameSelectionDialog() {}

void GameSelectionDialog::closeEvent(QCloseEvent* event) {
  // При закрытии через крестик устанавливаем результат как Rejected
  reject();
  event->accept();
}

void GameSelectionDialog::setupUI() {
  setWindowTitle("Game Selection");
  setFixedSize(300, 200);
  setModal(true);
  setStyleSheet(
      "QDialog { "
      "    background-color: #2c3e50; "
      "}");

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->setSpacing(20);
  mainLayout->setContentsMargins(20, 20, 20, 20);

  // Заголовок
  m_titleLabel->setAlignment(Qt::AlignCenter);
  m_titleLabel->setStyleSheet(
      "QLabel { "
      "    font-size: 18px; "
      "    font-weight: bold; "
      "    color: white; "
      "}");
  mainLayout->addWidget(m_titleLabel);

  // Кнопки
  QHBoxLayout* buttonLayout = new QHBoxLayout();
  buttonLayout->setSpacing(20);

  // Единый стиль для кнопок
  QString buttonStyle =
      "QPushButton { "
      "    background-color: #27ae60; "
      "    border: 2px solid #229954; "
      "    border-radius: 5px; "
      "    color: white; "
      "    font-size: 14px; "
      "    font-weight: bold; "
      "    padding: 8px; "
      "} "
      "QPushButton:hover { "
      "    background-color: #229954; "
      "} "
      "QPushButton:pressed { "
      "    background-color: #1e8449; "
      "}";

  m_tetrisButton->setFixedSize(100, 40);
  m_tetrisButton->setStyleSheet(buttonStyle);

  m_snakeButton->setFixedSize(100, 40);
  m_snakeButton->setStyleSheet(buttonStyle);

  buttonLayout->addWidget(m_tetrisButton);
  buttonLayout->addWidget(m_snakeButton);

  mainLayout->addLayout(buttonLayout);
  mainLayout->addStretch();
}

void GameSelectionDialog::onTetrisSelected() {
  emit gameSelected(GameType::TETRIS);
  accept();
}

void GameSelectionDialog::onSnakeSelected() {
  emit gameSelected(GameType::SNAKE);
  accept();
}
