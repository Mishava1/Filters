#include "elemwindow.h"
#include "ui_elemwindow.h"

#include <QDebug>

std::vector<int> structElement;

elemWindow::elemWindow(QWidget *parent) : QDialog(parent), ui(new Ui::elemWindow) {
    ui->setupUi(this);
    structElement = {0, 1, 0, 1, 1, 1, 0, 1, 0};
    setWindowTitle(u8"Структурный элемент");
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinbox_value_changed_slot()));
    connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(confirm_slot()));
    matrix = new QTableWidget(3, 3, this);
    QHeaderView *hHeader = new QHeaderView(Qt::Horizontal);
    matrix->setHorizontalHeader(hHeader);
    QHeaderView *vHeader = new QHeaderView(Qt::Vertical);
    matrix->setVerticalHeader(vHeader);
    hHeader->setVisible(false);
    vHeader->setVisible(false);
    for(int row = 0; row != matrix->rowCount(); ++row){
        for(int column = 0; column != matrix->columnCount(); ++column) {
            QTableWidgetItem *newItem = new QTableWidgetItem("0");
            matrix->horizontalHeader()->setDefaultSectionSize(35);
            matrix->setItem(row, column, newItem);
            matrix->item(row,column)->setTextAlignment(Qt::AlignCenter);
        }
        matrix->verticalHeader()->setDefaultSectionSize(35);
    }
    ui->confirmButton->setGeometry(175 - ui->confirmButton->width() / 2,  280, 100, 21);
    matrix->setGeometry(175 - matrix->width() / 2, 75, 119, 107);
}

elemWindow::~elemWindow()
{
    delete ui;
}


void elemWindow::spinbox_value_changed_slot() {
    int value = ui->spinBox->value();
    int ocount = matrix->columnCount();
    if (ocount != value) {
        matrix->setRowCount(value);
        matrix->setColumnCount(value);
    }
    for(int row = 0; row != matrix->rowCount(); ++row){
        for(int column = 0; column != matrix->columnCount(); ++column) {
            QTableWidgetItem *newItem = new QTableWidgetItem("0");
            matrix->horizontalHeader()->setDefaultSectionSize(35);
            matrix->setItem(row, column, newItem);
            matrix->item(row,column)->setTextAlignment(Qt::AlignCenter);
        }
        matrix->verticalHeader()->setDefaultSectionSize(35);
    }
    matrix->setGeometry(175 - matrix->width() / 2, 75, 119 + (value - 3) * 39 , 107 + (value - 3) * 36);
    matrix->setGeometry(175 - matrix->width() / 2, 75, 119 + (value - 3) * 39 , 107 + (value - 3) * 35);
}

void elemWindow::confirm_slot() {
    structElement.clear();
    for(int row = 0; row < matrix->rowCount(); row++){
        for(int column = 0; column < matrix->columnCount(); column++) {
             structElement.push_back(matrix->item(row, column)->text().toInt());
        }
    }
    matrix->setRowCount(3);
    matrix->setColumnCount(3);
    for(int row = 0; row != matrix->rowCount(); ++row){
        for(int column = 0; column != matrix->columnCount(); ++column) {
            QTableWidgetItem *newItem = new QTableWidgetItem("0");
            matrix->horizontalHeader()->setDefaultSectionSize(35);
            matrix->setItem(row, column, newItem);
            matrix->item(row,column)->setTextAlignment(Qt::AlignCenter);
        }
        matrix->verticalHeader()->setDefaultSectionSize(35);
    }
    matrix->setGeometry(175 - matrix->width() / 2, 75, 119, 107);
    matrix->setGeometry(175 - matrix->width() / 2, 75, 119, 107);
    ui->spinBox->setValue(3);
    this->close();

}
