#include "stdafx.h"
#include "CustomTableWidget.h"
#include <QKeyEvent>
#include <QTableWidgetItem>
#include <QClipboard>
#include <QApplication>

CustomTableWidget::CustomTableWidget(QWidget* parent) : QTableWidget(parent)
{
    rows = 2;
    setColumnCount(3);
    setRowCount(rows);
    QStringList listHeaders;
    listHeaders << "X" << "Y" << "Z";
    setHorizontalHeaderLabels(listHeaders); 
}

CustomTableWidget::~CustomTableWidget() {};

bool CustomTableWidget::event(QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->matches(QKeySequence::Paste))
        {
            handlePasteShortcut();
            return true;
        }
        else if (keyEvent->matches(QKeySequence::Copy))
        {
            handleCopyShortcut();
            return true;
        }
    }
    return QTableWidget::event(event);
}

void CustomTableWidget::handlePasteShortcut()
{
    QStringList table_row_data_list = copiedText.split("\n", QString::SkipEmptyParts);

    QList<QTableWidgetItem*> current_selected_items = selectedItems();
    if (current_selected_items.isEmpty())
    {
        return; // No cell selected to paste into
    }

    int startRow = current_selected_items.first()->row();
    int startCol = current_selected_items.first()->column();

    for (int i = 0; i < table_row_data_list.size() && (startRow + i) < rowCount(); ++i)
    {
        QStringList row_data_list = table_row_data_list.at(i).split("\t");
        for (int j = 0; j < row_data_list.size() && (startCol + j) < columnCount(); ++j)
        {
            item(startRow + i, startCol + j)->setText(row_data_list.at(j));
        }
    }
}

void CustomTableWidget::handleCopyShortcut()
{
    copiedText = copyToClipboard();
}

QString CustomTableWidget::copyToClipboard()
{
    QString copied_text;
    QList<QTableWidgetItem*> current_selected_items = selectedItems();
    int current_row = -1;

    for (QTableWidgetItem* item : current_selected_items)
    {
        if (current_row != item->row())
        {
            if (current_row != -1)
            {
                copied_text.append("\n");
            }
            current_row = item->row();
        }
        else if (!copied_text.isEmpty())
        {
            copied_text.append("\t");
        }

        copied_text.append(item->text());
    }

    // Update the copiedText member variable
    copiedText = copied_text;

    QApplication::clipboard()->setText(copied_text);
    return copied_text;
}