#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Data File"), "", tr("Data Files (*.csv *.txt);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot open file: %1").arg(file.errorString()));
        return;
    }

    QStandardItemModel *model = new QStandardItemModel(this);
    QTextStream in(&file);
    int row = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (row == 0) {
            model->setColumnCount(fields.size());
            model->setHorizontalHeaderLabels(fields);
        } else {
            QList<QStandardItem*> items;
            for (const QString &field : fields) {
                items.append(new QStandardItem(field));
            }
            model->appendRow(items);
        }
        row++;
    }
    file.close();

    ui->tableView->setModel(model);
}
