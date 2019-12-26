#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_toolButton_clicked();

    void is_Selectbin_triggered();

    void on_lineEdit_exec_textChanged(const QString &arg1);

    void on_radioButton_2_toggled(bool checked);

    void on_toolButton_2_clicked();

    void is_Selectdir_triggered();

private:
    Ui::MainWindow *ui;

    QAction *Selectbin;

    QAction *Selectdir;

    QString iconfile;

    QString isTerminal = "false";
};

#endif // MAINWINDOW_H
