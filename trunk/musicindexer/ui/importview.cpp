#include "importview.h"
#include "ui_importview.h"
#include <QGraphicsDropShadowEffect>

ImportView::ImportView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImportView)
{
    ui->setupUi(this);

    ui->progressBar->hide();

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(5);
    effect->setXOffset(0.0);
    effect->setYOffset(1.0);

    QGraphicsDropShadowEffect *effect2 = new QGraphicsDropShadowEffect();
    effect2->setBlurRadius(8);
    effect2->setXOffset(0.0);
    effect2->setYOffset(-1.0);

    QGraphicsDropShadowEffect *effect3 = new QGraphicsDropShadowEffect();
    effect3->setBlurRadius(5);
    effect3->setXOffset(0.0);
    effect3->setYOffset(-1.0);

    ui->line->setGraphicsEffect(effect);
    ui->leftColumn->setGraphicsEffect(effect2);
    ui->columnView->setGraphicsEffect(effect3);

    model.reset(new QFileSystemModel);
    model->setRootPath(QDir::homePath());

    ui->columnView->setModel(model.data());
    ui->columnView->show();
}

ImportView::~ImportView()
{
    delete ui;
}
