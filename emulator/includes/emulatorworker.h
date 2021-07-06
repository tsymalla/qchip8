#ifndef EMULATORWORKER_H
#define EMULATORWORKER_H

#include "cpu.h"
#include <QMutex>
#include <QObject>

class EmulatorWorker : public QObject
{
	Q_OBJECT

  public:
	explicit EmulatorWorker(QObject* parent = nullptr);
	void setROM(QString filename);
	void keyDown(int key);
	void keyUp(int key);

	bool isRunning() const;

  public slots:
	void onRunEmulation();
	void onStopEmulation();
	void onRefreshScreen(Chip8::FrameBuffer framebuffer);

  signals:
	void refreshScreen(Chip8::FrameBuffer framebuffer);
	void finishedEmulation();

  private:
	Chip8::CPU _emulator;
	QMutex _mutex;
};

#endif // EMULATORWORKER_H