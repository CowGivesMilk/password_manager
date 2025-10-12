#include "gui/test_window.hpp"

#include "ui_test_window.h"

TestWindow::TestWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TestWindow) {
  ui->setupUi(this);
}

TestWindow::~TestWindow() { delete ui; }
