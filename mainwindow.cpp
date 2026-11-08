#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolBar>
#include <QToolButton>
#include <QAction>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QColorDialog>
#include <QFontDialog>
#include <QFont>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QToolBar *toolbar1 = new QToolBar(this);


    //工具栏按键
    QToolButton *newtxt = new QToolButton;
    QToolButton *savetxt = new QToolButton;
    QToolButton *opentxt = new QToolButton;
    QToolButton *fonttxt = new QToolButton;
    toolbar1->addWidget(newtxt);
    toolbar1->addWidget(savetxt);
    toolbar1->addWidget(opentxt);
    toolbar1->addWidget(fonttxt);
    QAction *act1 = new QAction(QIcon(":/1"),"新建");
    QAction *act2 = new QAction(QIcon(":/2"),"保存");
    QAction *act3 = new QAction(QIcon(":/3"),"打开");
    QAction *act4 = new QAction(QIcon(":/4"),"改变字体颜色");
    newtxt->setDefaultAction(act1);
    savetxt->setDefaultAction(act2);
    opentxt->setDefaultAction(act3);
    fonttxt->setDefaultAction(act4);
    newtxt->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    savetxt->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    opentxt->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    fonttxt->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    //文本框
    diary_input_box  = new QTextEdit(this);

    //布局
    QWidget *container_general_layout = new QWidget;
    QVBoxLayout *general_layout = new QVBoxLayout(container_general_layout);
    general_layout->addWidget(diary_input_box);
    this->addToolBar(toolbar1);
    this->setCentralWidget(container_general_layout);

    //链接
    connect(savetxt,&QToolButton::clicked,this,&MainWindow::save_clicked);
    connect(newtxt,&QToolButton::clicked,this,&MainWindow::new_clicked);
    connect(opentxt,&QToolButton::clicked,this,&MainWindow::open_clicked);
    connect(fonttxt,&QToolButton::clicked,this,&MainWindow::font_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//保存槽函数
void MainWindow::save_clicked(){
    QString save_judge = diary_input_box->toPlainText();
    if(save_judge.isEmpty()){
        QMessageBox::warning(this,"警告","当前文件为空文件，无法保存",QMessageBox::Ok);
    }
    else{
        QString filepath = QFileDialog::getSaveFileName(this,"保存","D:\\2026code\\Demo\\notepad\\notepad\\文件保存路径\\新建文本.txt","txt 文件(*.txt)");
        if(filepath.isEmpty()){
            return;
        }

        QFile file (filepath);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::critical(this,"错误","该文件无法打开");
            return;
        }
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << diary_input_box->toPlainText();
        file.close();
    }
}

void MainWindow::new_clicked(){
   if(diary_input_box->document()->isModified() == true){
       int ret = QMessageBox::question(this,
                                       "是否保存？","是否保存文件？",
                                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
       QString current_filepath;
       //点击保存按钮
       if(ret == QMessageBox::Save){
           QString filepath = QFileDialog::getSaveFileName(this,"保存","D:\\2026code\\Demo\\notepad\\notepad\\文件保存路径\\新建文本.txt","txt文件(*.txt");
           if(filepath.isEmpty()){
               return;
           }
           current_filepath = filepath;
           QFile file(filepath);
           if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
               QMessageBox::critical(this,"警告","无法打开文件");
               return;
           }
           QTextStream stream(&file);
           stream.setCodec("UTF-8");
           stream << diary_input_box->toPlainText();//document已经封装好无需再写
           file.close();
           diary_input_box->document()->setModified(false);
       }
       //点击不保存按钮
       else if(ret == QMessageBox::Discard){
           diary_input_box->clear();
           current_filepath = "";
           diary_input_box->document()->setModified(false);
       }
       //点击取消按钮
       else{
           return;
       }
   }
}

void MainWindow::open_clicked(){
    QString current_filepath;
    if(diary_input_box->document()->isModified() == true){
        int ret = QMessageBox::question(this,"保存","请问你是否要保存该文件？",QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(ret == QMessageBox::Save){
            QString filepath = QFileDialog::getSaveFileName(this,
                                                            "保存",
                                                            "D:\\2026code\\Demo\\notepad\\notepad\\文件保存路径\\新建文本.txt",
                                                            "txt文件(*.txt)");
            if(filepath.isEmpty()){
                return;
            }
            current_filepath = filepath;
            QFile file(current_filepath);

            if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
                QMessageBox::critical(this,"警告","无法打开文件");
                return;
            }
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream << diary_input_box->toPlainText();
            file.close();
            diary_input_box->document()->setModified(false);
        }
        else if(ret == QMessageBox::Discard){
            diary_input_box->clear();
            diary_input_box->document()->setModified(false);
            //还要再加打开文件代码
            QString filepath = QFileDialog::getOpenFileName(this,
                                       "打开",
                                       "D:\\2026code\\Demo\\notepad\\notepad\\文件保存路径\\新建文本.txt",
                                       "txt文件(*.txt)");
            if(filepath.isEmpty()){
                return;
            }
            current_filepath = filepath;
            QFile file1(current_filepath);
            if(!file1.open(QIODevice::ReadOnly | QIODevice::Text)){
                QMessageBox::critical(this,"警告","无法打开文件");
                return;
            }

            QTextStream stream_open1(&file1);
            stream_open1.setCodec("UTF-8");
            QString text = stream_open1.readAll();
            diary_input_box->setPlainText(text);
            file1.close();
        }
        else{
            return;
        }
    }

    else{
        QString filepath = QFileDialog::getOpenFileName(this,
                                   "打开",
                                   "D:\\2026code\\Demo\\notepad\\notepad\\文件保存路径\\新建文本.txt",
                                   "txt文件(*.txt)");
        if(filepath.isEmpty()){
            return;
        }
        current_filepath = filepath;
        QFile openfile(current_filepath);
        if(!openfile.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::critical(this,"警告","无法打开文件");
            return;
        }

        QTextStream stream_open(&openfile);
        stream_open.setCodec("UTF-8");
        QString text = stream_open.readAll();
        diary_input_box->setPlainText(text);
        openfile.close();
    }
}

void MainWindow::font_clicked(){
    if(diary_input_box->document()->isEmpty()){
        QMessageBox::information(this,"提示","没有文字可供修改");
        return;
    }
    else{
        bool ok;
        QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
        QFont font = QFontDialog::getFont(&ok,this);
        diary_input_box->setTextColor(color);
        diary_input_box->setCurrentFont(font);

    }
}

