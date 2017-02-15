/* Copyright (C) 2009, 2010 Dark Sidious

This file is part of dTodo.

dTodo is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

dTodo is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with dTodo.  If not, see <http://www.gnu.org/licenses/>. */

#include "MainWindow.h"

MainWindow::MainWindow()
{
    checkConfig();
    setWindowTitle("dTodo 1.0.2");
    setWindowIcon(QIcon(":/images/icone.png"));
    setMinimumSize(700, 450);

    createMenu();

    currentDate = QDate::currentDate();

    /**** Main Window ****/

    mainWidget = new QWidget;

    mainLayout = new QVBoxLayout;

    viewTodo = new QTreeView;
    viewTodo->setContextMenuPolicy(Qt::CustomContextMenu);

    viewDoneTodo = new QTreeView;
    viewDoneTodo->setContextMenuPolicy(Qt::CustomContextMenu);

    readDatabase();

    createTrayIcon();

    mainLayout->addWidget(viewTodo);
    mainLayout->addWidget(viewDoneTodo);

    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);

    barreStat = statusBar();
    barreStat->showMessage(tr("Bienvenue sur dTodo"), 40000);

    connect(viewDoneTodo, SIGNAL(clicked(QModelIndex)), this, SLOT(clearSelectionTodo()));
    connect(viewTodo, SIGNAL(clicked(QModelIndex)), this, SLOT(clearSelectionDone()));
    connect(viewDoneTodo, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(menuRight(QPoint)));
    connect(viewTodo, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(menuRight(QPoint)));

    if(dtodoConfig.maj) majDtodoWin();
}

void MainWindow::menuRight(QPoint)
{
    QMenu menu(this);
    menu.addAction(actionSee);
    menu.addAction(actionDo);
    menu.addAction(actionModifie);
    menu.addAction(actionDelete);
    menu.exec(QCursor::pos());
    return;
}

void MainWindow::clearSelectionTodo()
{
    viewTodo->clearSelection();
    viewTodo->setCurrentIndex(QModelIndex());
}

void MainWindow::clearSelectionDone()
{
    viewDoneTodo->clearSelection();
    viewDoneTodo->setCurrentIndex(QModelIndex());
}

