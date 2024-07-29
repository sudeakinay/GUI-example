#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include "QString"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    int frequencyValue;
    int amplitudeValue;
    int amplitude, frequency;
    QString dataFromSerialPort;
    QVector<double> xData, yData;

private slots:
    void on_amplitudeDial_sliderReleased();

    void on_frequencyDial_sliderReleased();

    void read_data();

    void update_graph_with_new_data(double xValue, double yValue);

    void make_plot();

    void write_to_serial_data();

private:
    Ui::Widget *ui;
    QSerialPort *COMPORT;
};
#endif // WIDGET_H
