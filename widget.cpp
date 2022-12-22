#include "widget.h"
#include "ui_widget.h"
#include <QTabBar>
#include "stylehelper.h"
#include <QStyleOption>
#include <QDebug>
#include <QFontDatabase>
#include <QGridLayout>
#include <time.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    configurationTabWidget();
    addFonts();
    setInterfaceStyle();
    configurationGameAreaButtons();
    //timer = new QTimer(this);
    //connect(timer, &QTimer::timeout, this, &Widget::onComputerSlot);
}

Widget::~Widget()
{
    delete ui;
}

/* Применяем к Widget правила QSS  */
void Widget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

/* Назначаем правила QSS для элементов интерфейса  */
void Widget::setInterfaceStyle()
{
    this->setStyleSheet(StyleHelper::getMainWidgetStyle());
    ui->startButton->setStyleSheet(StyleHelper::getStartButtonsStyle());
    ui->aboutButton->setStyleSheet(StyleHelper::getStartButtonsStyle());
    ui->leftButton->setStyleSheet(StyleHelper::getLeftButtonActiveStyle());
    ui->rightButton->setStyleSheet(StyleHelper::getRightButtonStyle());
    ui->tabWidget->setStyleSheet(StyleHelper::getTabWidgetStyle());
    ui->tab->setStyleSheet(StyleHelper::getTabStyle());

    ui->messageLabel->setStyleSheet(StyleHelper::getVictoryMessageStyle());
    ui->messageLabel->setText("");

    setGameAreaButtonsStyle();
    ui->messageLabel->setText("");
    ui->messageLabel->setStyleSheet(StyleHelper::getNormalMessageStyle());
    ui->aboutTextLabel->setStyleSheet(StyleHelper::getAboutTextStyle());
}

/* Переключаем выбор крестики-нолики  */
void Widget::changeButtonStatus(int num)
{
    if(num ==1){
        ui->leftButton->setStyleSheet(StyleHelper::getLeftButtonActiveStyle());
        ui->rightButton->setStyleSheet(StyleHelper::getRightButtonStyle());
    }else{
        ui->leftButton->setStyleSheet(StyleHelper::getLeftButtonStyle());
        ui->rightButton->setStyleSheet(StyleHelper::getRightButtonActiveStyle());
    }
}

/* Если игрок выбрал крестики  */
void Widget::on_leftButton_clicked()
{
    changeButtonStatus(1);
    player = 'X';
}

/* Если игрок выбрал нолики  */
void Widget::on_rightButton_clicked()
{
    changeButtonStatus(2);
    player = '0';
}


/* Скрываем заголовок, выставляем высоту и текущую вкладку tabWidget  */
void Widget::configurationTabWidget()
{
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setMaximumHeight(320);
    ui->tabWidget->setCurrentIndex(0);
}

/* Добавляем шрифты из ресурсов в базу шрифтов  */
void Widget::addFonts()
{
    QFontDatabase::addApplicationFont(":/resourses/fonts/Roboto-Medium.ttf");
    int id=QFontDatabase::addApplicationFont(":/resourses/fonts/Roboto-MediumItalic.ttf");
    QFontDatabase::applicationFontFamilies(id).at(0);
    //qDebug() << family;
}

/* Изменяем QSS одной кнопки на игровом поле  */
void Widget::changeButtonStyle(int row, int colum, QString style)
{
    QGridLayout *grid = qobject_cast <QGridLayout*>(ui->tab->layout());
    QPushButton *btn = qobject_cast <QPushButton*>(grid->itemAtPosition(row, colum)->widget());
    btn->setStyleSheet(style);
}

void Widget::configurationGameAreaButtons()
{
     QGridLayout *grid = qobject_cast <QGridLayout*>(ui->tab->layout());
     for(int row=0; row<3; row++){
         for(int column=0; column<3; column++){
              QPushButton *btn = qobject_cast <QPushButton*>(grid->itemAtPosition(row, column)->widget());
              btn->setProperty("row", row);
              btn->setProperty("column", column);
              connect(btn, &QPushButton::clicked, this, &Widget::onGameAreaButtonClicked);
         }
     }
}

/* Нейтральный стиль для всех кнопок на игровом поле  */
void Widget::setGameAreaButtonsStyle()
{
    QString style = StyleHelper::getBlankButtonStyle();
    for(int row=0; row<3; row++){
        for(int column=0; column<3; column++){
            changeButtonStyle(row, column, StyleHelper::getBlankButtonStyle());
        }
    }
    changeButtonStyle(0, 0, StyleHelper::getBlankButtonStyle());
}


/* Начало игры  */
void Widget::start()
{
    playerLocked = false;
    setGameAreaButtonsStyle();
    for (int r=0;r<3;r++ ) {
        for(int c=0;c<3;c++){
            gameArea[r][c] = '-';
        }
    }
    gameArea[0][0] = '-';
    progress = 0;
    gameStart = true;
    stop = false;
    if(player!='X')
        onComputerSlot();
}

void Widget::lockPlayer()
{
    if(player == 'X'){
        playerLocked = false;
    }else{
        playerLocked = true;
    }
}

/* Клик по кнопке Играть-Сдаюсь  */
void Widget::on_startButton_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
    if(gameStart){
        playerLocked = true;
        ui->startButton->setText("Play");
        ui->startButton->setStyleSheet(StyleHelper::getStartButtonsStyle());
        ui->leftButton->setDisabled(false);
        ui->rightButton->setDisabled(false);
        gameStart = false;
        ui->messageLabel->setText("Player surrendered");

    }else{
        ui->messageLabel->setText("Go play");
        ui->messageLabel->setStyleSheet(StyleHelper::getNormalMessageStyle());
        start();
        gameArea[0][0] = '-';
        changeButtonStyle(0, 0, StyleHelper::getBlankButtonStyle());
        playerLocked = false;
        ui->startButton->setText("Surrender");
        ui->startButton->setStyleSheet(StyleHelper::getStartButtonGameStyle());
        ui->leftButton->setDisabled(true);
        ui->rightButton->setDisabled(true);

    }
}

