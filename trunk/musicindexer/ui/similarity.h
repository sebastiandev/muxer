#ifndef SIMILARITY_H
#define SIMILARITY_H

#include "ui_similarity.h"
#include <QLabel>
#include <QMouseEvent>
#include <QImage>

class Similarity : public QWidget
{
    Q_OBJECT

public:
    explicit Similarity(QWidget *parent = 0);


    void setArtists(const QStringList &artists);
    void setAlbums (const QList<QPair<QImage, QString> > &albums);
    void setSimilar(const QList<QPair<QImage, QString> > &items);

signals:

    void artistSimilaritySelected();
    void albumSimilaritySelected ();
    void artistSimilarityRequested(const QString&);
    void albumSimilarityRequested(const QString&, const QString&);


private slots:

    void artistOptionSelected();
    void albumOptionSelected ();
    void filterList(QString term);
    void itemForSimilaritySelected(QListWidgetItem* item);


private:

    typedef enum similarityType{ ALBUM, ARTIST} SimilarityType;

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    virtual bool eventFilter(QObject *obj, QEvent *e);
    void keyPressEvent(QKeyEvent *e);

    Ui::Similarity ui;

    QPoint         m_dragPosition;
    SimilarityType _currentView;
};

#endif // SIMILARITY_H
