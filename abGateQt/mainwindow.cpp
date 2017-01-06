#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    designer_ui(new Ui::MainWindow)
{
    designer_ui->setupUi(this);

    defaultKnobParams();
    KnobStyle = NULL;
    updateKnobStyle();

}

MainWindow::~MainWindow()
{
    delete designer_ui;
    if (KnobStyle) { delete KnobStyle; }
}


void MainWindow::defaultKnobParams()
{
    bypass_bool = BYPASS_DEFAULT > 0 ? true : false;
    designer_ui->bypass->setChecked(bypass_bool);

    designer_ui->threshold->setMouseTracking(false);
    designer_ui->threshold->setMinimum(THRESHOLD_MIN);
    designer_ui->threshold->setMaximum(THRESHOLD_MAX);
    designer_ui->threshold->setValue(THRESHOLD_DEFAULT);

    designer_ui->attack->setMouseTracking(false);
    designer_ui->attack->setMinimum(ATTACK_MIN);
    designer_ui->attack->setMaximum(ATTACK_MAX);
    designer_ui->attack->setValue(ATTACK_DEFAULT);

    designer_ui->hold->setMouseTracking(false);
    designer_ui->hold->setMinimum(HOLD_MIN);
    designer_ui->hold->setMaximum(HOLD_MAX);
    designer_ui->hold->setValue(HOLD_DEFAULT);

    designer_ui->decay->setMouseTracking(false);
    designer_ui->decay->setMinimum(DECAY_MIN);
    designer_ui->decay->setMaximum(DECAY_MAX);
    designer_ui->decay->setValue(DECAY_DEFAULT);

    designer_ui->range->setMouseTracking(false);
    designer_ui->range->setMinimum(RANGE_MIN);
    designer_ui->range->setMaximum(RANGE_MAX);
    designer_ui->range->setValue(RANGE_DEFAULT);
}

void MainWindow::updateKnobStyle()
{
    KnobStyleNum style = KnobStyleNum(abGate);
    if (KnobStyle) {
        delete KnobStyle;
    }
    switch (style) {
    case Classic:
        //KnobStyle = new DialClassicStyle();
        KnobStyle = NULL;
        break;
    case abGate:
        KnobStyle = new abGateStyle();
        break;
    default:
        KnobStyle = NULL;
        break;
    }
    // Change the style for all of the dials
     QList<QDial *> all_dials = findChildren<QDial *>();
    foreach(QDial* knob, all_dials) {
        knob->setStyle(KnobStyle);
        // Show nothes
        if (style != abGate) { knob->setNotchesVisible(true); } else { knob->setNotchesVisible(false); }
    }

    //ui->decay->setStyle(KnobStyle);
}

// Informing GUI about changes in the control ports
void MainWindow::gui_port_event(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void * buffer) {

        float val;
        val = * static_cast<const float*>(buffer);

    // Checking if params are the same as specified in the LV2 documentation
    if (format != 0) { return; }
    if (buffer_size != 4) { return; }

    // Updating values for GUI changes

        switch(port_index){
            case p_switch:
                bypass_bool = val > 0 ? true : false;
                designer_ui->bypass->setChecked(bypass_bool);
                //bypass->set_toggle_value(val);
                break;
            case p_threshold:
                designer_ui->threshold->setValue(val);
                break;
            case p_attack:
                designer_ui->attack->setValue(val);
                break;
            case p_hold:
                designer_ui->hold->setValue(val);
                break;
            case p_decay:
                designer_ui->decay->setValue(val);
                break;
            case p_gaterange:
                designer_ui->range->setValue(val);
                break;
        default:
            return;
    }


}

void MainWindow::write_control(uint32_t port_index, float value) {
    write_function(controller, port_index, 4, 0, &value);
}

void MainWindow::on_threshold_actionTriggered(int action)
{
    write_control(p_threshold, designer_ui->threshold->value());
}

void MainWindow::on_threshold_dialPressed()
{
     write_control(p_threshold, designer_ui->threshold->value());
}

void MainWindow::on_threshold_dialReleased()
{
     write_control(p_threshold, designer_ui->threshold->value());
}

void MainWindow::on_attack_actionTriggered(int action)
{
    write_control(p_attack, designer_ui->attack->value());
}

void MainWindow::on_attack_dialPressed()
{
    write_control(p_attack, designer_ui->attack->value());
}

void MainWindow::on_attack_dialReleased()
{
    write_control(p_attack, designer_ui->attack->value());
}

void MainWindow::on_hold_actionTriggered(int action)
{
    write_control(p_hold, designer_ui->hold->value());
}

void MainWindow::on_hold_dialPressed()
{
    write_control(p_hold, designer_ui->hold->value());
}

void MainWindow::on_hold_dialReleased()
{
    write_control(p_hold, designer_ui->hold->value());
}

void MainWindow::on_decay_actionTriggered(int action)
{
    write_control(p_decay, designer_ui->decay->value());
}

void MainWindow::on_decay_dialPressed()
{
    write_control(p_decay, designer_ui->decay->value());
}

void MainWindow::on_decay_dialReleased()
{
    write_control(p_decay, designer_ui->decay->value());
}

void MainWindow::on_range_actionTriggered(int action)
{
    write_control(p_gaterange, designer_ui->range->value());
}

void MainWindow::on_range_dialPressed()
{
    write_control(p_gaterange, designer_ui->range->value());
}

void MainWindow::on_range_dialReleased()
{
    write_control(p_gaterange, designer_ui->range->value());
}

void MainWindow::on_bypass_toggled(bool checked)
{
    bypass_float = checked != true ? 0 : 1;
    write_control(p_switch, bypass_float);
}
