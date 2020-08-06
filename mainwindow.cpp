#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow),
      _emulatorThread(nullptr),
      _emulatorWorker(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if (_emulatorWorker != nullptr)
    {
        _emulatorWorker->stopEmulation();
    }

    if (_emulatorThread != nullptr)
    {
        _emulatorThread->quit();
        _emulatorThread->wait();

        delete _emulatorThread;
        _emulatorThread = nullptr;
    }

    delete ui;
}

void MainWindow::on_action_Exit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_action_Load_ROM_triggered()
{
    const QString& filename = QFileDialog::getOpenFileName(this, tr("Select ROM file"), QDir::homePath(), tr("ROM files (*.ch8 *.bin)"));
    if (filename.isEmpty())
    {
        return;
    }

    if (_emulatorThread == nullptr)
    {
        _emulatorThread = new QThread(this);
    }

    if (_emulatorWorker == nullptr)
    {
        _emulatorWorker = new EmulatorWorker();
    }

    _emulatorWorker->setROM(filename);
    _emulatorWorker->moveToThread(_emulatorThread);
    _connectSignals();

    setWindowTitle(QString("qchip8 (%1)").arg(filename));

    _emulatorThread->start();
}

void MainWindow::onRefreshScreen(Chip8::FrameBuffer framebuffer)
{
    QImage image(Chip8::DISPLAY_WIDTH, Chip8::DISPLAY_HEIGHT, QImage::Format_Mono);
	
	for (size_t y = 0; y < Chip8::DISPLAY_HEIGHT; ++y)
	{
		for (size_t x = 0; x < Chip8::DISPLAY_WIDTH; ++x)
		{
            image.setPixel(x, y, framebuffer[(y * Chip8::DISPLAY_WIDTH) + x]);
		}
	}

    ui->lblImageBuffer->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::_connectSignals()
{
    connect(_emulatorThread, &QThread::started, _emulatorWorker, &EmulatorWorker::onRunEmulation);
    connect(_emulatorWorker, &EmulatorWorker::finishedEmulation, _emulatorThread, &QThread::quit);
    connect(_emulatorWorker, &EmulatorWorker::finishedEmulation, _emulatorWorker, &EmulatorWorker::deleteLater);
    connect(_emulatorWorker, &EmulatorWorker::refreshScreen, this, &MainWindow::onRefreshScreen);
}
