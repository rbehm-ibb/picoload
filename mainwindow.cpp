// ******************************************************
// * copyright (C) 2022 by rbehm@ibb-aviotec.com
// * File: mainwindow.cpp
// * created 2022-11-23 with pro-config V0.2
// ******************************************************

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include "lediconsource.h"

MainWindow::MainWindow(QString srcDir, QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m_hasSrc(false)
	, m_hasPico(false)
{
	ui->setupUi(this);
	ui->srcDir->addAction(ui->actionSrcDir, QLineEdit::TrailingPosition);
	ui->picoDir->addAction(ui->actionPicoDir, QLineEdit::TrailingPosition);
	if (srcDir.isEmpty())
	{
		srcDir = Config::stringValue("src");
	}
	QString pico = Config::stringValue("pico", "/run/media/behm/RPI-RP2/");
	ui->srcDir->setText(srcDir);
	ui->picoDir->setText(pico);
	m_srcDir =  QDir(srcDir, "*.uf2");
	m_dstDir = QDir(ui->picoDir->text(), "RPI-RP2");
	m_dstDir.cdUp();
	startTimer(1*500);
	ui->binary->setText(QString());
}

MainWindow::~MainWindow()
{
	Config::setValue("src", ui->srcDir->text());
	Config::setValue("pico", ui->picoDir->text());
	delete ui;
}

void MainWindow::quit()
{
	close();
}


void MainWindow::on_actionSrcDir_triggered()
{
	QString dir = QFileDialog::getExistingDirectory(this, "Build Dir", ui->srcDir->text());
	if (!  dir.isNull())
	{
		ui->srcDir->setText(dir);
		m_srcDir =  QDir(dir, "*.uf2");
		checkSrc();
	}
}

void MainWindow::on_actionPicoDir_triggered()
{
	QString dir = QFileDialog::getExistingDirectory(this, "Pico Dir", ui->picoDir->text());
	if (!  dir.isNull())
	{
		ui->picoDir->setText(dir);
		m_dstDir = QDir(ui->picoDir->text(), "RPI-RP2");
		m_dstDir.cdUp();
	}
}


void MainWindow::timerEvent(QTimerEvent *event)
{
	Q_UNUSED(event);
	checkSrc();
	checkPico();
	if (m_hasSrc && m_hasPico)
	{
		QFile f(m_srcDir.absoluteFilePath(ui->binary->text()));
		if (! f.open(QIODevice::ReadOnly))
		{
			qWarning() << Q_FUNC_INFO << f.fileName() << f.errorString();
			return;
		}
		QDir ddir(m_dstDir);
		ddir.cd("RPI-RP2");
		QString df(ddir.absoluteFilePath(ui->binary->text()));
		qApp->processEvents();
		if (! f.copy(df))
		{
			qWarning() << Q_FUNC_INFO << f.fileName() << df << "*** no copy";
			return;
		}
		f.remove();
	}
}

void MainWindow::checkSrc()
{
	m_srcDir.refresh();
	const QStringList uf2 = m_srcDir.entryList();
	if (uf2.isEmpty())
	{
		ui->binary->setText(QString());
		m_hasSrc = false;
	}
	else
	{
		ui->binary->setText(uf2.first());
		m_hasSrc = true;
	}
}

void MainWindow::checkPico()
{
	m_dstDir.refresh();
	const QStringList fl = m_dstDir.entryList();
	if (! fl.isEmpty())
	{
		ui->picoLabel->setPixmap(LedIconSource::pix(LedIconSource::Green));
		m_hasPico = true;
	}
	else
	{
		ui->picoLabel->setPixmap(LedIconSource::pix(LedIconSource::Yellow));
		m_hasPico = false;
	}
}