void MainWindow::createMenu()
{
    menuFile = menuBar()->addMenu(tr("&Fichier"));
    menuEdit = menuBar()->addMenu(tr("&Édition"));
    menuTache = menuBar()->addMenu(tr("&Tâche"));
    menuHelp = menuBar()->addMenu(tr("&Aide"));

    actionNew = new QAction(tr("&Nouveau"), this);
    actionNew->setIcon(QIcon(":/images/nouveau.png"));
    actionNew->setStatusTip(tr("Ajouter un dTodo"));
    menuFile->addAction(actionNew);

    actionQuit = new QAction(tr("&Quitter"), this);
    actionQuit->setIcon(QIcon(":/images/quitter.png"));
    actionQuit->setStatusTip(tr("Quitter le programme"));
    menuFile->addAction(actionQuit);

    actionPreference = new QAction(tr("&Préférences"), this);
    actionPreference->setIcon(QIcon(":/images/preferences.png"));
    actionPreference->setStatusTip(tr("Modifiez vos préférences"));
    menuEdit->addAction(actionPreference);

    actionMaj = new QAction(tr("&Mise à jour (windows)"), this);
    actionMaj->setIcon(QIcon(":/images/maj.png"));
    actionMaj->setStatusTip(tr("Mettre dTodo à jour (uniquement pour windows)"));
    menuEdit->addAction(actionMaj);
	
    actionSee = new QAction(tr("&Voir"), this);
    actionSee->setIcon(QIcon(":/images/voir.png"));
    actionSee->setStatusTip(tr("Voir le dTodo selectionné"));
    menuTache->addAction(actionSee);

    actionDo = new QAction(tr("&Marquer comme fait"), this);
    actionDo->setIcon(QIcon(":/images/do.png"));
    actionDo->setStatusTip(tr("Marquer le dTodo selectionné comme fait"));
    menuTache->addAction(actionDo);

    actionDelete = new QAction(tr("&Supprimer"), this);
    actionDelete->setIcon(QIcon(":/images/supprimer.png"));
    actionDelete->setStatusTip(tr("Supprimer un dTodo"));
    menuTache->addAction(actionDelete);

    actionModifie = new QAction(tr("&Modifier"), this);
    actionModifie->setIcon(QIcon(":/images/modif.png"));
    actionModifie->setStatusTip(tr("Modifier un dTodo"));
    menuTache->addAction(actionModifie);

    actionAbout = new QAction(tr("&A propos ..."), this);
    actionAbout->setIcon(QIcon(":/images/icone.png"));
    menuHelp->addAction(actionAbout);

    actionAboutQt = new QAction(tr("&A propos de Qt"), this);
    actionAboutQt->setIcon(QIcon(":/images/qt.png"));
    menuHelp->addAction(actionAboutQt);

    toolBar = addToolBar(tr("Barre d'outil"));
    toolBar->addAction(actionNew);
    toolBar->addAction(actionDelete);
    toolBar->addAction(actionSee);
    toolBar->addAction(actionDo);
    toolBar->addAction(actionModifie);

    connect(actionDo, SIGNAL(triggered()), this, SLOT(markDo()));
    connect(actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(actionMaj, SIGNAL(triggered()), this, SLOT(majDtodoWin()));
    connect(actionModifie, SIGNAL(triggered()), this, SLOT(modifieDtodo()));
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(actionSee, SIGNAL(triggered()), this, SLOT(showDtodo()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(createNewDtodo()));
    connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteDtodo()));
    connect(actionPreference, SIGNAL(triggered()), this, SLOT(changePreference()));

    actionNew->setShortcut(QKeySequence("Ctrl+N"));
    actionDelete->setShortcut(QKeySequence("Ctrl+S"));
    actionDo->setShortcut(QKeySequence("Ctrl+D"));
    actionPreference->setShortcut(QKeySequence("Ctrl+P"));
    actionQuit->setShortcut(QKeySequence("Ctrl+Q"));
    actionMaj->setShortcut(QKeySequence("Ctrl+U"));
    actionSee->setShortcut(QKeySequence("Ctrl+O"));
    actionModifie->setShortcut(QKeySequence("Ctrl+M"));
}

void MainWindow::createTrayIcon()
{
    if (dtodoConfig.tray)
    {
        minimizeAction = new QAction(tr("Réduire"), this);
        connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

        restoreAction = new QAction(tr("&Restaurer"), this);
        connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

        quitAction = new QAction(tr("&Fermer"), this);
        connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

        trayIconMenu = new QMenu(this);

        trayIconMenu->addAction(minimizeAction);
        trayIconMenu->addAction(restoreAction);
        trayIconMenu->addSeparator();

        dtodoDay = trayIconMenu->addMenu(QString::number(dayDtodo.size()) + tr(" dTodo(s) aujourd'hui"));
        dtodoOld = trayIconMenu->addMenu(QString::number(oldDtodo.size()) + tr(" dTodo(s) en retard"));
        for (unsigned int i=0; i < dayDtodo.size(); i++) dtodoDay->addAction(dayDtodo[i]);
        for (unsigned int i=0; i < oldDtodo.size(); i++) dtodoOld->addAction(oldDtodo[i]);

        trayIconMenu->addSeparator();
        trayIconMenu->addAction(quitAction);

        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setContextMenu(trayIconMenu);

        trayIcon->setIcon(QIcon(":/images/icone.png"));

        trayIcon->show();
        connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(activationSystemTray(QSystemTrayIcon::ActivationReason)));
    }
}

void MainWindow::activationSystemTray(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Unknown:
        case QSystemTrayIcon::DoubleClick:
            this->show();
            break;
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::MiddleClick:
        case QSystemTrayIcon::Context:
        break;
    }
    return;
}

