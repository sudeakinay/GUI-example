#include "imx8_v1.h"
#include "ui_imx8_v1.h"

const int AMPLITUDE_MIN = 1;                    // min genlik belirtilir
const int AMPLITUDE_MAX = 10;                   // max genlik belirtilir
const int FREQUENCY_MIN = 1;                    // min frekans belirtilir
const int FREQUENCY_MAX = 100;                  // max frekans belirtilir

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // UART ayarlari yapmak icin ilk olarak COMPORT sınıfı olusturulur
    COMPORT = new QSerialPort(this);

    COMPORT ->setPortName("COM6");                                      // 6.Port aktiflestirilir           // BURADAKI GOSTERIM WINDOWS ICIN GECERLI
    COMPORT->setBaudRate(QSerialPort::BaudRate::Baud9600);              // baudrate 9600 olarak belirtilir
    COMPORT->setParity(QSerialPort::Parity::NoParity);                  // parity degeri belirtilir
    COMPORT->setDataBits(QSerialPort::DataBits::Data8);                 // datanin 8bit oldugu belirtilir
    COMPORT->setStopBits(QSerialPort::StopBits::OneStop);               // stop bitleri belirtilir
    COMPORT->setFlowControl(QSerialPort::FlowControl::NoFlowControl);   // flow control belirtilr
    COMPORT->open(QIODevice::ReadWrite);                                // seri portun veri aktarimini nasil yapacagi belirtilir    // READ & WRITE

    // Seri portun dogru acilip acilmadigi kontrol edilir
    if (!COMPORT->open(QIODevice::ReadWrite)) {
        qDebug() << "Cannot open serial port. Error: " << COMPORT->errorString();
    } else {
        qDebug() << "Seri port açıldı!";
    }

    // porttan veri okumak için readyRead sinyali ve kendi olusturdugumuz Read_Data slotu birbirine baglanir
    connect(COMPORT, SIGNAL(readyRead()), this, SLOT(read_data()));

    // genlik icin olusturulan dial butonunun konfigurasyonlari yapilir
    connect(ui->amplitudeDial, &QDial::sliderReleased, this, &Widget::on_amplitudeDial_sliderReleased);
    // min ve max genlik degeri set edilir
    ui->amplitudeDial->setMinimum(AMPLITUDE_MIN);
    ui->amplitudeDial->setMaximum(AMPLITUDE_MAX);

    // baslangıc genllik degeri set edilir        // INTEGER
    amplitude = AMPLITUDE_MIN; // Default genlik degeri minimum deger olarak ayarlanir
    ui->amplitudeDial->setValue(static_cast<int>(amplitude));

    // frekans icin olusturulan dial butonunun konfigurasyonlari yapilir
    connect(ui->frequencyDial, &QDial::sliderReleased, this, &Widget::on_frequencyDial_sliderReleased);
    // min ve max genlik degeri set edilir
    ui->frequencyDial->setMinimum(FREQUENCY_MIN);
    ui->frequencyDial->setMaximum(FREQUENCY_MAX);

    // baslangıc frekans degeri set edilir        // INTEGER
    frequency = FREQUENCY_MIN; // Default frekans degeri minimum deger olarak ayarlanir
    ui->frequencyDial->setValue(static_cast<int>(frequency));

    // genlik ve frekans dial butonları tetiklendigi sinyal, verilerin UART hattina basildigi slota baglanir
    connect(ui->amplitudeDial, &QDial::sliderReleased, this, &Widget::write_to_serial_data);
    connect(ui->frequencyDial, &QDial::sliderReleased, this, &Widget::write_to_serial_data);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::read_data()
{

// x ve y alinacak

    if (COMPORT->isOpen())                                                  // bu slot sadece seri port aciksa aktiflesir
    {
        while (COMPORT->bytesAvailable())                                   // COMPORT'tan veri alindiysa burasi 1 olarak donecek
        {
            dataFromSerialPort += COMPORT->readAll();                       // porttan alinan tüm veri Data_FromSerialPort degiskenine eklenir

            if (dataFromSerialPort.endsWith('\n'))                          // Data_FromSerialPort degiskenindeki veiler \n ile bitiyorsa bu blok calisir
            {                                                               // yani; sürekli gelen mesaj paketleri \n ler ile birbirinden ayirt edilir.

                QStringList dataList = dataFromSerialPort.split(',');       // mesaj paketinde gelen x ve y verileri , ile birbirinden ayrilir.
                if (dataList.size() >= 2)                                   // En az 2 veri gelmiş mi diye kontrol edilir
                {
                    bool xConversionOk, yConversionOk;                      // gelen x ve y verilerinin dogrulugunu kontrol icin kullanilir

                    // gelen dataları double degiskene parslama islemi yapilir
                    // datalar dataList adindaki arraye aktarilir

                    double xValue = dataList[0].toDouble(&xConversionOk);   // QgraphWidget ddouble type kullaniyor
                    double yValue = dataList[1].toDouble(&yConversionOk);   // bu yuzden double type'a cevirdik

                    if (xConversionOk && yConversionOk)
                    {
                        update_graph_with_new_data(xValue, yValue);     // double olarak alinan x ve y degerleri plotlanmak uzere diger slota aktarilir

                        qDebug() << "(DOUBLE) x value: " << xValue << ", y value: " << yValue;
                        // x ve y degerleri ayri ayri ekrana yazdirilir
                    }
                    else
                    {
                        qDebug() << "Geçersiz veri formatı!";
                    }
                }
                else
                {
                    qDebug() << "Geçersiz veri formatı!";
                }

                qDebug() << "Data from Serial Port: " << dataFromSerialPort;
                ui->receivedDataText->setPlainText(dataFromSerialPort);

                // bu mesaj paketiyle isimiz bitti, \n'den sonraki, yani diger pakete gecmeden once datalari tuttugumuz degisken temizlenir
                dataFromSerialPort.clear();
                break;
            }
        }
    }
}

