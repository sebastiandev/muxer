#ifndef SIMILARITYVIEW_H
#define SIMILARITYVIEW_H

#include <QWidget>
#include <QStringList>
#include <QPair>
#include <QImage>
#include <QModelIndex>
#include <QKeyEvent>

namespace Ui {
    class SimilarityView;
}

class SimilarityView : public QWidget
{
    Q_OBJECT

public:
    explicit SimilarityView(QWidget *parent = 0);
    ~SimilarityView();

    void setArtists(const QStringList &artists);
    void setAlbums (const QList<QPair<QImage, QString> > &albums);

    void setSimilar(const QList<QPair<QImage, QString> > &items);

Q_SIGNALS:

    void artistSelected(const QString&);
    void artistSimilarityRequested(const QString&);
    void albumSimilarityRequested(const QString&, const QString&);

private Q_SLOTS:

    void slotArtistSelected(QModelIndex);
    void slotArtistSearch();
    void slotAlbumSearch();

private:

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    virtual bool eventFilter(QObject *obj, QEvent *e);
    void keyPressEvent(QKeyEvent *e);

    Ui::SimilarityView *ui;
    QPoint       m_dragPosition;

};

#endif // SIMILARITYVIEW_H
