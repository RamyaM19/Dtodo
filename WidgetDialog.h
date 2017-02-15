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

#ifndef HEADER_WIDGETDIALOG
#define HEADER_WIDGETDIALOG

#include <QtGui>
#include <iostream>
#include "Struct.h"

using namespace std;

class WidgetDialog : public QDialog
{
	Q_OBJECT
	public:
		WidgetDialog(QWidget *parent);
		void newDtodo(bool change = false, dTodo *currentDtodo = NULL);
		void about();
		void showDtodo(vector<dTodo> & currentDtodo, QString elementSelectionne);
		void showDtodoPopUp(vector<QString> & dayDtodo, vector<QString> & oldDtodo);
		void preferenceWidget(config *dtodoConfig);
		static void writeDtodoInFileStruct(dTodo *dtodoToWrite);
		static void writeDtodoInFile(QString name, QString prioritie, QString description, QDate date, QTime time, QString file, bool done, int repeat);

	private slots:
		void checkAndSetNewDtodo();
		void saveConfig();

	private:
		QWidget *firstPage;
		QWidget *secondPage;
		QWidget *thirdPage;
		QGridLayout *layoutMain;
		QGridLayout *firstLayout;
		QGridLayout *secondLayout;
		QGridLayout *thirdLayout;
		QFormLayout *informationLayout;
		QGridLayout *descriptionLayout;
		QGridLayout *languesLayout;
		QGridLayout *dateLayout;
		QFormLayout *repeatLayout;
		QGroupBox *groupBoxRepeat;
		QGroupBox *groupBoxInformations;
		QGroupBox *groupBoxDescription;
		QGroupBox *groupBoxDate;
		QGroupBox *groupBoxlangues;
		QComboBox *listPriorities;
		QComboBox *listLangues;
		QPushButton *acceptButton;
		QPushButton *cancelButton;
		QLineEdit *nameOfNewDtodo;
		QCalendarWidget *calendar;
		QTabWidget *onglets;
		QTextEdit *changelogText;
		QTextEdit *licenceText;
		QTextEdit *descriptionText;
		QGridLayout *generalLayout;
		QGroupBox *groupBoxGeneral;
		QCheckBox *trayCheckbox;
		QCheckBox *majCheckbox;
		QSpinBox *repeatTime;
		QTimeEdit *timeDtodo;
};


#endif
