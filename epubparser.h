#ifndef EPUBPARSER_H
#define EPUBPARSER_H

#include <QObject>
#include <QHash>
#include <QSet>
#include <QVector>
#include <QDomNode>

class KZip;
class KArchiveDirectory;
class KArchiveFile;
class QXmlStreamReader;

struct EpubItem {
    QString path;
    QString mimetype;
};

struct EpubPageReference {
    enum StandardType {
        CoverPage,
        TitlePage,
        TableOfContents,
        Index,
        Glossary,
        Acknowledgements,
        Bibliography,
        Colophon,
        CopyrightPage,
        Dedication,
        Epigraph,
        Foreword,
        ListOfIllustrations,
        ListOfTables,
        Notes,
        Preface,
        Text,
        Other
    };

    static StandardType typeFromString(const QString &name);

    QString target;
    QString title;
};

class EPubParser : public QObject
{
    Q_OBJECT
public:
    explicit EPubParser(QObject *parent = 0);
    ~EPubParser();

    bool loadFile(const QString path);

signals:
    void errorHappened(const QString &error);

public slots:

private:
    bool parseMimetype();
    bool parseContainer();
    bool parseContentFile(const QString filepath);
    bool parseMetadataItem(const QDomNode &metadataNode);
    bool parseManifestItem(const QDomNode &manifestNodes, const QString currentFolder);
    bool parseSpineItem(const QDomNode &spineNode);
    bool parseGuideItem(const QDomNode &guideItem);

    const KArchiveFile *getFile(const QString &path);

    KZip *m_archive;
    const KArchiveDirectory *m_rootFolder;

    QHash<QString, QString> m_metadata;

    QHash<QString, EpubItem> m_items;
    QVector<QString> m_orderedItems;
    QSet<QString> m_unorderedItems;

    QHash<EpubPageReference::StandardType, EpubPageReference> m_standardReferences;
    QHash<QString, EpubPageReference> m_otherReferences;
};

#endif // EPUBPARSER_H
