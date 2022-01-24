#include "simpletimer.h"

//��ʼ������
SimpleTimer::SimpleTimer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	isStart = false;    //��û�п�ʼ��ʱ
	timer = new QTimer; //��ʼ����ʱ��
    timer->setTimerType(Qt::PreciseTimer);
    TimeRecord = new QTime(0, 0, 0, 0); //��ʼ��ʱ��
	TimeWrite = new QDateTime();

//    timer->setInterval(1);
	ui.Timer->setDigitCount(8);
	ui.Timer->setSegmentStyle(QLCDNumber::Flat);
    ui.Timer->display(TimeRecord->toString("mm:ss.zzz"));

    NowTime = "00:00";
    FastTime = "60:00";
    FastTimeAll = "60:00";
    round_num = '0';

    ui.label_FastTime->setText(FastTime);
    ui.label_FastTimeAll->setText(FastTimeAll);
    ui.label_roundnum->setText(round_num);

	connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    connect(ui.Start, SIGNAL(clicked()), this, SLOT(on_puase_clicked()));
	connect(ui.Stop, SIGNAL(clicked()), this, SLOT(on_stop_clicked()));

//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));


    Timer0_Init();
    Timer1_Init();
    systemInit();
}
void SimpleTimer::updateTime()
{
    *TimeRecord = TimeRecord->addMSecs(10);
//    *TimeRecord = TimeRecord->addSecs(1);
    NowTime = TimeRecord->toString("mm:ss");
    ui.Timer->display(TimeRecord->toString("mm:ss.zzz"));
}

void SimpleTimer::on_start_clicked()
{
	if (!isStart)
	{
        timer->start(10);
		
		Write(TimeWrite->currentDateTime().toString("MM-dd hh-mm"));//����ʼʱ��д�뵽�ļ�
        isStart = 1;
	}
	else
	{
//		timer->stop();
	}
//	isStart = !isStart;
}

void SimpleTimer::on_puase_clicked()
{
    if (!isStart)
    {
//        timer->start(1);

        Write(TimeWrite->currentDateTime().toString("MM-dd hh-mm"));//����ʼʱ��д�뵽�ļ�
    }
    else
    {
        timer->stop();
        isStart = 0;
        QMessageBox * mBox = new QMessageBox(this);
        mBox->setWindowTitle(QString::fromLocal8Bit("��ʾ"));
        mBox->setText(QString::fromLocal8Bit("�Ƿ����ɼ���"));
        mBox->setStandardButtons(QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel);
        mBox->setButtonText(QMessageBox::StandardButton::Ok, QString::fromLocal8Bit("��"));
        mBox->setButtonText(QMessageBox::StandardButton::Cancel, QString::fromLocal8Bit("��"));
        mBox->exec();
        QMessageBox::StandardButton ret = mBox->standardButton(mBox->clickedButton());

        float time_error;
        QString FastTimeAll_min = FastTimeAll.mid(0,2);
        QString FastTimeAll_ms = FastTimeAll.mid(3);
        QString FastTime_min = FastTime.mid(0,2);
        QString FastTime_ms = FastTime.mid(3);
        QString NowTime_min = NowTime.mid(0,2);
        QString NowTime_ms = NowTime.mid(3);
        int fmin = FastTime_min.toInt();
        int fms = FastTime_ms.toInt();
        int nmin = NowTime_min.toInt();
        int nms = NowTime_ms.toInt();
        int fmina = FastTimeAll_min.toInt();
        int fmsa = FastTimeAll_ms.toInt();

        QString date = "0\r\n";
        QByteArray array = date.toLatin1();
        switch (ret)
        {
            case QMessageBox::NoButton:
                qDebug() << QString::fromLocal8Bit("û��ѡ��");
                break;
            case QMessageBox::Ok:

                time_error = (nmin-fmin)*60+(nms-fms);
                if(time_error < 0)
                {
                    FastTime = NowTime;
                    ui.label_FastTime->setText(FastTime);
                    FastTime_min = FastTime.mid(0,2);
                    FastTime_ms = FastTime.mid(3);
                    fmin = FastTime_min.toInt();
                    fms = FastTime_ms.toInt();
                    time_error = (fmin-fmina)*60+(fms-fmsa);
                    qDebug() << QString::fromLocal8Bit("ȫ��") << fmin << fms << fmina << fmsa << time_error;
                    if(time_error < 0)
                    {
                        FastTimeAll = FastTime;
                        ui.label_FastTimeAll->setText(FastTimeAll);
                    }
                }
//                FastTimeu = FastTimeQ.secsTo(NowTimeQ);
//                NowTimeu = NowTimeQ.secsTo(FastTimeQ);
//                FastTimeu = FastTimeQ.toTime_t();
//                NowTimeu = NowTimeQ.toTime_t();
//                time_error = (NowTime[0] - 48 + NowTime[1] - 48);
                qDebug() << QString::fromLocal8Bit("ȷ��") << nmin << nms << fmin << fms << time_error;
//                qDebug() << FastTime_min << "next" << NowTime_ms;
                break;
            case QMessageBox::Cancel:
                round_num = '0';
                ui.label_roundnum->setText(round_num);

                globlePort.write(array);
                qDebug() << QString::fromLocal8Bit("ȡ��");
                break;
            default:
                break;
        }
    }
//	isStart = !isStart;
}





