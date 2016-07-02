#include "mainwindow.h"
#include "ui_mainwindow.h"

#define iFatButtonHeight 40

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings save(QApplication::applicationDirPath().append("/").append("history.ina"), InaFormat);

    int size = save.beginReadArray("DirectoryHistory");
    for (int i = 0; i < size; ++i) {
        save.setArrayIndex(i);
        FolderHistoryList.append(save.value("HistoryFolder").toString());
    }

    foreach (QString folder, FolderHistoryList)
    {
        QListWidgetItem *itm = new QListWidgetItem();
        itm->setText(folder);
        itm->setSizeHint(QSize(itm->sizeHint().width(), iFatButtonHeight));
        ui->listWidget_Favourites->insertItem(0, itm);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Browse_clicked()
{
    QFileDialog parentFolderPicker;
       parentFolderPicker.setFileMode(QFileDialog::Directory);
       parentFolderPicker.setOptions(QFileDialog::ShowDirsOnly);

       if (parentFolderPicker.exec())
       {
           if (!parentFolderPicker.selectedFiles().isEmpty())
           {
               QStringList folderSubDirectories;
               QDirIterator subDirectoriesIterator(parentFolderPicker.selectedFiles().at(0), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator ::NoIteratorFlags);

               while(subDirectoriesIterator.hasNext()){
                   subDirectoriesIterator.next();
                   folderSubDirectories << subDirectoriesIterator.filePath();
               }
               QString openFolderURL = folderSubDirectories.at(rand()%((folderSubDirectories.count()-1) + 1));
               QDesktopServices::openUrl(QUrl::fromLocalFile(openFolderURL));

               if(!FolderHistoryList.contains(openFolderURL))
                    FolderHistoryList.append(openFolderURL);
//                  if(!FolderHistoryList.contains(parentFolderPicker.selectedFiles().at(0)))
//                       FolderHistoryList.append(parentFolderPicker.selectedFiles().at(0));


               QSettings save(QApplication::applicationDirPath().append("/").append("history.ina"), InaFormat);

               save.beginWriteArray("DirectoryHistory");
               for (int i = 0; i < FolderHistoryList.size(); ++i) {
                   save.setArrayIndex(i);
                   save.setValue("HistoryFolder", FolderHistoryList.at(i));
               }
               save.endArray();
               save.sync();

               ui->listWidget_Favourites->clear();

               foreach (QString folder, FolderHistoryList)
               {
                   QListWidgetItem *itm = new QListWidgetItem();
                   itm->setSizeHint(QSize(itm->sizeHint().width(), iFatButtonHeight));
                   itm->setText(folder);
                   ui->listWidget_Favourites->insertItem(0, itm);
               }

           }
       }
}

void MainWindow::on_pushButton_Delete_clicked()
{
    QString folderName = ui->listWidget_Favourites->item(iHistoryIndex)->text();
    qDebug() << "item - " << folderName;
    FolderHistoryList.removeAll(folderName);

    QSettings save(QApplication::applicationDirPath().append("/").append("history.ina"), InaFormat);
    save.beginWriteArray("DirectoryHistory");
    for (int i = 0; i < FolderHistoryList.size(); ++i) {
        save.setArrayIndex(i);
        save.setValue("HistoryFolder", FolderHistoryList.at(i));
    }
    save.endArray();
    save.sync();
    ui->listWidget_Favourites->clear();

    foreach (QString folder, FolderHistoryList)
    {
        QListWidgetItem *itm = new QListWidgetItem();
        itm->setText(folder);
        itm->setSizeHint(QSize(itm->sizeHint().width(), iFatButtonHeight));
        ui->listWidget_Favourites->insertItem(0, itm);
    }
}

void MainWindow::on_listWidget_Favourites_doubleClicked(const QModelIndex &index)
{
    QString folderFromHistory = ui->listWidget_Favourites->item(index.row())->text();
    QStringList folderSubDirectories;
    QDirIterator subDirectoriesIterator(folderFromHistory, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator ::NoIteratorFlags);

    while(subDirectoriesIterator.hasNext()){
        subDirectoriesIterator.next();
        folderSubDirectories << subDirectoriesIterator.filePath();
    }
    QString openFolderURL = folderSubDirectories.at(rand()%((folderSubDirectories.count()-1) + 1));
    QDesktopServices::openUrl(QUrl::fromLocalFile(openFolderURL));
}

void MainWindow::on_listWidget_Favourites_clicked(const QModelIndex &index)
{
    iHistoryIndex = index.row();
}
