#include "imagetopdf.h"
#include "ui_imagetopdf.h"

ImageToPDF::ImageToPDF(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ImageToPDF)
{
    ui->setupUi(this);
    setFixedSize(730, 531);

    ui->mLoadImage->setStyleSheet("border: 2px solid black");

    connect(ui->mHorizontal, SIGNAL(clicked(bool)), this, SLOT(HorizontalChange(bool)));
    connect(ui->mVertical, SIGNAL(clicked(bool)), this, SLOT(VerticalChange(bool)));
}

ImageToPDF::~ImageToPDF()
{
    delete ui;
}

void ImageToPDF::on_mLoadImages_clicked()
{
    filename = QFileDialog::getOpenFileName(this, "Otworz plik...", "", "Obraz (*.jpg; *.jpeg; *.png; *.bmp;)");

    if(!filename.isEmpty())
    {
        QFileInfo fi(filename);
        QString imageFile = fi.absoluteFilePath();
        ui->mListView->addItem(imageFile);
    }
}

void ImageToPDF::on_mAddFolder_clicked()
{
    folderPath = QFileDialog::getExistingDirectory(this, tr("Otwórz obrazy z folderu..."));
    if(!folderPath.isEmpty())
    {
        QDir dir(folderPath);
        QStringList filter;
        filter << QLatin1String("*.png");
        filter << QLatin1String("*.jpeg");
        filter << QLatin1String("*.jpg");
        filter << QLatin1String("*.bmp");
        dir.setNameFilters(filter);
        QFileInfoList filelistinfo = dir.entryInfoList();
        foreach (const QFileInfo &fileinfo, filelistinfo)
        {
            imageFile = fileinfo.absoluteFilePath();
            ui->mListView->addItem(imageFile);
        }
    }
}

void ImageToPDF::on_mDelete_clicked()
{
    ui->mListView->clear();
}

void ImageToPDF::on_mClear_clicked()
{
    ui->mListView->model()->removeRow(ui->mListView->currentIndex().row());
}

void ImageToPDF::on_mMoveUp_clicked()
{
    int currentIndex = ui->mListView->currentRow();
    QListWidgetItem *currentItem = ui->mListView->takeItem(currentIndex);
    ui->mListView->insertItem(currentIndex-1, currentItem);
    ui->mListView->setCurrentRow(currentIndex-1);
}

void ImageToPDF::on_mMoveDown_clicked()
{
    int currentIndex = ui->mListView->currentRow();
    QListWidgetItem *currentItem = ui->mListView->takeItem(currentIndex);
    ui->mListView->insertItem(currentIndex+1, currentItem);
    ui->mListView->setCurrentRow(currentIndex+1);
}

void ImageToPDF::on_mConvertNow_clicked()
{
    if (!ui->mHorizontal->isChecked() && !ui->mVertical->isChecked())
        {
            QMessageBox::information(this, tr("ImageToPDF"), QString::fromLocal8Bit("Prosze wybrac orientacje pliku PDF!"), QMessageBox::Ok);
            return;
        }

    QString saveThisFile = QFileDialog::getSaveFileName((QWidget*)0, "Plik PDF", QString(), "*.pdf");

    if (QFileInfo(saveThisFile).suffix().isEmpty())
        saveThisFile.append(".pdf");

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(saveThisFile);
    printer.setFullPage(true);
    printer.setPageSize(QPageSize(QPageSize::A4));

    if(ui->mHorizontal->isChecked())
        printer.setPageOrientation(QPageLayout::Landscape);
    else
        printer.setPageOrientation(QPageLayout::Portrait);

    QPainter painter(&printer);

    auto numItems  = ui->mListView->model()->rowCount();

    ui->mProgress->setMinimum(0);
    ui->mProgress->setMaximum(numItems-1);

    for(int i = 0; i < numItems; ++i)
    {
        auto path = ui->mListView->item(i)->text();
        QImage pixmap2(path);

        ui->mProgress->setValue(i);

        painter.drawImage(printer.pageRect(QPrinter::DevicePixel), QImage(pixmap2));
        printer.newPage();

        if(ui->mProgress->value() == numItems - 1)
        {
             painter.end();
             QMessageBox::information(this, QString::fromLocal8Bit("ImageToPDF"), QString::fromLocal8Bit("Konwertowanie zakonczono sukcesem!"), QMessageBox::Ok);
        }
    }
}

void ImageToPDF::HorizontalChange(bool checked)
{
    if (checked)
    {
        ui->mVertical->setEnabled(false);
    }
    else
    {
        ui->mVertical->setEnabled(true);
    }
}

void ImageToPDF::VerticalChange(bool checked)
{
    if (checked)
    {
        ui->mHorizontal->setEnabled(false);
    }
    else
    {
         ui->mHorizontal->setEnabled(true);
    }
}

void ImageToPDF::on_mListView_itemPressed(QListWidgetItem *item)
{
    auto items = ui->mListView->selectedItems();
    for(auto item : items)
    {
        auto path = item->text();
        QFileInfo fi(path);
        QImage image(path);
        ui->mLoadImage->setPixmap(QPixmap::fromImage(image.scaled(321, 231, Qt::IgnoreAspectRatio, Qt::FastTransformation)));
        QString mW = QString::number(image.width());
        QString mH = QString::number(image.height());
        QString infoSize = convertFileSizeToTypeSize(fi.size());
        QString format = fi.suffix().toUpper();
        // if you want to ensure all other letters are lowercase:
        ui->mWHandSizeOnHDD->setText("  " + mW + "x" + mH + " | " + infoSize + " | " + format);
        break;
    }
}

QString ImageToPDF::convertFileSizeToTypeSize(const qlonglong & bytes)
{
    QString number;

        if(bytes < 0x400)
        {
            number = QLocale::system().toString(bytes);
            number.append(" B");
            return number;
        }
        else
        {
            if(bytes >= 0x400 && bytes < 0x100000)
            {
                qlonglong result = (bytes + (0x400 / 2)) / 0x400;

                if(result < 0x400)
                {
                    number = QLocale::system().toString(result);
                    number.append(" KB");
                    return number;
                }
                else
                {
                    qlonglong result = (bytes + (0x100000 / 2)) / 0x100000;
                    number = QLocale::system().toString(result);
                    number.append(" MB");
                    return number;
                }
            }
            else
            {
                if(bytes >= 0x100000 && bytes < 0x40000000)
                {
                    qlonglong result = (bytes + (0x100000 / 2)) / 0x100000;

                    if(result < 0x100000)
                    {
                        number = QLocale::system().toString(result);
                        number.append(" MB");
                        return number;
                    }
                    else
                    {
                        qlonglong result = (bytes + (0x40000000 / 2)) / 0x40000000;
                        number = QLocale::system().toString(result);
                        number.append(" GB");
                        return number;
                    }
                }
                else
                {
                    if(bytes >= 0x40000000 && bytes < 0x10000000000)
                    {
                        qlonglong result = (bytes + (0x40000000 / 2)) / 0x40000000;

                        if(result < 0x40000000)
                        {
                            number = QLocale::system().toString(result);
                            number.append(" GB");
                            return number;
                        }
                        else
                        {
                            qlonglong result = (bytes + (0x10000000000 / 2)) / 0x10000000000;
                            number = QLocale::system().toString(result);
                            number.append(" TB");
                            return number;
                        }
                    }
                    else
                    {
                        qlonglong result = (bytes + (0x10000000000 / 2)) / 0x10000000000;
                        number = QLocale::system().toString(result);
                        number.append(" TB");
                        return number;
                }
            }
        }
    }
}