void SimpleTimer::on_stop_clicked()
{
	timer->stop();    //��ʱ��ֹͣ
	Write(TimeWrite->currentDateTime().toString("hh-mm "));//������ʱ��д�뵽�ļ�
	isStart = false;
    TimeRecord->setHMS(0, 0, 0, 0); //ʱ�临λΪ0
    ui.Timer->display(TimeRecord->toString("mm:ss.zzz"));

    round_num = '0';
    ui.label_roundnum->setText(round_num);

    QString date = "0\r\n";
    QByteArray array = date.toLatin1();
    globlePort.write(array);
	
}
void SimpleTimer::Write(QString write_str)
{
	
    QString fileName = "./record.txt";
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
	{
		QMessageBox::warning(this, "sdf", "can't open", QMessageBox::Yes);
	}
	QTextStream stream(&file);
	if(!isStart)
		stream <<  write_str+ "-";
	else
	{
		stream << write_str;
	}

	file.close();
}



void SimpleTimer::on_pushButton_2_clicked()
{
    QUrl url("https://au.njust.edu.cn/");
    QDesktopServices::openUrl(url);
}


/*----------------------------------------------------------
 *          function
 *----------------------------------------------------------*/
void SimpleTimer::systemInit()
{
    globlePort.setParity(QSerialPort::NoParity);
    globlePort.setDataBits(QSerialPort::Data8);
    globlePort.setStopBits(QSerialPort::OneStop);
    //�˿��趨
    //�źŰ󶨵���
    connect(ui.openButton,&QPushButton::clicked,this,&SimpleTimer::ButtonOpenPort);//�򿪴����ź�
//    connect(ui->sendTxtButton,&QPushButton::clicked,this,&SimpleTimer::ButtonSendPort);//�����ı��ź�
//    connect(ui->pushButton_3,&QPushButton::clicked,this,&SimpleTimer::ButtonStopShow);//��ͣ��ʾ�ı��ź�
    connect(&globlePort,&QSerialPort::readyRead,this,&SimpleTimer::ReciveDate);//���ڴ򿪾ͽ�����Ϣ
//    connect(ui->pushButton_2,&QPushButton::clicked,this,&SimpleTimer::ButtonClear);//����ı����ź�
//    connect(ui->checkBox,&QCheckBox::stateChanged,this,&SimpleTimer::AutoClear);//�Զ����

    connect(&globlePort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),  this, &SimpleTimer::handleSerialError);
    //connect(ui->checkBox_2,&QCheckBox::stateChanged,this,&Serial::ButtonClear);//16�����ź�
}
void SimpleTimer::handleSerialError(QSerialPort::SerialPortError error)
{
    qDebug()<<"��������";
    if (error == QSerialPort::ResourceError) {
        //QMessageBox::critical(this, tr("Error"), "���������жϣ������Ƿ���ȷ���ӣ�");
        //my_SerialPort->close();
        //ui->label_status->setText("δ����");
        //m_pTimer->start(1000);

    }
}
void SimpleTimer::on_textBrowser_textChanged()
{
   //���ı��򵽵׵�ʱ���Զ��»�
//   ui->textBrowser->moveCursor(QTextCursor::End);
}
/*--------------------------
 *      �ֶ�����ı�
 * ------------------------*/
void SimpleTimer::ButtonClear()

{
//   ui->textBrowser->clear();
}
/*--------------------------
 *      ��ͣ����ʼ��ʾ�ı�
 * ------------------------*/
void SimpleTimer::ButtonStopShow()
{
//    if(ui->pushButton_3->text() == QString("��ͣ��ʾ"))
//    {
//        ui->pushButton_3->setText(QString("��ʼ��ʾ"));
//    }
//    else if(ui->pushButton_3->text() == QString("��ʼ��ʾ"))
//    {
//        ui->pushButton_3->setText(QString("��ͣ��ʾ"));
//    }
}
/*--------------------------
 *      �Զ�����
 * ------------------------*/
