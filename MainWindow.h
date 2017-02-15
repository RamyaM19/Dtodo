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

#ifndef HEADER_MAINWINDOW
#define HEADER_MAINWINDOW

#include <QtGui>
#include <QtXml>
#include <QtNetwork>
#include <iostream>
#include <vector>
#include "WidgetDialog.h"

using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow();

	protected:
		void closeEvent(QCloseEvent *event);

	private slots:
		void about();
		void addItemToList(QString name, QStandardItem *item);
		void changePreference();
		void createNewDtodo();
		void deleteDtodo();
		void majDtodoWin();
		void showDtodo();
		void modifieDtodo();
		void markDo();
		void clearSelectionTodo();
		void clearSelectionDone();
		void httpRequestFinished(int requestId, bool error);
		WidgetDialog *createWidgetDialog();
		void activationSystemTray(QSystemTrayIcon::ActivationReason reason);
		void menuRight(QPoint);

	private:
		void checkConfig();
		bool checkMaj();
		void readDatabase(bool popup = true);
		void readXml(QString path);
		void changeLangue(QString langue);
		void createTrayIcon();
		void createMenu();
		void reconstruct();
		QString isSelection();

		QWidget *mainWidget;
		QMenu *menuFile;
		QMenu *menuHelp;
		QMenu *menuLanguage;
		QMenu *menuEdit;
		QMenu *menuTache;
		QMenu *trayIconMenu;
		QAction *actionQuit;
		QAction *actionAbout;
		QAction *actionAboutQt;
		QAction *actionNew;
		QAction *actionDelete;
		QAction *actionFr;
		QAction *actionPreference;
		QAction *actionEn;
		QAction *minimizeAction;
		QAction *restoreAction;
		QAction *quitAction;
		QAction *actionMaj;
		QAction *actionModifie;
		QMenu *dtodoDay;
		QMenu *dtodoOld;
		QAction *actionSee;
		QAction *actionDo;
		QStatusBar *barreStat;
		QVBoxLayout *mainLayout;
		QTreeView *viewTodo;
		QTreeView *viewDoneTodo;
		QStandardItemModel *modeleTodo;
		QStandardItemModel *modeleDone;
		QDate currentDate;
		QDate tempDate;
		vector<QString> dayDtodo;
		vector<QString> oldDtodo;
		config dtodoConfig;
		QSystemTrayIcon *trayIcon;
		QToolBar *toolBar;
		vector<dTodo> currentDtodo;
		QFile *file;
		QHttp *http;
		int httpGetId;
};


#endif

