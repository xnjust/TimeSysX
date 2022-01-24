#include "simpletimer.h"

//初始化函数
SimpleTimer::SimpleTimer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	isStart = false;    //还没有开始计时
	timer = new QTimer; //视始化定时器
    timer->setTimerType(Qt::PreciseTimer);
    TimeRecord = new QTime(0, 0, 0, 0); //初始化时间
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
		
		Write(TimeWrite->currentDateTime().toString("MM-dd hh-mm"));//将开始时间写入到文件
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

        Write(TimeWrite->currentDateTime().toString("MM-dd hh-mm"));//将开始时间写入到文件
    }
    else
    {
        timer->stop();
        isStart = 0;
        QMessageBox * mBox = new QMessageBox(this);
        mBox->setWindowTitle(QString::fromLocal8Bit("提示"));
        mBox->setText(QString::fromLocal8Bit("是否计入成绩？"));
        mBox->setStandardButtons(QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel);
        mBox->setButtonText(QMessageBox::StandardButton::Ok, QString::fromLocal8Bit("是"));
        mBox->setButtonText(QMessageBox::StandardButton::Cancel, QString::fromLocal8Bit("否"));
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
                qDebug() << QString::fromLocal8Bit("没有选择");
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
                    qDebug() << QString::fromLocal8Bit("全场") << fmin << fms << fmina << fmsa << time_error;
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
                qDebug() << QString::fromLocal8Bit("确定") << nmin << nms << fmin << fms << time_error;
//                qDebug() << FastTime_min << "next" << NowTime_ms;
                break;
            case QMessageBox::Cancel:
                round_num = '0';
                ui.label_roundnum->setText(round_num);

                globlePort.write(array);
                qDebug() << QString::fromLocal8Bit("取消");
                break;
            default:
                break;
        }
    }
//	isStart = !isStart;
}





void SimpleTimer::on_stop_clicked()
{
	timer->stop();    //计时器停止
	Write(TimeWrite->currentDateTime().toString("hh-mm "));//将结束时间写入到文件
	isStart = false;
    TimeRecord->setHMS(0, 0, 0, 0); //时间复位为0
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
    //端口设定
    //信号绑定到槽
    connect(ui.openButton,&QPushButton::clicked,this,&SimpleTimer::ButtonOpenPort);//打开串口信号
//    connect(ui->sendTxtButton,&QPushButton::clicked,this,&SimpleTimer::ButtonSendPort);//发送文本信号
//    connect(ui->pushButton_3,&QPushButton::clicked,this,&SimpleTimer::ButtonStopShow);//暂停显示文本信号
    connect(&globlePort,&QSerialPort::readyRead,this,&SimpleTimer::ReciveDate);//串口打开就接收消息
//    connect(ui->pushButton_2,&QPushButton::clicked,this,&SimpleTimer::ButtonClear);//清空文本框信号
//    connect(ui->checkBox,&QCheckBox::stateChanged,this,&SimpleTimer::AutoClear);//自动清除

    connect(&globlePort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),  this, &SimpleTimer::handleSerialError);
    //connect(ui->checkBox_2,&QCheckBox::stateChanged,this,&Serial::ButtonClear);//16进制信号
}
void SimpleTimer::handleSerialError(QSerialPort::SerialPortError error)
{
    qDebug()<<"发生错误";
    if (error == QSerialPort::ResourceError) {
        //QMessageBox::critical(this, tr("Error"), "串口连接中断，请检查是否正确连接！");
        //my_SerialPort->close();
        //ui->label_status->setText("未连接");
        //m_pTimer->start(1000);

    }
}
void SimpleTimer::on_textBrowser_textChanged()
{
   //当文本框到底的时候自动下滑
//   ui->textBrowser->moveCursor(QTextCursor::End);
}
/*--------------------------
 *      手动清除文本
 * ------------------------*/
void SimpleTimer::ButtonClear()

{
//   ui->textBrowser->clear();
}
/*--------------------------
 *      暂停、开始显示文本
 * ------------------------*/
