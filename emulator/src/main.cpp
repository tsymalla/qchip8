#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	qRegisterMetaType<Chip8::FrameBuffer>();

	MainWindow w;
	w.show();
	return a.exec();
}
