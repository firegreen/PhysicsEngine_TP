#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_elasticitySlider_valueChanged(int value);

    void on_glueSlider_valueChanged(int value);

    void on_resetBtn_clicked();

    void on_continuousLinkBtn_toggled(bool checked);

    void on_wallLinkBtn_toggled(bool checked);

    void on_generateBtn_clicked();

    void on_massGenBtn_clicked();

    void on_emitterBtn_toggled(bool checked);

    void on_freqSlider_valueChanged(int value);

    void on_genCount_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
