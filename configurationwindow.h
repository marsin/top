#ifndef CONFIGURATIONWINDOW_H
#define CONFIGURATIONWINDOW_H

#include <QDialog>

namespace Ui {
class ConfigurationWindow;
}

class ConfigurationWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigurationWindow(QWidget *parent = 0);
    ~ConfigurationWindow();
    
private:
    Ui::ConfigurationWindow *ui;
};

#endif // CONFIGURATIONWINDOW_H
