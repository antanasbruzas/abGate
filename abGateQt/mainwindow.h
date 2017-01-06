#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <lv2.h>

#include "abgatestyle.h"
#include "ui.h"
#include "gate_const.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum KnobStyleNum { Classic, abGate };

    // Informing GUI about changes in the control ports
    void gui_port_event(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void * buffer);

    LV2UI_Controller controller;
    LV2UI_Write_Function write_function;

    void write_control(uint32_t port_index, float value);

protected:
    void updateKnobStyle();


private slots:
    void on_threshold_actionTriggered(int action);

    void on_threshold_dialPressed();

    void on_threshold_dialReleased();

    void on_attack_actionTriggered(int action);

    void on_attack_dialPressed();

    void on_attack_dialReleased();

    void on_hold_actionTriggered(int action);

    void on_hold_dialPressed();

    void on_hold_dialReleased();

    void on_decay_actionTriggered(int action);

    void on_decay_dialPressed();

    void on_decay_dialReleased();

    void on_range_actionTriggered(int action);

    void on_range_dialPressed();

    void on_range_dialReleased();

    void on_bypass_toggled(bool checked);

private:
    Ui::MainWindow *designer_ui;
    // Style for QDial
    QStyle* KnobStyle;
    bool bypass_bool;
    float bypass_float;
    void defaultKnobParams();
};

#endif // MAINWINDOW_H
