#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QLabel>
#include <QMainWindow>
#include <QSpinBox>
#include <QToolButton>

#include "drawwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createToolBar();
public slots:
    void ShowStyle();
    void ShowColor();

private:
    DrawWidget * drawWidget;
    QLabel *     styleLabel;
    QComboBox *  styleComboBox;
    QLabel *     widthLabel;
    QSpinBox *   widthSpinBox;
    QToolButton *colorBtn;
    QToolButton *clearBtn;
};

#endif  // MAINWINDOW_H