void SimpleTimer::ButtonStopShow()
{
//    if(ui->pushButton_3->text() == QString("暂停显示"))
//    {
//        ui->pushButton_3->setText(QString("开始显示"));
//    }
//    else if(ui->pushButton_3->text() == QString("开始显示"))
//    {
//        ui->pushButton_3->setText(QString("暂停显示"));
//    }
}
/*--------------------------
 *      自动清理
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
    myTime->setInterval(1000);//1ms定时器，类似于单片机的中断函数
    connect(myTime,SIGNAL(timeout()),this,SLOT(Timer0_Task()));
    //当记满1000时执行Timer0_Task
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
    newTime->setInterval(4000);//1ms定时器，类似于单片机的中断函数
    connect(newTime,SIGNAL(timeout()),this,SLOT(Timer1_Task()));
    //当记满100时执行Timer1_Task
}
void SimpleTimer::Timer1_Task()
{
    qDebug()<<"定时器1";
//    ui->textBrowser->clear();
    /*
    qDebug()<<"定时器1";
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

            //盒子里的和链表的不一样
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
 *          COM自动识别
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
//    qDebug() << "寻找串口 返回值："<<Read_Serial_Port_Info() << endl;
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
//        qDebug()<<"链表1为"<<serialStrList[z];
//    qDebug()<<"size:"<<serialStrList.size();
    //如果有新增的COM口，则scanSerial()会变为："COMx",这样将以前的字符串添加在新增的前面即可
    for(int i = 0;i<staticList.size();i++)
    {
        serialStrList.insert(i, staticList[i]);
    }
    if(staticList != serialStrList)
    {
        for(int q = 0;q < serialStrList.size();q++)
            qDebug()<<"链表2为"<<serialStrList[q];
        staticList = serialStrList;
        staticList = compilerport.compiler_port(staticList,staticList.size());
        for(int c = 0;c < staticList.size();c++)
            qDebug()<<"链表3为"<<staticList[c];
        ui.portBox->clear();
        for (int i=0; i<staticList.size(); i++)
        {
            if(staticList[i] != "")
            ui.portBox->addItem(staticList[i]); // 在comboBox那添加串口号
        }
    }
    QFont font ( "已连接", 12, 70); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    ui.label_7->setFont(font);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::darkGreen);
    ui.label_7->setPalette(pe);
    ui.label_7->setText(QString::fromLocal8Bit("有设备"));
}
void SimpleTimer::Serial_Connect_NG_Label_Text()
{
    ui.portBox->clear();//当所有串口都拔出是清空
    staticList.clear();
    QFont font ( "等待中", 12, 70); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    ui.label_7->setFont(font);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui.label_7->setPalette(pe);
    ui.label_7->setText(QString::fromLocal8Bit("等待中"));
}
/*----------------------------------------------------------
 *          判定串口是否重复
 *----------------------------------------------------------*/
QStringList SimpleTimer::scanSerial()
{
    //读取串口信息
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        int judge = 0;//标志位；
        //1.利用portBox->count()统计portBox里标签的数量
        //2.利用for(i=0;count;)循环，以此判定标签里有没有重复
        //3.若没有重复则 标志位为0，否则为1.只有为0才会append()
        globlePort.setPort(info);
        for(int i=0;i<ui.portBox->count();i++)
        {
            if(globlePort.portName() == ui.portBox->itemText(i))
            {
                judge++;
            }
//            qDebug()<<judge<<"key测试";
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
    if(ui.openButton->text() == QString::fromLocal8Bit("打开串口"))  //串口未打开
        {
            //设置端口号
            globlePort.setPortName(ui.portBox->currentText());
            //设置波特率
            globlePort.setBaudRate(115200);
            //设置数据位
            globlePort.setDataBits(QSerialPort::Data8);
            //设置停止位
            globlePort.setStopBits(QSerialPort::OneStop);
            //设置校验方式

            globlePort.setParity(QSerialPort::NoParity);

            //设置流控制模式
            globlePort.setFlowControl(QSerialPort::NoFlowControl);
            //打开串口出现错误
            if(globlePort.open(QIODevice::ReadWrite)==false)
            {
                qDebug()<<"出现问题"<<ui.portBox->currentText();
                for(int i = 0;i < ui.portBox->count() ;i++)
                {
                    if( ui.portBox->itemText(i) == ui.portBox->currentText())
                    {
                        ui.portBox->removeItem(i);
                        staticList[i].remove(0,4);
                    }

                }

                QMessageBox::warning(NULL , "提示", "串口打开失败！");
                return;
            }
            // 失能串口设置控件
            ui.portBox->setEnabled(false);
            //ui->search_Button->setEnabled(false);
            //调整串口控制按钮的文字提示
            ui.openButton->setText(QString::fromLocal8Bit("关闭串口"));
        }
        else       //串口已经打开
        {
            globlePort.close();
            // 使能串口设置控件
            ui.portBox->setEnabled(true);
            //ui->search_Button->setEnabled(true);
            //调整串口控制按钮的文字提示
            ui.openButton->setText(QString::fromLocal8Bit("打开串口"));
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
//读取接收到的数据
void SimpleTimer::ReciveDate()
{
    static QByteArray Serial_buff;//定义static，否则会被清理
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
        Serial_buff.clear();//数据清理
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

//    timer->stop();    //计时器停止
//    isStart = false;
//    TimeRecord->setHMS(0, 0, 0, 0); //时间复位为0
//    ui.Timer->display(TimeRecord->toString("mm:ss.zzz"));
}

