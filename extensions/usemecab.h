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
    useMecab(std::wstring &sentence, int fontSize)
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
    const mecab_node_t *node_sentence;

private:
    mecab_t *mecab;
    void convertStr(std::wstring &sentence)
    {
        QString input_qs(S(sentence));
        char *char_s = new char[input_qs.toStdString().length() + 1];
        strcpy(char_s, input_qs.toStdString().c_str());
        char_sentence = char_s;
        getMecabNode();
    };
    void getMecabNode()
    {
        node_sentence = mecab_sparse_tonode(mecab, char_sentence);
    };
    void outputHtml(std::wstring &sentence, int fontSize)
    {

        QString style = "<style>td{padding:0 1px;}</style>";
        QString tr1 = "";
        QString tr2 = "";
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

                tr1 += "<td align=\"center\" style=\"font-size:" + QString::number(fontSize * 0.75) + "px;\">" + pronounce_list.takeLast() + "</td>";
                tr2 += "<td align=\"center\">" + current_str + "</td>";
            }
        }
        tr1 = "<tr align=\"center\">" + tr1 + "</tr>";
        tr2 = "<tr align=\"center\">" + tr2 + "</tr>";
        sentence = (style + "<table>" + tr1 + tr2 + "</table>").toStdWString();
    }
};
