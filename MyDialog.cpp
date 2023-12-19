#include "stdafx.h"
#include "MyDialog.h"

MyDialog::MyDialog() {
    // Create a layout for the dialog
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Create QTableWidget
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels({ "Column 1", "Column 2", "Column 3" });

    // Populate QTableWidget with some data
    QTableWidgetItem* item;
    QStringList data;
    for (int i = 1; i <= 3; ++i) {
        data << QString::number(i);
    }
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            item = new QTableWidgetItem(data.at(col));
            tableWidget->setItem(row, col, item);
        }
    }

    // Create QPushButton
    QPushButton* button = new QPushButton("Click me!", this);
    connect(button, &QPushButton::clicked, this, &MyDialog::onButtonClick);

    // Add QTableWidget and QPushButton to the layout
    layout->addWidget(tableWidget);
    layout->addWidget(button);

    // Set the layout for the dialog
    setLayout(layout);
}

void MyDialog::onButtonClick() {
    // Example function to handle button click
    qDebug("Button clicked!");
}