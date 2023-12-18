#include "stdafx.h"
#include "CustomTableWidget.h"
#include <QKeyEvent>
#include <QTableWidgetItem>
#include <QClipboard>
#include <QApplication>

CustomTableWidget::CustomTableWidget(QWidget* parent) : QTableWidget(parent)
{
    int rows = 4;
    setColumnCount(3);
    setRowCount(rows);
    QStringList listHeaders;
    listHeaders << "X" << "Y" << "Z";
    setHorizontalHeaderLabels(listHeaders);
    horizontalHeaderItem(0)->setTextAlignment(Qt::AlignLeft);
}

CustomTableWidget::~CustomTableWidget() {};

void CustomTableWidget::keyPressEvent(QKeyEvent* event) {
    if (event->matches(QKeySequence::Paste)) {
        // Your paste logic using copiedText
        QStringList table_row_data_list = copiedText.split("\n", QString::SkipEmptyParts);

        QList<QTableWidgetItem*> current_selected_items = selectedItems();
        if (current_selected_items.isEmpty()) {
            return;  // No cell selected to paste into
        }

        int startRow = current_selected_items.first()->row();
        int startCol = current_selected_items.first()->column();

        for (int i = 0; i < table_row_data_list.size() && (startRow + i) < rowCount(); ++i) {
            QStringList row_data_list = table_row_data_list.at(i).split("\t");
            for (int j = 0; j < row_data_list.size() && (startCol + j) < columnCount(); ++j) {
                item(startRow + i, startCol + j)->setText(row_data_list.at(j));
            }
        }

        event->accept();
    }
    else if (event->matches(QKeySequence::Copy)) {
        // Your copy logic
        QString copied_text;
        QList<QTableWidgetItem*> current_selected_items = selectedItems();
        int current_row = -1;

        for (QTableWidgetItem* item : current_selected_items) {
            if (current_row != item->row()) {
                if (current_row != -1) {
                    copied_text.append("\n");
                }
                current_row = item->row();
            }
            else if (!copied_text.isEmpty()) {
                copied_text.append("\t");
            }

            copied_text.append(item->text());
        }

        // Update the copiedText member variable
        copiedText = copied_text;

        QApplication::clipboard()->setText(copied_text);
        event->accept();
    }
    else {
        QTableWidget::keyPressEvent(event);
    }
}