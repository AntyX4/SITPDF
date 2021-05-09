#ifndef IMAGETOPDF_H
#define IMAGETOPDF_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStringList>
#include <QPrinter>
#include <QPainter>
#include <QPageSize>
#include <QMessageBox>
#include <QImage>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class ImageToPDF; }
QT_END_NAMESPACE

class ImageToPDF : public QMainWindow
{
    Q_OBJECT

public:
    ImageToPDF(QWidget *parent = nullptr);
    ~ImageToPDF();

private slots:
    void on_mLoadImages_clicked();
    void on_mAddFolder_clicked();
    void on_mDelete_clicked();
    void on_mClear_clicked();
    void on_mMoveUp_clicked();
    void on_mMoveDown_clicked();
    void on_mConvertNow_clicked();
    void HorizontalChange(bool checked);
    void VerticalChange(bool checked);
    void on_mListView_itemPressed(QListWidgetItem *item);
    QString convertFileSizeToTypeSize(const qlonglong &bytes);

private:
    Ui::ImageToPDF *ui;
    QString imageFile;
    QString filename;
    QString folderPath;

};
#endif // IMAGETOPDF_H
