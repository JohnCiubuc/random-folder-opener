#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings save(QApplication::applicationDirPath().append("/").append("history.ina"), InaFormat);
    save.beginGroup("General");
    FolderHistoryList = save.value("DirectoryHistory").value<QList<QString>>();
    save.endGroup();

    foreach (QString folder, FolderHistoryList)
    {
        QListWidgetItem *itm = new QListWidgetItem();
        itm->setText(folder);
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
               QDesktopServices::openUrl(openFolderURL);

               if(!FolderHistoryList.contains(openFolderURL))
                    FolderHistoryList.append(openFolderURL);

               QSettings save(QApplication::applicationDirPath().append("/").append("history.ina"), QSettings::IniFormat);

               save.beginGroup("General");
               save.setValue("DirectoryHistory", QVariant::fromValue(FolderHistoryList));
               save.endGroup();

               exit(0);
           }
       }
}

void MainWindow::on_pushButton_Delete_clicked()
{
    FolderHistoryList.removeAll(ui->listWidget_Favourites->item(iHistoryIndex)->text());
    ui->listWidget_Favourites->removeItemWidget(ui->listWidget_Favourites->item(iHistoryIndex));

    QSettings save(QApplication::applicationDirPath().append("/").append("history.ina"), QSettings::IniFormat);
    save.beginGroup("General");
    save.setValue("DirectoryHistory", QVariant::fromValue(FolderHistoryList));
    save.endGroup();
}

void MainWindow::on_listWidget_Favourites_doubleClicked(const QModelIndex &index)
{
    QDesktopServices::openUrl(ui->listWidget_Favourites->item(index.row())->text());
}

void MainWindow::on_listWidget_Favourites_clicked(const QModelIndex &index)
{
    iHistoryIndex = index.row();
}
