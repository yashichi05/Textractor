#include "mecab/mecab.h"

#define CHECK(eval)                                               \
    if (!eval)                                                    \
    {                                                             \
        fprintf(stderr, "Exception:%s\n", mecab_strerror(mecab)); \
        mecab_destroy(mecab);                                     \
        return false;                                             \
    }
// common.h 有提供
class useMecab
{
public:
    useMecab(std::wstring &sentence, int fontSize = 16)
    {
        char *argv = "";
        mecab = mecab_new(0, &argv);
        convertStr(sentence);
        outputHtml(sentence, fontSize);
    };
    ~useMecab()
    {
        mecab_destroy(mecab);
    };
    const char *char_sentence;
    std::wstring translate_sentence = L"\x200b \n";
    const mecab_node_t *node_sentence;

private:
    mecab_t *mecab;
    void convertStr(std::wstring &sentence)
    {
        QString input_qs(S(sentence));
        QStringList text_ary = input_qs.split(QString(S(L"\x200b \n")));
        QString origin_text = text_ary[0];
        if (text_ary.size() > 1)
            translate_sentence += text_ary[1].toStdWString();
        char *char_s = new char[origin_text.toStdString().length() + 1];
        strcpy(char_s, origin_text.toStdString().c_str());
        char_sentence = char_s;
        getMecabNode();
    };
    void getMecabNode()
    {
        node_sentence = mecab_sparse_tonode(mecab, char_sentence);
    };
    void outputHtml(std::wstring &sentence, int fontSize)
    {

        QString style = "<style>span{float:left;}</style>";
        QString content = "";
        for (; node_sentence; node_sentence = node_sentence->next)
        {
            if (node_sentence->stat == MECAB_NOR_NODE || node_sentence->stat == MECAB_UNK_NODE)
            {
                // 當下解析字
                QString current_str(node_sentence->surface);
                current_str = current_str.trimmed();
                current_str.truncate(node_sentence->length / 3);
                QString pronounce(node_sentence->feature);
                QStringList pronounce_list = pronounce.split(QString(","));

                content += "<span><div style=\"font-size:" + QString::number(fontSize * 0.75) + "px;\">" + pronounce_list.takeLast() + "</div>";
                content += "<div style=\"white-space:nowrap;\">" + current_str + "</div></span>";
            }
        }
        sentence = (style + content).toStdWString();
    }
};