void SimpleTimer::AutoClear()
{
//    if(ui->checkBox->isChecked()==true)
//    {
//        newTime->start();

//    }
//    else if(ui->checkBox->isChecked()==false)
//    {
//        newTime->stop();
//    }
}


void SimpleTimer::Timer0_Init()
{
    myTime = new QTimer();
    myTime->stop();
    myTime->setInterval(1000);//1ms��ʱ���������ڵ�Ƭ�����жϺ���
    connect(myTime,SIGNAL(timeout()),this,SLOT(Timer0_Task()));
    //������1000ʱִ��Timer0_Task
    myTime->start();
}

void SimpleTimer::Timer0_Task()
{
    Read_Serial_Connect_Success();
}
void SimpleTimer::Timer1_Init()
{
    newTime = new QTimer();
    newTime->stop();
    newTime->setInterval(4000);//1ms��ʱ���������ڵ�Ƭ�����жϺ���
    connect(newTime,SIGNAL(timeout()),this,SLOT(Timer1_Task()));
    //������100ʱִ��Timer1_Task
}
void SimpleTimer::Timer1_Task()
{
    qDebug()<<"��ʱ��1";
//    ui->textBrowser->clear();
    /*
    qDebug()<<"��ʱ��1";
    int z = 0;
    for(int i = 0; i < ui->portBox->count();i++)
    {
        for(int j = 0; j < ui->portBox->count();j++)
        {
            qDebug()<<"portname:"<<globlePort.portName();
            if(ui->portBox->itemText(j) == globlePort.portName())
            {
                z++;
            }

            //������ĺ�����Ĳ�һ��
        }
        if(z == 0)
        {
            ui->portBox->removeItem(i);
            //staticList[i].
        }
        z = 0;
    }
    */

}


/*----------------------------------------------------------
 *          COM�Զ�ʶ��
 *----------------------------------------------------------*/
static int Read_Serial_Port_Info()
{

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
//        qDebug() << "Name : " << info.portName();
//        qDebug() << "Description :"<<info.description();
        if(info.portName() != NULL)
            return true;
        else
            return false;
    }
    return false;
}
void SimpleTimer::Read_Serial_Connect_Success()
{
//    qDebug() << "Ѱ�Ҵ��� ����ֵ��"<<Read_Serial_Port_Info() << endl;
    if(Read_Serial_Port_Info())
    {
        Serial_Connect_Success_Label_Text();
    }
    else
        Serial_Connect_NG_Label_Text();
}
void SimpleTimer::Serial_Connect_Success_Label_Text()
{
    serialStrList.clear();
    serialStrList = scanSerial();
//    for(int z = 0;z < serialStrList.size();z++)
//        qDebug()<<"����1Ϊ"<<serialStrList[z];
//    qDebug()<<"size:"<<serialStrList.size();
    //�����������COM�ڣ���scanSerial()���Ϊ��"COMx",��������ǰ���ַ��������������ǰ�漴��
    for(int i = 0;i<staticList.size();i++)
    {
        serialStrList.insert(i, staticList[i]);
    }
    if(staticList != serialStrList)
    {
        for(int q = 0;q < serialStrList.size();q++)
            qDebug()<<"����2Ϊ"<<serialStrList[q];
        staticList = serialStrList;
        staticList = compilerport.compiler_port(staticList,staticList.size());
        for(int c = 0;c < staticList.size();c++)
            qDebug()<<"����3Ϊ"<<staticList[c];
        ui.portBox->clear();
        for (int i=0; i<staticList.size(); i++)
        {
            if(staticList[i] != "")
            ui.portBox->addItem(staticList[i]); // ��comboBox����Ӵ��ں�
        }
    }
    QFont font ( "������", 12, 70); //��һ�����������壨΢���źڣ����ڶ����Ǵ�С���������ǼӴ֣�Ȩ����75��
    ui.label_7->setFont(font);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::darkGreen);
    ui.label_7->setPalette(pe);
    ui.label_7->setText(QString::fromLocal8Bit("���豸"));
}
void SimpleTimer::Serial_Connect_NG_Label_Text()
{
    ui.portBox->clear();//�����д��ڶ��γ������
    staticList.clear();
    QFont font ( "�ȴ���", 12, 70); //��һ�����������壨΢���źڣ����ڶ����Ǵ�С���������ǼӴ֣�Ȩ����75��
    ui.label_7->setFont(font);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui.label_7->setPalette(pe);
    ui.label_7->setText(QString::fromLocal8Bit("�ȴ���"));
}
/*----------------------------------------------------------
 *          �ж������Ƿ��ظ�
 *----------------------------------------------------------*/
