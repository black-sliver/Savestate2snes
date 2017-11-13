#ifndef SAVESTATE2SNESW_H
#define SAVESTATE2SNESW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "handlestuff.h"
#include "usb2snes.h"

namespace Ui {
class Savestate2snesw;
}

class Savestate2snesw : public QMainWindow
{
    Q_OBJECT

public:
    explicit Savestate2snesw(QWidget *parent = 0);
    ~Savestate2snesw();

private slots:
    void    saveListShowContextMenu(QPoint point);
    void    categoryListShowContextMenu(QPoint point);

    void on_actionRemoveCategory_triggered();

    void on_actionAddCategory_triggered();

    void on_actionAddSubCategory_triggered();

    void on_newGamePushButton_clicked();

    void on_gameComboBox_currentIndexChanged(const QString &arg1);

    void on_addSaveStatePushButton_clicked();

    void    saveStateItemChanged(QStandardItem* item);

    void on_categoryTreeView_clicked(const QModelIndex &index);
    //void    onSaveStateModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ());
    void    onSaveStateDelegateDataCommited(QWidget *e);

    void    usb2snesStateChanged();

private:
    Ui::Savestate2snesw *ui;
    QStandardItemModel* saveStateModel;
    QStandardItemModel* repStateModel;
    QStandardItem*      currentRep;
    QMenu*              saveStateMenu;
    QMenu*              categoryMenu;
    QAction*            customAddCatAction;
    QModelIndex         indexCatUnderMenu;
    HandleStuff         handleStuff;
    QStandardItem*      newSaveInserted;
    USB2snes*           usb2snes;

    void    createMenus();
    void    loadGames();
    void    addSubName(QString name, QString parentPath);
    void    addState(QString name);
};

#endif // SAVESTATE2SNESW_H