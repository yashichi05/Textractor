#pragma once

#include <QString>
#include <QVector>
#include <QHash>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QMainWindow>
#include <QDialog>
#include <QApplication>
#include <QLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QListWidget>
#include <QMessageBox>
#include <QInputDialog>

static thread_local bool ok;

constexpr auto CONFIG_FILE = u8"Textractor.ini";
constexpr auto WINDOW = u8"Window";

// https://docs.microsoft.com/zh-tw/cpp/cpp/initializing-classes-and-structs-without-constructors-cpp?view=msvc-160
// struct 類似class，struct 預設public 繼承、宣告，class 預設private繼承、宣告
// struct 繼承 QSettings，裡面有一個建構子，須帶入QObject，並且呼叫繼承的QSettings 建構子且帶入parent
// CONFIG_FILE ini檔案位置，IniFormat使用ini檔案宣告
struct Settings : QSettings { Settings(QObject* parent = nullptr) : QSettings(CONFIG_FILE, QSettings::IniFormat, parent) {} };
struct QTextFile : QFile { QTextFile(QString name, QIODevice::OpenMode mode) : QFile(name) { open(mode | QIODevice::Text); } };
struct Localizer { Localizer() { Localize(); } };
inline std::wstring S(const QString& s) { return { s.toStdWString() }; }
inline QString S(const std::string& s) { return QString::fromStdString(s); }
inline QString S(const std::wstring& s) { return QString::fromStdWString(s); }
// TODO: allow paired surrogates
inline void sanitize(QString& s) { s.chop(std::distance(std::remove_if(s.begin(), s.end(), [](QChar ch) { return ch.isSurrogate(); }), s.end())); }
inline QString sanitize(QString&& s) { sanitize(s); return std::move(s); }
