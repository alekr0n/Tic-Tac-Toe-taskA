#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:
    void paintEvent(QPaintEvent *event);
private slots:
    void on_leftButton_clicked();
    void on_rightButton_clicked();
    void on_startButton_clicked();
    void onGameAreaButtonClicked();
    void onComputerSlot();

    void on_aboutButton_clicked();

private:
    Ui::Widget *ui;
    void setInterfaceStyle();
    void changeButtonStatus(int num);
    void configurationTabWidget();
    void addFonts();
    void changeButtonStyle(int row, int colum, QString style);
    void configurationGameAreaButtons();
    void setGameAreaButtonsStyle();
    void start();
    void lockPlayer();
    void chekGameStop();
    void endGame();
    char gameArea[3][3] = {
        {'-','-','-'},
        {'-','-','-'},
        {'-','-','-'}
    };
    char player = 'X'; // X или 0
    int progress = 0;
    bool gameStart = false;
    bool playerLocked = true;
    QTimer *timer;
    bool stop;
    char winner;
};
#endif // WIDGET_H
