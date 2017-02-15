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

typedef struct config config;
struct config
{
    QString langue;
    QString locales;
    bool minimize;
    bool tray;
    bool maj;
};

typedef struct dTodo dTodo;
struct dTodo
{
    QString name;
    QString prioritie;
    QString description;
    int repeat;
    QDate date;
    QTime time;
    QString file;
    bool done;
};

typedef struct version version;
struct version
{
    int major;
    int minor;
    int debug;
};

