#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
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

    _emulator = std::make_unique<Chip8::CPU>(filename);
    _connectSignals();
    _emulator->loadROM();
}

void MainWindow::onRomLoadingFailed()
{
    statusBar()->showMessage("Could not load ROM.");
}

void MainWindow::onRomLoaded()
{
    statusBar()->showMessage("ROM successfully loaded.");
    _emulator->run();
}

void MainWindow::_connectSignals()
{
    connect(_emulator.get(), &Chip8::CPU::romLoadingFailed, this, &MainWindow::onRomLoadingFailed);
    connect(_emulator.get(), &Chip8::CPU::romLoaded, this, &MainWindow::onRomLoaded);
}
