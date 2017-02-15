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

#include "WidgetDialog.h"

WidgetDialog::WidgetDialog(QWidget *parent = 0): QDialog(parent)
{
    layoutMain = new QGridLayout;
    setLayout(layoutMain);
}

void WidgetDialog::newDtodo(bool change, dTodo *currentDtodo)
{
    setMinimumSize(450, 250);

    setWindowTitle(tr("Nouveau dTodo"));
    informationLayout = new QFormLayout;

    dateLayout = new QGridLayout;

    nameOfNewDtodo = new QLineEdit;
    if(change) nameOfNewDtodo->setText(currentDtodo->name);

    listPriorities = new QComboBox;
    listPriorities->addItem("Normal");
    listPriorities->addItem(tr("Importante"));
    listPriorities->addItem(tr("Faible"));

    informationLayout->addRow(tr("Nom du dTodo :"), nameOfNewDtodo);
    informationLayout->addRow(tr("Priorité :"), listPriorities);

    groupBoxInformations = new QGroupBox("Informations");
    groupBoxInformations->setLayout(informationLayout);

    calendar = new QCalendarWidget;
    calendar->setMinimumDate(QDate(1900, 1, 1));
    calendar->setMaximumDate(QDate(3000, 1, 1));
    calendar->setFirstDayOfWeek(Qt::Monday);
    calendar->setGridVisible(true);
    if(change) calendar->setSelectedDate(currentDtodo->date);

    timeDtodo = new QTimeEdit();
    if(change) timeDtodo->setTime(currentDtodo->time);

    dateLayout->addWidget(timeDtodo);
    dateLayout->addWidget(calendar);

    groupBoxDate = new QGroupBox(tr("Date et heure d'échéanche"));
    groupBoxDate->setLayout(dateLayout);

    descriptionLayout = new QGridLayout;
	
    descriptionText = new QTextEdit();
    if(change) descriptionText->setText(currentDtodo->description);
    descriptionLayout->addWidget(descriptionText);
	
    groupBoxDescription = new QGroupBox("Description");
    groupBoxDescription->setLayout(descriptionLayout);
	
    repeatLayout = new QFormLayout;
	
    repeatTime = new QSpinBox;
    if(change) repeatTime->setValue(currentDtodo->repeat);
    repeatLayout->addRow(tr("Tous les (x) jours:"), repeatTime);
	
    groupBoxRepeat = new QGroupBox(tr("Répéter"));
    groupBoxRepeat->setCheckable(true);
    groupBoxRepeat->setLayout(repeatLayout);
	
    if (change) acceptButton = new QPushButton(tr("Modifier"));
    else acceptButton = new QPushButton(tr("Créer"));
    cancelButton = new QPushButton(tr("Annuler"));

    layoutMain->addWidget(groupBoxInformations,0,0,1,2); 
    layoutMain->addWidget(groupBoxDescription,1,0,1,2);
    layoutMain->addWidget(groupBoxRepeat,2,0,1,2);
    layoutMain->addWidget(groupBoxDate,3,0,1,2);
    layoutMain->addWidget(acceptButton,4,0);
    layoutMain->addWidget(cancelButton,4,1);

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(checkAndSetNewDtodo()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void WidgetDialog::about()
{
    setMinimumSize(430, 250);
    setWindowTitle(tr("A propos de dTodo"));
    acceptButton = new QPushButton("Ok");

    onglets = new QTabWidget;
    onglets->setGeometry(10, 10, 470, 200);

    /*** First Page ***/

    firstPage = new QWidget;

    firstLayout = new QGridLayout;
    
    QLabel *icone = new QLabel;
    icone->setAlignment(Qt::AlignHCenter);
    icone->setPixmap(QPixmap(":/images/icone.png"));
    QLabel *version = new QLabel("dTodo 1.0.2");
    QLabel *description = new QLabel(tr("dTodo va vous permettre de lister vos tâches à faire et vous les rappellera"));
    QLabel *website = new QLabel(tr("Site web: <a href=\"http://darksidious77.free.fr\">http://darksidious77.free.fr</a>"));
    QLabel *contact = new QLabel(tr("Contact: darksidious77@gmail.com"));
    QLabel *remerciement = new QLabel(tr("Remerciement: gaut974 pour les images"));
    QLabel *copyright = new QLabel("Copyright (C) 2009, 2010 Dark Sidious");

    firstLayout->addWidget(icone,0,0);
    firstLayout->addWidget(version,1,0);
    firstLayout->addWidget(description,2,0);
    firstLayout->addWidget(website,3,0);
    firstLayout->addWidget(contact,4,0);
    firstLayout->addWidget(remerciement,5,0);
    firstLayout->addWidget(copyright,6,0);

    firstPage->setLayout(firstLayout);

    /*** Second Page ***/

    secondPage = new QWidget;

    secondLayout = new QGridLayout;
    
    licenceText = new QTextEdit();
    
    #if defined(linux)
    QFile licence("/usr/share/dtodo/LICENCE");
    #elif defined(WIN32) || defined(WIN64)
    QFile licence("..\\share\\LICENCE");
    #endif
    
    if(licence.open(QIODevice::ReadOnly | QIODevice::Text))
    {
         licenceText->setPlainText(licence.readAll());
         licence.close();
    }
    licenceText->setReadOnly(true);

    secondLayout->addWidget(licenceText);
    secondPage->setLayout(secondLayout);

    /*** Second Page ***/

    thirdPage = new QWidget;

    thirdLayout = new QGridLayout;
    
    changelogText = new QTextEdit();

    #if defined(linux)
    QFile changelog("/usr/share/dtodo/CHANGELOG");
    #elif defined(WIN32) || defined(WIN64)
    QFile changelog("..\\share\\CHANGELOG");
    #endif

    if(changelog.open(QIODevice::ReadOnly | QIODevice::Text))
    {
         changelogText->setPlainText(changelog.readAll());
         changelog.close();
    }
    changelogText->setReadOnly(true);

    thirdLayout->addWidget(changelogText);
    thirdPage->setLayout(thirdLayout);

    onglets->addTab(firstPage, tr("General"));
    onglets->addTab(secondPage, tr("Licence"));
    onglets->addTab(thirdPage, tr("ChangeLog"));
    
    layoutMain->addWidget(onglets,0,0);
    layoutMain->addWidget(acceptButton,1,0);

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void WidgetDialog::preferenceWidget(config *dtodoConfig)
{
    setMinimumSize(250, 150);
    setWindowTitle(tr("Préférences"));

    acceptButton = new QPushButton(tr("Valider"));
    cancelButton = new QPushButton(tr("Annuler"));

    generalLayout = new QGridLayout;

    trayCheckbox = new QCheckBox(tr("Icone dans la barre des tâches"));
    trayCheckbox->setChecked(dtodoConfig->tray);

    majCheckbox = new QCheckBox(tr("Vérifier les mises à jour au démarrage"));
    majCheckbox->setChecked(dtodoConfig->maj);

    generalLayout->addWidget(majCheckbox);
    generalLayout->addWidget(trayCheckbox);

    groupBoxGeneral = new QGroupBox(tr("Général"));
    groupBoxGeneral->setLayout(generalLayout);

    languesLayout = new QGridLayout;

    listLangues = new QComboBox;
    listLangues->addItem(tr("Francais"));
    listLangues->addItem(tr("Anglais"));
    languesLayout->addWidget(listLangues);

    groupBoxlangues = new QGroupBox(tr("Langues"));
    groupBoxlangues->setLayout(languesLayout);

    layoutMain->addWidget(groupBoxGeneral,0,0,1,2);
    layoutMain->addWidget(groupBoxlangues,1,0,1,2);
    layoutMain->addWidget(acceptButton,2,0);
    layoutMain->addWidget(cancelButton,2,1);

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(saveConfig()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void WidgetDialog::writeDtodoInFile(QString name, QString prioritie, QString description, QDate date, QTime time, QString file, bool done, int repeat)
{
    QFile fileToSaveDtodo(file);
    fileToSaveDtodo.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream fluxToWrite(&fileToSaveDtodo);
    fluxToWrite << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    fluxToWrite << "<dtodo>" << "\n";
    fluxToWrite << "<name>" << name << "</name>" << "\n";
    fluxToWrite << "<prioritie>" << prioritie << "</prioritie>" << "\n";
    fluxToWrite << "<datestring>";
    if (date.day() > 9) fluxToWrite << date.day();
    else fluxToWrite << "0" << date.day();
    fluxToWrite << "/";
    if (date.month() > 9) fluxToWrite << date.month();
    else fluxToWrite << "0" << date.month();
    fluxToWrite << "/" << date.year() << "</datestring>\n";
    if (date.day() > 9) fluxToWrite << "<dateday>" << date.day() << "</dateday>" << "\n";
    else fluxToWrite << "<dateday>" << "0" << date.day() << "</dateday>" << "\n";
    if (date.month() > 9) fluxToWrite << "<datemonth>" << date.month() << "</datemonth>" << "\n";
    else fluxToWrite << "<datemonth>" << "0" << date.month() << "</datemonth>" << "\n";
    fluxToWrite << "<dateyear>" << date.year() << "</dateyear>" << "\n";
    fluxToWrite << "<timehour>" << time.hour() << "</timehour>" << "\n";
    fluxToWrite << "<timeminute>" << time.minute() << "</timeminute>" << "\n";
    fluxToWrite << "<description>" << description << "</description>" << "\n";
    fluxToWrite << "<repeat>" << repeat << "</repeat>" << "\n";
    if (done == true) fluxToWrite << "<done>true</done>" << "\n";
    else fluxToWrite << "<done>false</done>" << "\n";
    fluxToWrite << "</dtodo>" << "\n";
    fileToSaveDtodo.close();
}

void WidgetDialog::checkAndSetNewDtodo()
{
    if(nameOfNewDtodo->text().isEmpty())
        QMessageBox::warning(this, tr("Attention"), tr("Veuillez indiquer un nom pour le nouveau dTodo !"));
    else
    {
        int a = 0;
        #if defined(linux)
        QString dirPath (getenv("HOME"));
        dirPath += "/.dtodo/";
        #elif defined(WIN32) || defined(WIN64)
        QString dirPath (getenv("APPDATA"));
        dirPath += "\\dtodo\\";
        #endif
        QString nameFile;
        QString nameOfDtodo(nameOfNewDtodo->text());
        if (!groupBoxRepeat->isChecked()) repeatTime->setValue(0);
        for (int i=0; i < nameOfDtodo.size(); i++)
        {
            if (nameOfDtodo[i] != ' ')
            {
                nameFile.insert(a,nameOfDtodo[i]);
                a++;
            }
        }
		writeDtodoInFile(nameOfNewDtodo->text(), listPriorities->currentText(), descriptionText->toPlainText(), calendar->selectedDate(), timeDtodo->time(), dirPath + nameFile.toLower() + ".xml", false, repeatTime->value());
        accept();
    }
}

void WidgetDialog::showDtodo(vector<dTodo> & currentDtodo, QString elementSelectionne)
{
    int select = 0;
    for (unsigned int a = 0; a < currentDtodo.size(); a++)
    {
        if (currentDtodo[a].name == elementSelectionne) {select = a; break;}
    }

    setWindowTitle(tr("Information"));
    
    setMinimumSize(250, 150);

    QLabel *name = new QLabel(tr("Nom: ") + currentDtodo[select].name);
    QLabel *prioritie = new QLabel(tr("Priorité: ") + currentDtodo[select].prioritie);
    QLabel *date = new QLabel(tr("Date: ") + currentDtodo[select].date.toString("dd.MM.yyyy"));
    QLabel *hour = new QLabel(tr("Heure: ") + currentDtodo[select].time.toString("hh:mm"));
    QLabel *repetition = new QLabel(tr("Répétition: tous les ") + QString::number(currentDtodo[select].repeat) + tr(" jour(s)"));
    QLabel *description = new QLabel(tr("Description: ") + currentDtodo[select].description);
    acceptButton = new QPushButton("Ok");

    layoutMain->addWidget(name,0,0);
    layoutMain->addWidget(prioritie,1,0);
    layoutMain->addWidget(date,2,0);
    layoutMain->addWidget(hour,3,0);
    layoutMain->addWidget(repetition,4,0);
    layoutMain->addWidget(description,5,0);
    if(currentDtodo[select].done) { QLabel *stat = new QLabel(tr("Etat: Fait")); layoutMain->addWidget(stat,6,0); }
    else { QLabel *stat = new QLabel(tr("Etat: A Faire")); layoutMain->addWidget(stat,6,0); }
    layoutMain->addWidget(acceptButton,7,0);

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void WidgetDialog::showDtodoPopUp(vector<QString> & dayDtodo, vector<QString> & oldDtodo)
{
    setWindowTitle(tr("Informations"));

    setWindowFlags(Qt::Tool);

    acceptButton = new QPushButton("Ok");

    QString dayText( QString::number(dayDtodo.size()) + tr(" dTodo(s) aujourd'hui"));
    for (unsigned int i=0; i < dayDtodo.size(); i++) dayText += "\n  " + dayDtodo[i];
    QString oldText( QString::number(oldDtodo.size()) + tr(" dTodo(s) en retard"));
    for (unsigned int i=0; i < oldDtodo.size(); i++) oldText += "\n  " + oldDtodo[i];

    QLabel *day = new QLabel(dayText);
    QLabel *old = new QLabel(oldText);

    layoutMain->addWidget(day,0,0);
    layoutMain->addWidget(old,1,0);
    layoutMain->addWidget(acceptButton,2,0);

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void WidgetDialog::writeDtodoInFileStruct(dTodo *dtodoToWrite)
{
    writeDtodoInFile(dtodoToWrite->name, dtodoToWrite->prioritie, dtodoToWrite->description, dtodoToWrite->date, dtodoToWrite->time, dtodoToWrite->file, dtodoToWrite->done, dtodoToWrite->repeat);
}

void WidgetDialog::saveConfig()
{
    #if defined(linux)
    QString dirPath (getenv("HOME"));
    dirPath += "/.dtodo/";
    #elif defined(WIN32) || defined(WIN64)
    QString dirPath (getenv("APPDATA"));
    dirPath += "\\dtodo\\";
    #endif
    QFile fileToSaveDtodo(dirPath + "config.xml");
    fileToSaveDtodo.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream fluxToWrite(&fileToSaveDtodo);
    fluxToWrite << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    fluxToWrite << "<config>" << "\n";
    fluxToWrite << "<langue>" << listLangues->currentText() << "</langue>" << "\n";
    if (listLangues->currentText() == "Francais" || listLangues->currentText() == "French")
         fluxToWrite << "<locales>fr</locales>" << "\n";
    else if (listLangues->currentText() == "Anglais" || listLangues->currentText() == "English")
         fluxToWrite << "<locales>en</locales>" << "\n";
    fluxToWrite << "<minimize>true</minimize>" << "\n";
    if (trayCheckbox->isChecked())
        fluxToWrite << "<tray>true</tray>" << "\n";
    else
        fluxToWrite << "<tray>false</tray>" << "\n";
    if (majCheckbox->isChecked())
        fluxToWrite << "<maj>true</maj>" << "\n";
    else
        fluxToWrite << "<maj>false</maj>" << "\n";
    fluxToWrite << "</config>" << "\n";
    fileToSaveDtodo.close();
    accept();
}

