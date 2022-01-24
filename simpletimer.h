#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SimpleTimer.h"
#include "qdatetime.h"
#include "qtimer.h"
#include <QTime>
#include "qpixmap.h"
#include <qicon.h>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>

#include <QDesktopServices>
#include <QUrl>

#include <QDebug>

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

#include "comlilerport.h"

#include <QTextCodec>

class SimpleTimer : public QMainWindow
{
	Q_OBJECT

public:
	SimpleTimer(QWidget *parent = Q_NULLPTR);

    //定时器相关定义
    QTimer *myTime;
    void Timer0_Init();//定时器初始化
    QTimer *newTime;
    void Timer1_Init();//定时器初始化

    CompilerPort compilerport;
    QStringList staticList ;

    QString FastTime, FastTimeAll, NowTime;
    QChar round_num;

    void handleSerialError(QSerialPort::SerialPortError error);
private slots:
	void updateTime();
	void on_start_clicked();
	void on_stop_clicked();
    void on_puase_clicked();

    void on_pushButton_2_clicked();

    void Timer0_Task();//定时器任务
    void Timer1_Task();//定时器任务
    void ButtonOpenPort(bool);//打开端口功能
    void ButtonSendPort(bool);//发送文本功能
    void ReciveDate();//接收信息
    void ButtonClear();//清除文本框
    void ButtonStopShow();//暂停显示
    void on_textBrowser_textChanged();//文本框自动下拉
    void AutoClear();//自动清除

    void on_lineEdit_2_editingFinished();

private:
	Ui::SimpleTimerClass ui;
    QTimer* timer; //定时器，每秒更新时间
	QTime* TimeRecord; //记录时间
	QDateTime* TimeWrite;  //准备写如到文件里面的时间

	bool isStart;      //记录是否已经开始计时
//	QIcon* startIcon;  //一些图标
//	QIcon* pauseIcon;
//	QIcon* stopIcon;
	void Write(QString write_str);

    void systemInit();//初始化
    void Read_Serial_Connect_Success(); //成功检测到串口
    void Serial_Connect_Success_Label_Text();//串口自动检测连接成功以后的信息
    void Serial_Connect_NG_Label_Text();//串口自动检测连接失败以后的信息
    QStringList scanSerial();//向box里写COMx

    /*---------------variable---------------*/
    QSerialPort globlePort;//串口对象
    QStringList serialStrList;//为实现scanSerial()而定义的，串口字符串链
};