void MainWindow::majDtodoWin()
{
    QUrl url("http://darksidious77.free.fr/doc/dtodo/maj/VERSION");
    
    #if defined(linux)
    if (QFile::exists(QString(getenv("HOME")) + "/.dtodo/VERSION")) {
        QFile::remove(QString(getenv("HOME")) + "/.dtodo/VERSION");
    }
    file = new QFile(QString(getenv("HOME")) + "/.dtodo/VERSION");
    #elif defined(WIN32) || defined(WIN64)
    if (QFile::exists("../share/tmp/VERSION")) {
        QFile::remove("../share/tmp/VERSION");
    }
    file = new QFile("../share/tmp/VERSION");
    #endif

    http = new QHttp(this);
    
    QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
    http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());
    
    if (!url.userName().isEmpty()) http->setUser(url.userName(), url.password());

    QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    if (path.isEmpty()) path = "/";
    httpGetId = http->get(path, file);
    connect(http, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)));
}

void MainWindow::httpRequestFinished(int requestId, bool error)
{
    if (requestId != httpGetId || error) return;
    file->close();
    
    if(checkMaj())
    {
	    #if defined(linux)
		    QMessageBox::information(this, tr("Information"), tr("Une nouvelle version est disponible ! Malheuresement la mise à jour sur linux n'est pas supportée ! Rendez vous sur http://sourceforge.net/projects/dtodo/files/ pour mettre à jour"));
            #elif defined(WIN32) || defined(WIN64)
            int reponse = QMessageBox::question(this, tr("Attention"), tr("Une nouvelle version est disponible ! Voulez vous l'installer ?"), QMessageBox::Yes | QMessageBox::No);
            if (reponse == QMessageBox::Yes)
            {
                QProcess::startDetached("updater.exe");
                qApp->quit();
            }
            #endif
    }
    else
    {
        QMessageBox::information(this, tr("Information"), tr("Aucune nouvelle version est disponible !"));
    }
}

bool MainWindow::checkMaj()
{
    #if defined(linux)
    QFile fileCurrent("/usr/share/dtodo/VERSION"), fileDl(QString(getenv("HOME")) + "/.dtodo/VERSION");
    #elif defined(WIN32) || defined(WIN64)
    QFile fileCurrent("../share/VERSION"), fileDl("../share/tmp/VERSION");
    #endif
    fileCurrent.open(QIODevice::ReadOnly | QIODevice::Text);
    fileDl.open(QIODevice::ReadOnly | QIODevice::Text);
    version actual, dl;

    QTextStream streamCurrent(&fileCurrent), streamDl(&fileDl);
    actual.major = streamCurrent.readLine().toInt();
    actual.minor = streamCurrent.readLine().toInt();
    actual.debug = streamCurrent.readLine().toInt();
    dl.major = streamDl.readLine().toInt();
    dl.minor = streamDl.readLine().toInt();
    dl.debug = streamDl.readLine().toInt();

    if (dl.major > actual.major) return true;
    if (dl.major < actual.major) return false;
    if (dl.minor > actual.minor) return true;
    if (dl.minor < actual.minor) return false;
    if (dl.debug > actual.debug) return true;
    if (dl.debug < actual.debug) return false;
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (dtodoConfig.tray)
    {
        if (trayIcon->isVisible()) 
        {
            trayIcon->showMessage(tr("Information"), tr("dTodo est maintenant verrouillé dans la barre de notifictation"), QSystemTrayIcon::Information, 5000);
            hide();
            event->ignore();
        }
    }
}

