#include <QApplication>
#include <time.h>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDirIterator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand (time(NULL));
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
           }
       }

       return 0;
}