QStringList SimpleTimer::scanSerial()
{
    //��ȡ������Ϣ
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        int judge = 0;//��־λ��
        //1.����portBox->count()ͳ��portBox���ǩ������
        //2.����for(i=0;count;)ѭ�����Դ��ж���ǩ����û���ظ�
        //3.��û���ظ��� ��־λΪ0������Ϊ1.ֻ��Ϊ0�Ż�append()
        globlePort.setPort(info);
        for(int i=0;i<ui.portBox->count();i++)
        {
            if(globlePort.portName() == ui.portBox->itemText(i))
            {
                judge++;
            }
//            qDebug()<<judge<<"key����";
        }
        if(judge == 0)
        {
            serialStrList.append(globlePort.portName());
        }
    }
    return serialStrList;
}
/*----------------------------------------------------------
 *          slots
 *----------------------------------------------------------*/
void SimpleTimer::ButtonOpenPort(bool)
{
//    qDebug()<"open button";
    if(ui.openButton->text() == QString::fromLocal8Bit("�򿪴���"))  //����δ��
        {
            //���ö˿ں�
            globlePort.setPortName(ui.portBox->currentText());
            //���ò�����
            globlePort.setBaudRate(115200);
            //��������λ
            globlePort.setDataBits(QSerialPort::Data8);
            //����ֹͣλ
            globlePort.setStopBits(QSerialPort::OneStop);
            //����У�鷽ʽ

            globlePort.setParity(QSerialPort::NoParity);

            //����������ģʽ
            globlePort.setFlowControl(QSerialPort::NoFlowControl);
            //�򿪴��ڳ��ִ���
            if(globlePort.open(QIODevice::ReadWrite)==false)
            {
                qDebug()<<"��������"<<ui.portBox->currentText();
                for(int i = 0;i < ui.portBox->count() ;i++)
                {
                    if( ui.portBox->itemText(i) == ui.portBox->currentText())
                    {
                        ui.portBox->removeItem(i);
                        staticList[i].remove(0,4);
                    }

                }

                QMessageBox::warning(NULL , "��ʾ", "���ڴ�ʧ�ܣ�");
                return;
            }
            // ʧ�ܴ������ÿؼ�
            ui.portBox->setEnabled(false);
            //ui->search_Button->setEnabled(false);
            //�������ڿ��ư�ť��������ʾ
            ui.openButton->setText(QString::fromLocal8Bit("�رմ���"));
        }
        else       //�����Ѿ���
        {
            globlePort.close();
            // ʹ�ܴ������ÿؼ�
            ui.portBox->setEnabled(true);
            //ui->search_Button->setEnabled(true);
            //�������ڿ��ư�ť��������ʾ
            ui.openButton->setText(QString::fromLocal8Bit("�򿪴���"));
         }
}
void SimpleTimer::ButtonSendPort(bool)
{
//    QString date = ui->textEdit->toPlainText();
//    QByteArray array = date.toLatin1();
//    globlePort.write(array);
}
/*----------------------------------------------------------
 *          recive Date
 *----------------------------------------------------------*/
//��ȡ���յ�������
void SimpleTimer::ReciveDate()
{
    static QByteArray Serial_buff;//����static������ᱻ����
    Serial_buff += globlePort.readAll();
    if(Serial_buff.endsWith("\n"))
    {
        qDebug()<<Serial_buff;
        if(Serial_buff[0] == 'A')
        {
            on_start_clicked();
        }
        else if(Serial_buff[0] == 'B')
        {
            round_num = '1';
            ui.label_roundnum->setText(round_num);
        }
        else if(Serial_buff[0] == 'C')
        {
            round_num = '2';
            ui.label_roundnum->setText(round_num);
            on_puase_clicked();
            round_num = '0';
            ui.label_roundnum->setText(round_num);
        }
        Serial_buff.clear();//��������
    }
}

void SimpleTimer::on_lineEdit_2_editingFinished()
{
    FastTime = "60:00";
    ui.label_FastTime->setText(FastTime);
    round_num = '0';
    ui.label_roundnum->setText(round_num);
    QString date = "0\r\n";
    QByteArray array = date.toLatin1();
    globlePort.write(array);

//    timer->stop();    //��ʱ��ֹͣ
//    isStart = false;
//    TimeRecord->setHMS(0, 0, 0, 0); //ʱ�临λΪ0
//    ui.Timer->display(TimeRecord->toString("mm:ss.zzz"));
}

