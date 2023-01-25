#ifndef EXCELFILE_H
#define EXCELFILE_H

#include <QObject>
#include "UsingNames.h"



#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;


// класс работы с Excele файлом
class ExcelFile : public QObject {

    Q_OBJECT

private:

    /// \brief Описывает максимальное количество ячеек на листе
    struct MaxSheetSize {
        const ushort WIDTH = 11;
        const ushort HEIGHT = 500;
    } SheetSize;

public:

    /// \brief Конструктор
    /// \param Загружает данные из excel документа в структуру контейнеров
    explicit ExcelFile(const QString& excelLoadPath, QObject *parent = nullptr);

    ExcelFile() = delete;
    ~ExcelFile() = default;

    /// \brief GetSheetsNames
    /// \return возвращает осписок названий листов загруженного Excel файла
    const QStringList GetSheetsNames() const;

    /// \brief GetSheets
    /// \return
    const std::vector<std::pair<SheetName,Sheet>>& GetSheets() const;

    /// \brief формирует excel документ
    /// \param sheets структура контейнеров, описывающая excel документ
    /// \param fullNamePath полный адрес для сохранения excel документа
    const void CreateExcelDocument(const std::vector<std::pair<SheetName,Sheet>>& sheets, const QString& fullNamePath) const;

private:
    /// \brief настройки шапки колонок
    /// (цвет, шрифт, заливка, жирный)
    /// хардкод в конструкторе
   QXlsx::Format header;

   /// \brief структура контейнеров, описывающая excel документ
   std::vector<std::pair<SheetName,Sheet>> excelFileData;
};


#endif // EXCELFILE_H
