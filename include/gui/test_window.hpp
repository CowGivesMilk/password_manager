#ifndef TEST_WINDOW_HPP
#define TEST_WINDOW_HPP
#include <QMainWindow>

namespace Ui {
class TestWindow;
}

class TestWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit TestWindow(QWidget *parent = nullptr);
  ~TestWindow();

 private:
  Ui::TestWindow *ui;
};
#endif