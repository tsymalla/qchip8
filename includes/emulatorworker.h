#ifndef EMULATORWORKER_H
#define EMULATORWORKER_H

#include <QObject>
#include "cpu.h"
#include <QMutex>

class EmulatorWorker : public QObject
{
	Q_OBJECT

public:
	explicit EmulatorWorker(QObject* parent = nullptr);
	void setROM(QString filename);
	void keyDown(int key);
	void keyUp(int key);

	void stopEmulation();
	bool isRunning() const;

public slots:
	void onRunEmulation();
	void onRefreshScreen(Chip8::FrameBuffer framebuffer);

signals:
	void refreshScreen(Chip8::FrameBuffer framebuffer);
	void finishedEmulation();

private:
	Chip8::CPU _emulator;
	QMutex _mutex;
};

#endif // EMULATORWORKER_H