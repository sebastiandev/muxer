#ifndef IMPORTVIEW_H
#define IMPORTVIEW_H

#include <QWidget>
#include <QFileSystemModel>
#include <QScopedPointer>

namespace Ui {
class ImportView;
}

class ImportView : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImportView(QWidget *parent = 0);
    ~ImportView();
    
private:
    Ui::ImportView *ui;
    QScopedPointer<QFileSystemModel> model;
};

#endif // IMPORTVIEW_H
