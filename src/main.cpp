#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QMainWindow>

#include "main.h"

#include "library.hpp"
#include "savefiles.hpp"

#include "ui_Saveeditor_MainWindow.h"

static PathfinderKingmaker::SaveEditor::SaveFile* save;

static void saveFunc() {
    save->save();
}


int main(int argc, char* argv[]) {

    auto* app = new QApplication(argc, argv);
    QMainWindow* mainWindow = new QMainWindow;

    Ui::MainWindow ui;
    ui.setupUi(mainWindow);

    save = new PathfinderKingmaker::SaveEditor::SaveFile();

    /*try {
        save->open("Manual_65_Old_Sycamore___24_Gozran__IV__4710___22_19_31.zks");
        std::cout << save->getPlayerJson()["Encumbrance"].GetString() << std::endl;

        save->getPlayerJson()["Encumbrance"] = "Heavy";
    
        std::cout << save->getPlayerJson()["Encumbrance"].GetString() << std::endl;
    
        save->save();
    } catch (std::runtime_error &ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    } catch (std::invalid_argument &ex) {
        std::cerr << ex.what() << std::endl;
        return -2;
    }*/


    

    //sprint(*save.getPlayerJson());

    QObject::connect(ui.actionExit, &QAction::triggered, mainWindow, &QMainWindow::close);
    QObject::connect(ui.actionSave, &QAction::triggered, mainWindow, &saveFunc);

    mainWindow->show();
	return app->exec();
}