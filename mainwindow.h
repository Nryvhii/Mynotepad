#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTextEdit *diary_input_box = new QTextEdit;

private slots:
    void new_clicked();
    void save_clicked();
    void open_clicked();
    void font_clicked();
};
#endif // MAINWINDOW_H
