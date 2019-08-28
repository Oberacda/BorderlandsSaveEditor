/**
 * @file        main.cpp
 * @author      David Oberacker (david.oberacker@gmail.com)
 * @brief       Borderlands save editor main file.
 * @details     Main file of the borderlands2 save editor application.
 * @version     0.1
 * @date        2019-08-26
 *
 * @copyright   Copyright (c) 2019 David Oberacker
 */

#include <iostream>

#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QTranslator>

#include "main.hpp"
#include "ui_SaveEditor_MainWindow.h"

//static Borderlands::SaveEditor::SaveFile* save;

/**
 * @brief Function to save a loaded savefile again.
 *
 * @warning Function not implemented.
 */
static void saveFunc() {
    //save->save();
}

int main(int argc, char* argv[]) {

    auto* app = new QApplication(argc, argv);

    QTranslator translator;
    translator.load(":/translations/save_editor_en");
    app->installTranslator(&translator);

    QMainWindow* mainWindow = new QMainWindow;

    Ui::MainWindow ui;
    ui.setupUi(mainWindow);

    //save = new Borderlands::SaveEditor::SaveFile();

    QObject::connect(ui.actionExit, &QAction::triggered, mainWindow, &QMainWindow::close);
    QObject::connect(ui.actionSave, &QAction::triggered, mainWindow, &saveFunc);

    mainWindow->show();
	return app->exec();
}
