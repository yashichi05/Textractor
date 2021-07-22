## text.cpp
```c++
const char* ROW_MAX_SENTENCE_SIZE = u8"Row max sentence size";
const char* SELECT_LANGUAGE = u8"Select language";
```
## extensions\CMakeLists.txt
```cmake
find_qt5(Core Widgets TextToSpeech WebSockets)
link_directories(mecab)
target_link_libraries(Extra\ Window Qt5::Widgets Qt5::TextToSpeech libmecab)

COMMAND Qt5::windeployqt --dir ${CMAKE_FINAL_OUTPUT_DIRECTORY} "${CMAKE_FINAL_OUTPUT_DIRECTORY}/Extra\ Window.dll"

file(GLOB MECAB mecab/*)
file(COPY ${MECAB} DESTINATION ${CMAKE_FINAL_OUTPUT_DIRECTORY})
```
## extensions\extrawindow.cpp
```c++
#include <QTextToSpeech>
extern const char *ROW_MAX_SENTENCE_SIZE;
extern const char *SELECT_LANGUAGE;

//改為富文本
ui.display->setTextFormat(Qt::RichText);
//取得ini設定的語言、字串長度
rowMaxSentenceSize = settings.value(ROW_MAX_SENTENCE_SIZE, rowMaxSentenceSize).toInt();
selectLang = settings.value(SELECT_LANGUAGE,selectLang).toString();
//語音
m_speech = new QTextToSpeech(this);
const QVector<QLocale> locales = m_speech->availableLocales();
QStringList locales_list;
for (const QLocale &locale : locales)
{
  locales_list.push_back(locale.name());
}
getLangSetting(locales_list);
menu.addAction(SELECT_LANGUAGE, this, [this, locales_list] {
  settings.setValue(SELECT_LANGUAGE, selectLang = QInputDialog::getItem(this, SELECT_LANGUAGE, "", locales_list, this->selectLangIndex, nullptr, false, Qt::WindowCloseButtonHint));
  this->getLangSetting(locales_list);
});
menu.addAction(ROW_MAX_SENTENCE_SIZE, this, [this] {
  settings.setValue(ROW_MAX_SENTENCE_SIZE, rowMaxSentenceSize = QInputDialog::getInt(this, ROW_MAX_SENTENCE_SIZE, "", rowMaxSentenceSize, 0, INT_MAX, 1, nullptr, Qt::WindowCloseButtonHint));
});
		
int rowMaxSentenceSize;
QString selectLang;
int selectLangIndex = 0;
QTextToSpeech *m_speech = nullptr;
QString speakSentence
	
void getLangSetting(QStringList list)
{
  for (int i = 0; i < list.size(); i++)
  {
    if (selectLang == list[i])
    {
      selectLangIndex = i;
      const QVector<QLocale> locales = m_speech->availableLocales();
      m_speech->setLocale(locales[i]);
      break;
    }
  }
};
	
m_speech->stop();
int maxSentenceSize = 1000000;

useMecab mecabRes(sentence, extraWindow.ui, extraWindow.rowMaxSentenceSize);
QString speak_sentence = mecabRes.char_sentence;

QMetaObject::invokeMethod(&extraWindow, [sentence = S(sentence), speak_sentence] {
  extraWindow.AddSentence(sentence);
  extraWindow.speakSentence = speak_sentence;
});
}
```
## usebecab.h