#pragma once
#include <QTableWidget>

class CustomTableWidget : public QTableWidget
{

public:
    CustomTableWidget(QWidget* parent = nullptr);
    CustomTableWidget::~CustomTableWidget();
    int rows;
protected:
    bool event(QEvent* event) override;

private:
    void handlePasteShortcut();
    void handleCopyShortcut();
    QString copyToClipboard();

    QString copiedText;
};

///SAMPLES

/*
public:
  helloQtPalette() {
    // Install the event filter for the entire application
    qApp->installEventFilter(this);
  };

protected:
bool eventFilter(QObject *obj, QEvent *event) override {
    if (event->type() == QEvent::KeyPress) {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
      if (keyEvent->matches(QKeySequence::Copy)) {
        // Handle copy logic
        // ...
        return true;
      } else if (keyEvent->matches(QKeySequence::Paste)) {
        // Handle paste logic
        // ...
        return true;
      }
    }
    return false;
  }
*/