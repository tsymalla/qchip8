#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QMessageBox>
#include "emulatorworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
	
protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void on_action_Exit_triggered();

    void on_action_Load_ROM_triggered();

    void onRefreshScreen(Chip8::FrameBuffer framebuffer);

    void on_action_About_triggered();

signals:
    void stopEmulation();

private:
    Ui::MainWindow *ui;
    QThread* _emulatorThread;
    EmulatorWorker* _emulatorWorker;

    void _connectSignals() const;

    bool _isRunning() const;
};
#endif // MAINWINDOW_H
