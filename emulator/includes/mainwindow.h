#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QMessageBox>
#include "emulatorworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class EditorWindow;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

protected:
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

private slots:
	void on_action_Exit_triggered();

	void on_action_Load_ROM_triggered();

	void onRefreshScreen(Chip8::FrameBuffer framebuffer);

	void on_action_About_triggered();

	void on_action_Start_emulation_triggered();

	void on_actionStop_emulation_triggered();

	void on_actionTake_screenshot_triggered();

    void on_actionCreate_new_ROM_triggered();

signals:
	void stopEmulation();

private:
	Ui::MainWindow* ui;
	QThread* _emulatorThread;
	EmulatorWorker* _emulatorWorker;
    EditorWindow* _editorWindow;
	QString _lastFile;
	QImage _framebuffer;

	void _connectSignals() const;
	void _startEmulation();

	bool _isRunning() const;
};
#endif // MAINWINDOW_H