void Widget::update_graph_with_new_data(double xValue, double yValue)
{
    // yeni veri grafige eklenir
    xData.push_back(xValue);
    yData.push_back(yValue);

    // vektor boyutlari kontrol edilir ve eger cok fazlaysa ilk veriler silinir
    if (xData.size() > 500)
    {
        xData.remove(0);
        yData.remove(0);
    }

    // grafik güncellenir
    make_plot();
}

void Widget::make_plot()
{
    // UART'tan gelen verileri direkt olarak xData ve yData degiskenlerine aktararak grafiği güncellemek icin kullanilir
    ui->graphWidget->clearGraphs();
    ui->graphWidget->addGraph();                         // grafik olusturulur

    ui->graphWidget->graph(0)->setData(xData, yData);    // x ve y verileri grafikte direkt olarak cizdirilir

    QPen graphPen;
    graphPen.setColor(QColor(255, 107, 243));           // grafikteki cizginin rengi set edilir
    ui->graphWidget->graph(0)->setPen(graphPen);

    ui->graphWidget->xAxis->setLabel("TIME");            // x ve y axislerinin basliklari set edilir
    ui->graphWidget->yAxis->setLabel("VOLTAGE");

    // min ve max x - y degerleri hesaplanir
    double minX = *std::min_element(xData.constBegin(), xData.constEnd());
    double maxX = *std::max_element(xData.constBegin(), xData.constEnd());
    double minY = *std::min_element(yData.constBegin(), yData.constEnd());
    double maxY = *std::max_element(yData.constBegin(), yData.constEnd());

    // grafigin kenarlarindaki bosluklar hesaplanir
    double xPadding = (maxX - minX) * 0.1;              // x ekseninde veri araligi farkının % 10'u kadar uzaklasilarak xPadding degeri hesaplanir
    double yPadding = (maxY - minY) * 0.1;              // y ekseninde veri araligi farkının % 10'u kadar uzaklasilarak yPadding degeri hesaplanir

    // axis araliklari grafigin yanina eklenecek bosluklar da hesaba katilarak tekrar set edilir.
    ui->graphWidget->xAxis->setRange(minX - xPadding, maxX + xPadding);
    ui->graphWidget->yAxis->setRange(minY - yPadding, maxY + yPadding);

    // olusturulan grafigin arka planindaki kare sayisini arttirir
    // bu adim grafigin okunabilirligini arttirir
    ui->graphWidget->xAxis->grid()->setSubGridVisible(true);
    ui->graphWidget->xAxis->grid()->setSubGridPen(QPen(Qt::gray, 1, Qt::DotLine));
    ui->graphWidget->yAxis->grid()->setSubGridVisible(true);
    ui->graphWidget->yAxis->grid()->setSubGridPen(QPen(Qt::gray, 1, Qt::DotLine));

    // grafik icin ayrilan alanin dort kenarına 40 piksel bosluk eklemek icin kullanilir
    ui->graphWidget->plotLayout()->setMargins(QMargins(40, 40, 40, 40));     // Set margins to 5 pixels
    ui->graphWidget->axisRect()->setAutoMargins(QCP::MarginSide::msNone);    // otomatik kenar boslugu olusturmayi durdurur
    ui->graphWidget->axisRect()->setMargins(QMargins(0, 0, 0, 0));           // eksenlerin cizim alaninin kenarlarina tam olarak yerlestirir
    // yani kenarlara sifir piksel bosluk eklenmesini saglar

    ui->graphWidget->axisRect()->setupFullAxesBox(); // cizim alaninin tamamini kullanarak grafgi gorsellestirmek icin kullanilir

    ui->graphWidget->replot();                       // guncellenmis verilere göre grafigin tekrar cizilmesini saglar
}

void Widget::write_to_serial_data()
{
    qDebug() << "****************";
    int freqValue = ui->frequencyDial->value();               // frequency dial butonunun verisi alinir
    QString dataToSend = QString::number(freqValue) + ",";    // paket yapisina uymak icin frekans verisinden sonra ',' eklenir
    COMPORT->write(dataToSend.toLatin1());                    // UART hattina basmak icin veri uygun forma getirilir -> string type
    qDebug() << "FREKANS:" << dataToSend;

    int amplitudeValue = ui->amplitudeDial->value();          // amplitude dial butonunun verisi alinir
    dataToSend = QString::number(amplitudeValue) + "\n";    // paket yapisina uymak icin genlik verisinden sonra '\n' eklenir
    COMPORT->write(dataToSend.toLatin1());                    // UART hattina basmak icin veri uygun forma getirilir -> string type
    qDebug() << "GENLIK : " << dataToSend;
    qDebug() << "****************";
}

void Widget::on_amplitudeDial_sliderReleased()
{
    amplitudeValue = ui->amplitudeDial->value();                                 // QDial'in değeri alinir
    ui->amplitudeLabel->setText(QString("Amplitude: %1").arg(amplitudeValue));   // Alinan deger ekrana bastirilir
}


void Widget::on_frequencyDial_sliderReleased()
{
    frequencyValue = ui->frequencyDial->value();                                 // QDial'in değeri alinir
    ui->frequencyLabel->setText(QString("Frequency: %1").arg(frequencyValue));   // Alinan deger ekrana bastirilir
}