void MainWindow::readDatabase(bool popup)
{
    #if defined(linux)
    QString dirPath (getenv("HOME"));
    dirPath += "/.dtodo";
    #elif defined(WIN32) || defined(WIN64)
    QString dirPath (getenv("APPDATA"));
    dirPath += "\\dtodo";
    #endif
    QDir dirConfig(dirPath);
    dirConfig.setFilter( QDir::Files);
    QFileInfoList listEntryToCheck = dirConfig.entryInfoList();

    modeleTodo = new QStandardItemModel;
    modeleTodo->setHorizontalHeaderLabels(QStringList (tr("À faire")));
    modeleDone = new QStandardItemModel;
    modeleDone->setHorizontalHeaderLabels(QStringList (tr("Fait")));

    oldDtodo.clear();
    dayDtodo.clear();
    currentDtodo.clear();
    for (int i = 0; i < listEntryToCheck.size(); i++) {
        QFileInfo fileInfo = listEntryToCheck.at(i);
        if (fileInfo.suffix() == "xml" && fileInfo.baseName() != "config")
        {
            cout << "read " << fileInfo.absoluteFilePath().toStdString()  << "... ";
            readXml(fileInfo.absoluteFilePath());
        }
    }

    viewTodo->setModel(modeleTodo);
    viewDoneTodo->setModel(modeleDone);
    
    if (popup)
    {
        WidgetDialog *widget = createWidgetDialog();
        widget->showDtodoPopUp(dayDtodo, oldDtodo);
        widget->exec();
    }
}

void MainWindow::readXml(QString path)
{
    QDomDocument domCurrentDtodo("dtodo_xml");
    QFile xmlDtodo(path);
    if(!xmlDtodo.open(QIODevice::ReadOnly))
    {
        cout << " Erreur de lecture" << endl;
        return;
    }
    if (!domCurrentDtodo.setContent(&xmlDtodo)) 
    {
        xmlDtodo.close();
        cout << " Erreur assignement Xml" << endl;
        return;
    }
    QDomElement domCurrentDtodoElement = domCurrentDtodo.documentElement();
    QDomNode domCurrentDtodoNoeud = domCurrentDtodoElement.firstChild();
    QStandardItem *item = new QStandardItem;
    item->setEditable(false);
    dTodo tempDtodo;
    QString tempYear, tempMonth, tempDay, tempHour, tempMinute;
    while(!domCurrentDtodoNoeud.isNull())
    {
         QDomElement element = domCurrentDtodoNoeud.toElement();
         if (element.tagName() == "name") {item->setText(element.text()); tempDtodo.name = element.text();}
         else if (element.tagName() == "timehour") tempHour = element.text();
         else if (element.tagName() == "timehour") tempMinute = element.text();
         else if (element.tagName() == "prioritie") 
         {
             QStandardItem *temp = new QStandardItem(tr("Priorite: ") + element.text());
             tempDtodo.prioritie = element.text();
             temp->setEditable(false);
             item->appendRow(temp);
         }
         else if (element.tagName() == "description") tempDtodo.description = element.text();
         else if (element.tagName() == "datestring")
         {
             QStandardItem *temp = new QStandardItem(tr("Date: ") + element.text());
             temp->setEditable(false);
             item->appendRow(temp);
         }
         else if (element.tagName() == "dateyear") tempYear = element.text();
         else if (element.tagName() == "repeat") tempDtodo.repeat = element.text().toInt();
         else if (element.tagName() == "datemonth") tempMonth = element.text();
         else if (element.tagName() == "dateday") tempDay = element.text();
         else if (element.tagName() == "done")  
         {
             if (element.text() == "true") tempDtodo.done = true;
             else tempDtodo.done = false;
         }
         domCurrentDtodoNoeud = domCurrentDtodoNoeud.nextSibling();
    }
    if (tempDtodo.done) modeleDone->appendRow(item);
    else modeleTodo->appendRow(item);
    tempDtodo.time.setHMS(tempHour.toInt(), tempMinute.toInt(), 0);
    tempDtodo.date.setDate (tempYear.toInt(), tempMonth.toInt(), tempDay.toInt());
    tempDtodo.file = path;

    addItemToList(tr("Heure: ") + tempDtodo.time.toString("hh:mm"), item);

    if (currentDate == tempDtodo.date && !tempDtodo.done) dayDtodo.push_back(tempDtodo.name); 
    else if (currentDate >= tempDtodo.date && !tempDtodo.done) 
    {
        oldDtodo.push_back(tempDtodo.name);
        tempDtodo.date = tempDtodo.date.addDays(tempDtodo.repeat);
        WidgetDialog::writeDtodoInFileStruct(&tempDtodo);
    }
    xmlDtodo.close();
    currentDtodo.push_back(tempDtodo);
    cout << "done" << endl;
}

