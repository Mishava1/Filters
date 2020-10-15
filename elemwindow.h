#ifndef ELEMWINDOW_H
#define ELEMWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include <QHeaderView>

extern std::vector<int> structElement;

namespace Ui {
class elemWindow;
}

class elemWindow : public QDialog
{
    Q_OBJECT

public:
    explicit elemWindow(QWidget *parent = nullptr);
    ~elemWindow();
    std::vector<int> getElement();
    int getLength();

signals:
    void valueChanged(int newValue);

private slots:
    void confirm_slot();
    void spinbox_value_changed_slot();

private:
    Ui::elemWindow *ui;
    QTableWidget *matrix;

};


#endif // ELEMWINDOW_H
