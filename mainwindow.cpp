#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include "particule.h"
#include "edge.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GLWidget* widget = ui->view;
    resize(850,850);

    Edge* e1 = new Edge(QPointF(-350,-200), QPointF(350,-200));
    Edge* e2 = new Edge(QPointF(-350,400), QPointF(-150,100));
    Edge* e7 = new Edge(QPointF(150,250), QPointF(300,400));
    Edge* e3 = new Edge(QPointF(-50,200), e7->point1());
    Edge* e4 = new Edge(e7->point2(), e2->point1());
    Edge* e5 = new Edge(e1->point2(), e3->point1());
    Edge* e6 = new Edge(e2->point2(), e1->point1());

    widget->addActor(*e1);
    widget->addActor(*e2);
    widget->addActor(*e3);
    widget->addActor(*e4);
    widget->addActor(*e5);
    widget->addActor(*e6);
    widget->addActor(*e7);

    ui->view->setGlue(ui->glueSlider->value()/100.f);
    widget->resetMovableActors(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_elasticitySlider_valueChanged(int value)
{
    ui->view->setElasticity(value/100.f);
}

void MainWindow::on_glueSlider_valueChanged(int value)
{
    ui->view->setGlue(value/100.f);
}

void MainWindow::on_resetBtn_clicked()
{
    ui->view->resetMovableActors(ui->particlesCount->value());
    ui->view->setGlue(ui->glueSlider->value()/100.f);
    ui->view->setElasticity(ui->elasticitySlider->value()/100.f);
}

void MainWindow::on_continuousLinkBtn_toggled(bool checked)
{
    ui->view->linkedParticules = checked;
}

void MainWindow::on_wallLinkBtn_toggled(bool checked)
{
    ui->view->linkWithWall = checked;
}

void MainWindow::on_generateBtn_clicked()
{
    ui->view->addRandomParticule();
    ui->view->setGlue(ui->glueSlider->value()/100.f);
    ui->view->setElasticity(ui->elasticitySlider->value()/100.f);
}

void MainWindow::on_massGenBtn_clicked()
{
    for (int i=0; i < ui->particlesCount->value(); ++i)
    {
        ui->view->addRandomParticule();
    }
    ui->view->setGlue(ui->glueSlider->value()/100.f);
    ui->view->setElasticity(ui->elasticitySlider->value()/100.f);
}

void MainWindow::on_emitterBtn_toggled(bool checked)
{
    ui->view->emitter = checked;
}

void MainWindow::on_freqSlider_valueChanged(int value)
{
    ui->view->emitterFreq = value;
}

void MainWindow::on_genCount_valueChanged(int value)
{
    ui->view->emitterGenCount = value;
}
