#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QProcessEnvironment>
#include <QMessageBox>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(this->width(), this->height()); //固定窗口大小
    ui->lineEdit_type->setText("Application");
    Selectbin = new QAction(this);
    Selectbin->setIcon(QIcon(":/icon/application-x-executable.svg"));
    ui->lineEdit_exec->addAction(Selectbin,QLineEdit::TrailingPosition);
    connect(Selectbin,SIGNAL(triggered()),this,SLOT(is_Selectbin_triggered()));
    ui->radioButton_2->setChecked(true);
    Selectdir = new QAction(this);
    Selectdir->setIcon(QIcon(":/icon/folder.svg"));
    ui->lineEdit_dir->addAction(Selectdir,QLineEdit::TrailingPosition);
    connect(Selectdir,SIGNAL(triggered()),this,SLOT(is_Selectdir_triggered()));
    QString home = QProcessEnvironment::systemEnvironment().value("HOME");
    ui->lineEdit_dir->setText(home + "/Desktop");
    //setAttribute(Qt::WA_TranslucentBackground,true); //主窗体全透明，控件不透明，不可取
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_toolButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,"选择应用图标","~","图片文件(*.png *.svg *.jpg *.icon *.jpeg);;全部文件(*)");
    if(!file.isEmpty()) iconfile = file;
    QIcon appicon;
    appicon.addFile(iconfile);
    ui->toolButton->setIcon(appicon);
}
void MainWindow::is_Selectbin_triggered()
{
    QString file = QFileDialog::getOpenFileName(this,"选择应用程序",ui->lineEdit_exec->text(),"脚本文件(*.sh);;全部文件(*)");
    if(!file.isEmpty())ui->lineEdit_exec->setText(file);
}
void MainWindow::on_lineEdit_exec_textChanged(const QString &arg1)
{
    QString path = arg1;
    int start = path.lastIndexOf("/") + 1;
    int end = path.lastIndexOf(".");
    int length;
    if(end < start) length = path.length() - start;
    else length = end - start;
    ui->lineEdit_name->setText(path.mid(start,length));
    ui->lineEdit_file->setText(ui->lineEdit_name->text());
}
void MainWindow::on_radioButton_2_toggled(bool checked)
{
    if(checked) isTerminal = "false";
    else isTerminal = "true";
}
void MainWindow::on_toolButton_2_clicked()
{
    QString file = ui->lineEdit_file->text() + ".desktop";
    QString dir = ui->lineEdit_dir->text();
    QFile desktopfile(dir + "/" + file);
    QMessageBox mbq(QMessageBox::Question,"文件已存在","覆盖？");
    mbq.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    mbq.setDefaultButton(QMessageBox::No);
    mbq.setButtonText(QMessageBox::Yes,QString("是"));
    mbq.setButtonText(QMessageBox::No,QString("否"));  //设置按钮文字
    int xcenterpoint = this->frameGeometry().x();
    int ycenterpoint = this->frameGeometry().y() + this->frameGeometry().height()/2;
    mbq.move(xcenterpoint,ycenterpoint);
    if(desktopfile.exists())
        if(QMessageBox::No == mbq.exec())
            file = "/dev/null";         //如果否，则将文件写入/dev/null，避免跳转，使功能复杂化
    desktopfile.open(QIODevice::WriteOnly|QIODevice::Text);
    QMessageBox mbc(QMessageBox::Critical,"错误","当前路径不存在或不可写");
    mbc.setStandardButtons(QMessageBox::Yes);
    mbc.setButtonText(QMessageBox::Yes,QString("确定"));
    mbc.move(xcenterpoint,ycenterpoint);
    if(!desktopfile.isWritable()) mbc.exec();  //不可写的情况下，没必要更改
    QTextStream write(&desktopfile);
    write << "[Desktop Entry]" << "\n" << \
             "Name=" << ui->lineEdit_name->text() << "\n" << \
             "Exec=" << ui->lineEdit_exec->text() << " " << ui->lineEdit_data->text() << "\n" << \
             "Type=" << ui->lineEdit_type->text() << "\n" << \
             "Terminal=" << isTerminal << "\n" << \
             "Comment=" << ui->lineEdit_comment->text() << "\n" << \
             "Icon=" << iconfile << endl;
    desktopfile.flush();
    desktopfile.close();
}
void MainWindow::is_Selectdir_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this,"选择保存路径",ui->lineEdit_dir->text());
    if(!dir.isEmpty()) ui->lineEdit_dir->setText(dir);
}
