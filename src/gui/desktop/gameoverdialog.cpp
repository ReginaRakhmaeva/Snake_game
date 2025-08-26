#include "../../../include/gui/desktop/gameoverdialog.h"

#include <QApplication>

GameOverDialog::GameOverDialog(bool isWon, QWidget* parent)
    : QDialog(parent),
      m_restartButton(new QPushButton("Restart", this)),
      m_quitButton(new QPushButton("Quit", this)),
      m_messageLabel(new QLabel(this)) {
  setupUI(isWon);

  connect(m_restartButton, &QPushButton::clicked, this,
          &GameOverDialog::onRestartClicked);
  connect(m_quitButton, &QPushButton::clicked, this,
          &GameOverDialog::onQuitClicked);
}

GameOverDialog::~GameOverDialog() {}

void GameOverDialog::setupUI(bool isWon) {
  setWindowTitle(isWon ? "You Won!" : "Game Over");
  setFixedSize(300, 200);
  setModal(true);
  setStyleSheet(
      "QDialog { "
      "    background-color: #2c3e50; "
      "}");

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->setSpacing(20);
  mainLayout->setContentsMargins(20, 20, 20, 20);

  QString message = isWon ? "Congratulations! You won!" : "Game Over!";
  m_messageLabel->setText(message);
  m_messageLabel->setAlignment(Qt::AlignCenter);
  m_messageLabel->setStyleSheet(
      "QLabel { "
      "    font-size: 18px; "
      "    font-weight: bold; "
      "    color: white; "
      "}");
  mainLayout->addWidget(m_messageLabel);

  QHBoxLayout* buttonLayout = new QHBoxLayout();
  buttonLayout->setSpacing(20);

  QString buttonStyle =
      "QPushButton { "
      "    background-color: #3498db; "
      "    border: 2px solid #2980b9; "
      "    border-radius: 5px; "
      "    color: white; "
      "    font-size: 14px; "
      "    font-weight: bold; "
      "    padding: 8px; "
      "} "
      "QPushButton:hover { "
      "    background-color: #2980b9; "
      "} "
      "QPushButton:pressed { "
      "    background-color: #21618c; "
      "}";

  m_restartButton->setFixedSize(100, 40);
  m_restartButton->setStyleSheet(buttonStyle);

  m_quitButton->setFixedSize(100, 40);
  m_quitButton->setStyleSheet(buttonStyle);

  buttonLayout->addWidget(m_restartButton);
  buttonLayout->addWidget(m_quitButton);

  mainLayout->addLayout(buttonLayout);
  mainLayout->addStretch();
}

void GameOverDialog::onRestartClicked() {
  emit restartRequested();
  accept();
}

void GameOverDialog::onQuitClicked() {
  accept();
  emit quitRequested();
}