void Widget::onGameAreaButtonClicked()
{
    //playerLocked = false;
    if(!playerLocked){
        QPushButton *btn = qobject_cast<QPushButton*>(sender());
        int row = btn->property("row").toInt();
        int column = btn->property("column").toInt();
        QString style;
        if(gameArea[row][column] == '-'){
            if(player == 'X'){
                style = StyleHelper::getCrossNormalStyle();
                gameArea[row][column] = 'X';
                player = '0';
            }else{
                style = StyleHelper::getZeroNormalStyle();
                gameArea[row][column] = '0';
                player = 'X';
            }
            changeButtonStyle(row, column, style);
            progress++;

            chekGameStop();
            endGame();
            onComputerSlot();
        }
    }
}

void Widget::chekGameStop()
{
    winner = '-';
    QString style;
    char symbols[2] = {'X','0'};
    for(int i=0;i<2;i++){
        for (int row=0;row<3;row++ ) {
            if((gameArea[row][0]==symbols[i])and(gameArea[row][1]==symbols[i])and(gameArea[row][2]==symbols[i])){
                stop = true;
                winner = symbols[i];
                if(winner=='X'){
                    if(player == '0'){
                        style=StyleHelper::getCrossVictoryStyle();
                    }else{
                        style=StyleHelper::getZeroVictoryStyle();
                    }
                }else{
                    if(player == 'X'){
                         style=StyleHelper::getZeroLostStyle();
                    }else{
                        style=StyleHelper::getCrossLostStyle();
                    }
                }
                changeButtonStyle(row,0,style);
                changeButtonStyle(row,1,style);
                changeButtonStyle(row,2,style);
                return;
            }
        }
        for (int col=0;col<3;col++ ) {
            if((gameArea[0][col]==symbols[i])and(gameArea[1][col]==symbols[i])and(gameArea[2][col]==symbols[i])){
                stop = true;
                winner = symbols[i];
                if(winner=='X'){
                    if(player == '0'){
                         style=StyleHelper::getCrossVictoryStyle();
                    }else{
                        style=StyleHelper::getZeroVictoryStyle();
                    }
                }else{
                    if(player == 'X'){
                         style=StyleHelper::getZeroLostStyle();
                    }else{
                        style=StyleHelper::getCrossLostStyle();
                    }
                }
                changeButtonStyle(0,col,style);
                changeButtonStyle(1,col,style);
                changeButtonStyle(2,col,style);
                return;
            }
        }
        if((gameArea[0][0]==symbols[i])and(gameArea[1][1]==symbols[i])and(gameArea[2][2]==symbols[i])){
             stop = true;
             winner = symbols[i];
             if(winner=='X'){
                 if(player == '0'){
                      style=StyleHelper::getCrossVictoryStyle();
                 }else{
                     style=StyleHelper::getZeroVictoryStyle();
                 }
             }else{
                 if(player == 'X'){
                      style=StyleHelper::getZeroLostStyle();
                 }else{
                     style=StyleHelper::getCrossLostStyle();
                 }
             }
             changeButtonStyle(0,0,style);
             changeButtonStyle(1,1,style);
             changeButtonStyle(2,2,style);
             return;
        }
        if((gameArea[0][2]==symbols[i])and(gameArea[1][1]==symbols[i])and(gameArea[2][0]==symbols[i])){
             stop = true;
             winner = symbols[i];
             if(winner=='X'){
                 if(player == '0'){
                      style=StyleHelper::getCrossVictoryStyle();
                 }else{
                     style=StyleHelper::getZeroVictoryStyle();
                 }
             }else{
                 if(player == 'X'){
                      style=StyleHelper::getZeroLostStyle();
                 }else{
                     style=StyleHelper::getCrossLostStyle();
                 }
             }
             changeButtonStyle(0,2,style);
             changeButtonStyle(1,1,style);
             changeButtonStyle(2,0,style);
             return;
        }

    }
    if(progress==9){
        stop = true;
    }

}

void Widget::endGame()
{
    if(stop){
        if(winner == 'X'){
            ui->messageLabel->setText("Cross winner!");
            ui->messageLabel->setStyleSheet(StyleHelper::getVictoryMessageStyle());
        }else if(winner=='-'){
            ui->messageLabel->setText("Draw");
        }else{
            ui->messageLabel->setText("Zero winner!");
            ui->messageLabel->setStyleSheet(StyleHelper::getLostMessageStyle());
        }
        playerLocked = true;
        ui->startButton->setText("Play");
        ui->startButton->setStyleSheet(StyleHelper::getStartButtonsStyle());
        ui->leftButton->setDisabled(false);
        ui->rightButton->setDisabled(false);
        gameStart = false;
    }
}

void Widget::onComputerSlot()
{
    //playerLocked = false;
    char comp;
    QString style;
    if(player == 'X'){
        comp = '0';
        style = StyleHelper::getZeroNormalStyle();
    }else{
        comp = 'X';
        style = StyleHelper::getCrossNormalStyle();
    }

    if(!playerLocked){
        QPushButton *btn = qobject_cast<QPushButton*>(sender());
        int row = btn->property("row").toInt();
        int column = btn->property("column").toInt();
        QString style;
        if(gameArea[row][column]=='-'){
            gameArea[row][column] = comp;
            changeButtonStyle(row, column, style);
            progress++;
            chekGameStop();
            endGame();

        }
    }

}

void Widget::on_aboutButton_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}
