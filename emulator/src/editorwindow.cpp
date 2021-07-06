#include "editorwindow.h"
#include "ui_editorwindow.h"
#include <QString>
#include <lexer.hpp>
#include <parser.hpp>
#include <string>

void EditorWindow::parseInput(std::string input)
{
	compiler::Lexer lexer(std::move(input));
	auto result = lexer.Lex();

	compiler::Parser parser(result);

	ui->lstLogOutput->clear();

	const auto parseResult = parser.Parse();
	if (!parseResult->GetErrorState()->HasErrors())
	{
		ui->lstLogOutput->addItem(QString("Success!"));
	}
	else
	{
		ui->lstLogOutput->addItem(QString("Failure:"));
		/*for (const std::string& line : parseResult->GetErrorMessages())
		{
		    ui->lstLogOutput->addItem(QString::fromStdString(line));
		}*/
	}
}

EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::EditorWindow)
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
