#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QTranslator>

#include "save_editor/main.h"
#include "save_editor/savefiles.hpp"

#include "common/common.hpp"


#include "ui_SaveEditor_MainWindow.h"

//static Borderlands::SaveEditor::SaveFile* save;

static void saveFunc() {
    //save->save();
}


int main(int argc, char* argv[]) {

    auto* app = new QApplication(argc, argv);

    QTranslator translator;
    translator.load(":/translations/save_editor_de");
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
