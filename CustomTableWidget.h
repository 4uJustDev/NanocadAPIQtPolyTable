#pragma once
#include <QTableWidget>

class CustomTableWidget : public QTableWidget
{
public:
    CustomTableWidget(QWidget* parent = nullptr);
    CustomTableWidget::~CustomTableWidget();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    QString copiedText;
};
