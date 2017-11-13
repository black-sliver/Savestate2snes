#include <QDebug>

#include "handlestuff.h"

#define ORDERCATFILE "svt2snesordercat.txt"
#define ORDERSAVEFILE "svt2snesordersave.txt"

HandleStuff::HandleStuff()
{

}

QStringList HandleStuff::loadGames()
{
    QFileInfoList listDir = saveDirectory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    qDebug() << "Loading games" << listDir.size();
    foreach(QFileInfo fi, listDir)
    {
        games << fi.baseName();
        qDebug() << fi.baseName();
    }
    return games;
}

void    HandleStuff::findCategory(QStandardItem* parent, QDir dir)
{
    QFileInfoList listDir = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(QFileInfo fi, listDir)
    {
        QStandardItem* item = new QStandardItem(fi.baseName());
        item->setData(fi.absoluteFilePath(), MyRolePath);
        categoriesByPath[item->data(MyRolePath).toString()] = item;
        dir.cd(fi.baseName());
        findCategory(item, dir);
        dir.cdUp();
        parent->appendRow(item);
    }
}

QStringList HandleStuff::getCacheOrderList(QString file, QString dirPath)
{
    QStringList toRet;
    QFile cache(dirPath + "/" + file);
    if (cache.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!cache.atEnd())
        {
            QString line = cache.readLine();
            QFileInfo fi(dirPath + "/" + line);
            toRet << fi.completeBaseName();
        }
        cache.close();
    }
    return toRet;
}

void HandleStuff::writeCacheOrderFile(QString file, QString dirPath)
{
    if (file == ORDERSAVEFILE)
    {
        QFile   cache(dirPath + "/" + file);
        if (cache.open(QIODevice::WriteOnly))
        {
           foreach(QString save, saveStates[dirPath])
           {
               cache.write(QByteArray(save.toUtf8() + ".svt\n"));
           }
           cache.close();
        }
    }
}


QStandardItem *HandleStuff::loadCategories(QString game)
{
    qDebug() << "Loading category for " << game;
    if (!categories.contains(game))
    {
        QStandardItem *root = new QStandardItem();
        saveDirectory.cd(game);
        root->setData(saveDirectory.absolutePath(), MyRolePath);
        categoriesByPath[root->data(MyRolePath).toString()] = root;
        findCategory(root, saveDirectory);
        saveDirectory.cdUp();
        categories[game] = root;
    }
    gameLoaded = game;
    return categories[game];
}

bool HandleStuff::addGame(QString newGame)
{
    if (saveDirectory.mkdir(newGame))
    {
        games.append(newGame);
        QStandardItem *newItem = new QStandardItem();
        categories[newGame] = newItem;
        saveDirectory.cd(newGame);
        newItem->setData(saveDirectory.absolutePath(), MyRolePath);
        saveDirectory.cdUp();
        return true;
    }
    return false;
}

void HandleStuff::setSaveStateDir(QString dir)
{
    saveDirectory.setPath(dir);
}

bool HandleStuff::addCategory(QStandardItem *newCategory, QStandardItem *parent)
{
    QString parentPath;
    if (parent->index().isValid())
        parentPath = parent->data(MyRolePath).toString();
    else
        parentPath = saveDirectory.absolutePath() + "/" + gameLoaded;
    QFileInfo fi(parentPath + "/" + newCategory->text());
    QDir di(parentPath);
    di.mkdir(newCategory->text());
    newCategory->setData(fi.absoluteFilePath(), MyRolePath);
    QStandardItem* cloned = newCategory->clone();
    categoriesByPath[newCategory->data(MyRolePath).toString()] = cloned;
    if (parent->index().isValid())
        categoriesByPath[parentPath]->appendRow(cloned);
    else
        categories[gameLoaded]->appendRow(cloned);
    parent->appendRow(newCategory);
    return true;
}

bool HandleStuff::addSubCategory(QStandardItem *newCategory, QStandardItem *parent)
{
    QString parentPath = parent->data(MyRolePath).toString();
    qDebug() << parent->text() << parentPath;
    QFileInfo fi(parentPath + "/" + newCategory->text());
    QDir di(parentPath);
    di.mkdir(newCategory->text());
    newCategory->setData(fi.absoluteFilePath(), MyRolePath);
    categoriesByPath[parentPath]->appendRow(newCategory->clone());
    parent->appendRow(newCategory);
    QStandardItem* cloned = newCategory->clone();
    categoriesByPath[newCategory->data(MyRolePath).toString()] = cloned;
    categoriesByPath[parentPath]->appendRow(cloned);
    return true;
}

QStringList HandleStuff::loadSaveStates(QStandardItem *category)
{
    QString savePath = category->data(MyRolePath).toString();
    if (!saveStates.contains(savePath))
    {
        QStringList cachedList = getCacheOrderList(ORDERSAVEFILE, savePath);
        if (cachedList.isEmpty())
        {

            QDir dir(savePath);
            QFileInfoList fil = dir.entryInfoList(QDir::Files);
            foreach(QFileInfo fi, fil)
            {
                saveStates[savePath] << fi.baseName();
            }
        } else {
            foreach(QString s, cachedList)
            {
                saveStates[savePath] << QFileInfo(s).baseName();
            }
        }
    }
    catLoaded = category;
    return saveStates[savePath];
}

bool HandleStuff::addSaveState(QString name)
{
    //QStandardItem*  newItem = new QStandardItem(name);
    QFileInfo fi(catLoaded->data(MyRolePath).toString() + "/" + name + ".svt");
    saveStates[catLoaded->data(MyRolePath).toString()] << fi.baseName();
    writeCacheOrderFile(ORDERSAVEFILE, catLoaded->data(MyRolePath).toString());
    QFile f(fi.absoluteFilePath()); f.open(QIODevice::WriteOnly); f.write("Hello"); f.close();
    return true;
}

bool HandleStuff::removeCategory(QStandardItem *category)
{
    QString path = category->data(MyRolePath).toString();
    if (QDir::root().rmpath(path))
    {
        categoriesByPath[path]->parent()->removeRow(category->row());
        return true;
    }
    return false;
}