void MainWindow::addItemToList(QString name, QStandardItem *item)
{
    QStandardItem *temp = new QStandardItem(name);
    temp->setEditable(false);
    item->appendRow(temp);
}

void MainWindow::checkConfig()
{
    #if defined(linux)
    QString dirPath (getenv("HOME"));
    dirPath += "/.dtodo";
    QString sep ("/");
    #elif defined(WIN32) || defined(WIN64)
    QString dirPath (getenv("APPDATA"));
    dirPath += "\\dtodo";
    QString sep ("\\");
    #endif
    QDir dirConfig(dirPath);
    if (!dirConfig.exists()) dirConfig.mkdir(dirPath);
    #if defined(linux)
    if (!QFile::exists(dirPath + "/config.xml")) QFile::copy("/usr/share/conf/config.xml",  dirPath + "/config.xml");
    #elif defined(WIN32) || defined(WIN64)
    if (!QFile::exists(dirPath + sep + "config.xml")) QFile::copy("..\\share\\conf\\config.xml",  dirPath + "\\config.xml");
    #endif
    QDomDocument domCurrentDtodo("dtodo_xml");
    QFile xmlDtodo(dirPath + sep + "config.xml");
    if(!xmlDtodo.open(QIODevice::ReadOnly))
    {
	cout << " Erreur de lecture" << endl;
        return;
    }
    if (!domCurrentDtodo.setContent(&xmlDtodo)) 
    {
        xmlDtodo.close();
        cout << " Erreur assignement Xml" << endl;
        return;
    }
    QDomElement domCurrentDtodoElement = domCurrentDtodo.documentElement();
    QDomNode domCurrentDtodoNoeud = domCurrentDtodoElement.firstChild();
    while(!domCurrentDtodoNoeud.isNull())
    {
         QDomElement element = domCurrentDtodoNoeud.toElement();
         if (element.tagName() == "langue")  dtodoConfig.langue = element.text();
         else if (element.tagName() == "locales")  dtodoConfig.locales = element.text();
         else if (element.tagName() == "tray")  
         {
             if (element.text() == "true") dtodoConfig.tray = true;
             else dtodoConfig.tray = false;
         }
         else if (element.tagName() == "maj")  
         {
             if (element.text() == "true") dtodoConfig.maj = true;
             else dtodoConfig.maj = false;
         }
         domCurrentDtodoNoeud = domCurrentDtodoNoeud.nextSibling();
    }

    changeLangue(dtodoConfig.locales);
}

void MainWindow::markDo()
{
    QItemSelectionModel *selection = viewTodo->selectionModel();
    QModelIndex indexElementSelectionne = selection->currentIndex();
    QVariant elementSelectionne = modeleTodo->data(indexElementSelectionne, Qt::DisplayRole);

	if (elementSelectionne.toString().isNull()) return;
    int select = 0;
    for (unsigned int a = 0; a < currentDtodo.size(); a++)
    {
        if (currentDtodo[a].name == elementSelectionne) {select = a; break;}
    }

    currentDtodo[select].done = true;
    WidgetDialog::writeDtodoInFileStruct(&currentDtodo[select]);
    reconstruct();
    QMessageBox::information(this, tr("Informations"), currentDtodo[select].name + tr(" marqué comme fait"));
}

void MainWindow::deleteDtodo()
{
    QString textSelect = isSelection();
    if(!textSelect.isEmpty())
    {
        int reponse = QMessageBox::question(this, tr("Attention"), tr("Êtes vous sur de vouloir supprimer ") + textSelect + " ?", QMessageBox::Yes | QMessageBox::No);
        if (reponse == QMessageBox::Yes)
        {
            #if defined(linux)
            QString dirPath (getenv("HOME"));
            dirPath += "/.dtodo/";
            #elif defined(WIN32) || defined(WIN64)
            QString dirPath (getenv("APPDATA"));
            dirPath += "\\dtodo\\";
            #endif
            QString nameFile; int a = 0;
            QString nameOfDtodo(textSelect);
            for (int i=0; i < nameOfDtodo.size(); i++)
            {
                if (nameOfDtodo[i] != ' ')
                {
                    nameFile.insert(a,nameOfDtodo[i]);
                    a++;
                }
            }
            remove((dirPath + nameFile.toLower() + ".xml").toStdString().c_str());
            readDatabase(false);
            delete trayIcon;
            createTrayIcon();
        }

    }
}

