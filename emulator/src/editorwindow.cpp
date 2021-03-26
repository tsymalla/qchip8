#include "editorwindow.h"
#include "ui_editorwindow.h"
#include <QString>
#include <lexer.hpp>
#include <parser.hpp>

void EditorWindow::parseInput(std::string input)
{
    compiler::Lexer lexer(std::move(input));
    auto result = lexer.Lex();

    compiler::Parser parser(result);

    ui->lstLogOutput->clear();

    if (parser.Parse() != nullptr)
    {
        ui->lstLogOutput->addItem(QString("Success!"));
    }
    else
    {
        ui->lstLogOutput->addItem(QString("Failure."));
    }
}

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::on_action_Create_binary_triggered()
{
    parseInput(ui->txtProgramText->toPlainText().toStdString());
}
