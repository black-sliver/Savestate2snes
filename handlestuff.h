/*
    This file is part of the SaveState2snes software
    Copyright (C) 2017  Sylvain "Skarsnik" Colinet <scolinet@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HANDLESTUFF_H
#define HANDLESTUFF_H

#include <QString>
#include <QStandardItem>
#include <QDir>

#define MyRolePath Qt::UserRole + 1

struct Category {
    struct Category*    parent;
    QVector<Category*>  children;
    QString             name;
    QString             path;
    QIcon               icon;
};

struct  GameInfos
{
    quint16 saveShortcut;
    quint16 loadShortcut;
    QString name;
};

class HandleStuff
{
public:
    HandleStuff();

    QStringList loadGames();
    QVector<Category*>  loadCategories(QString game);
    QIcon       getGameIcon(QString game);
    bool        addGame(QString newGame);
    void        setSaveStateDir(QString dir);
    Category*   addCategory(QString newCategory, QString parentPath);
    QStringList loadSaveStates(QString categoryPath);
    bool        addSaveState(QString name, bool trigger = true);
    bool        removeCategory(QString categoryPath);
    bool        renameSaveState(int row, QString newName);
    void        changeStateOrder(int from, int to);
    bool        loadSaveState(QString name);
    bool        deleteSaveState(int row);
    void        setCategoryIcon(QString categoryPath, QString iconPath);

    QPixmap     getScreenshot(QString name);
    QString     getScreenshotPath(QString name);
    QString     getSavestatePath(QString name);
    GameInfos   gameInfos();
    void        setGameShortCut(quint16 save, quint16 load);
    virtual QByteArray  getScreenshotData() = 0;
    virtual bool        hasShortcutsEdit() = 0;
    virtual bool        hasScreenshots() = 0;
    virtual void        setShortcutSave(quint16 shortcut) = 0;
    virtual void        setShortcutLoad(quint16 shortcut) = 0;
    virtual quint16     shortcutSave() = 0;
    virtual quint16     shortcutLoad() = 0;


protected:
    virtual QByteArray  saveState(bool trigger) = 0;
    virtual void        loadState(QByteArray data) = 0;

private:
    QDir                                    saveDirectory;
    QStringList                             games;
    QMap<QString, Category*>                categoriesByPath;
    QMap<QString, Category*>                categories;
    QMap<QString, QStringList>              saveStates;
    QString                                 gameLoaded;
    Category*                               catLoaded;
    GameInfos                               m_gameInfo;

    void findCategory(Category *parent, QDir dir);
    QStringList getCacheOrderList(QString file, QString dirPath);
    void        writeCacheOrderFile(QString file, QString dirPath);
};

#endif // HANDLESTUFF_H