void MainWindow::reconstruct()
{
    delete viewTodo; 
    delete viewDoneTodo;

    viewTodo = new QTreeView;
    viewTodo->setContextMenuPolicy(Qt::CustomContextMenu);
    viewDoneTodo = new QTreeView;
    viewDoneTodo->setContextMenuPolicy(Qt::CustomContextMenu);

    readDatabase(false);

    mainLayout->addWidget(viewTodo);
    mainLayout->addWidget(viewDoneTodo);

    delete trayIcon;
    createTrayIcon();

    connect(viewDoneTodo, SIGNAL(clicked(QModelIndex)), this, SLOT(clearSelectionTodo()));
    connect(viewTodo, SIGNAL(clicked(QModelIndex)), this, SLOT(clearSelectionDone()));
    connect(viewDoneTodo, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(menuRight(QPoint)));
    connect(viewTodo, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(menuRight(QPoint)));
}

QString MainWindow::isSelection()
{
    QItemSelectionModel *selection = viewTodo->selectionModel();
    QModelIndex indexElementSelectionne = selection->currentIndex();
    QVariant elementSelectionne = modeleTodo->data(indexElementSelectionne, Qt::DisplayRole);
    if(!elementSelectionne.toString().isNull()) return elementSelectionne.toString();

    QItemSelectionModel *selectionDone = viewDoneTodo->selectionModel();
    QModelIndex indexElementSelectionneDone = selectionDone->currentIndex();
    QVariant elementSelectionneDone = modeleDone->data(indexElementSelectionneDone, Qt::DisplayRole);
    if(!elementSelectionneDone.toString().isNull()) return elementSelectionneDone.toString();

    return "";
}

void MainWindow::changeLangue(QString langue)
{
    QTranslator *translator = new QTranslator( 0 );
    #if defined(linux)
    translator->load(QString("dtodo_") + langue + ".qm", "/usr/share/dtodo/langue");
    #elif defined(WIN32) || defined(WIN64)
    translator->load(QString("dtodo_") + langue + ".qm", "..\\share\\langue");
    #endif
    qApp->installTranslator(translator);
}

WidgetDialog *MainWindow::createWidgetDialog()
{
    WidgetDialog *widgetapp = new WidgetDialog(this);
    return widgetapp;
}

void MainWindow::showDtodo()
{
    QString textSelect = isSelection();
    if(!textSelect.isEmpty()) 
    {
        WidgetDialog *widget = createWidgetDialog();
        widget->showDtodo(currentDtodo, textSelect);
        widget->exec();
    }
}

void MainWindow::modifieDtodo()
{
    QString textSelect = isSelection();
    if(textSelect.isEmpty()) return;
    int select = 0;

    for (unsigned int a = 0; a < currentDtodo.size(); a++)
    {
        if (currentDtodo[a].name == textSelect) {select = a; break;}
    }

    
    WidgetDialog *widget = createWidgetDialog();
    widget->newDtodo(true, &currentDtodo[select]);
    widget->exec();

    if (widget->result() == QDialog::Accepted) reconstruct();
}

void MainWindow::createNewDtodo()
{
    WidgetDialog *widget = createWidgetDialog();
    widget->newDtodo();
    widget->exec();
    if (widget->result() == QDialog::Accepted) reconstruct();
}

void MainWindow::changePreference()
{
    WidgetDialog *widget = createWidgetDialog();
    widget->preferenceWidget(&dtodoConfig);
    widget->exec();
}

void MainWindow::about()
{
    WidgetDialog *widget = createWidgetDialog();
    widget->about();
    widget->show();
}

