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

    //��ʱ����ض���
    QTimer *myTime;
    void Timer0_Init();//��ʱ����ʼ��
    QTimer *newTime;
    void Timer1_Init();//��ʱ����ʼ��

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

    void Timer0_Task();//��ʱ������
    void Timer1_Task();//��ʱ������
    void ButtonOpenPort(bool);//�򿪶˿ڹ���
    void ButtonSendPort(bool);//�����ı�����
    void ReciveDate();//������Ϣ
    void ButtonClear();//����ı���
    void ButtonStopShow();//��ͣ��ʾ
    void on_textBrowser_textChanged();//�ı����Զ�����
    void AutoClear();//�Զ����

    void on_lineEdit_2_editingFinished();

private:
	Ui::SimpleTimerClass ui;
    QTimer* timer; //��ʱ����ÿ�����ʱ��
	QTime* TimeRecord; //��¼ʱ��
	QDateTime* TimeWrite;  //׼��д�絽�ļ������ʱ��

	bool isStart;      //��¼�Ƿ��Ѿ���ʼ��ʱ
//	QIcon* startIcon;  //һЩͼ��
//	QIcon* pauseIcon;
//	QIcon* stopIcon;
	void Write(QString write_str);

    void systemInit();//��ʼ��
    void Read_Serial_Connect_Success(); //�ɹ���⵽����
    void Serial_Connect_Success_Label_Text();//�����Զ�������ӳɹ��Ժ����Ϣ
    void Serial_Connect_NG_Label_Text();//�����Զ��������ʧ���Ժ����Ϣ
    QStringList scanSerial();//��box��дCOMx

    /*---------------variable---------------*/
    QSerialPort globlePort;//���ڶ���
    QStringList serialStrList;//Ϊʵ��scanSerial()������ģ������ַ�����
};
