#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>

namespace Ui {
class EditorWindow;
}

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = nullptr);
    ~EditorWindow();

private slots:
    void on_action_Create_binary_triggered();

private:
    Ui::EditorWindow *ui;
    void parseInput(std::string input);
};

#endif // EDITORWINDOW_H
