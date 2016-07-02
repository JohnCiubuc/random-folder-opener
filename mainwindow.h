#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDirIterator>
#include <QDebug>
#include <QMetaType>

#define InaFormat QSettings::IniFormat


Q_DECLARE_METATYPE(QList<QString>)

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_Browse_clicked();

    void on_pushButton_Delete_clicked();

    void on_listWidget_Favourites_doubleClicked(const QModelIndex &index);

    void on_listWidget_Favourites_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QList<QString> FolderHistoryList;
    int iHistoryIndex;
};

#endif // MAINWINDOW_H
