#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->signsComboBox->addItem(">");
    ui->signsComboBox->addItem("<");
    ui->kbytesSpinBox->setMaximum(1024*1024*1024);
    ui->daysSpinBox->setMaximum(365*10);
    ui->extensionsEdit->setText("*.txt *.png *.xlsx");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_browseButton_clicked()
{
    QString curFilename = QFileDialog::getExistingDirectory(this, tr("Open directory"), "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if(!curFilename.isEmpty()){
        filename = curFilename;
        ui->searchPathEdit->setText(filename);
    }
}

void Widget::on_startButton_clicked()
{
    QString curFilename = ui->searchPathEdit->text();
    if(curFilename == QString() || !QDir(curFilename).exists()){
        QMessageBox::warning(this, "Warning", "Current dir is not exist.", QMessageBox::QMessageBox::Ok);
        return;
    }

    ui->listWidget->clear();

    plugin->filters = ui->extensionsEdit->text().split(" ");

    plugin->withDays = ui->notOlderThanCheckBox->checkState() == Qt::Checked ? true : false;
    plugin->days = plugin->withDays ? ui->daysSpinBox->value() : 0;

    plugin->withSize = ui->sizeFileCheckBox->checkState() == Qt::Checked ? true : false;
    plugin->size = plugin->withSize ? ui->kbytesSpinBox->value() : 0;

    plugin->isLessThen = ui->signsComboBox->currentText() == "<" ? true : false;

    QtConcurrent::run(plugin, &Plugin::findFilterFiles, ui->searchPathEdit->text());
}

QStringList Widget::getPlugins(QString path){
    QStringList filters;
    filters << "*.dll" << "*.so" << "*.dylib";

    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(filters);

    QStringList plugins;

    foreach (QFileInfo file, list) {
        plugins.append(file.filePath());
        qInfo() << "try: " << file.filePath();
    }
    return plugins;
}

void Widget::updateListView(int index){
    ui->listWidget->addItem(plugin->watcher.resultAt(index).filePath());
}

void Widget::testPlugins(QStringList plugins){
    foreach(QString file, plugins){
        qDebug() << "Loading:" << file;

        QPluginLoader loader(file);
        if(!loader.load()){
            qInfo() << "Error:" << loader.fileName() << loader.errorString();
        }

        qInfo() << "Loaded: " << loader.fileName();

        Plugin* plugin_ = qobject_cast<Plugin*>(loader.instance());
        if(plugin_){
            plugin_->test("Jerry");
            plugin = plugin_;
            connect(&plugin->watcher, &QFutureWatcher<QFileInfo>::resultReadyAt, this, &Widget::updateListView);
        }
        else{
            qInfo() << "Plugin " << loader.fileName() << "doesn't support Plugin interface" ;
        }
    }
}

void Widget::on_cancelButton_clicked()
{
    plugin->stop();
}
