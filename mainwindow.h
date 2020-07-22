#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "cpu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_Exit_triggered();

    void on_action_Load_ROM_triggered();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<Chip8::CPU> _emulator;
};
#endif // MAINWINDOW_H
